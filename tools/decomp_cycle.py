#!/usr/bin/env python3
"""Run the WarioWare matching-decompilation lifecycle.

The command is deliberately small and repository-local.  A JSON manifest names
one or more C candidates and their target assembly/object.  ``isolate`` compiles
the whole manifest in one devkitARM container, compares every object with
objdiff, and writes a durable run receipt.  ``apply`` and ``apply-batch`` are
guarded transactional paths: they require isolated matches, perform the
mechanical source/linker/assembly move, run the clean ROM gate, and restore the
exact pre-apply files (then rebuild the baseline) if the ROM is not
byte-identical.  ``apply-batch`` deliberately performs one full-ROM build for
the entire isolated manifest.

This tool never commits or pushes.  The repository hooks use the same verifier,
and the accepted run receipt is intended to be committed with the source and
documentation by the normal decomp workflow.
"""

from __future__ import annotations

import argparse
import datetime as dt
import hashlib
import json
import os
import re
import shlex
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path
from typing import Any

from check_decomp_policy import has_nonempty_asm


IMAGE = "devkitpro/devkitarm:latest"
FUNCTION_RE = re.compile(r"^(?:func|asm)_([0-9A-Fa-f]{8})$")
ROM_AFFECTING_PREFIXES = (
    "src/",
    "asm/",
    "include/",
    "data/",
    "audio/",
    "graphics/",
)
ROM_AFFECTING_FILES = {
    "Makefile",
    "wariowareinc.ld",
    "wariowareinc_modern.ld",
    "undefined_syms.ld",
    "tools/agbcc-swi.patch",
}
EVIDENCE_PREFIXES = (".decomp-runs/", ".nearmiss/", ".mizuchi-tmp/")
EVIDENCE_FILES = {"tools/attempts.tsv"}


class CycleError(RuntimeError):
    """A user-facing lifecycle/setup error."""


def utc_now() -> str:
    return dt.datetime.now(dt.timezone.utc).replace(microsecond=0).isoformat()


def run_command(args: list[str], root: Path, *, timeout: int = 30, check: bool = True) -> subprocess.CompletedProcess[str]:
    return subprocess.run(args, cwd=root, text=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                          timeout=timeout, check=check)


def git_output(root: Path, *args: str) -> str:
    try:
        return run_command(["git", *args], root, timeout=30).stdout.strip()
    except (OSError, subprocess.CalledProcessError, subprocess.TimeoutExpired):
        return ""


def file_sha1(path: Path) -> str | None:
    if not path.is_file():
        return None
    digest = hashlib.sha1()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def file_sha256(path: Path) -> str | None:
    if not path.is_file():
        return None
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def entry_identity(root: Path, entry: dict[str, Any]) -> dict[str, Any]:
    """Return content identities for the inputs that a receipt actually used."""
    identity: dict[str, Any] = {
        "candidate": entry.get("candidate_rel"),
        "candidate_sha256": file_sha256(entry["candidate"]),
        "target": entry.get("target_rel"),
        "target_sha256": file_sha256(entry["target"]) if entry.get("target") else None,
        "target_object": entry.get("target_object_rel"),
        "target_object_sha256": file_sha256(entry["target_object"])
        if entry.get("target_object") else None,
    }
    return identity


def root_relative(root: Path, value: Path) -> str:
    try:
        return value.resolve().relative_to(root.resolve()).as_posix()
    except ValueError as exc:
        raise CycleError(f"path must be inside the repository: {value}") from exc


def display_path(root: Path, value: Path) -> str:
    try:
        return root_relative(root, value)
    except CycleError:
        return str(value)


def root_path(root: Path, value: str | Path, *, must_exist: bool = False) -> Path:
    path = Path(value)
    if not path.is_absolute():
        path = root / path
    path = path.resolve()
    root_relative(root, path)
    if must_exist and not path.is_file():
        raise CycleError(f"file not found: {root_relative(root, path)}")
    return path


def addr_for(function: str) -> str | None:
    match = FUNCTION_RE.match(function)
    return match.group(1).lower() if match else None


def default_paths(root: Path, function: str) -> dict[str, str]:
    address = addr_for(function)
    if address is None:
        raise CycleError(f"cannot derive WarioWare paths from function name {function!r}")
    return {
        "source": f"src/decomp/asm_{address}.c",
        "target": f"asm/asm_{address}.s",
        "target_object": f"build/asm/asm_{address}.s.o",
        "converted": f"asm/converted/asm_{address}.s",
        "linker_old": f"build/asm/asm_{address}.s.o",
        "linker_new": f"build/src/decomp/asm_{address}.c.o",
    }


def load_manifest(root: Path, manifest_path: Path) -> list[dict[str, Any]]:
    try:
        raw = json.loads(manifest_path.read_text())
    except (OSError, json.JSONDecodeError) as exc:
        raise CycleError(f"could not read manifest {manifest_path}: {exc}") from exc

    entries = raw.get("candidates") if isinstance(raw, dict) else raw
    if not isinstance(entries, list) or not entries:
        raise CycleError("manifest must contain a non-empty 'candidates' array")

    normalized: list[dict[str, Any]] = []
    for index, raw_entry in enumerate(entries):
        if not isinstance(raw_entry, dict):
            raise CycleError(f"manifest candidate {index} is not an object")
        function = str(raw_entry.get("function", ""))
        candidate_value = raw_entry.get("candidate")
        if not function or not candidate_value:
            raise CycleError(f"manifest candidate {index} needs function and candidate")
        candidate = root_path(root, str(candidate_value), must_exist=True)
        defaults = default_paths(root, function)

        target_object_value = raw_entry.get("target_object", defaults["target_object"])
        target_object = root_path(root, str(target_object_value)) if target_object_value else None
        if target_object is not None and not target_object.is_file():
            target_object = None

        target_value = raw_entry.get("target")
        if target_value:
            target = root_path(root, str(target_value), must_exist=True)
        else:
            default_target = root_path(root, defaults["target"])
            target = default_target if default_target.is_file() else None
        if target_object is None and target is None:
            raise CycleError(f"{function}: target assembly or existing target_object is required")

        entry: dict[str, Any] = {
            "function": function,
            "candidate": candidate,
            "candidate_rel": root_relative(root, candidate),
            "target": target,
            "target_rel": root_relative(root, target) if target else None,
            "target_object": target_object,
            "target_object_rel": root_relative(root, target_object) if target_object else None,
            "file": str(raw_entry.get("file") or (root_relative(root, target) if target else root_relative(root, target_object))),
            "mode": str(raw_entry.get("mode", "standalone_tu")),
            "source": root_path(root, str(raw_entry.get("source", defaults["source"]))),
            "converted": root_path(root, str(raw_entry.get("converted", defaults["converted"]))),
            "linker_old": str(raw_entry.get("linker_old", defaults["linker_old"])),
            "linker_new": str(raw_entry.get("linker_new", defaults["linker_new"])),
            "host_source": str(raw_entry.get("host_source", "")),
            "include_line": str(raw_entry.get("include_line", "")),
            "decomp_include_line": str(raw_entry.get("decomp_include_line", "")),
        }
        for key in ("source", "converted"):
            root_relative(root, entry[key])
        if entry["mode"] not in ("standalone_tu", "included_stub"):
            raise CycleError(f"{function}: unsupported mode {entry['mode']!r}")
        if entry["mode"] == "included_stub" and entry["host_source"]:
            entry["host_source_path"] = root_path(root, entry["host_source"], must_exist=True)
        elif entry["mode"] == "included_stub":
            raise CycleError(f"{function}: included_stub requires host_source")
        normalized.append(entry)
    return normalized


def container_script(root: Path, entries: list[dict[str, Any]], run_dir: Path) -> str:
    """Build one shell script that compiles and normalizes every pair."""

    def q(value: str) -> str:
        return shlex.quote(value)

    lines = [
        "set +e",
        'export PATH="/opt/devkitpro/devkitARM/bin:/opt/devkitpro/tools/bin:$PATH"',
        "mkdir -p /run",
    ]
    for index, entry in enumerate(entries):
        candidate = f"/workspace/{entry['candidate_rel']}"
        candidate_obj = f"/run/candidate-{index}.o"
        status = f"/run/candidate-{index}.status"
        error = f"/run/candidate-{index}.stderr"
        pp = f"/run/candidate-{index}.i"
        asm = f"/run/candidate-{index}.s"
        lines.extend([
            f"rm -f {q(candidate_obj)} {q(status)} {q(error)} {q(pp)} {q(asm)}",
            f"if arm-none-eabi-gcc -E -P -I tools/agbcc -I tools/agbcc/include -I . -iquote include -nostdinc -undef {q(candidate)} -o {q(pp)} >{q(error)} 2>&1; then",
            f"  if tools/agbcc/bin/agbcc {q(pp)} -o {q(asm)} -mthumb-interwork -Wimplicit -Wparentheses -Werror -O2 -g -fhex-asm >>{q(error)} 2>&1; then",
            # Keep the isolated object byte-compatible with Makefile's C
            # pipeline.  In particular, the assembler's normal code-section
            # alignment fill is a Thumb NOP; the real build appends a
            # zero-filled aligned .text tail, so omitting it produces false
            # near-misses for functions whose body ends at offset 2 mod 4.
            f"    sed '/\\.size/d' {q(asm)} > {q(asm)}.stripped && printf '.text\\n\\t.align\\t2, 0\\n' >> {q(asm)}.stripped && printf '.section .note.GNU-stack,\"\",%%progbits\\n' >> {q(asm)}.stripped && mv {q(asm)}.stripped {q(asm)}",
            f"    if arm-none-eabi-as -march=armv4t -o {q(candidate_obj)} {q(asm)} >>{q(error)} 2>&1; then",
            f"      echo ok > {q(status)}",
            "    else",
            f"      echo compile_error > {q(status)}",
            "    fi",
            "  else",
            f"    echo compile_error > {q(status)}",
            "  fi",
            "else",
            f"  echo compile_error > {q(status)}",
            "fi",
        ])

        if entry["target_object"] is None:
            target = f"/workspace/{entry['target_rel']}"
            target_obj = f"/run/target-{index}.o"
            target_status = f"/run/target-{index}.status"
            target_error = f"/run/target-{index}.stderr"
            target_pp = f"/run/target-{index}.s.preprocessed"
            lines.extend([
                f"rm -f {q(target_obj)} {q(target_status)} {q(target_error)} {q(target_pp)}",
                f"if arm-none-eabi-gcc -E -P -I tools/agbcc -I tools/agbcc/include -I . -iquote include -nostdinc -undef -x assembler-with-cpp {q(target)} -o {q(target_pp)} >{q(target_error)} 2>&1; then",
                f"  if arm-none-eabi-as -march=armv4t -o {q(target_obj)} {q(target_pp)} >>{q(target_error)} 2>&1; then",
                f"    echo ok > {q(target_status)}",
                "  else",
                f"    echo assemble_error > {q(target_status)}",
                "  fi",
                "else",
                f"  echo preprocess_error > {q(target_status)}",
                "fi",
            ])

        target_input = (f"/workspace/{entry['target_object_rel']}"
                        if entry["target_object"] is not None else f"/run/target-{index}.o")
        candidate_link = f"/run/candidate-{index}.elf"
        target_link = f"/run/target-{index}.elf"
        link_error = f"/run/link-{index}.stderr"
        defsym = f"/run/target-{index}.defsym"
        unit_defsym = f"/run/target-{index}.unit-defsym"
        lines.extend([
            f"rm -f {q(candidate_link)} {q(target_link)} {q(link_error)} {q(defsym)} {q(unit_defsym)}",
            f"if [ -f {q(candidate_obj)} ] && [ -f {q(target_input)} ]; then",
            # gba.inc makes target symbols absolute, while agbcc leaves the
            # same globals as relocations/common symbols. Link both sides at
            # address zero with the target's absolute symbol map so isolation
            # compares the bytes seen by the real ROM linker.
            f"  if arm-none-eabi-nm -a --defined-only {q(target_input)} | awk 'NF >= 3 && $2 == \"a\" && $3 !~ /^\\./ {{printf \"--defsym=%s=0x%s\\n\", $3, $1}}' > {q(defsym)}; then",
            # Included stubs are compiled inside a larger host TU.  The
            # candidate object is intentionally standalone, so calls to
            # sibling functions otherwise remain unresolved and objdiff sees
            # artificial BL-to-zero differences.  Reuse the host object's
            # section-relative symbol offsets for the candidate link.  Keep
            # the entry function out of this map: it must remain at offset 0
            # in the candidate ELF while the target keeps its host-TU offset.
            f"    arm-none-eabi-nm -a --defined-only {q(target_input)} | awk -v entry={q(entry['function'])} 'NF >= 3 && $2 ~ /^[TtRrDdBb]$/ && $3 !~ /^\\./ && $3 != entry {{printf \"--defsym=%s=0x%s\\n\", $3, $1}}' > {q(unit_defsym)}",
            "    defsym_args=()",
            f"    while read -r defsym_arg; do defsym_args+=(\"$defsym_arg\"); done < {q(defsym)}",
            "    unit_defsym_args=()",
            f"    while read -r unit_defsym_arg; do unit_defsym_args+=(\"$unit_defsym_arg\"); done < {q(unit_defsym)}",
            f"    if arm-none-eabi-ld -Ttext=0 -e {q(entry['function'])} \"${{defsym_args[@]}}\" --unresolved-symbols=ignore-all --noinhibit-exec {q(target_input)} -o {q(target_link)} >>{q(link_error)} 2>&1; then",
            f"      arm-none-eabi-ld -Ttext=0 -e {q(entry['function'])} \"${{defsym_args[@]}}\" \"${{unit_defsym_args[@]}}\" --unresolved-symbols=ignore-all --noinhibit-exec {q(candidate_obj)} -o {q(candidate_link)} >>{q(link_error)} 2>&1",
            "    fi",
            "  fi",
            "fi",
        ])
    lines.append("exit 0")
    return "\n".join(lines) + "\n"


def compact_diff(data: Any, function: str) -> Any:
    """Keep function-level objdiff evidence without copying ELF symtabs."""
    if not isinstance(data, dict):
        return data

    def compact_side(side: Any) -> Any:
        if not isinstance(side, dict):
            return side
        symbols = [symbol for symbol in side.get("symbols", [])
                   if isinstance(symbol, dict) and
                   (symbol.get("name") == function or
                    str(symbol.get("name", "")).lower() == function.lower() or
                    symbol.get("name") == function + "+1")]
        sections = [section for section in side.get("sections", [])
                    if isinstance(section, dict) and section.get("name") in (".text", ".data", ".bss")]
        return {"sections": sections, "symbols": symbols}

    return {"left": compact_side(data.get("left")), "right": compact_side(data.get("right"))}


def parse_diff(data: Any, function: str) -> dict[str, Any]:
    if not isinstance(data, dict):
        return {"match_percent": None, "symbol_found": False, "diff_count": None}

    def find_symbol(side: Any) -> dict[str, Any] | None:
        if not isinstance(side, dict):
            return None
        for symbol in side.get("symbols", []):
            if not isinstance(symbol, dict):
                continue
            name = str(symbol.get("name", ""))
            if name == function or name.lower() == function.lower() or name == function + "+1":
                return symbol
        return None

    left = find_symbol(data.get("left"))
    right = find_symbol(data.get("right"))
    symbol = right or left
    if symbol is None:
        return {"match_percent": None, "symbol_found": False, "diff_count": None}
    value = symbol.get("match_percent", symbol.get("matchPercent"))
    try:
        match_percent = float(value)
    except (TypeError, ValueError):
        match_percent = None

    diff_count = 0
    saw_instruction = False
    for side in (left, right):
        if not side:
            continue
        for instruction in side.get("instructions", []):
            if not isinstance(instruction, dict):
                continue
            saw_instruction = True
            kind = instruction.get("diff_kind", instruction.get("diffKind"))
            if kind not in (None, "NONE", "none", "Equal", "equal"):
                diff_count += 1
    if saw_instruction:
        diff_count //= 2
    return {
        "match_percent": match_percent,
        "symbol_found": True,
        "diff_count": diff_count if saw_instruction else None,
    }


def objdiff(root: Path, entry: dict[str, Any], target_obj: Path, candidate_obj: Path) -> tuple[dict[str, Any] | None, str, str]:
    cli = root / "tools/objdiff-cli"
    if not cli.exists():
        raise CycleError(f"objdiff CLI is missing: {root_relative(root, cli)}")
    command = [
        str(cli), "diff", "-1", str(target_obj), "-2", str(candidate_obj), entry["function"],
        "--format", "json", "-o", "-", "-c", "arm.archVersion=v4t", "-c", "functionRelocDiffs=none",
    ]
    completed = subprocess.run(command, cwd=root, text=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                               timeout=60, check=False)
    stdout = completed.stdout or ""
    stderr = completed.stderr or ""
    try:
        return json.loads(stdout), stdout, stderr
    except json.JSONDecodeError:
        return None, stdout, stderr


def record_near_miss(root: Path, entry: dict[str, Any], score: float, diff: Any, command: str) -> str:
    with tempfile.NamedTemporaryFile("w", suffix=".json", prefix="warioware-diff-", delete=False) as stream:
        json.dump(diff, stream, indent=2)
        diff_path = Path(stream.name)
    try:
        recorder = root / "tools/record_nearmiss.py"
        completed = subprocess.run([
            sys.executable, str(recorder), entry["function"], entry["file"], str(score),
            str(entry["candidate"]), "--reason", "isolated_mismatch", "--score-kind", "isolated_objdiff_gap",
            "--diff-json", str(diff_path),
            "--command", command, "--root", str(root),
        ], cwd=root, text=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=False)
        if completed.returncode != 0:
            return f"recording failed: {completed.stderr.strip() or completed.stdout.strip()}"
        return completed.stdout.strip()
    finally:
        diff_path.unlink(missing_ok=True)


def run_isolation(root: Path, entries: list[dict[str, Any]], *, record: bool = True) -> dict[str, Any]:
    started = utc_now()
    run_dir = Path(tempfile.mkdtemp(prefix="warioware-cycle-"))
    command = ""
    try:
        script = container_script(root, entries, run_dir)
        command_args = [
            "docker", "run", "--rm", "-v", f"{root}:/workspace:ro", "-v", f"{run_dir}:/run", "-w", "/workspace",
            IMAGE, "bash", "-lc", script,
        ]
        command = shlex.join(command_args)
        try:
            docker = subprocess.run(command_args, cwd=root, text=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                    timeout=600, check=False)
        except (OSError, subprocess.TimeoutExpired) as exc:
            docker = None
            docker_error = str(exc)
        else:
            docker_error = docker.stderr.strip()

        results: list[dict[str, Any]] = []
        for index, entry in enumerate(entries):
            candidate_obj = run_dir / f"candidate-{index}.o"
            candidate_status = run_dir / f"candidate-{index}.status"
            candidate_error = run_dir / f"candidate-{index}.stderr"
            if docker is None:
                results.append({"function": entry["function"], "candidate": entry["candidate_rel"],
                                "status": "docker_error", "error": docker_error})
                continue
            if not candidate_status.is_file() or candidate_status.read_text().strip() != "ok":
                results.append({
                    "function": entry["function"], "file": entry["file"], "candidate": entry["candidate_rel"],
                    "target": entry["target_rel"], "target_object": entry["target_object_rel"],
                    "status": "compile_error", "score": None,
                    "stderr": candidate_error.read_text(errors="replace") if candidate_error.is_file() else docker_error,
                })
                continue

            if entry["target_object"] is not None:
                target_obj = entry["target_object"]
                target_status = "existing"
                target_error_text = ""
            else:
                target_obj = run_dir / f"target-{index}.o"
                target_status_path = run_dir / f"target-{index}.status"
                target_error_path = run_dir / f"target-{index}.stderr"
                target_status = target_status_path.read_text().strip() if target_status_path.is_file() else "missing"
                target_error_text = target_error_path.read_text(errors="replace") if target_error_path.is_file() else docker_error
            if not target_obj.is_file() or target_status not in ("existing", "ok"):
                results.append({
                    "function": entry["function"], "file": entry["file"], "candidate": entry["candidate_rel"],
                    "target": entry["target_rel"], "target_object": entry["target_object_rel"],
                    "status": "target_error", "score": None, "stderr": target_error_text,
                })
                continue

            candidate_link = run_dir / f"candidate-{index}.elf"
            target_link = run_dir / f"target-{index}.elf"
            if candidate_link.is_file() and target_link.is_file():
                compare_target = target_link
                compare_candidate = candidate_link
                comparison = "linked_elf"
            else:
                compare_target = target_obj
                compare_candidate = candidate_obj
                comparison = "raw_object"
            fallback_reason = ""
            try:
                diff, diff_stdout, diff_stderr = objdiff(root, entry, compare_target, compare_candidate)
            except (OSError, subprocess.TimeoutExpired, CycleError) as exc:
                diff = None
                diff_stdout = ""
                diff_stderr = str(exc)
            if diff is None:
                # A few legacy asm units have malformed symbol metadata (for
                # example .thumb_func appears after glabel).  The linker
                # cannot load those targets, but a raw-object comparison is
                # still a valid byte-level isolation check; the later ROM
                # gate remains authoritative for relocations and placement.
                if comparison == "linked_elf":
                    fallback_reason = diff_stderr[-4000:] or diff_stdout[-4000:]
                    try:
                        diff, diff_stdout, diff_stderr = objdiff(root, entry, target_obj, candidate_obj)
                    except (OSError, subprocess.TimeoutExpired, CycleError) as exc:
                        diff = None
                        diff_stdout = ""
                        diff_stderr = str(exc)
                    if diff is not None:
                        comparison = "raw_object_fallback"
                if diff is None:
                    results.append({
                        "function": entry["function"], "file": entry["file"], "candidate": entry["candidate_rel"],
                        "target": entry["target_rel"], "target_object": entry["target_object_rel"],
                        "status": "diff_error", "score": None, "stdout": diff_stdout[-4000:],
                        "stderr": diff_stderr[-4000:],
                    })
                    continue
            parsed = parse_diff(diff, entry["function"])
            match_percent = parsed["match_percent"]
            exact = bool(parsed["symbol_found"] and match_percent is not None and match_percent >= 100.0)
            score = None if match_percent is None else round(max(0.0, 100.0 - match_percent), 6)
            compact = compact_diff(diff, entry["function"])
            result: dict[str, Any] = {
                "function": entry["function"], "file": entry["file"], "candidate": entry["candidate_rel"],
                "target": entry["target_rel"], "target_object": entry["target_object_rel"],
                "status": "exact" if exact else "near_miss", "score": score,
                "match_percent": match_percent, "diff_count": parsed["diff_count"],
                "symbol_found": parsed["symbol_found"], "comparison": comparison, "diff": compact,
            }
            if fallback_reason:
                result["fallback_reason"] = fallback_reason
            if not exact and score is not None and record:
                result["near_miss_record"] = record_near_miss(root, entry, score, compact, command)
            results.append(result)

        for result, entry in zip(results, entries):
            result.update(entry_identity(root, entry))

        return {
            "schema": 1,
            "kind": "isolation",
            "started": started,
            "finished": utc_now(),
            "branch": git_output(root, "branch", "--show-current"),
            "commit": git_output(root, "rev-parse", "HEAD"),
            "command": command,
            "docker_image": IMAGE,
            "docker_invocations": 1,
            "candidate_count": len(entries),
            "results": results,
            "ok": all(result.get("status") == "exact" for result in results),
        }
    finally:
        shutil.rmtree(run_dir, ignore_errors=True)


def default_run_path(root: Path, kind: str, suffix: str = "") -> Path:
    stamp = dt.datetime.now(dt.timezone.utc).strftime("%Y%m%dT%H%M%SZ")
    safe_suffix = ("-" + re.sub(r"[^A-Za-z0-9_.-]+", "-", suffix)) if suffix else ""
    return root / ".decomp-runs" / f"{stamp}-{kind}{safe_suffix}.json"


def write_receipt(root: Path, record: dict[str, Any], output: Path | None) -> Path:
    path = output if output is not None else default_run_path(root, str(record.get("kind", "run")), str(record.get("function", "")))
    if not path.is_absolute():
        path = root / path
    path.parent.mkdir(parents=True, exist_ok=True)
    temporary = path.with_suffix(path.suffix + ".tmp")
    temporary.write_text(json.dumps(record, indent=2) + "\n")
    temporary.replace(path)
    return path


def tail(text: str, limit: int = 12000) -> str:
    return text if len(text) <= limit else "…\n" + text[-limit:]


def full_verify(root: Path, *, report: bool) -> dict[str, Any]:
    started = utc_now()
    log_path = "/tmp/warioware-full-build.log"
    shell = (
        "set -euo pipefail; "
        f"rm -rf build; make -j4 2>&1 | tee {log_path}; "
        f"grep -Fq 'wariowareinc.gba: OK' {log_path}"
    )
    if report:
        shell += f"; make report 2>&1 | tee -a {log_path}"
    command_args = [
        "docker", "run", "--rm", "-v", f"{root}:/workspace", "-w", "/workspace", IMAGE, "bash", "-lc", shell,
    ]
    command = shlex.join(command_args)
    try:
        completed = subprocess.run(command_args, cwd=root, text=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                   timeout=900, check=False)
        output = (completed.stdout or "") + ("\n" + completed.stderr if completed.stderr else "")
        docker_ok = completed.returncode == 0
    except (OSError, subprocess.TimeoutExpired) as exc:
        output = str(exc)
        docker_ok = False
    rom = root / "build/wariowareinc.gba"
    baseline = root / "baserom.gba"
    rom_hash = file_sha1(rom)
    baseline_hash = file_sha1(baseline)
    exact = bool(rom_hash and baseline_hash and rom_hash == baseline_hash)
    report_output = ""
    metrics_ok = True
    if docker_ok and report:
        metrics = root / "build/report.json"
        metrics_ok = metrics.is_file()
        report_output = "build/report.json present" if metrics_ok else "build/report.json missing"
    return {
        "started": started,
        "finished": utc_now(),
        "command": command,
        "docker_ok": docker_ok,
        "rom_exact": exact,
        "ok": docker_ok and exact and metrics_ok,
        "rom_sha1": rom_hash,
        "baseline_rom_sha1": baseline_hash,
        "report_requested": report,
        "report_ok": metrics_ok,
        "output_tail": tail(output),
        "report_output": report_output,
    }


def gen_objdiff(root: Path) -> dict[str, Any]:
    try:
        completed = run_command([sys.executable, "tools/gen_objdiff.py"], root, timeout=120)
        return {"ok": True, "output_tail": tail(completed.stdout + completed.stderr)}
    except (OSError, subprocess.CalledProcessError, subprocess.TimeoutExpired) as exc:
        output = getattr(exc, "stdout", "") or ""
        output += getattr(exc, "stderr", "") or ""
        return {"ok": False, "output_tail": tail(output or str(exc))}


def git_status(root: Path) -> str:
    return git_output(root, "status", "--porcelain")


def blocking_dirty_paths(root: Path) -> list[str]:
    """Ignore only generated evidence when guarding an apply transaction.

    Isolation deliberately leaves receipts, near-miss seeds, and the append-only
    attempt ledger behind.  Those are not source inputs and should not prevent
    the next exact candidate from entering the transactional ROM gate.  Any
    other tracked or untracked path remains a hard dirty-worktree stop.
    """
    blocked: list[str] = []
    for line in git_status(root).splitlines():
        if len(line) < 4:
            continue
        path = line[3:]
        if " -> " in path:
            path = path.split(" -> ", 1)[1]
        path = path.strip().replace("\\", "/")
        if path in EVIDENCE_FILES or any(path.startswith(prefix) for prefix in EVIDENCE_PREFIXES):
            continue
        blocked.append(path)
    return blocked


def changed_rom_files(root: Path) -> list[str]:
    output = git_output(root, "diff", "--name-only")
    output += ("\n" if output else "") + git_output(root, "diff", "--cached", "--name-only")
    return sorted(set(line.strip() for line in output.splitlines() if line.strip()))


def snapshot_files(root: Path, paths: list[Path]) -> dict[Path, bytes | None]:
    snapshot: dict[Path, bytes | None] = {}
    for path in paths:
        path = path.resolve()
        root_relative(root, path)
        snapshot[path] = path.read_bytes() if path.is_file() else None
    return snapshot


def entry_paths(root: Path, entry: dict[str, Any]) -> list[Path]:
    """Return every repository file touched by an apply transaction."""
    paths = [entry["source"], entry["converted"], root / "wariowareinc.ld"]
    if entry["target"] is not None:
        paths.append(entry["target"])
    if entry.get("host_source_path"):
        paths.append(entry["host_source_path"])
    return paths


def restore_snapshot(snapshot: dict[Path, bytes | None]) -> None:
    for path, content in reversed(list(snapshot.items())):
        if content is None:
            path.unlink(missing_ok=True)
        else:
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_bytes(content)


def apply_entry(root: Path, entry: dict[str, Any]) -> list[Path]:
    source = entry["source"]
    target = entry["target"]
    converted = entry["converted"]
    linker = root / "wariowareinc.ld"
    paths = [source, converted, linker]
    if target is not None:
        paths.append(target)
    if entry.get("host_source_path"):
        paths.append(entry["host_source_path"])
    snapshot = snapshot_files(root, paths)
    candidate_text = entry["candidate"].read_text()
    source.parent.mkdir(parents=True, exist_ok=True)
    source.write_text(candidate_text if candidate_text.endswith("\n") else candidate_text + "\n")

    if entry["mode"] == "standalone_tu":
        if not linker.is_file():
            raise CycleError("wariowareinc.ld is missing")
        linker_text = linker.read_text()
        old = entry["linker_old"]
        new = entry["linker_new"]
        if old not in linker_text:
            raise CycleError(f"linker entry disappeared before edit: {old}")
        linker.write_text(linker_text.replace(old, new, 1))
    else:
        host = entry["host_source_path"]
        host_text = host.read_text()
        old_include = entry["include_line"]
        if not old_include or old_include not in host_text:
            raise CycleError(f"host include disappeared before edit: {old_include or '<not supplied>'}")
        decomp_include = entry["decomp_include_line"]
        if not decomp_include:
            decomp_include = '#include "' + os.path.relpath(source, host.parent).replace(os.sep, "/") + '"'
        host.write_text(host_text.replace(old_include, decomp_include, 1))

    if target is None or not target.is_file():
        raise CycleError(f"assembly source to move is missing: {entry['target_rel']}")
    if converted.exists():
        raise CycleError(f"converted assembly destination already exists: {root_relative(root, converted)}")
    converted.parent.mkdir(parents=True, exist_ok=True)
    target.rename(converted)
    return list(snapshot)


def verify_current(root: Path, output: Path | None, *, report: bool) -> int:
    result = full_verify(root, report=report)
    receipt = {
        "schema": 1,
        "kind": "full_verify",
        "branch": git_output(root, "branch", "--show-current"),
        "commit": git_output(root, "rev-parse", "HEAD"),
        "files_with_changes": changed_rom_files(root),
        "verify": result,
        "ok": result["ok"],
    }
    receipt_path = write_receipt(root, receipt, output)
    print(json.dumps({"ok": result["ok"], "receipt": display_path(root, receipt_path),
                      "rom_sha1": result["rom_sha1"], "baseline_rom_sha1": result["baseline_rom_sha1"]}, indent=2))
    return 0 if result["ok"] else 1


def isolate_command(root: Path, manifest: Path, output: Path | None, *, record: bool, require_exact: bool) -> int:
    entries = load_manifest(root, manifest)
    record_data = run_isolation(root, entries, record=record)
    record_data["manifest"] = root_relative(root, manifest)
    record_data["manifest_sha256"] = file_sha256(manifest)
    record_data["candidate_identities"] = [entry_identity(root, entry) for entry in entries]
    path = write_receipt(root, record_data, output)
    print(json.dumps({"ok": record_data["ok"], "receipt": display_path(root, path),
                      "results": [{"function": r.get("function"), "status": r.get("status"),
                                   "score": r.get("score")} for r in record_data["results"]]}, indent=2))
    if require_exact and not record_data["ok"]:
        return 1
    return 0


def apply_command(root: Path, manifest: Path, function: str, output: Path | None, *, force: bool) -> int:
    entries = load_manifest(root, manifest)
    selected = [entry for entry in entries if entry["function"].lower() == function.lower()]
    if len(selected) != 1:
        raise CycleError(f"manifest must contain exactly one candidate named {function!r} for apply")
    entry = selected[0]
    dirty = blocking_dirty_paths(root)
    if dirty and os.environ.get("WARIOWARE_ALLOW_DIRTY") != "1":
        raise CycleError("refusing apply with unrelated dirty paths: " + ", ".join(dirty) +
                         "; generated evidence paths are allowed, source/tool edits are not")
    if has_nonempty_asm(entry["candidate"].read_text(errors="replace")):
        raise CycleError("refusing apply: candidate contains naked/original/instruction-bearing asm")

    isolation = run_isolation(root, [entry], record=True)
    isolated_result = isolation["results"][0] if isolation["results"] else {}
    if isolated_result.get("status") != "exact" and not force:
        receipt = {
            "schema": 1, "kind": "apply", "function": function,
            "branch": git_output(root, "branch", "--show-current"),
            "commit": git_output(root, "rev-parse", "HEAD"), "status": "refused_isolation",
            "manifest": root_relative(root, manifest),
            "manifest_sha256": file_sha256(manifest),
            "candidate_identities": [entry_identity(root, entry)],
            "isolation": isolation,
            "note": "No source/linker/assembly files were changed because isolated comparison was not exact.",
        }
        path = write_receipt(root, receipt, output)
        print(json.dumps({"ok": False, "status": receipt["status"], "receipt": display_path(root, path)}, indent=2))
        return 2

    snapshot: dict[Path, bytes | None] = {}
    full: dict[str, Any] | None = None
    rollback: dict[str, Any] | None = None
    status = "failed_before_verify"
    try:
        snapshot = snapshot_files(root, entry_paths(root, entry))
        apply_entry(root, entry)
        full = full_verify(root, report=True)
        if full["ok"]:
            metrics = gen_objdiff(root)
            status = "accepted" if metrics["ok"] else "accepted_report_metric_error"
            receipt = {
                "schema": 1, "kind": "apply", "function": function,
                "branch": git_output(root, "branch", "--show-current"),
                "commit": git_output(root, "rev-parse", "HEAD"), "status": status,
                "manifest": root_relative(root, manifest),
                "manifest_sha256": file_sha256(manifest),
                "candidate_identities": [entry_identity(root, entry)],
                "isolation": isolation, "full_verify": full, "objdiff_metrics": metrics,
                "changed_paths": [root_relative(root, path) for path in snapshot],
                "note": "Matching source/linker/assembly changes intentionally remain in the worktree for review and commit.",
            }
            path = write_receipt(root, receipt, output)
            print(json.dumps({"ok": status == "accepted", "status": status, "receipt": display_path(root, path)}, indent=2))
            return 0 if status == "accepted" else 1
        raise CycleError("clean Docker build did not produce a byte-identical ROM")
    except Exception as exc:
        if snapshot:
            restore_snapshot(snapshot)
            rollback = full_verify(root, report=True)
        receipt = {
            "schema": 1, "kind": "apply", "function": function,
            "branch": git_output(root, "branch", "--show-current"),
            "commit": git_output(root, "rev-parse", "HEAD"), "status": "rolled_back",
            "manifest": root_relative(root, manifest),
            "manifest_sha256": file_sha256(manifest),
            "candidate_identities": [entry_identity(root, entry)],
            "isolation": isolation, "full_verify": full, "rollback_verify": rollback,
            "error": str(exc),
            "changed_paths": [root_relative(root, path) for path in snapshot],
            "note": "The candidate transaction was restored. The rollback verifier rebuilt the clean baseline.",
        }
        path = write_receipt(root, receipt, output)
        print(json.dumps({"ok": False, "status": receipt["status"], "receipt": display_path(root, path),
                          "error": str(exc)}, indent=2))
        return 1


def apply_batch_command(root: Path, manifest: Path, output: Path | None, *, force: bool) -> int:
    """Apply a manifest as one rollback-capable full-ROM transaction."""
    entries = load_manifest(root, manifest)
    dirty = blocking_dirty_paths(root)
    if dirty and os.environ.get("WARIOWARE_ALLOW_DIRTY") != "1":
        raise CycleError("refusing apply-batch with unrelated dirty paths: " + ", ".join(dirty) +
                         "; generated evidence paths are allowed, source/tool edits are not")
    for entry in entries:
        if has_nonempty_asm(entry["candidate"].read_text(errors="replace")):
            raise CycleError(f"{entry['function']}: refusing apply: candidate contains naked/original/instruction-bearing asm")

    isolation = run_isolation(root, entries, record=True)
    failed = [result for result in isolation["results"] if result.get("status") != "exact"]
    if failed and not force:
        receipt = {
            "schema": 1, "kind": "apply_batch",
            "functions": [entry["function"] for entry in entries],
            "branch": git_output(root, "branch", "--show-current"),
            "commit": git_output(root, "rev-parse", "HEAD"), "status": "refused_isolation",
            "manifest": root_relative(root, manifest),
            "manifest_sha256": file_sha256(manifest),
            "candidate_identities": [entry_identity(root, entry) for entry in entries],
            "isolation": isolation,
            "note": "No source/linker/assembly files were changed because every isolated comparison was not exact.",
        }
        path = write_receipt(root, receipt, output)
        print(json.dumps({"ok": False, "status": receipt["status"], "receipt": display_path(root, path),
                          "failed": [{"function": result.get("function"), "status": result.get("status")}
                                     for result in failed]}, indent=2))
        return 2

    paths: list[Path] = []
    for entry in entries:
        paths.extend(entry_paths(root, entry))
    snapshot: dict[Path, bytes | None] = {}
    full: dict[str, Any] | None = None
    rollback: dict[str, Any] | None = None
    status = "failed_before_verify"
    try:
        snapshot = snapshot_files(root, paths)
        for entry in entries:
            apply_entry(root, entry)
        full = full_verify(root, report=True)
        if full["ok"]:
            metrics = gen_objdiff(root)
            status = "accepted" if metrics["ok"] else "accepted_report_metric_error"
            receipt = {
                "schema": 1, "kind": "apply_batch",
                "functions": [entry["function"] for entry in entries],
                "branch": git_output(root, "branch", "--show-current"),
                "commit": git_output(root, "rev-parse", "HEAD"), "status": status,
                "manifest": root_relative(root, manifest),
                "manifest_sha256": file_sha256(manifest),
                "candidate_identities": [entry_identity(root, entry) for entry in entries],
                "isolation": isolation, "full_verify": full, "objdiff_metrics": metrics,
                "changed_paths": [root_relative(root, path) for path in snapshot],
                "note": "Matching source/linker/assembly changes intentionally remain in the worktree for review and commit. One full-ROM gate covered the batch.",
            }
            path = write_receipt(root, receipt, output)
            print(json.dumps({"ok": status == "accepted", "status": status,
                              "receipt": display_path(root, path),
                              "functions": [entry["function"] for entry in entries]}, indent=2))
            return 0 if status == "accepted" else 1
        raise CycleError("clean Docker build did not produce a byte-identical ROM")
    except Exception as exc:
        if snapshot:
            restore_snapshot(snapshot)
            rollback = full_verify(root, report=True)
        receipt = {
            "schema": 1, "kind": "apply_batch",
            "functions": [entry["function"] for entry in entries],
            "branch": git_output(root, "branch", "--show-current"),
            "commit": git_output(root, "rev-parse", "HEAD"), "status": "rolled_back",
            "manifest": root_relative(root, manifest),
            "manifest_sha256": file_sha256(manifest),
            "candidate_identities": [entry_identity(root, entry) for entry in entries],
            "isolation": isolation, "full_verify": full, "rollback_verify": rollback,
            "error": str(exc),
            "changed_paths": [root_relative(root, path) for path in snapshot],
            "note": "The candidate transaction was restored. The rollback verifier rebuilt the clean baseline.",
        }
        path = write_receipt(root, receipt, output)
        print(json.dumps({"ok": False, "status": receipt["status"], "receipt": display_path(root, path),
                          "error": str(exc)}, indent=2))
        return 1


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    subparsers = parser.add_subparsers(dest="command", required=True)

    isolate = subparsers.add_parser("isolate", help="compile a candidate batch in one Docker container")
    isolate.add_argument("--manifest", type=Path, required=True)
    isolate.add_argument("--output", type=Path)
    isolate.add_argument("--no-record", action="store_true", help="do not update .nearmiss or tools/attempts.tsv")
    isolate.add_argument("--require-exact", action="store_true")

    apply = subparsers.add_parser("apply", help="isolate, apply, verify, and rollback one candidate transaction")
    apply.add_argument("--manifest", type=Path, required=True)
    apply.add_argument("--function", required=True)
    apply.add_argument("--output", type=Path)
    apply.add_argument("--force", action="store_true",
                       help="research-only: apply even when isolation is not exact; the ROM gate still controls acceptance")

    apply_batch = subparsers.add_parser("apply-batch", help="isolate, apply, verify, and rollback a candidate batch as one transaction")
    apply_batch.add_argument("--manifest", type=Path, required=True)
    apply_batch.add_argument("--output", type=Path)
    apply_batch.add_argument("--force", action="store_true",
                             help="research-only: apply even when one or more isolated comparisons are not exact; the ROM gate still controls acceptance")

    verify = subparsers.add_parser("verify", help="run the clean Docker ROM gate for the current worktree")
    verify.add_argument("--output", type=Path)
    verify.add_argument("--no-report", action="store_true")

    return parser


def main(argv: list[str] | None = None) -> int:
    parser = build_parser()
    args = parser.parse_args(argv)
    root = Path(__file__).resolve().parents[1]
    try:
        if args.command == "isolate":
            return isolate_command(root, args.manifest.resolve(), args.output, record=not args.no_record,
                                   require_exact=args.require_exact)
        if args.command == "apply":
            return apply_command(root, args.manifest.resolve(), args.function, args.output, force=args.force)
        if args.command == "apply-batch":
            return apply_batch_command(root, args.manifest.resolve(), args.output, force=args.force)
        if args.command == "verify":
            return verify_current(root, args.output, report=not args.no_report)
    except (CycleError, OSError, json.JSONDecodeError) as exc:
        print(f"decomp_cycle: {exc}", file=sys.stderr)
        return 2
    return 2


if __name__ == "__main__":
    raise SystemExit(main())
