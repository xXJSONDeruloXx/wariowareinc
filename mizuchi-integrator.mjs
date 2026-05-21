/**
 * WarioWare Inc. Mizuchi Integrator Module
 *
 * Called by the Mizuchi integrator plugin after a function achieves a perfect match.
 * Handles the wariowareinc-specific standalone-TU pattern:
 *
 *   1. Write the matched C code to src/decomp/asm_XXXXXXXX.c
 *   2. Update wariowareinc.ld: replace build/asm/asm_XXXXXXXX.s.o with build/src/decomp/asm_XXXXXXXX.c.o
 *   3. Move asm/asm_XXXXXXXX.s → asm/converted/asm_XXXXXXXX.s
 *
 * The Mizuchi integrator plugin then runs the verifyBuildScript (make compare) and
 * commits/pushes/opens a PR depending on autoAction config.
 *
 * @param {object} params
 * @param {string} params.functionName   - e.g. "func_08002468"
 * @param {string} params.generatedCode  - the matched C code
 * @param {string} params.worktreePath   - absolute path to git worktree
 * @param {string} params.projectRoot    - absolute path to project root (main working tree)
 * @param {object} params.helpers        - IntegratorHelpers from mizuchi
 */

import fs from "node:fs";
import path from "node:path";

/** Extract hex address from function name: func_08002468 → "08002468" */
function addrFromFunctionName(functionName) {
  const m = /(?:func|asm)_(0[0-9a-fA-F]+)/i.exec(functionName ?? "");
  return m ? m[1].toLowerCase() : null;
}

export async function integrate({ functionName, generatedCode, worktreePath, projectRoot, helpers }) {
  helpers.log(`WarioWare integrator: starting for ${functionName}`);

  const addr = addrFromFunctionName(functionName);
  if (!addr) {
    throw new Error(`Cannot derive ROM address from function name "${functionName}". Expected pattern: func_XXXXXXXX or asm_XXXXXXXX.`);
  }

  const fileBase = `asm_${addr}`;
  const cFileName = `${fileBase}.c`;
  const sFileName = `${fileBase}.s`;

  // Paths inside the worktree
  const cFileDest = path.join(worktreePath, "src", "decomp", cFileName);
  const asmSrc = path.join(worktreePath, "asm", sFileName);
  const asmDest = path.join(worktreePath, "asm", "converted", sFileName);
  const ldScript = path.join(worktreePath, "wariowareinc.ld");

  // ── 1. Write C file ────────────────────────────────────────────────────────
  helpers.log(`Writing ${path.relative(worktreePath, cFileDest)}`);
  fs.mkdirSync(path.dirname(cFileDest), { recursive: true });

  // Ensure the code has a reasonable header comment
  const header = `// Decompiled by Mizuchi — matched ${functionName}\n// ROM address: 0x${addr.toUpperCase()}\n\n`;
  const finalCode = generatedCode.startsWith("//") ? generatedCode : header + generatedCode;
  fs.writeFileSync(cFileDest, finalCode, "utf8");

  // ── 2. Update linker script ────────────────────────────────────────────────
  helpers.log(`Updating wariowareinc.ld`);

  if (!fs.existsSync(ldScript)) {
    throw new Error(`wariowareinc.ld not found in worktree at ${worktreePath}`);
  }

  const ldContent = fs.readFileSync(ldScript, "utf8");

  // Old entry:   build/asm/asm_XXXXXXXX.s.o(.text*);
  // New entry:   build/src/decomp/asm_XXXXXXXX.c.o(.text*);
  const oldEntry = new RegExp(
    `(\\s*)(build/asm/${fileBase}\\.s\\.o)(\\([^)]*\\);)`,
    "g",
  );

  if (!oldEntry.test(ldContent)) {
    throw new Error(
      `Could not find linker entry for "build/asm/${fileBase}.s.o" in wariowareinc.ld.\n` +
      `Either already converted or the address is wrong.`,
    );
  }

  const newLdContent = ldContent.replace(
    new RegExp(`(\\s*)(build/asm/${fileBase}\\.s\\.o)(\\([^)]*\\);)`, "g"),
    `$1build/src/decomp/${fileBase}.c.o$3`,
  );
  fs.writeFileSync(ldScript, newLdContent, "utf8");
  helpers.log(`Linker updated: build/asm/${fileBase}.s.o → build/src/decomp/${fileBase}.c.o`);

  // ── 3. Move asm file to converted/ ────────────────────────────────────────
  if (fs.existsSync(asmSrc)) {
    helpers.log(`Moving ${path.relative(worktreePath, asmSrc)} → asm/converted/`);
    fs.mkdirSync(path.dirname(asmDest), { recursive: true });
    fs.renameSync(asmSrc, asmDest);
  } else {
    helpers.log(`WARNING: asm source not found at ${asmSrc} — skipping move (may already be in converted/)`);
  }

  helpers.log(`Integration complete for ${functionName}`);

  return {
    filesModified: [
      path.relative(worktreePath, cFileDest),
      "wariowareinc.ld",
      ...(fs.existsSync(asmDest) ? [path.relative(worktreePath, asmDest)] : []),
    ],
    summary: `Integrated ${functionName}: created src/decomp/${cFileName}, updated linker, moved asm to converted/`,
  };
}
