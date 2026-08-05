#!/usr/bin/env python3
import tempfile
import unittest
from pathlib import Path
import sys

sys.path.insert(0, str(Path(__file__).resolve().parent))
import asmlift_warioware


class AsmliftAdapterTests(unittest.TestCase):
    def test_embedded_c_string_labels_survive_normalization(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            source = Path(tmp) / "embedded.s"
            source.write_text(
                'asm(".syntax unified \\n\\\n'
                'thumb_func_start func_0800BF7C \\n\\\n'
                '_0800BFB6: \\n\\\n'
                '/* 0800BFAC */ BEQ _0800BFB6 \\n\\\n'
                '/* 0800BFB6 */ MOVS R0, #0 \\n\\\n'
                '");\n'
            )

            lines, discovered, omitted_data = asmlift_warioware.collect_lines(
                source, "func_0800BF7C", lowercase=True, keep_data=False
            )

            self.assertEqual(discovered, "func_0800BF7C")
            self.assertFalse(omitted_data)
            self.assertIn("_0800BFB6:", lines)
            self.assertIn("beq _0800BFB6", lines)

    def test_rendered_embedded_function_has_branch_target(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            source = Path(tmp) / "embedded.s"
            source.write_text(
                'asm("thumb_func_start func_0800BF7C \\n\\\n'
                '_0800BFB6: \\n\\\n'
                '/* 0800BFAC */ BEQ _0800BFB6 \\n\\\n'
                '/* 0800BFB6 */ MOVS R0, #0 \\n\\\n'
                '");\n'
            )

            rendered, _ = asmlift_warioware.render_assembly(
                source, "func_0800BF7C", lowercase=True, keep_data=False
            )

            self.assertIn(".global func_0800BF7C", rendered)
            self.assertIn("_0800BFB6:", rendered)
            self.assertIn("beq _0800BFB6", rendered)


if __name__ == "__main__":
    unittest.main()
