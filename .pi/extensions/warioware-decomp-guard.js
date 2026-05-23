/**
 * WarioWare Decomp Guard Extension
 *
 * Enforces code quality and safety rules for the decompilation project:
 *
 * 1. BLOCKS inline asm containing ARM/Thumb instructions in src/decomp/*.c
 *    (the anti-cheese rule — decompilation means writing REAL C, not
 *    embedding assembly in C wrappers)
 *
 * 2. BLOCKS direct edits to asm/ (auto-generated from ROM)
 *
 * 3. BLOCKS edits to build/ (generated artifacts)
 *
 * 4. BLOCKS git commit --no-verify
 *
 * 5. INJECTS decomp coding standards into the system prompt every turn
 *
 * Allowed exceptions for inline asm:
 *   - __attribute__((naked)) functions (body IS asm by design)
 *   - GBA BIOS SVC calls: asm volatile("svc #N")
 *   - Empty barrier hints: asm volatile("" : ...)
 *
 * Blocked inline asm patterns in non-naked decomp functions:
 *   - asm volatile("bl ...")     → use real C function calls
 *   - asm volatile("ldr/str/...") → use real C pointer dereference
 *   - asm volatile("sub sp/...")  → use real C local variables
 *   - asm volatile("mov/bx/...")  → use real C control flow
 */

import * as fs from "node:fs";
import * as path from "node:path";
import type { ExtensionAPI } from "@earendil-works/pi-coding-agent";

// ARM/Thumb instruction mnemonics that indicate real inline asm
// (not just compiler shaping hints)
const BLOCKED_MNEMONICS =
  /\b(bl|bx|blr|ldr|str|strh|strb|ldrh|ldrb|push|pop|add\s+sp|sub\s+sp|mov\s+r|adds\s+r|subs\s+r|cmp\s+r|bne|beq|bge|blt|ble|bhi|blo|bpl|bmi|svc|swi|stm|ldm|lsls\s+r|lsrs\s+r|asrs\s+r|ands\s+r|orrs\s+r|eors\s+r|bics\s+r|rsbs\s+r|negs\s+r|muls|r[0-9])\b/i;

// Patterns that are ALLOWED even in asm volatile
const ALLOWED_PATTERNS = [
  /^\s*""/,                    // empty string barriers: asm volatile("" : ...)
  /svc\s+#\d+/,               // GBA BIOS calls: asm volatile("svc #6")
  /swi\s+#\d+/,               // GBA BIOS calls (alt syntax)
];

export default function decompGuard(pi: ExtensionAPI) {
  // ── Tool Call Guard ──────────────────────────────────────────────────

  pi.on("tool_call", async (event, ctx) => {
    const { toolName, input } = event;

    // ── Rule 1: Block inline asm in src/decomp/ writes/edits ────────
    if (toolName === "write" || toolName === "edit") {
      const filePath = (input.path as string) || "";

      // Check if this is a src/decomp/*.c file
      const normalizedPath = filePath.replace(/\\/g, "/");
      const isDecompFile =
        normalizedPath.includes("src/decomp/") &&
        normalizedPath.endsWith(".c");

      if (isDecompFile) {
        // Get the content to check
        const content =
          (input.content as string) ||
          (input.edits as Array<{ newText: string }>)?.map((e) => e.newText).join("\n") ||
          "";

        if (content) {
          // Skip check if the file uses __attribute__((naked))
          // (naked functions ARE asm by design)
          if (/__attribute__\s*\(\s*\(\s*naked\s*\)\s*\)/.test(content)) {
            return undefined; // allow
          }

          // Find all asm volatile blocks and check for blocked mnemonics
          const asmVolatileRegex = /asm\s+volatile\s*\(/g;
          let match;
          const violations: string[] = [];

          while ((match = asmVolatileRegex.exec(content)) !== null) {
            const startIdx = match.index;

            // Extract the asm string content (rough — finds the string literal)
            // Look for the string starting after the opening paren
            const afterParen = content.slice(
              startIdx + match[0].length,
              startIdx + match[0].length + 500
            );

            // Find the string literal (either "..." or R"...")
            const strMatch = afterParen.match(/^\s*"/);
            if (!strMatch) continue;

            // Find closing quote (simple approach — handles most cases)
            let strContent = "";
            let i = afterParen.indexOf('"') + 1;
            let escaped = false;
            while (i < afterParen.length) {
              const ch = afterParen[i];
              if (escaped) {
                strContent += ch;
                escaped = false;
              } else if (ch === "\\") {
                escaped = true;
              } else if (ch === '"') {
                break;
              } else {
                strContent += ch;
              }
              i++;
            }

            // Check allowed patterns first
            const isAllowed = ALLOWED_PATTERNS.some((p) => p.test(strContent));
            if (isAllowed) continue;

            // Check for blocked mnemonics
            if (BLOCKED_MNEMONICS.test(strContent)) {
              // Find line number for reporting
              const beforeMatch = content.slice(0, startIdx);
              const lineNum = beforeMatch.split("\n").length;
              const firstInstruction = strContent
                .split("\n")
                .map((l: string) => l.trim())
                .filter((l: string) => l.length > 0)[0];
              violations.push(
                `Line ${lineNum}: asm volatile("${firstInstruction?.slice(0, 60)}...")`
              );
            }
          }

          if (violations.length > 0) {
            const violationList = violations.join("\n  ");
            if (ctx.hasUI) {
              ctx.ui.notify(
                `BLOCKED: Inline ARM/Thumb asm in decomp file\n  ${violationList}`,
                "error"
              );
            }
            return {
              block: true,
              reason: `🚫 INLINE ASM BLOCKED in ${path.basename(filePath)}:\n\n  ${violationList}\n\nDecompilation means writing REAL C CODE that the compiler translates to matching assembly.\nEmbedding assembly instructions in C is NOT decompilation — it's hiding the asm.\n\nInstead:\n  • Use real C function calls instead of asm volatile("bl ...")\n  • Use real C pointer dereference instead of asm volatile("ldr/str...")\n  • Use real C local variables instead of asm volatile("sub sp/add sp...")\n  • Use real C if/switch/goto instead of asm volatile("cmp/bne/beq...")\n  • Use __attribute__((naked)) if the function truly cannot be expressed in C\n\nSee docs/decomp-pattern-library.md for legitimate C shaping techniques.`,
            };
          }
        }
      }
    }

    // ── Rule 2: Block edits to asm/ (except asm/converted/) ─────────
    if (toolName === "write" || toolName === "edit") {
      const filePath = (input.path as string || "").replace(/\\/g, "/");
      if (
        filePath.includes("/asm/") &&
        !filePath.includes("/asm/converted/")
      ) {
        if (ctx.hasUI) {
          ctx.ui.notify(`Blocked edit to asm/: ${path.basename(filePath)}`, "warning");
        }
        return {
          block: true,
          reason: `Files in asm/ are auto-generated from the original ROM and must not be edited directly. Use apply_conversion to move stubs to asm/converted/. Path: ${filePath}`,
        };
      }
    }

    // ── Rule 3: Block edits to build/ ──────────────────────────────
    if (toolName === "write" || toolName === "edit") {
      const filePath = (input.path as string || "").replace(/\\/g, "/");
      if (filePath.includes("/build/")) {
        return {
          block: true,
          reason: `Files in build/ are generated artifacts. Run the build to regenerate them. Path: ${filePath}`,
        };
      }
    }

    // ── Rule 4: Block git commit --no-verify ───────────────────────
    if (toolName === "bash") {
      const command = (input.command as string) || "";
      if (/git\s+commit\s+.*--no-verify/.test(command)) {
        return {
          block: true,
          reason: `git commit --no-verify is not allowed. All commits must pass pre-commit checks to ensure ROM integrity.`,
        };
      }
    }

    return undefined; // allow all other tool calls
  });

  // ── System Prompt Injection ──────────────────────────────────────────

  pi.on("before_agent_start", async (event) => {
    const decompRules = `
## WarioWare Decomp Quality Rules (ENFORCED BY GUARD EXTENSION)

### 🚫 ABSOLUTELY NO inline asm in src/decomp/*.c (EXCEPT naked functions)

Decompilation means writing **real C code** that the compiler translates to matching assembly. Do NOT embed ARM/Thumb assembly instructions inside C function bodies using \`asm volatile(...)\`. This is enforced by the decomp-guard Pi extension which will BLOCK any write/edit that contains:

- \`asm volatile("bl ...")\` → Use real C function calls
- \`asm volatile("ldr/str/strh/ldrb/...")\` → Use real C pointer dereference
- \`asm volatile("sub sp/add sp")\` → Use real C local variables
- \`asm volatile("mov rN/bx/pop/push")\` → Use real C control flow
- \`asm volatile("cmp/bne/beq/...")\` → Use real C conditionals

**Allowed exceptions** (the guard lets these through):
- \`__attribute__((naked))\` functions — the entire body IS asm by design (use sparingly)
- \`asm volatile("svc #N")\` — GBA BIOS hardware interface
- \`asm volatile("" : ...)\` — empty-string compiler shaping hints / barriers

**If you can't match a function in pure C:**
1. Try different C shaping techniques from docs/decomp-pattern-library.md
2. Try different variable declaration orders (C89 matters)
3. Try different types (s16 vs u16 vs s32 can change codegen)
4. As a LAST RESORT, use \`__attribute__((naked))\` with full asm body
5. If even naked doesn't work, SKIP the function — don't cheese it

### Other enforced rules
- Do NOT edit files in \`asm/\` (auto-generated from ROM)
- Do NOT edit files in \`build/\` (generated artifacts)
- Do NOT use \`git commit --no-verify\`
- Always verify with Docker build, not local make
`;

    return {
      systemPrompt: event.systemPrompt + decompRules,
    };
  });
}
