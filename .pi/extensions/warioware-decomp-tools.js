/**
 * WarioWare Decomp Tools
 *
 * LLM-callable tools that expose the Mizuchi pipeline components:
 *   - compile_and_view_asm   : isolated compile + objdiff diff (fast iteration, no full build)
 *   - get_function_context   : preprocessed headers for a specific function via Docker gcc -E
 *   - m2c_decompile          : initial C skeleton from assembly via m2c
 *   - query_candidates       : ranked unmatched function list from mizuchi-db.json
 *
 * Also registers /decomp-setup to verify prerequisites.
 */
import fs from "node:fs";
import path from "node:path";
import os from "node:os";
import { execFileSync, execSync } from "node:child_process";
import { Type } from "typebox";

const REPO_SENTINEL = "wariowareinc.ld";
const DB_FILE = "mizuchi-db.json";
const DOCKER_IMAGE = "devkitpro/devkitarm:latest";
const MIZUCHI_ENV_VARS = ["MIZUCHI_ROOT", "PI_MIZUCHI_ROOT"];

function shellQuote(value) {
  return `'${String(value).replace(/'/g, `'"'"'`)}'`;
}

function commandExists(command) {
  try {
    execSync(`command -v ${shellQuote(command)} >/dev/null 2>&1`, {
      stdio: "pipe",
      shell: "/bin/bash",
      timeout: 10_000,
    });
    return true;
  } catch {
    return false;
  }
}

function looksLikeMizuchiRoot(dir) {
  if (!dir) return false;
  return fs.existsSync(path.join(dir, "package.json"));
}

function resolveMizuchiRoot(repoRoot) {
  const checked = [];
  const envProblems = [];

  for (const envName of MIZUCHI_ENV_VARS) {
    const value = process.env[envName]?.trim();
    if (!value) continue;
    const resolved = path.resolve(value);
    checked.push(`${envName}=${resolved}`);
    if (looksLikeMizuchiRoot(resolved)) {
      return { root: resolved, source: `$${envName}`, checked, envProblems };
    }
    envProblems.push(`${envName}=${resolved} is not a Mizuchi checkout`);
  }

  const autoCandidates = [
    path.resolve(repoRoot, "..", "mizuchi"),
    path.resolve(repoRoot, "..", "..", "mizuchi"),
    path.join(os.homedir(), "Developer", "mizuchi"),
    path.join(os.homedir(), "mizuchi"),
  ];

  const seen = new Set();
  for (const candidate of autoCandidates) {
    const resolved = path.resolve(candidate);
    if (seen.has(resolved)) continue;
    seen.add(resolved);
    checked.push(resolved);
    if (looksLikeMizuchiRoot(resolved)) {
      return { root: resolved, source: "autodetect", checked, envProblems };
    }
  }

  return { root: null, source: null, checked, envProblems };
}

function formatMizuchiRootFix(repoRoot) {
  const sibling = path.resolve(repoRoot, "..", "mizuchi");
  return [
    `Clone Mizuchi next to this repo (recommended): git clone <mizuchi-repo-url> ${shellQuote(sibling)}`,
    "or if you keep Mizuchi elsewhere, persist it in zsh:",
    "echo 'export MIZUCHI_ROOT=/absolute/path/to/mizuchi' >> ~/.zshrc && source ~/.zshrc",
  ].join("\n      ");
}

function formatIndexCodebaseCommand(repoRoot, mizuchiRoot) {
  const root = mizuchiRoot || "<path-to-mizuchi>";
  return `cd ${shellQuote(root)} && npm start -- index-codebase --config ${shellQuote(path.join(repoRoot, "mizuchi.yaml"))} --skip-embeddings`;
}

function formatM2cSetupCommand(mizuchiRoot) {
  const root = mizuchiRoot || "<path-to-mizuchi>";
  return `cd ${shellQuote(root)} && git clone https://github.com/matt-kempster/m2c.git vendor/m2c && ./scripts/setup-m2c.sh`;
}

// ── Repo root ─────────────────────────────────────────────────────────────────

function findRepoRoot(startDir) {
  let cur = path.resolve(startDir);
  while (true) {
    if (fs.existsSync(path.join(cur, REPO_SENTINEL))) return cur;
    const parent = path.dirname(cur);
    if (parent === cur) return path.resolve(startDir);
    cur = parent;
  }
}

function runDockerShell(repoRoot, shellCommand, timeout = 600_000) {
  return execFileSync(
    "docker",
    [
      "run", "--rm",
      "-v", `${repoRoot}:/workspace`,
      "-w", "/workspace",
      DOCKER_IMAGE,
      "bash", "-lc",
      shellCommand,
    ],
    { cwd: repoRoot, timeout, stdio: "pipe" },
  ).toString();
}

// ── Mizuchi DB ────────────────────────────────────────────────────────────────

let _dbCache = null;
let _dbCacheFile = null;
let _dbCacheMtime = null;

function loadDb(repoRoot) {
  const dbFile = path.join(repoRoot, DB_FILE);
  if (!fs.existsSync(dbFile)) return null;
  const stat = fs.statSync(dbFile);
  const mtime = stat.mtimeMs;
  if (_dbCache && _dbCacheFile === dbFile && _dbCacheMtime === mtime) return _dbCache;
  _dbCache = JSON.parse(fs.readFileSync(dbFile, "utf8"));
  _dbCacheFile = dbFile;
  _dbCacheMtime = mtime;
  return _dbCache;
}

function findFn(db, name) {
  if (!db) return null;
  return (db.decompFunctions || []).find(
    (f) => f.name === name || f.name.toLowerCase() === name.toLowerCase(),
  ) ?? null;
}

/** Extract hex address string from an asmModulePath like .mizuchi-asm/asm/foo/asm_08002468.s */
function extractAddr(asmModulePath) {
  const m = /asm_(0[0-9a-fA-F]{7})\.s$/i.exec(asmModulePath ?? "");
  return m ? m[1].toLowerCase() : null;
}

/** Derive target .o path from mizuchi-db entry */
function targetObjPath(repoRoot, fn) {
  const addr = extractAddr(fn?.asmModulePath);
  if (!addr) return null;
  return path.join(repoRoot, "build", "asm", `asm_${addr}.s.o`);
}

/** Derive addr hex from function name like func_08002468 or asm_08002468 */
function addrFromName(name) {
  const m = /(?:func|asm)_(0[0-9a-fA-F]+)/i.exec(name ?? "");
  return m ? m[1].toLowerCase() : null;
}

function functionAddr(fn, fallbackName) {
  return extractAddr(fn?.asmModulePath) || addrFromName(fn?.name) || addrFromName(fallbackName);
}

function asmRelPath(fn) {
  const p = fn?.asmModulePath;
  if (!p) return null;
  return p.replace(/^\.mizuchi-asm\//, "").replace(/\\/g, "/");
}

function walkSourceFiles(dir) {
  if (!fs.existsSync(dir)) return [];
  const out = [];
  for (const ent of fs.readdirSync(dir, { withFileTypes: true })) {
    const full = path.join(dir, ent.name);
    if (ent.isDirectory()) {
      out.push(...walkSourceFiles(full));
    } else if (/\.(c|h)$/i.test(ent.name)) {
      out.push(full);
    }
  }
  return out;
}

function findIncludingSource(repoRoot, asmRel) {
  if (!asmRel) return null;
  const needle = `#include "${asmRel}"`;
  for (const file of walkSourceFiles(path.join(repoRoot, "src"))) {
    let text = "";
    try {
      text = fs.readFileSync(file, "utf8");
    } catch {
      continue;
    }
    if (text.includes(needle)) return path.relative(repoRoot, file).replace(/\\/g, "/");
  }
  return null;
}

function linkerEntryFor(repoRoot, objRel) {
  if (!objRel) return null;
  const ldPath = path.join(repoRoot, "wariowareinc.ld");
  if (!fs.existsSync(ldPath)) return null;
  const lines = fs.readFileSync(ldPath, "utf8").split("\n");
  const idx = lines.findIndex((line) => line.includes(objRel));
  return idx >= 0 ? { lineNumber: idx + 1, text: lines[idx].trim() } : null;
}

/** Extract function calls from asm code by looking for BL instructions */
function extractCallsFromAsm(asmCode) {
  if (!asmCode) return [];
  const calls = [];
  // Match patterns like: BL func_080024D0 or BL func_080024D0-0x4
  const blRegex = /\bBL\s+(func_[0-9a-fA-F]{8})(?:-\w+)?/g;
  let match;
  while ((match = blRegex.exec(asmCode)) !== null) {
    calls.push(match[1].toLowerCase());
  }
  return [...new Set(calls)]; // deduplicate
}

/** Check if any of the function's callees have been converted to C (have cCode in mizuchi-db) */
function hasConvertedCallees(db, fn) {
  const allFns = db?.decompFunctions || [];
  // Use callsFunctions from db if available, otherwise extract from asmCode
  const calleeNames = fn?.callsFunctions?.length
    ? new Set(fn.callsFunctions)
    : new Set(extractCallsFromAsm(fn?.asmCode));
  for (const f of allFns) {
    if (calleeNames.has(f.name.toLowerCase()) && f.cCode?.trim()) {
      return true;
    }
  }
  return false;
}

/** Get list of callees that have been converted to C */
function getConvertedCallees(db, fn) {
  const allFns = db?.decompFunctions || [];
  // Use callsFunctions from db if available, otherwise extract from asmCode
  const calleeNames = fn?.callsFunctions?.length
    ? new Set(fn.callsFunctions)
    : new Set(extractCallsFromAsm(fn?.asmCode));
  const converted = [];
  for (const f of allFns) {
    if (calleeNames.has(f.name.toLowerCase()) && f.cCode?.trim()) {
      converted.push(f.name);
    }
  }
  return converted;
}

function preflightFunction(repoRoot, fn, fallbackName, db) {
  const name = fn?.name ?? fallbackName;
  const addr = functionAddr(fn, fallbackName);
  const asmRel = asmRelPath(fn);
  const asmAbs = asmRel ? path.join(repoRoot, asmRel) : null;
  const targetObjRel = addr ? `build/asm/asm_${addr}.s.o` : null;
  const targetObj = targetObjRel ? path.join(repoRoot, targetObjRel) : null;
  const includingSource = findIncludingSource(repoRoot, asmRel);
  const linkerEntry = linkerEntryFor(repoRoot, targetObjRel);
  const decompRel = addr ? `src/decomp/asm_${addr}.c` : null;
  const decompObjRel = addr ? `build/src/decomp/asm_${addr}.c.o` : null;
  const convertedAsmRel = addr ? `asm/converted/asm_${addr}.s` : null;
  const decompAbs = decompRel ? path.join(repoRoot, decompRel) : null;
  const convertedAsmAbs = convertedAsmRel ? path.join(repoRoot, convertedAsmRel) : null;
  const decompLinkerEntry = linkerEntryFor(repoRoot, decompObjRel);

  let conversionMode = "unknown_skip";
  let safeForAutonomous = false;
  const reasons = [];
  const nextSteps = [];
  let calleeRisk = false;
  let convertedCallees = [];

  if (!fn) {
    reasons.push("function was not found in mizuchi-db.json");
    nextSteps.push("Run the Mizuchi indexer or pick a candidate from query_candidates.");
  } else if (!fs.existsSync(asmAbs ?? "") && (fs.existsSync(decompAbs ?? "") || fs.existsSync(convertedAsmAbs ?? "") || decompLinkerEntry)) {
    conversionMode = "already_converted";
    reasons.push("mizuchi-db still lists this function as unmatched, but the working tree already has converted artifacts");
    if (decompRel && fs.existsSync(decompAbs ?? "")) reasons.push(`found ${decompRel}`);
    if (convertedAsmRel && fs.existsSync(convertedAsmAbs ?? "")) reasons.push(`found ${convertedAsmRel}`);
    if (decompLinkerEntry) reasons.push(`linker already points at ${decompObjRel}`);
    nextSteps.push("Refresh the Mizuchi index before using this function again; do not select it for a new chunk.");
  } else if (includingSource) {
    conversionMode = "included_stub";
    // Check for callee risk
    convertedCallees = getConvertedCallees(db, fn);
    calleeRisk = convertedCallees.length > 0;
    // Still autonomous-safe, but with a warning about callee risk
    safeForAutonomous = true;
    reasons.push(`asm stub is included by ${includingSource}`);
    reasons.push("use include-shim conversion: replace the asm include with a guarded src/decomp C include so code stays in the original host TU order");
    if (calleeRisk) {
      reasons.push(`⚠️ CALLEE RISK: calls already-converted C functions: ${convertedCallees.slice(0, 3).join(", ")}${convertedCallees.length > 3 ? "..." : ""}`);
      reasons.push("Isolated compile_and_view_asm may show 100% match, but ROM can mismatch due to register allocation differences with C callees");
      nextSteps.push("Consider decompiling the callees first (strategy: callees), or verify at linked ROM level only");
    }
    nextSteps.push("Use compile_and_view_asm until 100%, then apply_conversion; it will use the included-stub shim workflow automatically.");
  } else if (linkerEntry) {
    conversionMode = "standalone_tu";
    // Check for callee risk
    convertedCallees = getConvertedCallees(db, fn);
    calleeRisk = convertedCallees.length > 0;
    safeForAutonomous = true;
    reasons.push(`linker script contains ${targetObjRel}`);
    if (calleeRisk) {
      reasons.push(`⚠️ CALLEE RISK: calls already-converted C functions: ${convertedCallees.slice(0, 3).join(", ")}${convertedCallees.length > 3 ? "..." : ""}`);
      reasons.push("Isolated compile_and_view_asm may show 100% match, but ROM can mismatch due to register allocation differences with C callees");
      nextSteps.push("Consider decompiling the callees first (strategy: callees), or verify at linked ROM level only");
    }
    nextSteps.push("Use compile_and_view_asm until 100%, then apply_conversion for the mechanical edits and ROM check.");
  } else {
    reasons.push("no including C source and no matching linker-script object entry were found");
    nextSteps.push("Use this for research only until a dedicated build integration strategy is added for this layout.");
  }

  return {
    name,
    addr,
    asmRel,
    asmExists: asmAbs ? fs.existsSync(asmAbs) : false,
    targetObjRel,
    targetObjectExists: targetObj ? fs.existsSync(targetObj) : false,
    includingSource,
    linkerEntry,
    decompLinkerEntry,
    conversionMode,
    safeForAutonomous,
    calleeRisk,
    convertedCallees,
    decompRel,
    convertedAsmRel,
    reasons,
    nextSteps,
  };
}

function formatPreflight(pf) {
  const calleeRiskTag = pf.calleeRisk ? " ⚠️ (callee-risk)" : "";
  const lines = [
    `Preflight for ${pf.name}: ${pf.conversionMode}${pf.safeForAutonomous ? " ✅" : " ⚠️"}${calleeRiskTag}`,
    `  asm: ${pf.asmRel ?? "?"}${pf.asmExists ? "" : " (missing)"}`,
    `  target object: ${pf.targetObjRel ?? "?"}${pf.targetObjectExists ? " (exists)" : " (not built/found)"}`,
    `  including source: ${pf.includingSource ?? "none"}`,
    `  linker entry: ${pf.linkerEntry ? `${pf.linkerEntry.lineNumber}: ${pf.linkerEntry.text}` : "none"}`,
    `  decomp linker entry: ${pf.decompLinkerEntry ? `${pf.decompLinkerEntry.lineNumber}: ${pf.decompLinkerEntry.text}` : "none"}`,
    `  planned C: ${pf.decompRel ?? "?"}`,
    `  planned converted asm: ${pf.convertedAsmRel ?? "?"}`,
  ];
  if (pf.reasons.length) lines.push("\nReasons:\n" + pf.reasons.map((r) => `  - ${r}`).join("\n"));
  if (pf.nextSteps.length) lines.push("\nNext steps:\n" + pf.nextSteps.map((r) => `  - ${r}`).join("\n"));
  return lines.join("\n");
}

function asmForStandaloneObject(asmCode) {
  let out = asmCode ?? "";
  // Handle included-stub format: asm("...") C-asm string with possible line continuations
  // e.g. asm(".syntax unified \n\\\nthumb_func_start ...\n\\\n.syntax divided");
  const asmStringRe = /^asm\s*\(["']([\s\S]*?)["']\)\s*;?\s*$/m;
  const asmStringMatch = out.match(asmStringRe);
  if (asmStringMatch) {
    out = asmStringMatch[1]
      // Remove line-continuation backslash+newline sequences
      .replace(/\\\n/g, "\n")
      // Unescape \n -> newline
      .replace(/\\n/g, "\n")
      // Unescape \t -> tab
      .replace(/\\t/g, "\t")
      // Unescape \" -> double-quote
      .replace(/\\"/g, '"')
      // Unescape \\ -> backslash
      .replace(/\\\\/g, "\\");
  }
  out = out.replace(/^\s*thumb_func_start\s+(\w+)\s*$/gim, ".thumb\n.thumb_func\n.global $1\n$1:");
  out = out.replace(/^\s*arm_func_start\s+(\w+)\s*$/gim, ".arm\n.global $1\n$1:");
  out = out.replace(/^\s*(thumb_func_end|arm_func_end)\s+\w+\s*$/gim, "");
  if (!/^\s*\.syntax\s+unified\b/im.test(out)) out = `.syntax unified\n${out}`;
  return out;
}

function assembleAsmTargetInDocker(repoRoot, asmCode, outObj) {
  const tmpDir = fs.mkdtempSync(path.join(os.tmpdir(), "ww-target-asm-"));
  const asmFile = path.join(tmpDir, "target.s");
  try {
    fs.writeFileSync(asmFile, asmForStandaloneObject(asmCode), "utf8");
    fs.mkdirSync(path.dirname(outObj), { recursive: true });
    execFileSync(
      "docker",
      [
        "run", "--rm",
        "-v", `${tmpDir}:/tmp/ww-target`,
        "-v", `${repoRoot}:/workspace`,
        "-w", "/workspace",
        "devkitpro/devkitarm:latest",
        "bash", "-lc",
        "/opt/devkitpro/devkitARM/bin/arm-none-eabi-as -march=armv4t -o /tmp/ww-target/target.o /tmp/ww-target/target.s",
      ],
      { cwd: repoRoot, timeout: 120_000, stdio: "pipe" },
    );
    fs.copyFileSync(path.join(tmpDir, "target.o"), outObj);
  } finally {
    fs.rmSync(tmpDir, { recursive: true, force: true });
  }
}

function wrapIncludedStubCode(cCode) {
  const body = cCode.endsWith("\n") ? cCode : cCode + "\n";
  if (/__INCLUDE_LEVEL__/.test(body)) return body;
  return `#if __INCLUDE_LEVEL__ > 0\n${body}#endif\n`;
}

// ── objdiff JSON parser ───────────────────────────────────────────────────────

function parseDiffJson(jsonStr, symbolName) {
  let data;
  try {
    data = JSON.parse(jsonStr);
  } catch (e) {
    return { ok: false, error: `JSON parse failed: ${e.message}`, raw: jsonStr.slice(0, 400) };
  }

  // Find the function symbol in both left (target) and right (compiled)
  const leftSide = data.left ?? {};
  const rightSide = data.right ?? {};

  const findSym = (side, name) => {
    for (const s of side.symbols ?? []) {
      if (s.name === name || s.name === name + "+1" || s.name?.toLowerCase() === name.toLowerCase()) return s;
    }
    return null;
  };

  const targetSym = findSym(leftSide, symbolName);
  const compiledSym = findSym(rightSide, symbolName);
  const sym = compiledSym || targetSym;

  const matchPercent = sym?.match_percent ?? 0;

  // Build side-by-side instruction listing
  const targetInstrs = targetSym?.instructions ?? [];
  const compiledInstrs = compiledSym?.instructions ?? [];
  const maxLen = Math.max(targetInstrs.length, compiledInstrs.length);

  const instrList = [];
  for (let i = 0; i < maxLen; i++) {
    const t = targetInstrs[i];
    const c = compiledInstrs[i];
    const tText = t?.instruction?.formatted ?? "";
    const cText = c?.instruction?.formatted ?? "";
    // An instruction differs if diff_kind is set on either side
    const differs = (t?.diff_kind && t.diff_kind !== "NONE") || (c?.diff_kind && c.diff_kind !== "NONE");
    instrList.push({ target: tText, compiled: cText, differs });
  }

  return {
    ok: true,
    matchPercent,
    isPerfectMatch: matchPercent >= 100.0,
    instrList,
    targetInstrs,
    compiledInstrs,
    symbolFound: !!sym,
  };
}

function formatDiffResult(result, functionName) {
  if (!result.ok) return `Diff parse error: ${result.error}`;

  const pct = result.matchPercent != null ? `${result.matchPercent.toFixed(1)}%` : "?%";

  if (result.isPerfectMatch) {
    return `✅ PERFECT MATCH (100%) — ${functionName}`;
  }

  const lines = [`❌ MISMATCH (${pct}) — ${functionName}`];

  if (result.instrList.length > 0) {
    lines.push(`\nTarget  vs  Compiled  (${result.instrList.length} instrs):`);
    let diffCount = 0;
    for (const instr of result.instrList) {
      if (instr.differs) diffCount++;
      const marker = instr.differs ? "  ✗" : "  ✓";
      lines.push(`${marker}  target:    ${instr.target || "(end)"}`);
      if (instr.differs) {
        lines.push(`      compiled: ${instr.compiled || "(end)"}`);
      }
    }
    lines.push(`\n${diffCount} instruction(s) differ`);
  } else {
    lines.push(`\n(No instruction-level diff data — run full Docker build for byte-level comparison)`);
  }

  if (!result.symbolFound) {
    lines.push(`(Note: symbol "${functionName}" not found in diff output)`);
  }

  return lines.join("\n");
}

// ── Tool: compile_and_view_asm ────────────────────────────────────────────────

function registerCompileAndViewAsm(pi) {
  pi.registerTool({
    name: "compile_and_view_asm",
    label: "Compile & View ASM Diff",
    description:
      "Compiles C code for a single function using the project toolchain (Docker + agbcc) and shows the objdiff comparison against the target binary.\n" +
      "Use this for fast iteration without running a full ROM build.\n" +
      "Supports standalone_tu objects and included_stub asm snippets by assembling a temporary target object when needed.\n" +
      "Returns: match percentage, instruction diffs, and whether it is a perfect match.",
    parameters: Type.Object({
      functionName: Type.String({
        description:
          "Function name as it appears in the asm file or mizuchi-db (e.g. func_08002468 or asm_08002468)",
      }),
      cCode: Type.String({
        description: "Complete C code to compile and test against the target",
      }),
    }),
    async execute(_id, { functionName, cCode }, _signal, _onUpdate, ctx) {
      const repoRoot = findRepoRoot(ctx.cwd);
      const db = loadDb(repoRoot);
      const fn = findFn(db, functionName);
      let generatedTargetDir = null;

      // Resolve target .o
      let targetObj = fn ? targetObjPath(repoRoot, fn) : null;
      if (!targetObj) {
        const addr = addrFromName(functionName);
        if (addr) targetObj = path.join(repoRoot, "build", "asm", `asm_${addr}.s.o`);
      }

      if (!targetObj || !fs.existsSync(targetObj)) {
        const pf = preflightFunction(repoRoot, fn, functionName, db);
        if (pf.conversionMode === "included_stub" && fn?.asmCode?.trim()) {
          generatedTargetDir = fs.mkdtempSync(path.join(os.tmpdir(), "ww-included-target-"));
          targetObj = path.join(generatedTargetDir, `${functionName}.target.o`);
          assembleAsmTargetInDocker(repoRoot, fn.asmCode, targetObj);
        } else {
          const guidance =
            pf.conversionMode === "standalone_tu"
              ? "This is a standalone TU, but the comparison object is missing. Run one clean Docker build to populate build/asm objects, then retry compile_and_view_asm."
              : "The build layout is not yet supported for isolated comparison. Run preflight_candidate and prefer standalone_tu or included_stub candidates.";
          return {
            content: [
              {
                type: "text",
                text:
                  `ERROR: Target object not found: ${targetObj ?? "unknown"}\n\n` +
                  `${guidance}\n\n` +
                  formatPreflight(pf),
              },
            ],
            details: { error: "target_not_found", targetObj, preflight: pf },
          };
        }
      }

      const tmpDir = fs.mkdtempSync(path.join(os.tmpdir(), "ww-compile-"));
      const cFile = path.join(tmpDir, `${functionName}.c`);
      const compiledObj = path.join(tmpDir, `${functionName}.o`);

      try {
        fs.writeFileSync(cFile, cCode, "utf8");

        // Compile
        const compileScript = path.join(repoRoot, "tools/mizuchi/compile-in-docker.sh");
        execFileSync(compileScript, [cFile, compiledObj, functionName], {
          cwd: repoRoot,
          timeout: 120_000,
          stdio: "pipe",
        });

        // Diff
        const objdiffCli = path.join(repoRoot, "tools/objdiff-cli");
        let diffJson;
        try {
          diffJson = execFileSync(
            objdiffCli,
            [
              "diff",
              "-1", targetObj,
              "-2", compiledObj,
              functionName,
              "--format", "json",
              "-o", "-",
              "-c", "arm.archVersion=v4t",
              "-c", "functionRelocDiffs=none",
            ],
            { cwd: repoRoot, timeout: 30_000, stdio: "pipe" },
          ).toString();
        } catch (diffErr) {
          // objdiff exits non-zero on mismatch — stderr has error, stdout has json
          diffJson = diffErr.stdout?.toString?.() ?? "";
          if (!diffJson) {
            return {
              content: [{ type: "text", text: `objdiff error: ${diffErr.stderr?.toString?.() || diffErr.message}` }],
              details: { error: "objdiff_failed" },
            };
          }
        }

        const result = parseDiffJson(diffJson, functionName);
        const text = formatDiffResult(result, functionName);
        return {
          content: [{ type: "text", text }],
          details: result,
        };
      } catch (err) {
        const stderr = err.stderr?.toString?.() || "";
        const stdout = err.stdout?.toString?.() || "";
        return {
          content: [{ type: "text", text: `COMPILE ERROR:\n${stderr || stdout || err.message}` }],
          details: { error: err.message, stderr, stdout },
        };
      } finally {
        fs.rmSync(tmpDir, { recursive: true, force: true });
        if (generatedTargetDir) fs.rmSync(generatedTargetDir, { recursive: true, force: true });
      }
    },
  });
}

// ── Tool: get_function_context ────────────────────────────────────────────────

function registerGetFunctionContext(pi) {
  pi.registerTool({
    name: "get_function_context",
    label: "Get Function Context",
    description:
      "Returns preprocessed C headers and type definitions for a specific function.\n" +
      "Runs gcc -E inside Docker against the source file that includes the function's asm stub.\n" +
      "Use this to get accurate struct definitions, typedefs, and globals before writing C code.",
    parameters: Type.Object({
      functionName: Type.String({
        description: "Function name (e.g. func_08002468 or asm_08002468)",
      }),
    }),
    async execute(_id, { functionName }, _signal, _onUpdate, ctx) {
      const repoRoot = findRepoRoot(ctx.cwd);
      const script = path.join(repoRoot, "tools/mizuchi/get-context.sh");

      if (!fs.existsSync(script)) {
        return {
          content: [{ type: "text", text: "ERROR: tools/mizuchi/get-context.sh not found" }],
          details: { error: "script_missing" },
        };
      }

      try {
        const output = execFileSync(script, [functionName], {
          cwd: repoRoot,
          timeout: 120_000,
          stdio: "pipe",
        }).toString();

        const lineCount = output.split("\n").length;
        return {
          content: [
            {
              type: "text",
              text: `Context for ${functionName} (${lineCount} lines):\n\n${output}`,
            },
          ],
          details: { lineCount, contextContent: output },
        };
      } catch (err) {
        const stderr = err.stderr?.toString?.() || err.message;

        // Two common failure modes:
        // 1. rg not installed (get-context.sh uses ripgrep)
        // 2. Function is a standalone TU (not #include'd from C) — script can't find source
        const help =
          stderr.includes("rg: command not found")
            ? `get-context.sh needs ripgrep. Install:\n  brew install ripgrep\nor alias: alias rg=grep -r\n\nError: ${stderr}`
            : stderr.includes("Could not find source file")
              ? `Function "${functionName}" is a standalone asm TU — not #include\'d from a C file.` +
                `\nThe context script can only gather headers for functions that are stubs inside C files.` +
                `\nTry reading the asm file directly, or check the include/ directory for shared headers.` +
                `\n\nError: ${stderr}`
              : stderr.includes("Could not find asm stub")
                ? `Function "${functionName}" not found in asm/ directory.` +
                  `\nThe function may be in a subdirectory (e.g. asm/scenes/main_menu/). Check with ls asm/*/.` +
                  `\n\nError: ${stderr}`
                : `Context script failed:\n${stderr}`;
        return {
          content: [{ type: "text", text: help }],
          details: { error: stderr },
        };
      }
    },
  });
}

// ── Tool: preflight_candidate ─────────────────────────────────────────────────

function registerPreflightCandidate(pi) {
  pi.registerTool({
    name: "preflight_candidate",
    label: "Preflight Decomp Candidate",
    description:
      "Classifies a WarioWare decomp candidate before iteration. " +
      "Use this immediately after query_candidates. Only conversionMode=standalone_tu is safe for the generic autonomous src/decomp + linker-swap workflow.",
    parameters: Type.Object({
      functionName: Type.String({
        description: "Function name as shown by query_candidates (e.g. func_08006E94)",
      }),
    }),
    async execute(_id, { functionName }, _signal, _onUpdate, ctx) {
      const repoRoot = findRepoRoot(ctx.cwd);
      const db = loadDb(repoRoot);
      let fn = findFn(db, functionName);
      // If not in db, synthesize a minimal entry from the function name so preflight
      // can still check the filesystem (linker entry, including source, asm existence).
      if (!fn) {
        const addr = addrFromName(functionName);
        if (addr) {
          fn = { name: functionName, asmModulePath: `.mizuchi-asm/asm/asm_${addr}.s`, callsFunctions: [] };
        }
      }
      const pf = preflightFunction(repoRoot, fn, functionName, db);
      return {
        content: [{ type: "text", text: formatPreflight(pf) }],
        details: pf,
      };
    },
  });
}

// ── Tool: m2c_decompile ───────────────────────────────────────────────────────

function registerM2cDecompile(pi) {
  pi.registerTool({
    name: "m2c_decompile",
    label: "m2c Decompile",
    description:
      "Generates an initial C skeleton from assembly using m2c with the GBA target by default (-t gba). " +
      "The result is a starting point, not proof of a match; always verify with compile_and_view_asm.",
    parameters: Type.Object({
      functionName: Type.String({
        description: "Function name (e.g. func_08002468 or asm_08002468)",
      }),
      target: Type.Optional(Type.String({
        description: "m2c target triple/platform (default: gba; examples: gba, gba-gcc-c, arm-gcc-c)",
      })),
    }),
    async execute(_id, { functionName, target = "gba" }, _signal, _onUpdate, ctx) {
      const repoRoot = findRepoRoot(ctx.cwd);
      const db = loadDb(repoRoot);
      let fn = findFn(db, functionName);
      if (!fn) {
        const addr = addrFromName(functionName);
        if (addr) {
          fn = { name: functionName, asmModulePath: `.mizuchi-asm/asm/asm_${addr}.s`, callsFunctions: [] };
        }
      }
      const pf = preflightFunction(repoRoot, fn, functionName);

      const mizuchi = resolveMizuchiRoot(repoRoot);
      if (!mizuchi.root) {
        return {
          content: [
            {
              type: "text",
              text:
                "m2c is unavailable because Mizuchi was not found.\n" +
                `Set one of ${MIZUCHI_ENV_VARS.map((name) => `$${name}`).join(" / ")} or clone Mizuchi beside this repo.\n\n` +
                "Recommended fix:\n" +
                `  ${formatMizuchiRootFix(repoRoot)}\n\n` +
                "Then run /reload and /decomp-setup.",
            },
          ],
          details: { error: "mizuchi_missing", preflight: pf, mizuchi },
        };
      }

      const m2cPy = path.join(mizuchi.root, "vendor/m2c/m2c.py");
      // setup-m2c.sh creates venv at vendor/m2c/.venv
      const venvPython = path.join(mizuchi.root, "vendor/m2c/.venv/bin/python3");

      // Check setup
      if (!fs.existsSync(m2cPy)) {
        return {
          content: [
            {
              type: "text",
              text:
                "m2c not set up. To enable:\n" +
                `  ${formatM2cSetupCommand(mizuchi.root)}\n\n` +
                "Then reload the extension with /reload.",
            },
          ],
          details: { error: "m2c_not_setup", preflight: pf, mizuchi },
        };
      }

      const python = fs.existsSync(venvPython) ? venvPython : "python3";

      // Get asm for the function from mizuchi-db.json
      if (!fn) {
        return {
          content: [{ type: "text", text: `Function "${functionName}" not found in mizuchi-db.json. Run index-codebase first.` }],
          details: { error: "function_not_found", preflight: pf },
        };
      }

      const asmContent = fn.asmCode;
      if (!asmContent?.trim()) {
        return {
          content: [{ type: "text", text: `No asm content found for "${functionName}" in the database.` }],
          details: { error: "no_asm", preflight: pf },
        };
      }

      // Also try to find the actual .mizuchi-asm file for more accurate m2c input
      let asmForM2c = asmContent;
      const asmFile = fn.asmModulePath ? path.join(repoRoot, fn.asmModulePath) : null;
      if (asmFile && fs.existsSync(asmFile)) {
        asmForM2c = fs.readFileSync(asmFile, "utf8");
      }

      // Mizuchi's sanitized asm snippets may omit the leading syntax marker.
      // m2c's ARM/GBA parser requires it before Thumb instructions like LSLS.
      if (!/^\s*\.syntax\s+unified\b/im.test(asmForM2c)) {
        asmForM2c = `.syntax unified\n.thumb\n${asmForM2c}`;
      }

      const tmpDir = fs.mkdtempSync(path.join(os.tmpdir(), "ww-m2c-"));
      const asmTmp = path.join(tmpDir, `${functionName}.s`);

      try {
        fs.writeFileSync(asmTmp, asmForM2c, "utf8");

        const output = execFileSync(python, [m2cPy, "-t", target, asmTmp], {
          cwd: repoRoot,
          timeout: 30_000,
          stdio: "pipe",
        }).toString();

        return {
          content: [
            {
              type: "text",
              text: `m2c initial decompilation of ${functionName} (target: ${target}):\n\n${output}\n\n` +
                `(This is a rule-based starting point. Use compile_and_view_asm to iterate toward a match.)`,
            },
          ],
          details: { generatedCode: output, target, preflight: pf },
        };
      } catch (err) {
        const stderr = err.stderr?.toString?.() || err.message;
        const stdout = err.stdout?.toString?.() || "";
        return {
          content: [{ type: "text", text: `m2c failed for target ${target}:\n${stderr || stdout}` }],
          details: { error: stderr || stdout, target, preflight: pf },
        };
      } finally {
        fs.rmSync(tmpDir, { recursive: true, force: true });
      }
    },
  });
}

// ── Tool: query_candidates ────────────────────────────────────────────────────

function registerQueryCandidates(pi) {
  pi.registerTool({
    name: "query_candidates",
    label: "Query Decomp Candidates",
    description:
      "Returns a ranked list of unmatched functions from the mizuchi-db index.\n" +
      "Strategies:\n" +
      "  smallest    – fewest asm lines (easiest to convert)\n" +
      "  families    – functions that share callers/callees with already-matched functions\n" +
      "  address     – ordered by ROM address (useful for sequential batches)\n" +
      "  random      – random sample for exploration\n" +
      "Default conversionMode is recommended, which includes standalone TUs and included asm stubs that have supported integration workflows.\n" +
      "Optional `family` filter: only return functions in the same source module (e.g. graphics_table).",
    parameters: Type.Object({
      count: Type.Optional(Type.Number({ description: "How many candidates to return (default 10)" })),
      strategy: Type.Optional(
        Type.Union(
          [
            Type.Literal("smallest"),
            Type.Literal("families"),
            Type.Literal("address"),
            Type.Literal("random"),
          ],
          { description: "Ranking strategy (default: smallest)" },
        ),
      ),
      family: Type.Optional(
        Type.String({
          description: "Filter to functions in this source module/folder (e.g. graphics_table, beatscript)",
        }),
      ),
      conversionMode: Type.Optional(
        Type.Union(
          [
            Type.Literal("recommended"),
            Type.Literal("standalone_tu"),
            Type.Literal("included_stub"),
            Type.Literal("unknown_skip"),
            Type.Literal("all"),
          ],
          { description: "Filter by conversion workflow (default: recommended)" },
        ),
      ),
    }),
    async execute(_id, { count = 10, strategy = "smallest", family, conversionMode = "recommended" }, _signal, _onUpdate, ctx) {
      const repoRoot = findRepoRoot(ctx.cwd);
      const db = loadDb(repoRoot);
      if (!db) {
        return {
          content: [{ type: "text", text: `mizuchi-db.json not found. Run: ${formatIndexCodebaseCommand(repoRoot, resolveMizuchiRoot(repoRoot).root)}` }],
          details: { error: "db_missing", mizuchi: resolveMizuchiRoot(repoRoot) },
        };
      }

      const allFns = db.decompFunctions || [];

      // Unmatched = no real C code (empty or asm-only stubs)
      let candidates = allFns.filter((f) => !f.cCode?.trim());

      // Count functions per asm file — one function per .s is a useful first pass,
      // but not sufficient: many one-function .s files are included stubs inside C TUs.
      const pathCounts = new Map();
      for (const f of candidates) {
        const p = f.asmModulePath;
        if (p) pathCounts.set(p, (pathCounts.get(p) ?? 0) + 1);
      }

      candidates = candidates.filter((f) => {
        const count = f.asmModulePath ? (pathCounts.get(f.asmModulePath) ?? 9) : 9;
        return count === 1;
      });

      // Must be a real function (has a function label in asm)
      candidates = candidates.filter((f) =>
        f.asmCode && (
          f.asmCode.includes("thumb_func_start") ||
          f.asmCode.includes("arm_func_start") ||
          f.asmCode.includes(".thumb_func") ||
          f.asmCode.includes("glabel")
        ),
      );

      const preflights = new Map();
      const getPreflight = (f) => {
        if (!preflights.has(f.name)) preflights.set(f.name, preflightFunction(repoRoot, f, f.name, db));
        return preflights.get(f.name);
      };

      const modeCounts = {};
      for (const f of candidates) {
        const mode = getPreflight(f).conversionMode;
        modeCounts[mode] = (modeCounts[mode] ?? 0) + 1;
      }

      // Hide stale already-converted entries from candidate lists. They come from an old
      // mizuchi-db index after a successful conversion and should not be selectable.
      candidates = candidates.filter((f) => getPreflight(f).conversionMode !== "already_converted");

      // Conversion-mode filter. "recommended" includes all layouts this extension can
      // mechanically test/apply today.
      if (conversionMode === "recommended") {
        candidates = candidates.filter((f) => ["standalone_tu", "included_stub"].includes(getPreflight(f).conversionMode));
      } else if (conversionMode !== "all") {
        candidates = candidates.filter((f) => getPreflight(f).conversionMode === conversionMode);
      }

      // Family filter
      if (family) {
        candidates = candidates.filter((f) =>
          (f.asmModulePath ?? "").toLowerCase().includes(family.toLowerCase()),
        );
      }

      // Sort by strategy
      switch (strategy) {
        case "smallest":
          candidates.sort((a, b) => (a.asmCode?.split("\n").length ?? 999) - (b.asmCode?.split("\n").length ?? 999));
          break;
        case "families": {
          // Score = how many of the function's callsFunctions are already matched (have cCode)
          const matchedNames = new Set(allFns.filter((f) => f.cCode?.trim()).map((f) => f.name));
          candidates.sort((a, b) => {
            const aScore = (a.callsFunctions ?? []).filter((n) => matchedNames.has(n)).length;
            const bScore = (b.callsFunctions ?? []).filter((n) => matchedNames.has(n)).length;
            return bScore - aScore; // higher score first
          });
          break;
        }
        case "address":
          candidates.sort((a, b) => {
            const aAddr = parseInt(functionAddr(a, a.name) ?? "0", 16);
            const bAddr = parseInt(functionAddr(b, b.name) ?? "0", 16);
            return aAddr - bAddr;
          });
          break;
        case "random":
          candidates.sort(() => Math.random() - 0.5);
          break;
      }

      const top = candidates.slice(0, count);

      const lines = [];
      lines.push(
        `Mode counts before stale-converted filtering: ${Object.entries(modeCounts).map(([k, v]) => `${k}=${v}`).join(", ") || "none"}`,
      );
      lines.push(
        `Found ${candidates.length} ${conversionMode} unmatched function candidates. Showing top ${top.length} by "${strategy}"${family ? ` in module "${family}"` : ""}:\n`,
      );

      for (const fn of top) {
        const pf = getPreflight(fn);
        const addr = functionAddr(fn, fn.name);
        const asmLines = fn.asmCode?.split("\n").length ?? 0;
        const asmSrc = asmRelPath(fn) ?? "?";
        const targetObj = addr ? `build/asm/asm_${addr}.s.o` : "?";
        const calledMatched = (fn.callsFunctions ?? []).filter(
          (n) => allFns.find((f) => f.name === n)?.cCode?.trim(),
        ).length;
        const calleeRiskTag = pf.calleeRisk ? " ⚠️ CALLEE-RISK" : "";
        lines.push(
          `• ${fn.name}  [${asmLines} asm lines]${calleeRiskTag}\n` +
          `  workflow: ${pf.conversionMode}${pf.safeForAutonomous ? " (supported)" : " (research/manual)"}\n` +
          `  asm: ${asmSrc}\n` +
          `  target: ${targetObj}${pf.targetObjectExists ? " (exists)" : " (not built/found)"}\n` +
          `  included-by: ${pf.includingSource ?? "none"}\n` +
          `  linker: ${pf.linkerEntry ? `line ${pf.linkerEntry.lineNumber}` : "none"}\n` +
          `  matched-calls: ${calledMatched}/${(fn.callsFunctions ?? []).length}` +
          `${pf.calleeRisk ? `\n  ⚠️ calls converted: ${pf.convertedCallees?.slice(0, 2).join(", ")}${(pf.convertedCallees?.length ?? 0) > 2 ? "..." : ""}` : ""}`,
        );
      }

      return {
        content: [{ type: "text", text: lines.join("\n") }],
        details: {
          totalCandidates: candidates.length,
          strategy,
          family: family ?? null,
          conversionMode,
          modeCounts,
          noRecommendedCandidates: conversionMode === "recommended" && candidates.length === 0,
          candidates: top.map((f) => ({
            name: f.name,
            asmLines: f.asmCode?.split("\n").length ?? 0,
            module: f.asmModulePath?.replace(".mizuchi-asm/asm/", "").replace(/\/asm_[^/]+\.s$/, "") ?? null,
            asmCode: f.asmCode ?? null,
            callsFunctions: f.callsFunctions ?? [],
            preflight: getPreflight(f),
          })),
        },
      };
    },
  });
}

// ── Tool: apply_conversion ────────────────────────────────────────────────────

function restoreFiles(repoRoot, backups) {
  for (const [rel, content] of [...backups].reverse()) {
    const abs = path.join(repoRoot, rel);
    if (content == null) {
      fs.rmSync(abs, { force: true });
    } else {
      fs.mkdirSync(path.dirname(abs), { recursive: true });
      fs.writeFileSync(abs, content, "utf8");
    }
  }
}

function registerApplyConversion(pi) {
  pi.registerTool({
    name: "apply_conversion",
    label: "Apply Decomp Conversion",
    description:
      "Mechanically applies a verified conversion. standalone_tu uses src/decomp + linker swap; included_stub uses a guarded include-shim in the original host C TU to preserve ROM order. " +
      "Runs a clean Docker ROM build by default and auto-restores files if verification fails.",
    parameters: Type.Object({
      functionName: Type.String({
        description: "Function name that preflight_candidate classified as standalone_tu or included_stub",
      }),
      cCode: Type.String({
        description: "Complete C source for src/decomp/asm_xxxxxxxx.c (should already be 100% in compile_and_view_asm)",
      }),
      verify: Type.Optional(Type.Boolean({
        description: "Run clean Docker make -j4 and require wariowareinc.gba: OK (default: true)",
      })),
      dryRun: Type.Optional(Type.Boolean({
        description: "Show the planned edits without changing files (default: false)",
      })),
    }),
    async execute(_id, { functionName, cCode, verify = true, dryRun = false }, _signal, _onUpdate, ctx) {
      const repoRoot = findRepoRoot(ctx.cwd);
      const db = loadDb(repoRoot);
      let fn = findFn(db, functionName);
      // If not in db, synthesize a minimal entry so preflight can check the filesystem.
      if (!fn) {
        const addr = addrFromName(functionName);
        if (addr) {
          fn = { name: functionName, asmModulePath: `.mizuchi-asm/asm/asm_${addr}.s`, callsFunctions: [] };
        }
      }
      const pf = preflightFunction(repoRoot, fn, functionName, db);

      if (!["standalone_tu", "included_stub"].includes(pf.conversionMode)) {
        return {
          content: [
            {
              type: "text",
              text:
                `Refusing to apply conversion for ${functionName}: workflow is ${pf.conversionMode}; supported workflows are standalone_tu and included_stub.\n\n` +
                formatPreflight(pf),
            },
          ],
          details: { error: "unsupported_conversion_mode", preflight: pf },
        };
      }

      const addr = pf.addr;
      const asmRel = pf.asmRel;
      const targetObjRel = pf.targetObjRel;
      const decompRel = pf.decompRel;
      const convertedRel = pf.convertedAsmRel;
      const decompObjRel = `build/src/decomp/asm_${addr}.c.o`;
      const includeLine = `#include "${asmRel}"`;
        // Compute the correct relative include path from the host source directory to the decomp file
  const hostDir = path.dirname(pf.includingSource); // e.g. "src/scenes" or "src"
  const decompFile = `src/decomp/asm_${addr}.c`;
  const decompIncludePath = path.relative(hostDir, decompFile).replace(/\\/g, "/"); // e.g. "../decomp/asm_0801214c.c"
  const decompIncludeLine = `#include "${decompIncludePath}"`;
      const workflow = pf.conversionMode;

      const plan = workflow === "standalone_tu"
        ? [
            `write ${decompRel}`,
            `replace ${targetObjRel} -> ${decompObjRel} in wariowareinc.ld`,
            `move ${asmRel} -> ${convertedRel}`,
            verify ? "run clean Docker build and require wariowareinc.gba: OK" : "skip build verification",
          ]
        : [
            `write guarded include-shim ${decompRel}`,
            `replace ${includeLine} -> ${decompIncludeLine} in ${pf.includingSource}`,
            `move ${asmRel} -> ${convertedRel}`,
            "leave wariowareinc.ld unchanged so host TU order is preserved",
            verify ? "run clean Docker build and require wariowareinc.gba: OK" : "skip build verification",
          ];

      if (dryRun) {
        return {
          content: [{ type: "text", text: `Dry-run apply_conversion plan for ${functionName} (${workflow}):\n` + plan.map((x) => `  - ${x}`).join("\n") + "\n\n" + formatPreflight(pf) }],
          details: { dryRun: true, workflow, plan, preflight: pf },
        };
      }

      const backups = [];
      const backup = (rel) => {
        const abs = path.join(repoRoot, rel);
        backups.push([rel, fs.existsSync(abs) ? fs.readFileSync(abs, "utf8") : null]);
      };

      try {
        backup(decompRel);
        backup(asmRel);
        backup(convertedRel);
        if (workflow === "standalone_tu") backup("wariowareinc.ld");
        if (workflow === "included_stub") backup(pf.includingSource);

        fs.mkdirSync(path.dirname(path.join(repoRoot, decompRel)), { recursive: true });
        fs.writeFileSync(
          path.join(repoRoot, decompRel),
          workflow === "included_stub"
            ? wrapIncludedStubCode(cCode)
            : (cCode.endsWith("\n") ? cCode : cCode + "\n"),
          "utf8",
        );

        if (workflow === "standalone_tu") {
          const ldPath = path.join(repoRoot, "wariowareinc.ld");
          const ldText = fs.readFileSync(ldPath, "utf8");
          if (!ldText.includes(targetObjRel)) {
            throw new Error(`linker entry disappeared before edit: ${targetObjRel}`);
          }
          fs.writeFileSync(ldPath, ldText.replace(targetObjRel, decompObjRel), "utf8");
        } else {
          const sourcePath = path.join(repoRoot, pf.includingSource);
          const sourceText = fs.readFileSync(sourcePath, "utf8");
          if (!sourceText.includes(includeLine)) {
            throw new Error(`source include disappeared before edit: ${includeLine}`);
          }
          fs.writeFileSync(sourcePath, sourceText.replace(includeLine, decompIncludeLine), "utf8");
        }

        fs.mkdirSync(path.dirname(path.join(repoRoot, convertedRel)), { recursive: true });
        fs.renameSync(path.join(repoRoot, asmRel), path.join(repoRoot, convertedRel));

        let buildOutput = "";
        if (verify) {
          buildOutput = execFileSync(
            "docker",
            [
              "run", "--rm",
              "-v", `${repoRoot}:/workspace`,
              "-w", "/workspace",
              "devkitpro/devkitarm:latest",
              "bash", "-lc",
              "set -euo pipefail; rm -rf build; make -j4",
            ],
            { cwd: repoRoot, timeout: 600_000, stdio: "pipe" },
          ).toString();
          if (!buildOutput.includes("wariowareinc.gba: OK")) {
            throw new Error("clean Docker build finished without wariowareinc.gba: OK\n" + buildOutput.split("\n").slice(-20).join("\n"));
          }

      // Run make report to regenerate build/report.json for accurate metric tracking
      execFileSync(
        "docker",
        [
          "run", "--rm", "-v", `${repoRoot}:/workspace`, "-w", "/workspace",
          "devkitpro/devkitarm:latest", "bash", "-lc",
          "set -euo pipefail; make report",
        ],
        { cwd: repoRoot, timeout: 120_000, stdio: "pipe" },
      ).toString();
        }

        return {
          content: [
            {
              type: "text",
              text:
                `✅ Applied ${workflow} conversion for ${functionName}.\n` +
                plan.map((x) => `  - ${x}`).join("\n") +
                (verify ? `\n\nBuild tail:\n${buildOutput.split("\n").slice(-6).join("\n")}` : ""),
            },
          ],
          details: { ok: true, workflow, plan, preflight: pf, buildOutputTail: buildOutput.split("\n").slice(-20).join("\n") },
        };
      } catch (err) {
        restoreFiles(repoRoot, backups);
        return {
          content: [
            {
              type: "text",
              text:
                `❌ apply_conversion failed for ${functionName}; restored edited files.\n\n` +
                `${err.stderr?.toString?.() || err.message}`,
            },
          ],
          details: { error: err.message, stderr: err.stderr?.toString?.() ?? "", restored: true, workflow, preflight: pf },
        };
      }
    },
  });
}

// ── Command: /decomp-setup ────────────────────────────────────────────────────

function registerSetupCommand(pi) {
  const runSetupCheck = async (ctx) => {
    const repoRoot = findRepoRoot(ctx.cwd);
    const mizuchi = resolveMizuchiRoot(repoRoot);
    const dbExists = fs.existsSync(path.join(repoRoot, DB_FILE));
    const checks = [];

    const check = (label, ok, fix, options = {}) => {
      checks.push({ label, ok, fix, required: options.required !== false });
      return ok;
    };

    let dockerOk = false;
    try {
      execSync("docker info", { stdio: "pipe", timeout: 10_000 });
      dockerOk = true;
    } catch {}

    check("Docker running", dockerOk, "Start Docker Desktop");
    check("python3", commandExists("python3"), "Install python3");
    check(
      "Mizuchi root (for reindexing / Atlas / m2c)",
      !!mizuchi.root,
      `${formatMizuchiRootFix(repoRoot)}`,
      { required: !dbExists },
    );
    check(
      "npm (for reindexing / Atlas)",
      commandExists("npm"),
      "Install npm / Node.js",
      { required: false },
    );
    check(
      "tools/mizuchi/compile-in-docker.sh",
      fs.existsSync(path.join(repoRoot, "tools/mizuchi/compile-in-docker.sh")),
      "Missing — check git status",
    );
    check(
      "tools/mizuchi/get-context.sh",
      fs.existsSync(path.join(repoRoot, "tools/mizuchi/get-context.sh")),
      "Missing — check git status",
    );
    check(
      "tools/mizuchi/export-asm.py",
      fs.existsSync(path.join(repoRoot, "tools/mizuchi/export-asm.py")),
      "Missing — check git status",
    );
    check(
      ".mizuchi-asm/asm mirror (recommended)",
      fs.existsSync(path.join(repoRoot, ".mizuchi-asm", "asm")),
      `Run: python3 ${shellQuote(path.join(repoRoot, "tools", "mizuchi", "export-asm.py"))}`,
      { required: false },
    );
    check(
      "tools/objdiff-cli (executable)",
      fs.existsSync(path.join(repoRoot, "tools/objdiff-cli")) &&
        (fs.statSync(path.join(repoRoot, "tools/objdiff-cli")).mode & 0o111) !== 0,
      "chmod +x tools/objdiff-cli",
    );
    check(
      "mizuchi-db.json",
      dbExists,
      `Run: ${formatIndexCodebaseCommand(repoRoot, mizuchi.root)}`,
    );

    const buildAsmDir = path.join(repoRoot, "build/asm");
    const hasBuildObjs =
      fs.existsSync(buildAsmDir) && fs.readdirSync(buildAsmDir).some((f) => f.endsWith(".s.o"));
    check(
      "build/asm/*.s.o objects (required for standalone_tu compile_and_view_asm)",
      hasBuildObjs,
      "Run: docker run --rm -v \"$PWD:/workspace\" -w /workspace devkitpro/devkitarm:latest bash -lc 'set -euo pipefail; rm -rf build; make -j4'",
    );
    check(
      "m2c (optional — GBA target skeletons)",
      !!mizuchi.root && fs.existsSync(path.join(mizuchi.root, "vendor/m2c/m2c.py")),
      `Run: ${formatM2cSetupCommand(mizuchi.root)}`,
      { required: false },
    );

    const requiredChecks = checks.filter((c) => c.required);
    const allRequired = requiredChecks.every((c) => c.ok);
    const lines = [
      allRequired ? "✅ WarioWare decomp machine health: ready" : "⚠️  WarioWare decomp machine health: action needed",
      `repo: ${repoRoot}`,
      `mizuchi: ${mizuchi.root ? `${mizuchi.root} (${mizuchi.source})` : "not found"}`,
      "first-run: /reload -> /decomp-setup -> /decomp-verify",
    ];

    if (mizuchi.envProblems.length) {
      lines.push(`env warnings: ${mizuchi.envProblems.join("; ")}`);
    }

    lines.push("", "Required checks:");
    for (const c of checks.filter((c) => c.required)) {
      lines.push(`  ${c.ok ? "✅" : "❌"} ${c.label}${!c.ok ? `\n      Fix: ${c.fix}` : ""}`);
    }

    lines.push("", "Optional checks:");
    for (const c of checks.filter((c) => !c.required)) {
      lines.push(`  ${c.ok ? "✅" : "⚪"} ${c.label}${!c.ok ? `\n      Fix: ${c.fix}` : ""}`);
    }

    const reportText = lines.join("\n");
    ctx.ui.notify(reportText, allRequired ? "info" : "warning");
    pi.sendMessage({
      customType: "warioware-decomp-setup-report",
      content: reportText,
      display: true,
      details: {
        repoRoot,
        mizuchi,
        allRequired,
        checks,
      },
    }, {
      deliverAs: "nextTurn",
    });
  };

  pi.registerCommand("decomp-setup", {
    description: "Check WarioWare decomp prerequisites and machine portability health",
    handler: async (_args, ctx) => runSetupCheck(ctx),
  });

  pi.registerCommand("decomp-health", {
    description: "Alias for /decomp-setup",
    handler: async (_args, ctx) => runSetupCheck(ctx),
  });
}

function registerVerifyCommands(pi) {
  pi.registerCommand("decomp-verify", {
    description: "Run clean Docker build, Docker make report, and objdiff refresh",
    handler: async (_args, ctx) => {
      const repoRoot = findRepoRoot(ctx.cwd);
      try {
        const buildOutput = runDockerShell(repoRoot, "set -euo pipefail; rm -rf build; make -j4", 600_000);
        if (!buildOutput.includes("wariowareinc.gba: OK")) {
          throw new Error("clean Docker build finished without wariowareinc.gba: OK");
        }

        const reportOutput = runDockerShell(repoRoot, "set -euo pipefail; make report", 180_000);
        const objdiffOutput = execFileSync("python3", ["tools/gen_objdiff.py"], {
          cwd: repoRoot,
          timeout: 120_000,
          stdio: "pipe",
        }).toString();

        const report = JSON.parse(fs.readFileSync(path.join(repoRoot, "build", "report.json"), "utf8"));
        const measures = report?.measures ?? {};

        ctx.ui.notify(
          [
            "✅ Docker verify complete",
            `matched_functions: ${measures.matched_functions}/${measures.total_functions} (${Number(measures.matched_functions_percent ?? 0).toFixed(3)}%)`,
            `matched_code: ${Number(measures.matched_code_percent ?? 0).toFixed(4)}%`,
            ...objdiffOutput.trim().split(/\r?\n/).slice(-2),
          ].join("\n"),
          "info",
        );
      } catch (err) {
        ctx.ui.notify(
          `❌ /decomp-verify failed\n${err.stderr?.toString?.() || err.message}`,
          "warning",
        );
      }
    },
  });

  pi.registerCommand("decomp-report", {
    description: "Run Docker make report and refresh objdiff metrics",
    handler: async (_args, ctx) => {
      const repoRoot = findRepoRoot(ctx.cwd);
      try {
        const reportOutput = runDockerShell(repoRoot, "set -euo pipefail; make report", 180_000);
        const objdiffOutput = execFileSync("python3", ["tools/gen_objdiff.py"], {
          cwd: repoRoot,
          timeout: 120_000,
          stdio: "pipe",
        }).toString();
        const report = JSON.parse(fs.readFileSync(path.join(repoRoot, "build", "report.json"), "utf8"));
        const measures = report?.measures ?? {};
        ctx.ui.notify(
          [
            "✅ Docker report refreshed",
            `matched_functions: ${measures.matched_functions}/${measures.total_functions} (${Number(measures.matched_functions_percent ?? 0).toFixed(3)}%)`,
            `matched_code: ${Number(measures.matched_code_percent ?? 0).toFixed(4)}%`,
            ...objdiffOutput.trim().split(/\r?\n/).slice(-2),
          ].join("\n"),
          "info",
        );
      } catch (err) {
        ctx.ui.notify(
          `❌ /decomp-report failed\n${err.stderr?.toString?.() || err.message}`,
          "warning",
        );
      }
    },
  });
}

// ── Tool: decomp_siblings ─────────────────────────────────────────────────────

function registerDecompSiblings(pi) {
  pi.registerTool({
    name: "decomp_siblings",
    label: "Find Decomp Siblings",
    description:
      "Finds functions similar to a matched one for batch conversion opportunities.\n" +
      "Strategies:\n" +
      "  same_file      – Functions in the same asm file (best for family conversion)\n" +
      "  same_module    – Functions in the same source module (e.g., graphics_table, beatscript)\n" +
      "  callers        – Functions that call this one\n" +
      "  callees        – Functions that this one calls\n" +
      "  pattern        – Functions with similar instruction patterns (loop, conditional, etc.)\n" +
      "Returns a ranked list with preflight info ready for conversion.",
    parameters: Type.Object({
      functionName: Type.String({
        description: "Function name to find siblings for (e.g. func_080024E4)",
      }),
      strategy: Type.Optional(
        Type.Union(
          [
            Type.Literal("same_file"),
            Type.Literal("same_module"),
            Type.Literal("callers"),
            Type.Literal("callees"),
            Type.Literal("pattern"),
          ],
          { description: "Sibling detection strategy (default: same_file)" },
        ),
      ),
      count: Type.Optional(Type.Number({ description: "How many siblings to return (default 10)" })),
    }),
    async execute(_id, { functionName, strategy = "same_file", count = 10 }, _signal, _onUpdate, ctx) {
      const repoRoot = findRepoRoot(ctx.cwd);
      const db = loadDb(repoRoot);

      if (!db) {
        return {
          content: [{ type: "text", text: `mizuchi-db.json not found. Run: ${formatIndexCodebaseCommand(repoRoot, resolveMizuchiRoot(repoRoot).root)}` }],
          details: { error: "db_missing" },
        };
      }

      const allFns = db.decompFunctions || [];
      const targetFn = findFn(db, functionName);

      if (!targetFn) {
        return {
          content: [{ type: "text", text: `Function "${functionName}" not found in mizuchi-db.json. Check the name and run index-codebase if needed.` }],
          details: { error: "function_not_found" },
        };
      }

      // Unmatched = no real C code (empty or asm-only stubs)
      const unmatchedFns = allFns.filter((f) => !f.cCode?.trim());

      let siblings = [];
      const pf = preflightFunction(repoRoot, targetFn, functionName);

      switch (strategy) {
        case "same_file": {
          // Find functions in the same asm file
          const targetAsmPath = targetFn.asmModulePath;
          if (targetAsmPath) {
            siblings = unmatchedFns.filter((f) => {
              const fPath = f.asmModulePath;
              return fPath && path.dirname(fPath) === path.dirname(targetAsmPath);
            });
          }
          // Sort by asm line count (smallest first)
          siblings.sort((a, b) => (a.asmCode?.split("\n").length ?? 999) - (b.asmCode?.split("\n").length ?? 999));
          break;
        }

        case "same_module": {
          // Extract module name from path (e.g., "graphics_table" from "asm/graphics_table/asm_080024e4.s")
          const targetModule = targetFn.asmModulePath?.replace(".mizuchi-asm/asm/", "").split("/")[0];
          if (targetModule) {
            siblings = unmatchedFns.filter((f) => {
              const fModule = f.asmModulePath?.replace(".mizuchi-asm/asm/", "").split("/")[0];
              return fModule === targetModule;
            });
          }
          siblings.sort((a, b) => (a.asmCode?.split("\n").length ?? 999) - (b.asmCode?.split("\n").length ?? 999));
          break;
        }

        case "callers": {
          // Find functions that call this one
          const targetAddr = functionAddr(targetFn, functionName);
          siblings = unmatchedFns.filter((f) => {
            const calls = f.callsFunctions || [];
            return calls.some((n) => n === functionName || n === targetAddr);
          });
          // Sort by how many matched functions they also call
          const matchedNames = new Set(allFns.filter((f) => f.cCode?.trim()).map((f) => f.name));
          siblings.sort((a, b) => {
            const aMatchedCalls = (a.callsFunctions ?? []).filter((n) => matchedNames.has(n)).length;
            const bMatchedCalls = (b.callsFunctions ?? []).filter((n) => matchedNames.has(n)).length;
            return bMatchedCalls - aMatchedCalls;
          });
          break;
        }

        case "callees": {
          // Find functions that this one calls
          const targetCalls = targetFn.callsFunctions || [];
          siblings = unmatchedFns.filter((f) => {
            const fAddr = functionAddr(f, f.name);
            return targetCalls.some((n) => n === f.name || n === fAddr);
          });
          siblings.sort((a, b) => (a.asmCode?.split("\n").length ?? 999) - (b.asmCode?.split("\n").length ?? 999));
          break;
        }

        case "pattern": {
          // Find functions with similar instruction patterns
          // Heuristic: similar instruction sequences, loop structures, etc.
          const targetPattern = extractPattern(targetFn.asmCode ?? "");
          siblings = unmatchedFns.filter((f) => {
            const fPattern = extractPattern(f.asmCode ?? "");
            // Check for similar patterns (same instruction types, similar structure)
            return (
              fPattern.hasLoop === targetPattern.hasLoop &&
              fPattern.hasConditional === targetPattern.hasConditional &&
              fPattern.instructionTypes === targetPattern.instructionTypes &&
              Math.abs((f.asmCode?.split("\n").length ?? 0) - (targetFn.asmCode?.split("\n").length ?? 0)) <= 5
            );
          });
          // Sort by similarity score (line count difference as proxy)
          siblings.sort((a, b) => {
            const aDiff = Math.abs((a.asmCode?.split("\n").length ?? 0) - (targetFn.asmCode?.split("\n").length ?? 0));
            const bDiff = Math.abs((b.asmCode?.split("\n").length ?? 0) - (targetFn.asmCode?.split("\n").length ?? 0));
            return aDiff - bDiff;
          });
          break;
        }
      }

      // Filter out already_converted functions
      siblings = siblings.filter((f) => {
        const fPf = preflightFunction(repoRoot, f, f.name);
        return fPf.conversionMode !== "already_converted";
      });

      // Prefer supported workflows
      const supportedSiblings = siblings.filter((f) => {
        const fPf = preflightFunction(repoRoot, f, f.name);
        return fPf.safeForAutonomous;
      });

      // If we have enough supported siblings, use those; otherwise mix in research candidates
      const finalSiblings = supportedSiblings.length >= count ? supportedSiblings.slice(0, count) : siblings.slice(0, count);

      const lines = [
        `Siblings of ${functionName} (${strategy} strategy)`,
        `  Target workflow: ${pf.conversionMode}${pf.safeForAutonomous ? " ✅" : " ⚠️"}`,
        `  Found ${siblings.length} unmatched siblings (${supportedSiblings.length} supported)\n`,
      ];

      for (const fn of finalSiblings) {
        const fPf = preflightFunction(repoRoot, fn, fn.name, db);
        const addr = functionAddr(fn, fn.name);
        const asmLines = fn.asmCode?.split("\n").length ?? 0;
        const asmSrc = asmRelPath(fn) ?? "?";
        const targetObj = addr ? `build/asm/asm_${addr}.s.o` : "?";
        const calledMatched = (fn.callsFunctions ?? []).filter(
          (n) => allFns.find((f) => f.name === n)?.cCode?.trim(),
        ).length;
        const calleeRiskTag = fPf.calleeRisk ? " ⚠️ CALLEE-RISK" : "";

        lines.push(
          `• ${fn.name}  [${asmLines} asm lines]${calleeRiskTag}
` +
          `  workflow: ${fPf.conversionMode}${fPf.safeForAutonomous ? " (supported)" : " (research/manual)"}
` +
          `  asm: ${asmSrc}
` +
          `  target: ${targetObj}${fPf.targetObjectExists ? " (exists)" : " (not built/found)"}
` +
          `  matched-calls: ${calledMatched}/${(fn.callsFunctions ?? []).length}
` +
          `  calls: ${(fn.callsFunctions ?? []).slice(0, 3).join(", ")}${(fn.callsFunctions?.length ?? 0) > 3 ? "..." : ""}${fPf.calleeRisk ? `
  callee-risk: calls ${fPf.convertedCallees?.slice(0, 2).join(", ") || "unknown C functions"}${(fPf.convertedCallees?.length ?? 0) > 2 ? "..." : ""}` : ""}`,
        );
      }

      if (finalSiblings.length === 0) {
        lines.push("No unmatched siblings found with this strategy.");
        lines.push("  Try a different strategy or run query_candidates for general exploration.");
      }

      return {
        content: [{ type: "text", text: lines.join("\n") }],
        details: {
          functionName,
          strategy,
          totalSiblings: siblings.length,
          supportedSiblings: supportedSiblings.length,
          siblings: finalSiblings.map((f) => ({
            name: f.name,
            asmLines: f.asmCode?.split("\n").length ?? 0,
            preflight: preflightFunction(repoRoot, f, f.name),
            callsFunctions: f.callsFunctions ?? [],
          })),
        },
      };
    },
  });
}

// Helper to extract pattern features from asm code
function extractPattern(asmCode) {
  const lines = asmCode.split("\n");
  const hasLoop = lines.some((l) => /\bb(ne|eq|gt|lt|ge|le|cc|cs|hi|ls)\b/i.test(l));
  const hasConditional = lines.some((l) => /\bcmp\b/i.test(l));
  const instrTypes = new Set();
  for (const line of lines) {
    const m = line.match(/^\s*[/*]*\s*([a-z]+)\s+/i);
    if (m) instrTypes.add(m[1].toLowerCase());
  }
  return {
    hasLoop,
    hasConditional,
    instructionTypes: Array.from(instrTypes).sort().join(","),
  };
}

// ── Export ────────────────────────────────────────────────────────────────────

export default function wariowareDecompTools(pi) {
  registerCompileAndViewAsm(pi);
  registerGetFunctionContext(pi);
  registerPreflightCandidate(pi);
  registerM2cDecompile(pi);
  registerQueryCandidates(pi);
  registerDecompSiblings(pi);
  registerApplyConversion(pi);
  registerSetupCommand(pi);
  registerVerifyCommands(pi);
}
