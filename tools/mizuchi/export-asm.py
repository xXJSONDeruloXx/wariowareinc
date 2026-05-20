#!/usr/bin/env python3
from __future__ import annotations

import shutil
from pathlib import Path


def normalize_inline_asm(text: str) -> str:
    lines = text.splitlines()
    if not lines:
        return text

    if lines[0].startswith('asm("'):
        lines[0] = lines[0][5:]
    if lines[-1].endswith('");'):
        lines[-1] = lines[-1][:-3]

    out: list[str] = []
    for line in lines:
        if line.endswith(" \\n\\"):
            line = line[:-4]
        elif line.endswith("\\n\\"):
            line = line[:-3]
        out.append(line.replace(r'\"', '"').rstrip())

    return '\n'.join(out).strip() + '\n'


def main() -> None:
    repo_root = Path(__file__).resolve().parents[2]
    src_root = repo_root / 'asm'
    dst_root = repo_root / '.mizuchi-asm' / 'asm'

    if dst_root.parent.exists():
        shutil.rmtree(dst_root.parent)
    dst_root.mkdir(parents=True, exist_ok=True)

    total = 0
    normalized = 0

    for src_path in src_root.rglob('*.s'):
        rel = src_path.relative_to(src_root)
        dst_path = dst_root / rel
        dst_path.parent.mkdir(parents=True, exist_ok=True)

        text = src_path.read_text()
        if text.lstrip().startswith('asm("'):
            text = normalize_inline_asm(text)
            normalized += 1

        dst_path.write_text(text)
        total += 1

    print(f'Exported {total} asm files to {dst_root}')
    print(f'Normalized inline asm stubs: {normalized}')


if __name__ == '__main__':
    main()
