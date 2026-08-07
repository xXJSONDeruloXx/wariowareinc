#!/usr/bin/env python3
"""Check repository policy for decomp commits and hook verification.

This is intentionally independent of the Pi extension.  It gives Git hooks a
small, stable check for the rules that must hold even when no agent runtime is
present: new ``src/decomp`` files may not smuggle in instruction asm, register
pins, or asm barriers, and the ROM verification gate may not be weakened in a
normal commit.
"""

from __future__ import annotations

import argparse
import re
import subprocess
import sys
from pathlib import Path
from typing import Any


ROM_PREFIXES = ("src/", "asm/", "include/", "data/", "audio/", "graphics/")
ROM_FILES = {
    "Makefile",
    "wariowareinc.ld",
    "wariowareinc_modern.ld",
    "undefined_syms.ld",
    "tools/agbcc-swi.patch",
}
GATE_RE = re.compile(r"BASEROM_SHA1|TARGET_SHA1|wariowareinc\.gba:\s+OK|NONMATCHING")
NAKED_RE = re.compile(
    r"__attribute__\s*\(\s*\(\s*naked\s*\)\s*\)|thumb_func_start|"
    r"#\s*include\s*[\"<][^\">]*asm/[^\">]*\.s[\">]|"
    r"#\s*pragma\s+GLOBAL_ASM\b"
)
ASM_HEAD_RE = re.compile(
    r"(?<![A-Za-z0-9_])(?:__asm__|asm)\b"
    r"(?:\s+(?:__volatile__|volatile))?\s*\(",
    re.MULTILINE,
)
ASM_LITERAL_RE = re.compile(r'"(?:\\.|[^"\\])*"')
REGISTER_PIN_LITERAL_RE = re.compile(r'^"r(?:1[0-5]|[0-9])"$', re.IGNORECASE)
RAW_POINTER_RE = re.compile(
    r"(?:\*\s*\(\s*(?:const\s+)?[us](?:8|16|32|64)\s*\*|"
    r"\(\s*(?:const\s+)?(?:u8|u16|u32|s8|s16|s32|void)\s*\*\s*\))"
)
POINTER_OFFSET_RE = re.compile(r"\(\s*(?:const\s+)?(?:u8|u16|u32|s8|s16|s32)\s*\*\s*\)[^\n;]*\+\s*(?:0x[0-9A-Fa-f]+|[0-9]+)")
NUMERIC_OFFSET_RE = re.compile(r"(?:\+|\[)\s*(0x[0-9A-Fa-f]+|[0-9]+)")
NUMERIC_ASSIGN_OFFSET_RE = re.compile(
    r"\b(?:offset|base|scene|ptr|data)\s*(?:\+=|=)\s*(0x[0-9A-Fa-f]+|[0-9]+)"
)
POINTER_OFFSET_CONTEXT_RE = re.compile(r"\b(?:offset|base|store|scene|ptr|data)\b")


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


def _matching_paren(text: str, open_index: int) -> int | None:
    """Find an asm expression's closing parenthesis without parsing C."""

    depth = 0
    index = open_index
    in_string = False
    escaped = False
    while index < len(text):
        char = text[index]
        if in_string:
            if escaped:
                escaped = False
            elif char == "\\":
                escaped = True
            elif char == '"':
                in_string = False
        elif char == '"':
            in_string = True
        elif char == "(":
            depth += 1
        elif char == ")":
            depth -= 1
            if depth == 0:
                return index
        index += 1
    return None


def _first_asm_template(body: str) -> str:
    """Return the first asm template, excluding extended-asm constraints."""

    match = ASM_LITERAL_RE.search(body)
    if not match:
        return ""
    raw = match.group(0)[1:-1]
    return decode_literal_group(raw)


def _asm_prefix_is_register_pin(text: str, start: int, template: str) -> bool:
    if not REGISTER_PIN_LITERAL_RE.fullmatch(f'"{template}"'):
        return False
    boundary = max(text.rfind(";", 0, start), text.rfind("{", 0, start), text.rfind("}", 0, start))
    declaration = text[boundary + 1:start]
    return bool(re.search(r"\bregister\b", declaration))


def asm_findings(text: str) -> list[dict[str, Any]]:
    """Classify GNU asm expressions as instruction text, pins, or barriers.

    This deliberately uses a small balanced-parenthesis scanner instead of a
    single regex.  That catches extended asm forms such as ``asm("..." :
    "+r"(value))`` while keeping an empty output constraint distinct from an
    instruction template.
    """

    findings: list[dict[str, Any]] = []
    for match in ASM_HEAD_RE.finditer(text):
        close = _matching_paren(text, match.end() - 1)
        if close is None:
            continue
        body = text[match.end():close]
        template = _first_asm_template(body)
        is_pin = _asm_prefix_is_register_pin(text, match.start(), template)
        if is_pin:
            kind = "register_pin"
        elif template.strip():
            kind = "instruction"
        else:
            kind = "barrier"
        line = text.count("\n", 0, match.start()) + 1
        line_start = text.rfind("\n", 0, match.start()) + 1
        line_end = text.find("\n", match.start())
        if line_end < 0:
            line_end = len(text)
        findings.append({
            "kind": kind,
            "line": line,
            "volatile": bool(re.search(r"\b(?:__volatile__|volatile)\b", match.group(0))),
            "template": template,
            "text": text[line_start:line_end].strip(),
        })
    return findings


def has_nonempty_asm(text: str) -> bool:
    """Return whether an asm expression contains instruction template text."""

    return bool(NAKED_RE.search(text)) or any(
        finding["kind"] == "instruction" for finding in asm_findings(text)
    )


def has_asm_pin(text: str) -> bool:
    return any(finding["kind"] == "register_pin" for finding in asm_findings(text))


def has_inline_asm(text: str) -> bool:
    """Return whether source contains any inline asm, including empty barriers."""

    return bool(NAKED_RE.search(text)) or bool(asm_findings(text))


def source_audit(text: str) -> dict[str, Any]:
    """Return an auditable quality summary without judging valid low-level C.

    Raw pointer casts and numeric offsets are reported, not rejected: GBA
    decompilation often has to express packed/partially-known layouts this way.
    The hard admission rule is that the function must not use asm or include an
    original asm wrapper; the byte-match and full-ROM gates remain the semantic
    proof.
    """

    findings = asm_findings(text)
    pointer_lines: list[dict[str, Any]] = []
    numeric_offsets: list[dict[str, Any]] = []
    for line_number, line in enumerate(text.splitlines(), 1):
        if RAW_POINTER_RE.search(line) or POINTER_OFFSET_RE.search(line):
            pointer_lines.append({"line": line_number, "text": line.strip()})
        values = NUMERIC_OFFSET_RE.findall(line) + NUMERIC_ASSIGN_OFFSET_RE.findall(line)
        if values and (
            RAW_POINTER_RE.search(line)
            or POINTER_OFFSET_RE.search(line)
            or (POINTER_OFFSET_CONTEXT_RE.search(line) and ("offset" in line or "+=" in line))
        ):
            numeric_offsets.append({"line": line_number, "values": values, "text": line.strip()})

    functions = re.findall(r"\b(func_[A-Za-z0-9_]+|asm_[A-Za-z0-9_]+)\s*\([^;{}]*\)\s*\{", text)
    return {
        "function_count": len(functions),
        "functions": functions,
        "asm": {
            "instruction": [item for item in findings if item["kind"] == "instruction"],
            "register_pin": [item for item in findings if item["kind"] == "register_pin"],
            "barrier": [item for item in findings if item["kind"] == "barrier"],
        },
        "raw_pointer_accesses": pointer_lines,
        "numeric_pointer_offsets": numeric_offsets,
        "strict_real_c": not bool(NAKED_RE.search(text) or findings),
    }


def policy_violations(root: Path, files: list[str], *, staged: bool) -> list[str]:
    violations: list[str] = []
    for rel in files:
        if rel.startswith("src/decomp/") and rel.endswith(".c"):
            path = root / rel
            if not path.is_file():
                continue
            text = path.read_text(errors="replace")
            if has_nonempty_asm(text):
                violations.append(f"{rel}: src/decomp content contains naked/original/instruction-bearing asm")
            elif has_inline_asm(text):
                violations.append(f"{rel}: src/decomp content contains an asm barrier or register pin")

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
