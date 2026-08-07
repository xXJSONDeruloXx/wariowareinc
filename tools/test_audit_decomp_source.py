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
        self.assertGreaterEqual(len(report["raw_pointer_accesses"]), 1)
        self.assertGreaterEqual(len(report["numeric_pointer_offsets"]), 1)

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
