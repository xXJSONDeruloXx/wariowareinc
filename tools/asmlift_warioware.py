#!/usr/bin/env python3
"""Run m2c and asmlift against a WarioWare Splat assembly function.

The repository's assembly is Splat-shaped (address comments, ``glabel``,
uppercase Thumb mnemonics, and sometimes C-string embedded asm). asmlift's
agbcc frontend wants ordinary GNU assembly instead, so this script extracts a
function-sized stream and normalizes only syntax tokens while preserving game
symbol spelling. m2c remains pointed at the original source whenever it is
already a normal assembly file.
"""

from __future__ import annotations

import argparse
import os
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
ASM_ROOT = ROOT / "asm"
ADDRESS_RE = re.compile(r"(?:func_)?(080[0-9a-f]{5})", re.IGNORECASE)
LABEL_RE = re.compile(r"^\s*([A-Za-z_.$][A-Za-z0-9_.$]*):\s*$")
DIRECTIVE_FUNCTION_RE = re.compile(
    r"^\s*(?:glabel|thumb_func_start|\.global|\.globl)\s+"
    r"([A-Za-z_.$][A-Za-z0-9_.$]*)\s*$"
)
ADDRESS_COMMENT_RE = re.compile(r"/\*\s*[0-9A-Fa-f]{8}\s*\*/(.*)")
REGISTER_RE = re.compile(
    r"(?<![A-Za-z0-9_])"
    r"(?:R(?:1[0-5]|[0-9])|SP|LR|PC|IP|SL|SB)"
    r"(?![A-Za-z0-9_])",
    re.IGNORECASE,
)


class AdapterError(RuntimeError):
    pass


def function_address(value: str) -> str | None:
    match = ADDRESS_RE.search(value)
    return match.group(1).lower() if match else None


def canonical_name(value: str, address: str | None) -> str:
    if value.startswith("func_"):
        return value
    if address:
        return f"func_{address.upper()}"
    return value


def resolve_asm(value: str) -> Path:
    address = function_address(value)
    if address:
        matches = [
            path
            for path in ASM_ROOT.rglob("*.s")
            if path.name.lower() == f"asm_{address}.s"
            and "converted" not in path.parts
        ]
        if matches:
            # Prefer a root standalone TU when it exists; otherwise retain the
            # module path for an included stub.
            matches.sort(key=lambda path: (len(path.parts), str(path)))
            return matches[0]

    symbol_re = re.compile(rf"\b{re.escape(value)}\b", re.IGNORECASE)
    matches = [
        path
        for path in ASM_ROOT.rglob("*.s")
        if "converted" not in path.parts and symbol_re.search(path.read_text())
    ]
    if matches:
        matches.sort(key=lambda path: (len(path.parts), str(path)))
        return matches[0]
    raise AdapterError(f"could not find an unconverted asm source for {value}")


def strip_c_string_suffix(fragment: str) -> str:
    """Remove the literal ``\\n\\`` continuation from embedded asm lines."""

    fragment = fragment.strip()
    if fragment.endswith("\\"):
        fragment = fragment[:-1].rstrip()
    if fragment.endswith("\\n"):
        fragment = fragment[:-2].rstrip()
    return fragment


def normalize_instruction(fragment: str) -> str:
    fragment = fragment.replace("0X", "0x")
    match = re.match(r"^(\s*)([A-Za-z][A-Za-z0-9_.]*)(\b.*)$", fragment)
    if match:
        fragment = f"{match.group(1)}{match.group(2).lower()}{match.group(3)}"
    return REGISTER_RE.sub(lambda match: match.group(0).lower(), fragment)


def collect_lines(
    source: Path,
    function: str,
    *,
    lowercase: bool,
    keep_data: bool,
) -> tuple[list[str], str, bool]:
    """Extract asm lines and return (lines, discovered_function, omitted_data)."""

    lines: list[str] = []
    discovered_function: str | None = None
    omitted_data = False

    for raw_line in source.read_text().splitlines():
        # Included stubs are C string literals.  Their standalone labels and
        # function directives therefore carry the same ``\\n\\`` suffix as
        # instruction comments (for example ``_0800BFB6: \\n\\``).  Strip
        # that transport spelling before matching labels; otherwise m2c sees
        # a branch to an undefined target and rejects an otherwise valid
        # function skeleton.
        logical_line = strip_c_string_suffix(raw_line)

        directive = DIRECTIVE_FUNCTION_RE.match(logical_line)
        if directive and not directive.group(1).startswith("."):
            if discovered_function is None:
                discovered_function = directive.group(1)
            continue

        label = LABEL_RE.match(logical_line)
        if label:
            lines.append(f"{label.group(1)}:")
            continue

        comment = ADDRESS_COMMENT_RE.search(raw_line)
        if not comment:
            continue
        fragment = strip_c_string_suffix(comment.group(1))
        if not fragment or fragment.startswith("@"):
            continue

        first_token = fragment.split(None, 1)[0].lower()
        if first_token in {
            ".section",
            ".text",
            ".thumb",
            ".syntax",
            ".include",
            ".global",
            ".globl",
            ".type",
            ".size",
            ".end",
            ".ltorg",
        }:
            continue
        if first_token in {".word", ".short", ".byte", ".hword", ".4byte"}:
            if keep_data:
                lines.append(fragment)
            else:
                omitted_data = True
            continue
        if first_token in {".balign", ".align", ".p2align", ".space", ".fill"}:
            omitted_data = True
            continue

        lines.append(normalize_instruction(fragment) if lowercase else fragment)

    if discovered_function is None:
        discovered_function = function
    if not lines:
        raise AdapterError(f"no address-commented instructions found in {source}")
    return lines, discovered_function, omitted_data


def render_assembly(
    source: Path,
    function: str,
    *,
    lowercase: bool,
    keep_data: bool,
) -> tuple[str, bool]:
    lines, _discovered, omitted_data = collect_lines(
        source, function, lowercase=lowercase, keep_data=keep_data
    )
    # The requested symbol is authoritative when the source has several
    # directives or uses a different case spelling.
    lines = [line for line in lines if line != f"{function}:"]
    rendered = [
        ".text",
        ".thumb",
        ".syntax unified",
        f".global {function}",
        f".type {function}, %function",
        ".thumb_func",
        f"{function}:",
        *lines,
        f".size {function}, .-{function}",
    ]
    return "\n".join(rendered) + "\n", omitted_data


def mizuchi_python() -> Path:
    configured = os.environ.get("MIZUCHI_ROOT") or os.environ.get("PI_MIZUCHI_ROOT")
    mizuchi_root = Path(configured) if configured else ROOT.parent / "mizuchi"
    candidates = [
        mizuchi_root / "vendor/m2c/.venv/bin/python3",
        mizuchi_root / "vendor/m2c/.venv/bin/python",
    ]
    for candidate in candidates:
        if candidate.exists():
            return candidate
    raise AdapterError(
        "m2c venv not found; set MIZUCHI_ROOT or install "
        f"the expected environment under {mizuchi_root}"
    )


def run_m2c(source: Path, function: str, temp_dir: Path) -> int:
    m2c_python = mizuchi_python()
    m2c_root = m2c_python.parent.parent.parent
    m2c_script = m2c_root / "m2c.py"
    if not m2c_script.exists():
        raise AdapterError(f"m2c script not found at {m2c_script}")

    m2c_source = source
    # m2c can consume the repository's normal .s files directly. Embedded
    # asm("...") files need the same extraction used for asmlift first.
    source_text = source.read_text()
    if "asm(" in source_text or "asm (" in source_text:
        raw_text, _ = render_assembly(source, function, lowercase=False, keep_data=True)
        temporary_source = temp_dir / f"{function}.m2c.s"
        temporary_source.write_text(raw_text)
        m2c_source = temporary_source

    result = subprocess.run(
        [str(m2c_python), str(m2c_script), "-t", "gba", str(m2c_source)],
        cwd=ROOT,
        text=True,
        capture_output=True,
    )
    print("=== m2c ===")
    if result.stdout:
        print(result.stdout, end="" if result.stdout.endswith("\n") else "\n")
    if result.stderr:
        print("--- m2c diagnostics ---", file=sys.stderr)
        print(
            result.stderr,
            file=sys.stderr,
            end="" if result.stderr.endswith("\n") else "\n",
        )
    return result.returncode


def write_asmlift_config(path: Path) -> None:
    compiler = (ROOT / "tools/asmlift-compile.sh").resolve()
    path.write_text(
        "platform: gba\n"
        "tools:\n"
        "  asmlift:\n"
        "    target: agbcc\n"
        f'    compiler: "{compiler} {{{{inputPath}}}} {{{{outputPath}}}} {{{{symbol}}}}"\n'
    )


def run_asmlift(
    source: Path,
    function: str,
    *,
    temp_dir: Path,
    target_object: Path | None,
    headers: str,
    keep_data: bool,
    strict: bool,
) -> int:
    asmlift = shutil.which("asmlift")
    if not asmlift:
        raise AdapterError("asmlift is not on PATH")

    normalized, omitted_data = render_assembly(
        source, function, lowercase=True, keep_data=keep_data
    )
    normalized_path = temp_dir / f"{function}.asmlift.s"
    normalized_path.write_text(normalized)
    config_path = temp_dir / "decomp.yaml"
    write_asmlift_config(config_path)

    command = [
        asmlift,
        "--target",
        "agbcc",
        "--name",
        function,
        "--config",
        str(config_path),
    ]
    if strict:
        command.append("--strict")
    if target_object is not None:
        command.extend(["--score-against", str(target_object)])
    command.append(str(normalized_path))

    environment = os.environ.copy()
    environment["ASMLIFT_HEADERS"] = headers
    result = subprocess.run(
        command,
        cwd=ROOT,
        text=True,
        capture_output=True,
        env=environment,
    )
    print("=== asmlift ===")
    if omitted_data:
        print(
            "[adapter] literal/alignment data was omitted from normalized input",
            file=sys.stderr,
        )
    if result.stdout:
        print(result.stdout, end="" if result.stdout.endswith("\n") else "\n")
    if result.stderr:
        print("--- asmlift diagnostics ---", file=sys.stderr)
        print(
            result.stderr,
            file=sys.stderr,
            end="" if result.stderr.endswith("\n") else "\n",
        )
    return result.returncode


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("function", help="function symbol or 8-digit ROM address")
    parser.add_argument("--no-m2c", action="store_true", help="skip the m2c skeleton")
    parser.add_argument("--no-asmlift", action="store_true", help="skip the asmlift lift")
    parser.add_argument(
        "--score",
        action="store_true",
        help="score the asmlift candidate against build/asm/asm_<address>.s.o",
    )
    parser.add_argument(
        "--headers",
        default="global.h",
        help="comma-separated project headers for asmlift scoring (default: global.h)",
    )
    parser.add_argument(
        "--keep-data",
        action="store_true",
        help="retain .word/.short data in the normalized asmlift input",
    )
    parser.add_argument("--strict", action="store_true", help="pass --strict to asmlift")
    parser.add_argument(
        "--normalized-output",
        type=Path,
        help="copy the normalized asmlift assembly to this path",
    )
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    address = function_address(args.function)
    source = resolve_asm(args.function)
    # Prefer the symbol spelled in the source's glabel/thumb_func_start so
    # objdiff receives the exact case-sensitive ELF symbol.
    _lines, discovered, _omitted = collect_lines(
        source, args.function, lowercase=False, keep_data=True
    )
    function = discovered if discovered else canonical_name(args.function, address)
    target_object: Path | None = None
    if args.score:
        if not address:
            raise AdapterError("--score needs a function name containing an 080xxxxx address")
        target_object = ROOT / "build/asm" / f"asm_{address}.s.o"
        if not target_object.exists():
            raise AdapterError(
                f"target object not found; run the baseline build first: {target_object}"
            )

    print(f"source: {source.relative_to(ROOT)}")
    print(f"function: {function}")
    statuses: list[int] = []
    with tempfile.TemporaryDirectory(prefix="warioware-asmlift-") as temporary:
        temp_dir = Path(temporary)
        if not args.no_m2c:
            statuses.append(run_m2c(source, function, temp_dir))
        normalized, _ = render_assembly(
            source, function, lowercase=True, keep_data=args.keep_data
        )
        if args.normalized_output:
            args.normalized_output.parent.mkdir(parents=True, exist_ok=True)
            args.normalized_output.write_text(normalized)
            print(f"normalized: {args.normalized_output}")
        if not args.no_asmlift:
            statuses.append(
                run_asmlift(
                    source,
                    function,
                    temp_dir=temp_dir,
                    target_object=target_object,
                    headers=args.headers,
                    keep_data=args.keep_data,
                    strict=args.strict,
                )
            )

    # A non-zero asmlift score is useful information, not an adapter failure:
    # callers can still inspect the generated lift and the m2c skeleton. The
    # no-tool mode is also valid when the caller only wants normalized asm.
    del statuses
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except AdapterError as error:
        print(f"asmlift adapter: {error}", file=sys.stderr)
        raise SystemExit(2)
