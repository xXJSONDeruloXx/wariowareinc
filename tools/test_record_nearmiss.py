#!/usr/bin/env python3
import json
import subprocess
import tempfile
import unittest
from pathlib import Path


class RecordNearMissTests(unittest.TestCase):
    def test_keep_best_and_record_provenance(self) -> None:
        root = Path(__file__).resolve().parents[1]
        with tempfile.TemporaryDirectory() as tmp:
            work = Path(tmp)
            (work / "tools").mkdir()
            (work / ".nearmiss").mkdir()
            candidate = work / "candidate.c"
            candidate.write_text("void func(void) {}\n")
            diff = work / "diff.json"
            diff.write_text(json.dumps({"first_diff": "0x10"}))
            cmd = ["python3", str(root / "tools/record_nearmiss.py"), "func_test", "asm/test.s", "4",
                   str(candidate), "--root", str(work), "--diff-json", str(diff)]
            first = json.loads(subprocess.check_output(cmd, text=True))
            self.assertTrue(first["kept_best"])
            candidate.write_text("void func(void) { return; }\n")
            cmd[cmd.index("4")] = "8"
            second = json.loads(subprocess.check_output(cmd, text=True))
            self.assertFalse(second["kept_best"])
            self.assertIn("void func(void) {}", (work / ".nearmiss/func_test.full.c").read_text())
            history = json.loads((work / ".nearmiss/func_test.json").read_text())["history"]
            self.assertEqual(len(history), 2)
            self.assertEqual(history[-1]["score"], 8.0)
            self.assertEqual(len((work / "tools/attempts.tsv").read_text().splitlines()), 3)


if __name__ == "__main__":
    unittest.main()
