#!/usr/bin/env python3
"""Audit decomp C for asm escapes and low-level memory access.

The audit has two quality rules for newly accepted decomp functions: ordinary
C with no asm escape, and no opaque offset-heavy byte-pointer stand-in.  A
small amount of raw layout evidence remains valid, as does a named struct
overlay, because partially-known GBA layouts genuinely need an intermediate
representation during struct recovery.
"""

from __future__ import annotations

import argparse
import datetime as dt
import json
import subprocess
from pathlib import Path
from typing import Any

from check_decomp_policy import source_audit


def git(root: Path, *args: str) -> str:
    try:
        return subprocess.check_output(["git", *args], cwd=root, text=True, stderr=subprocess.DEVNULL).strip()
    except (OSError, subprocess.CalledProcessError):
        return ""


def resolve_paths(root: Path, values: list[str], include_all: bool) -> list[Path]:
    paths = [root / value for value in values]
    if include_all:
        paths.extend(sorted((root / "src/decomp").glob("*.c")))
    unique = sorted({path.resolve() for path in paths})
    if not unique:
        raise SystemExit("provide one or more source paths or --all")
    for path in unique:
        if not path.is_file() or path.suffix != ".c":
            raise SystemExit(f"source file not found or not C: {path}")
        if not path.is_relative_to(root / "src/decomp"):
            raise SystemExit(f"audit path must be under src/decomp: {path}")
    return unique


def build_record(root: Path, paths: list[Path]) -> dict[str, Any]:
    files = []
    for path in paths:
        report = source_audit(path.read_text(errors="replace"))
        files.append({"path": path.relative_to(root).as_posix(), **report})
    return {
        "schema": 1,
        "kind": "decomp_source_audit",
        "timestamp": dt.datetime.now(dt.timezone.utc).replace(microsecond=0).isoformat(),
        "branch": git(root, "branch", "--show-current"),
        "commit": git(root, "rev-parse", "HEAD"),
        "files": files,
        "ok": all(file["strict_real_c"] for file in files),
        "layout_ok": all(file["layout_quality_ok"] for file in files),
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("paths", nargs="*", help="src/decomp/*.c files to inspect")
    parser.add_argument("--all", action="store_true", help="audit every standalone decomp C file")
    parser.add_argument("--strict", action="store_true", help="fail if any asm or wrapper is found")
    parser.add_argument(
        "--strict-layout",
        action="store_true",
        help="fail if any source has an opaque offset-heavy byte-pointer layout",
    )
    parser.add_argument("--output", type=Path, help="write JSON receipt to this path")
    args = parser.parse_args()

    root = Path(__file__).resolve().parents[1]
    paths = resolve_paths(root, args.paths, args.all)
    record = build_record(root, paths)
    encoded = json.dumps(record, indent=2) + "\n"
    if args.output:
        output = args.output if args.output.is_absolute() else root / args.output
        output.parent.mkdir(parents=True, exist_ok=True)
        output.write_text(encoded)
        print(json.dumps({"ok": record["ok"], "receipt": output.relative_to(root).as_posix()}))
    else:
        print(encoded, end="")
    failed_asm = args.strict and not record["ok"]
    failed_layout = args.strict_layout and not record["layout_ok"]
    return 0 if not (failed_asm or failed_layout) else 1


if __name__ == "__main__":
    raise SystemExit(main())
