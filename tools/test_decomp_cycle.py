#!/usr/bin/env python3
import json
import sys
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

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

    def test_dirty_guard_preserves_porcelain_status_column(self) -> None:
        with patch.object(
            decomp_cycle,
            "git_status",
            return_value=" M tools/attempts.tsv\n?? .decomp-runs/run.json\n M src/decomp/new.c\n",
        ):
            self.assertEqual(
                decomp_cycle.blocking_dirty_paths(Path("/repo")),
                ["src/decomp/new.c"],
            )

    def test_batch_receipt_selection_ignores_unselected_near_misses(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            exact_candidate = root / "exact.c"
            near_candidate = root / "near.c"
            exact_candidate.write_text("void func_0800D23C(void) {}\n")
            near_candidate.write_text("void func_080020FC(void) {}\n")
            exact_entry = {
                "function": "func_0800D23C",
                "candidate": exact_candidate,
                "candidate_rel": "exact.c",
            }
            isolation = {
                "results": [
                    {
                        "function": "func_080020FC",
                        "candidate": "near.c",
                        "candidate_sha256": decomp_cycle.file_sha256(near_candidate),
                        "status": "near_miss",
                    },
                    {
                        "function": "func_0800D23C",
                        "candidate": "exact.c",
                        "candidate_sha256": decomp_cycle.file_sha256(exact_candidate),
                        "status": "exact",
                    },
                ]
            }
            result = decomp_cycle.isolation_result_for_entry(isolation, exact_entry)
            self.assertEqual(result["status"], "exact")

    def test_isolation_script_links_target_and_candidate_with_target_symbols(self) -> None:
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
            script = decomp_cycle.container_script(root, [entry], Path("/tmp/run"))

            self.assertIn("arm-none-eabi-ld -Ttext=0", script)
            self.assertIn("--defsym=%s=0x%s", script)
            self.assertIn("printf '.text\\n\\t.align\\t2, 0\\n'", script)
            self.assertIn(".section .note.GNU-stack", script)
            self.assertIn("/run/target-0.elf", script)
            self.assertIn("/run/candidate-0.elf", script)
            self.assertIn("target-0.unit-defsym", script)
            self.assertIn("unit_defsym_args", script)

    def test_isolation_script_matches_makefile_alignment_tail(self) -> None:
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
            script = decomp_cycle.container_script(root, [entry], Path("/tmp/run"))

            self.assertIn("printf '.text\\n\\t.align\\t2, 0\\n'", script)
            self.assertIn(".section .note.GNU-stack", script)

    def test_entry_paths_include_linker_and_assembly_move(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            candidate = root / "candidate.c"
            candidate.write_text("void func_08002038(void) {}\n")
            target = root / "asm/asm_08002038.s"
            target.parent.mkdir(parents=True)
            target.write_text(".text\n")
            manifest = root / "manifest.json"
            manifest.write_text(json.dumps({"candidates": [{
                "function": "func_08002038",
                "candidate": "candidate.c",
                "target": "asm/asm_08002038.s",
            }]}))

            entry = decomp_cycle.load_manifest(root, manifest)[0]
            paths = decomp_cycle.entry_paths(root, entry)
            relative = {decomp_cycle.root_relative(root, path) for path in paths}

            self.assertEqual(relative, {
                "src/decomp/asm_08002038.c",
                "asm/converted/asm_08002038.s",
                "asm/asm_08002038.s",
                "wariowareinc.ld",
            })

    def test_included_stub_apply_adds_include_level_guard_and_rewires_host(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            candidate = root / "candidate.c"
            candidate.write_text('#include "global.h"\nvoid func_0800BF7C(void) {}\n')
            target = root / "asm/bitmap_font/asm_0800bf7c.s"
            target.parent.mkdir(parents=True)
            target.write_text(".text\nfunc_0800BF7C:\n\tbx lr\n")
            host = root / "src/bitmap_font.c"
            host.parent.mkdir(parents=True)
            host.write_text(
                '#include "asm/bitmap_font/asm_0800bf7c.s"\n'
                "void after(void) {}\n"
            )
            manifest = root / "manifest.json"
            manifest.write_text(json.dumps({"candidates": [{
                "function": "func_0800BF7C",
                "candidate": "candidate.c",
                "target": "asm/bitmap_font/asm_0800bf7c.s",
                "mode": "included_stub",
                "host_source": "src/bitmap_font.c",
                "include_line": '#include "asm/bitmap_font/asm_0800bf7c.s"',
                "decomp_include_line": '#include "decomp/asm_0800bf7c.c"',
            }]}))

            entry = decomp_cycle.load_manifest(root, manifest)[0]
            _, transform = decomp_cycle.source_for_apply(entry, candidate.read_text())
            self.assertEqual(transform, "added_include_level_guard")
            decomp_cycle.apply_entry(root, entry)

            source = root / "src/decomp/asm_0800bf7c.c"
            self.assertTrue(source.read_text().startswith("#if __INCLUDE_LEVEL__ > 0\n"))
            self.assertTrue(source.read_text().rstrip().endswith("#endif"))
            self.assertIn('#include "decomp/asm_0800bf7c.c"', host.read_text())
            self.assertFalse(target.exists())
            self.assertTrue((root / "asm/converted/asm_0800bf7c.s").is_file())

    def test_source_for_apply_preserves_existing_include_level_guard(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            candidate = root / "candidate.c"
            candidate.write_text("#if __INCLUDE_LEVEL__ > 0\nvoid f(void) {}\n#endif\n")
            entry = {"mode": "included_stub"}
            prepared, transform = decomp_cycle.source_for_apply(entry, candidate.read_text())
            self.assertEqual(transform, "preserved_existing_include_level_guard")
            self.assertEqual(prepared, candidate.read_text())

    def test_reuse_isolation_receipt_checks_current_input_hashes(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            candidate = root / "candidate.c"
            candidate.write_text("void func_08002038(void) {}\n")
            target = root / "target.s"
            target.write_text(".text\n")
            manifest = root / "manifest.json"
            manifest.write_text(json.dumps({"candidates": [{
                "function": "func_08002038",
                "candidate": "candidate.c",
                "target": "target.s",
            }]}))
            entry = decomp_cycle.load_manifest(root, manifest)[0]
            receipt = root / "isolation.json"
            receipt.write_text(json.dumps({
                "schema": 1,
                "kind": "isolation",
                "results": [{
                    "function": "func_08002038",
                    "candidate": "candidate.c",
                    "candidate_sha256": decomp_cycle.file_sha256(candidate),
                    "target_sha256": decomp_cycle.file_sha256(target),
                    "target_object_sha256": None,
                    "status": "exact",
                }],
            }))

            isolation = decomp_cycle.reuse_isolation_receipt(root, receipt, entry)
            self.assertEqual(isolation["results"][0]["status"], "exact")

            candidate.write_text("void func_08002038(void) { return; }\n")
            with self.assertRaises(decomp_cycle.CycleError):
                decomp_cycle.reuse_isolation_receipt(root, receipt, entry)


if __name__ == "__main__":
    unittest.main()
