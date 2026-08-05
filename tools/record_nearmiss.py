#!/usr/bin/env python3
"""Record a WarioWare decomp attempt and keep the best near-miss seed.

The score is intentionally project-neutral: lower is closer, and zero means an
exact candidate.  This tool records evidence only; it never edits source,
linker files, or acceptance state.  A candidate should be recorded after an
isolated compile or a failed strict-ROM attempt, and accepted separately by
the normal Docker build workflow.
"""

from __future__ import annotations

import argparse
import datetime as dt
import hashlib
import json
import subprocess
from pathlib import Path


def command(root: Path, *args: str) -> str:
    try:
        return subprocess.check_output([*args], cwd=root, text=True, stderr=subprocess.DEVNULL).strip()
    except (OSError, subprocess.CalledProcessError):
        return ""


def sha1(path: Path) -> str | None:
    if not path.is_file():
        return None
    digest = hashlib.sha1()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def load_diff(path: Path | None) -> object:
    if path is None:
        return []
    return json.loads(path.read_text())


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("function")
    parser.add_argument("file", help="target assembly or decomp source path")
    parser.add_argument("score", type=float, help="near-miss score; lower is closer")
    parser.add_argument("candidate", type=Path, help="full candidate C file")
    parser.add_argument("--reason", default="near_miss")
    parser.add_argument("--score-kind", default="generic",
                        help="unit used by score (for example rom_bytes or isolated_objdiff_gap)")
    parser.add_argument("--diff-json", type=Path, help="JSON diff evidence")
    parser.add_argument("--command", dest="build_command", default="")
    parser.add_argument("--rom", type=Path, default=Path("build/wariowareinc.gba"))
    parser.add_argument("--baseline-rom", type=Path, default=Path("baserom.gba"))
    parser.add_argument("--root", type=Path, default=Path(__file__).resolve().parents[1])
    args = parser.parse_args()

    root = args.root.resolve()
    candidate = args.candidate if args.candidate.is_absolute() else root / args.candidate
    if not candidate.is_file():
        parser.error(f"candidate file not found: {candidate}")
    try:
        diff = load_diff(args.diff_json)
    except (OSError, json.JSONDecodeError) as exc:
        parser.error(f"invalid diff JSON: {exc}")

    now = dt.datetime.now(dt.timezone.utc).replace(microsecond=0).isoformat()
    record = {
        "func": args.function,
        "file": args.file,
        "score": args.score,
        "score_kind": args.score_kind,
        "reason": args.reason,
        "c": candidate.read_text(),
        "evidence": {
            "timestamp": now,
            "branch": command(root, "git", "branch", "--show-current"),
            "commit": command(root, "git", "rev-parse", "HEAD"),
            "candidate": str(candidate.relative_to(root)) if candidate.is_relative_to(root) else str(candidate),
            "candidate_sha1": sha1(candidate),
            "rom_sha1": sha1((root / args.rom) if not args.rom.is_absolute() else args.rom),
            "baseline_rom_sha1": sha1((root / args.baseline_rom) if not args.baseline_rom.is_absolute() else args.baseline_rom),
            "command": args.build_command,
            "diff": diff,
        },
    }

    near_dir = root / ".nearmiss"
    near_dir.mkdir(exist_ok=True)
    json_path = near_dir / f"{args.function}.json"
    full_path = near_dir / f"{args.function}.full.c"
    old_score = None
    old_score_kind = None
    if json_path.is_file():
        try:
            old_record = json.loads(json_path.read_text())
            old_score = float(old_record.get("score"))
            old_score_kind = old_record.get("score_kind")
            if old_score_kind is None:
                old_score_kind = "rom_bytes" if old_record.get("reason") == "rom_mismatch" else "generic"
        except (OSError, TypeError, ValueError, json.JSONDecodeError):
            pass

    attempts = root / "tools" / "attempts.tsv"
    if not attempts.exists():
        attempts.write_text("timestamp\tfunc\tfile\tscore\treason\tcommit\tcandidate\tscore_kind\n")
    else:
        header = attempts.read_text().splitlines()
        if header and "score_kind" not in header[0].split("\t"):
            attempts.write_text(header[0] + "\tscore_kind\n" + "\n".join(header[1:]) + ("\n" if len(header) > 1 else ""))
    with attempts.open("a") as stream:
        stream.write("\t".join((now, args.function, args.file, str(args.score), args.reason,
                                 record["evidence"]["commit"], record["evidence"]["candidate"],
                                 args.score_kind)) + "\n")

    if old_score is not None and old_score_kind == args.score_kind and old_score <= args.score:
        print(json.dumps({"recorded": True, "kept_best": False, "existing_score": old_score,
                          "existing_score_kind": old_score_kind}))
        return 0

    if old_score is not None and old_score_kind != args.score_kind:
        print(json.dumps({"recorded": True, "kept_best": False, "existing_score": old_score,
                          "existing_score_kind": old_score_kind, "score_kind_mismatch": True}))
        return 0

    json_path.write_text(json.dumps(record, indent=2) + "\n")
    full_path.write_text(record["c"])
    print(json.dumps({"recorded": True, "kept_best": True, "score": args.score,
                      "score_kind": args.score_kind,
                      "json": str(json_path.relative_to(root)),
                      "full_c": str(full_path.relative_to(root))}))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
