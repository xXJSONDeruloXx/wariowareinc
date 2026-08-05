#!/usr/bin/env python3
"""Screen C spellings as one isolated batch, then apply an exact winner.

This is the WarioWare equivalent of Conker's candidate fan-in step.  It does
not invent C syntax: m2c, asmlift, or an agent places independent spellings in
one directory, and this command compiles every spelling in one Docker
invocation.  Near misses are recorded by ``decomp_cycle``; only a candidate
reported exact may be passed to ``accept``, which delegates the real source /
assembly move and clean ROM gate to the transactional cycle.
"""

from __future__ import annotations

import argparse
import datetime as dt
import json
import subprocess
import sys
import tempfile
from pathlib import Path
from typing import Any

import decomp_cycle


def now_stamp() -> str:
    return dt.datetime.now(dt.timezone.utc).strftime("%Y%m%dT%H%M%SZ")


def read_manifest(path: Path) -> dict[str, Any]:
    try:
        data = json.loads(path.read_text())
    except (OSError, json.JSONDecodeError) as exc:
        raise decomp_cycle.CycleError(f"could not read permutation manifest {path}: {exc}") from exc
    if not isinstance(data, dict) or not isinstance(data.get("candidates"), list) or len(data["candidates"]) != 1:
        raise decomp_cycle.CycleError("permutation manifest must contain exactly one template candidate")
    if not isinstance(data["candidates"][0], dict):
        raise decomp_cycle.CycleError("permutation manifest template must be an object")
    return data


def candidate_files(root: Path, directory: Path) -> list[Path]:
    directory = directory.resolve()
    decomp_cycle.root_relative(root, directory)
    files = sorted(path for path in directory.glob("*.c") if path.is_file())
    if not files:
        raise decomp_cycle.CycleError(f"no C candidates found in {decomp_cycle.root_relative(root, directory)}")
    return files


def expanded_entries(root: Path, template: dict[str, Any], files: list[Path]) -> tuple[list[dict[str, Any]], list[dict[str, Any]]]:
    raw_template = template["candidates"][0]
    raw_entries: list[dict[str, Any]] = []
    for path in files:
        raw = dict(raw_template)
        raw["candidate"] = decomp_cycle.root_relative(root, path)
        raw_entries.append(raw)

    # load_manifest performs all normal path/mode validation once for the
    # expanded set; a temporary manifest keeps this front end aligned with the
    # canonical cycle instead of duplicating its normalization rules.
    temp_dir = root / ".mizuchi-tmp"
    temp_dir.mkdir(parents=True, exist_ok=True)
    with tempfile.NamedTemporaryFile("w", suffix=".json", prefix="permutation-", dir=temp_dir, delete=False) as stream:
        json.dump({"candidates": raw_entries}, stream, indent=2)
        temp_path = Path(stream.name)
    try:
        entries = decomp_cycle.load_manifest(root, temp_path)
    finally:
        temp_path.unlink(missing_ok=True)
    return entries, raw_entries


def screen(root: Path, manifest_path: Path, candidates_dir: Path, output: Path | None,
           *, record: bool, require_exact: bool) -> int:
    template = read_manifest(manifest_path)
    files = candidate_files(root, candidates_dir)
    entries, raw_entries = expanded_entries(root, template, files)
    isolation = decomp_cycle.run_isolation(root, entries, record=record)
    results = isolation["results"]
    exact = [result for result in results if result.get("status") == "exact"]
    receipt = {
        "schema": 1,
        "kind": "permutation_screen",
        "started": isolation["started"],
        "finished": isolation["finished"],
        "branch": decomp_cycle.git_output(root, "branch", "--show-current"),
        "commit": decomp_cycle.git_output(root, "rev-parse", "HEAD"),
        "template_manifest": decomp_cycle.root_relative(root, manifest_path),
        "template_manifest_sha256": decomp_cycle.file_sha256(manifest_path),
        "candidate_directory": decomp_cycle.root_relative(root, candidates_dir),
        "candidate_count": len(entries),
        "candidate_identities": [decomp_cycle.entry_identity(root, entry) for entry in entries],
        "candidates": raw_entries,
        "isolation": isolation,
        "exact_candidates": [result.get("candidate") for result in exact],
        "ok": bool(exact),
    }
    receipt_path = output or root / ".decomp-runs" / f"{now_stamp()}-permutation-{entries[0]['function']}.json"
    receipt_path = receipt_path if receipt_path.is_absolute() else root / receipt_path
    decomp_cycle.write_receipt(root, receipt, receipt_path)
    print(json.dumps({
        "ok": bool(exact),
        "receipt": decomp_cycle.display_path(root, receipt_path),
        "exact_candidates": [result.get("candidate") for result in exact],
        "results": [{"candidate": result.get("candidate"), "status": result.get("status"),
                     "score": result.get("score")} for result in results],
    }, indent=2))
    return 0 if (exact or not require_exact) else 1


def choose_candidate(receipt: dict[str, Any], variant: str | None) -> tuple[dict[str, Any], dict[str, Any]]:
    exact = set(receipt.get("exact_candidates", []))
    if not exact:
        raise decomp_cycle.CycleError("permutation receipt contains no exact candidate")
    chosen = variant or sorted(exact)[0]
    chosen = chosen.replace("\\", "/")
    if chosen not in exact:
        raise decomp_cycle.CycleError(f"variant is not an exact candidate in the receipt: {chosen}")
    result = next(item for item in receipt["isolation"]["results"] if item.get("candidate") == chosen)
    template = next(item for item in receipt["candidates"] if item.get("candidate") == chosen)
    return result, template


def accept(root: Path, receipt_path: Path, variant: str | None, output: Path | None) -> int:
    try:
        receipt = json.loads(receipt_path.read_text())
    except (OSError, json.JSONDecodeError) as exc:
        raise decomp_cycle.CycleError(f"could not read permutation receipt: {exc}") from exc
    if receipt.get("kind") != "permutation_screen":
        raise decomp_cycle.CycleError("accept requires a permutation_screen receipt")
    result, template = choose_candidate(receipt, variant)
    if result.get("status") != "exact":
        raise decomp_cycle.CycleError("refusing to apply a non-exact permutation")

    # Keep the exact apply manifest beside the screen/apply receipts.  A
    # deleted temporary manifest made the receipt's hash unverifiable after
    # the run, which defeats the provenance value of the lifecycle record.
    run_dir = root / ".decomp-runs"
    run_dir.mkdir(parents=True, exist_ok=True)
    manifest = run_dir / f"{receipt_path.stem}-apply-manifest.json"
    manifest.write_text(json.dumps({"candidates": [template]}, indent=2) + "\n")
    command = [sys.executable, "tools/decomp_cycle.py", "apply", "--manifest", str(manifest),
               "--function", str(template["function"])]
    if output:
        command.extend(["--output", str(output)])
    completed = subprocess.run(command, cwd=root, text=True, check=False)
    return completed.returncode


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    sub = parser.add_subparsers(dest="command", required=True)

    screen_parser = sub.add_parser("screen", help="compile all C variants in one isolation pass")
    screen_parser.add_argument("--manifest", type=Path, required=True,
                               help="one-entry cycle manifest used as the variant template")
    screen_parser.add_argument("--candidate-dir", type=Path, required=True)
    screen_parser.add_argument("--output", type=Path)
    screen_parser.add_argument("--no-record", action="store_true")
    screen_parser.add_argument("--require-exact", action="store_true")

    accept_parser = sub.add_parser("accept", help="apply one exact candidate from a screen receipt")
    accept_parser.add_argument("--receipt", type=Path, required=True)
    accept_parser.add_argument("--variant", help="candidate path from exact_candidates; defaults to first exact")
    accept_parser.add_argument("--output", type=Path)

    args = parser.parse_args(argv)
    root = Path(__file__).resolve().parents[1]
    try:
        if args.command == "screen":
            return screen(root, args.manifest.resolve(), args.candidate_dir.resolve(), args.output,
                          record=not args.no_record, require_exact=args.require_exact)
        return accept(root, args.receipt.resolve(), args.variant, args.output)
    except (decomp_cycle.CycleError, OSError, json.JSONDecodeError) as exc:
        print(f"decomp_permute: {exc}", file=sys.stderr)
        return 2


if __name__ == "__main__":
    raise SystemExit(main())
