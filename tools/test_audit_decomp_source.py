import json
import subprocess
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from check_decomp_policy import asm_findings, source_audit


class DecompSourceAuditTests(unittest.TestCase):
    def test_empty_barrier_is_not_instruction_but_is_not_strict_real_c(self) -> None:
        report = source_audit('void func_08000000(void) { int x = 1; asm("" : "+r"(x)); }')
        self.assertEqual(report["asm"]["barrier"][0]["kind"], "barrier")
        self.assertFalse(report["strict_real_c"])

    def test_register_pin_is_separate_from_instruction_asm(self) -> None:
        findings = asm_findings('void func_08000000(void) { register u32 x asm("r0"); }')
        self.assertEqual([finding["kind"] for finding in findings], ["register_pin"])

    def test_extended_instruction_asm_is_detected(self) -> None:
        report = source_audit('void func_08000000(void) { asm("mov r0, r1" : : : "r0"); }')
        self.assertEqual(report["asm"]["instruction"][0]["template"], "mov r0, r1")
        self.assertFalse(report["strict_real_c"])

    def test_raw_pointer_offsets_are_evidence_not_a_bypass(self) -> None:
        report = source_audit(
            'void func_08000000(void) { u8 *p = (u8 *)gCurrentSceneData; *(u16 *)(p + 0x1BA) = 1; }'
        )
        self.assertTrue(report["strict_real_c"])
        self.assertTrue(report["layout_quality_ok"])
        self.assertEqual(report["layout"]["classification"], "bounded_layout_evidence")
        self.assertGreaterEqual(len(report["raw_pointer_accesses"]), 1)
        self.assertGreaterEqual(len(report["numeric_pointer_offsets"]), 1)

    def test_offset_heavy_opaque_layout_is_not_admitted(self) -> None:
        report = source_audit(
            "void func_08000000(void) {\n"
            " u8 *p = (u8 *)gCurrentSceneData;\n"
            " *(u16 *)(p + 0x10) = 1;\n"
            " *(u16 *)(p + 0x12) = 2;\n"
            " *(u16 *)(p + 0x14) = 3;\n"
            "}\n"
        )
        self.assertTrue(report["strict_real_c"])
        self.assertFalse(report["layout_quality_ok"])
        self.assertEqual(report["layout"]["classification"], "opaque_offset_heavy")

    def test_raw_pointer_alias_offsets_are_counted_after_the_cast_line(self) -> None:
        report = source_audit(
            "void func_08000000(void) {\n"
            " u8 *p = (u8 *)gCurrentSceneData;\n"
            " p[0x10] = 1;\n"
            " p[0x12] = 2;\n"
            " p[0x14] = 3;\n"
            "}\n"
        )
        self.assertEqual(report["raw_pointer_aliases"], ["p"])
        self.assertFalse(report["layout_quality_ok"])
        self.assertEqual(report["layout"]["classification"], "opaque_offset_heavy")

    def test_non_mmio_volatile_is_not_a_clean_source_candidate(self) -> None:
        report = source_audit(
            "void func_08000000(void) {\n"
            " volatile u32 value = 1;\n"
            " value += 2;\n"
            "}\n"
        )
        self.assertFalse(report["semantic_quality_ok"])
        self.assertEqual(len(report["volatile_accesses"]), 1)

    def test_direct_gba_io_volatile_is_allowed(self) -> None:
        report = source_audit(
            "void func_08000000(void) { *(volatile u16 *)0x04000000 = 0; }"
        )
        self.assertTrue(report["semantic_quality_ok"])
        self.assertEqual(report["volatile_accesses"], [])

    def test_named_overlay_is_preferred_to_opaque_offsets(self) -> None:
        report = source_audit(
            "struct Scene { unsigned short state; };"
            "void func_08000000(void) {"
            " struct Scene *scene = (struct Scene *)gCurrentSceneData;"
            " scene->state = 1;"
            "}"
        )
        self.assertTrue(report["layout_quality_ok"])
        self.assertEqual(report["layout"]["classification"], "typed_or_direct")

    def test_unrelated_named_overlay_does_not_excuse_an_opaque_blob(self) -> None:
        report = source_audit(
            "struct Scene { unsigned short state; };\n"
            "void func_08000000(void) {\n"
            " u8 *p = (u8 *)gCurrentSceneData;\n"
            " struct Scene *scene = (struct Scene *)gCurrentSceneData;\n"
            " scene->state = 1;\n"
            " p[0x10] = 1;\n"
            " p[0x12] = 2;\n"
            " p[0x14] = 3;\n"
            "}\n"
        )
        self.assertFalse(report["layout_quality_ok"])
        self.assertEqual(report["layout"]["classification"], "named_overlay_with_opaque_raw_evidence")

    def test_current_scene_helpers_pass_strict_audit(self) -> None:
        root = Path(__file__).resolve().parents[1]
        command = [
            sys.executable,
            str(root / "tools/audit_decomp_source.py"),
            "src/decomp/asm_0801646c.c",
            "src/decomp/asm_080164cc.c",
            "--strict",
        ]
        result = subprocess.run(command, cwd=root, text=True, capture_output=True, check=False)
        self.assertEqual(result.returncode, 0, result.stderr or result.stdout)
        self.assertTrue(json.loads(result.stdout)["ok"] if result.stdout.strip().startswith("{") else True)


if __name__ == "__main__":
    unittest.main()
