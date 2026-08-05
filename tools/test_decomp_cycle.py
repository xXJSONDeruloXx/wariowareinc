#!/usr/bin/env python3
import json
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import decomp_cycle


class DecompCycleTests(unittest.TestCase):
    def test_manifest_normalizes_existing_target_object(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            candidate = root / "candidate.c"
            candidate.write_text("void func_08002038(void) {}\n")
            target = root / "build/asm/asm_08002038.s.o"
            target.parent.mkdir(parents=True)
            target.write_bytes(b"target")
            manifest = root / "manifest.json"
            manifest.write_text(json.dumps({"candidates": [{
                "function": "func_08002038",
                "candidate": "candidate.c",
                "target_object": "build/asm/asm_08002038.s.o",
            }]}))

            entry = decomp_cycle.load_manifest(root, manifest)[0]

            self.assertEqual(entry["candidate_rel"], "candidate.c")
            self.assertEqual(entry["target_object_rel"], "build/asm/asm_08002038.s.o")
            self.assertIsNone(entry["target"])

    def test_parse_diff_exact_and_near_miss(self) -> None:
        exact = {"left": {"symbols": [{"name": "func_1", "match_percent": 100.0}]},
                 "right": {"symbols": [{"name": "func_1", "match_percent": 100.0}]}}
        near = {"left": {"symbols": [{"name": "func_1", "match_percent": 80.0}]},
                "right": {"symbols": [{"name": "func_1", "match_percent": 80.0}]}}

        self.assertEqual(decomp_cycle.parse_diff(exact, "func_1")["match_percent"], 100.0)
        self.assertEqual(decomp_cycle.parse_diff(near, "func_1")["match_percent"], 80.0)

    def test_compact_diff_drops_unrelated_elf_symbols(self) -> None:
        diff = {"left": {"sections": [{"name": ".text", "size": "4"}],
                          "symbols": [{"name": "func_1"}, {"name": "unrelated"}]},
                "right": {"sections": [], "symbols": [{"name": "func_1"}]}}

        compact = decomp_cycle.compact_diff(diff, "func_1")

        self.assertEqual([s["name"] for s in compact["left"]["symbols"]], ["func_1"])

    def test_rom_path_policy(self) -> None:
        self.assertTrue(decomp_cycle.ROM_AFFECTING_FILES)
        self.assertIn("src/decomp/example.c", decomp_cycle.ROM_AFFECTING_PREFIXES[0] + "decomp/example.c")


if __name__ == "__main__":
    unittest.main()
