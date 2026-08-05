#!/usr/bin/env python3
import json
import tempfile
import unittest
from pathlib import Path
import sys

sys.path.insert(0, str(Path(__file__).resolve().parent))
import decomp_permute


class DecompPermuteTests(unittest.TestCase):
    def test_expanded_entries_preserve_template_and_replace_candidate(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / ".mizuchi-tmp").mkdir()
            (root / "variants").mkdir()
            (root / "variants/a.c").write_text("void func_08002038(void) {}\n")
            (root / "variants/b.c").write_text("void func_08002038(void) { return; }\n")
            target = root / "target.s"
            target.write_text(".text\n")
            template = {
                "candidates": [{
                    "function": "func_08002038",
                    "candidate": "variants/a.c",
                    "target": "target.s",
                    "file": "target.s",
                }]
            }
            entries, raw = decomp_permute.expanded_entries(
                root, template, sorted((root / "variants").glob("*.c"))
            )
            self.assertEqual([item["candidate"] for item in raw], ["variants/a.c", "variants/b.c"])
            self.assertEqual([item["candidate_rel"] for item in entries], ["variants/a.c", "variants/b.c"])
            self.assertEqual(entries[0]["function"], "func_08002038")

    def test_choose_candidate_rejects_non_exact_variant(self) -> None:
        receipt = {
            "kind": "permutation_screen",
            "exact_candidates": ["variants/exact.c"],
            "candidates": [{"function": "func_08002038", "candidate": "variants/exact.c"}],
            "isolation": {"results": [{"candidate": "variants/exact.c", "status": "exact"}]},
        }
        result, entry = decomp_permute.choose_candidate(receipt, None)
        self.assertEqual(result["status"], "exact")
        self.assertEqual(entry["candidate"], "variants/exact.c")
        with self.assertRaises(decomp_permute.decomp_cycle.CycleError):
            decomp_permute.choose_candidate(receipt, "variants/near.c")

    def test_choose_candidate_prefers_immutable_snapshot(self) -> None:
        receipt = {
            "kind": "permutation_screen",
            "exact_candidates": [".mizuchi-tmp/variants/exact.c"],
            "candidates": [{
                "function": "func_08002038",
                "candidate": ".mizuchi-tmp/variants/exact.c",
            }],
            "candidate_snapshots": [{
                "candidate": ".mizuchi-tmp/variants/exact.c",
                "snapshot": ".decomp-runs/run-candidates/000-exact.c",
            }],
            "isolation": {"results": [{
                "candidate": ".mizuchi-tmp/variants/exact.c",
                "status": "exact",
            }]},
        }
        _, entry = decomp_permute.choose_candidate(receipt, None)
        self.assertEqual(entry["candidate"], ".decomp-runs/run-candidates/000-exact.c")


if __name__ == "__main__":
    unittest.main()
