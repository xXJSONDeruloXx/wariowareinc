#!/usr/bin/env python3
"""Check repository policy for decomp commits and hook verification.

This is intentionally independent of the Pi extension.  It gives Git hooks a
small, stable check for the two rules that must hold even when no agent runtime
is present: new ``src/decomp`` files may not smuggle in instruction asm, and
the ROM verification gate may not be weakened in a normal commit.
"""

from __future__ import annotations

import argparse
import re
import subprocess
import sys
from pathlib import Path


ROM_PREFIXES = ("src/", "asm/", "include/", "data/", "audio/", "graphics/")
ROM_FILES = {
    "Makefile",
    "wariowareinc.ld",
    "wariowareinc_modern.ld",
    "undefined_syms.ld",
    "tools/agbcc-swi.patch",
}
GATE_RE = re.compile(r"BASEROM_SHA1|TARGET_SHA1|wariowareinc\.gba:\s+OK|NONMATCHING")
NAKED_RE = re.compile(r"__attribute__\s*\(\s*\(\s*naked\s*\)\s*\)|thumb_func_start|#\s*include\s*[\"<][^\">]*asm/[^\">]*\.s[\">]")
VOLATILE_ASM_RE = re.compile(r"(?:__asm__|asm)\s+volatile\s*\(\s*((?:\"(?:\\.|[^\"\\])*\"\s*)+)\)", re.MULTILINE | re.DOTALL)
STATEMENT_ASM_RE = re.compile(r"(?:^|[;{}]\s*)(?:__asm__|asm)\s*\(\s*((?:\"(?:\\.|[^\"\\])*\"\s*)+)\)\s*;", re.MULTILINE | re.DOTALL)


def git(root: Path, args: list[str]) -> str:
    completed = subprocess.run(["git", *args], cwd=root, text=True, stdout=subprocess.PIPE,
                               stderr=subprocess.PIPE, check=True)
    return completed.stdout


def changed_files(root: Path, mode: str, revision_range: tuple[str, str] | None = None) -> list[str]:
    if mode == "staged":
        output = git(root, ["diff", "--cached", "--name-only", "--diff-filter=ACMRTUXB"])
    elif mode == "worktree":
        output = git(root, ["diff", "--name-only", "--diff-filter=ACMRTUXB"])
        output += git(root, ["diff", "--cached", "--name-only", "--diff-filter=ACMRTUXB"])
    elif mode == "range":
        assert revision_range is not None
        output = git(root, ["diff", "--name-only", "--diff-filter=ACMRTUXB", revision_range[0], revision_range[1]])
    else:
        raise ValueError(mode)
    return sorted({line.strip().replace("\\", "/") for line in output.splitlines() if line.strip()})


def is_rom_affecting(path: str) -> bool:
    return path in ROM_FILES or path.startswith(ROM_PREFIXES)


def decode_literal_group(group: str) -> str:
    return re.sub(r"\\(?:\r\n|\n|\r)", "", group)


def has_nonempty_asm(text: str) -> bool:
    if NAKED_RE.search(text):
        return True
    for pattern in (VOLATILE_ASM_RE, STATEMENT_ASM_RE):
        for match in pattern.finditer(text):
            if decode_literal_group(match.group(1)).strip():
                return True
    return False


def policy_violations(root: Path, files: list[str], *, staged: bool) -> list[str]:
    violations: list[str] = []
    for rel in files:
        if rel.startswith("src/decomp/") and rel.endswith(".c"):
            path = root / rel
            if path.is_file() and has_nonempty_asm(path.read_text(errors="replace")):
                violations.append(f"{rel}: new src/decomp content contains naked/original/instruction-bearing asm")

    if staged:
        patch = git(root, ["diff", "--cached", "--", "Makefile", "wariowareinc.ld", "wariowareinc_modern.ld", "undefined_syms.ld"])
        for line in patch.splitlines():
            if not line.startswith(("+", "-")) or line.startswith(("+++", "---")):
                continue
            if GATE_RE.search(line[1:]):
                violations.append("staged verification-gate edit: " + line)
    return violations


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    source = parser.add_mutually_exclusive_group(required=True)
    source.add_argument("--staged", action="store_true")
    source.add_argument("--worktree", action="store_true")
    source.add_argument("--range", nargs=2, metavar=("BASE", "HEAD"))
    parser.add_argument("--check", action="store_true", help="fail on policy violations")
    parser.add_argument("--needs-verification", action="store_true", help="exit 0 when changed paths affect ROM output")
    parser.add_argument("--dirty-rom-paths", action="store_true", help="fail when unstaged ROM-affecting paths exist")
    args = parser.parse_args(argv)
    root = Path(__file__).resolve().parents[1]
    mode = "staged" if args.staged else "worktree" if args.worktree else "range"
    files = changed_files(root, mode, tuple(args.range) if args.range else None)

    if args.needs_verification:
        return 0 if any(is_rom_affecting(path) for path in files) else 1

    if args.dirty_rom_paths:
        dirty = [path for path in files if is_rom_affecting(path)]
        if dirty:
            print("unstaged ROM-affecting paths are present:", file=sys.stderr)
            print("\n".join(f"- {path}" for path in dirty), file=sys.stderr)
            return 1
        return 0

    violations = policy_violations(root, files, staged=args.staged)
    if violations:
        print("decomp policy blocked:", file=sys.stderr)
        print("\n".join(f"- {violation}" for violation in violations), file=sys.stderr)
        return 1
    if args.check:
        print(f"decomp policy OK ({len(files)} changed path(s))")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
