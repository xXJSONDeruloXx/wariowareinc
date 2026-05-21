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
const MIZUCHI_ROOT = "/Users/kurt/Developer/mizuchi";

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

// ── Mizuchi DB ────────────────────────────────────────────────────────────────

let _dbCache = null;
let _dbCacheFile = null;

function loadDb(repoRoot) {
  const dbFile = path.join(repoRoot, DB_FILE);
  if (_dbCache && _dbCacheFile === dbFile) return _dbCache;
  if (!fs.existsSync(dbFile)) return null;
  _dbCache = JSON.parse(fs.readFileSync(dbFile, "utf8"));
  _dbCacheFile = dbFile;
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

function preflightFunction(repoRoot, fn, fallbackName) {
  const name = fn?.name ?? fallbackName;
  const addr = functionAddr(fn, fallbackName);
  const asmRel = asmRelPath(fn);
  const asmAbs = asmRel ? path.join(repoRoot, asmRel) : null;
  const targetObjRel = addr ? `build/asm/asm_${addr}.s.o` : null;
  const targetObj = targetObjRel ? path.join(repoRoot, targetObjRel) : null;
  const includingSource = findIncludingSource(repoRoot, asmRel);
  const linkerEntry = linkerEntryFor(repoRoot, targetObjRel);
  const decompRel = addr ? `src/decomp/asm_${addr}.c` : null;
  const convertedAsmRel = addr ? `asm/converted/asm_${addr}.s` : null;

  let conversionMode = "unknown_skip";
  let safeForAutonomous = false;
  const reasons = [];
  const nextSteps = [];

  if (!fn) {
    reasons.push("function was not found in mizuchi-db.json");
    nextSteps.push("Run the Mizuchi indexer or pick a candidate from query_candidates.");
  } else if (includingSource) {
    conversionMode = "included_stub";
    reasons.push(`asm stub is included by ${includingSource}`);
    reasons.push("the generic src/decomp + linker-swap workflow changes object layout for included stubs");
    nextSteps.push("Skip this candidate for autonomous chunks unless an included-stub-specific workflow is implemented.");
  } else if (linkerEntry) {
    conversionMode = "standalone_tu";
    safeForAutonomous = true;
    reasons.push(`linker script contains ${targetObjRel}`);
    nextSteps.push("Use compile_and_view_asm until 100%, then apply_conversion for the mechanical edits and ROM check.");
  } else {
    reasons.push("no including C source and no matching linker-script object entry were found");
    nextSteps.push("Do not mechanically convert this candidate; inspect the build layout first or pick a standalone_tu candidate.");
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
    conversionMode,
    safeForAutonomous,
    decompRel,
    convertedAsmRel,
    reasons,
    nextSteps,
  };
}

function formatPreflight(pf) {
  const lines = [
    `Preflight for ${pf.name}: ${pf.conversionMode}${pf.safeForAutonomous ? " ✅" : " ⚠️"}`,
    `  asm: ${pf.asmRel ?? "?"}${pf.asmExists ? "" : " (missing)"}`,
    `  target object: ${pf.targetObjRel ?? "?"}${pf.targetObjectExists ? " (exists)" : " (not built/found)"}`,
    `  including source: ${pf.includingSource ?? "none"}`,
    `  linker entry: ${pf.linkerEntry ? `${pf.linkerEntry.lineNumber}: ${pf.linkerEntry.text}` : "none"}`,
    `  planned C: ${pf.decompRel ?? "?"}`,
    `  planned converted asm: ${pf.convertedAsmRel ?? "?"}`,
  ];
  if (pf.reasons.length) lines.push("\nReasons:\n" + pf.reasons.map((r) => `  - ${r}`).join("\n"));
  if (pf.nextSteps.length) lines.push("\nNext steps:\n" + pf.nextSteps.map((r) => `  - ${r}`).join("\n"));
  return lines.join("\n");
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
      "Compiles C code for a single standalone_tu function using the project toolchain (Docker + agbcc) and shows the objdiff comparison against the target binary.\n" +
      "Use this for fast iteration without running a full ROM build.\n" +
      "Returns: match percentage, instruction diffs, and whether it is a perfect match.\n" +
      "Requires: Docker running and a standalone_tu target object in build/asm. Included stubs are rejected/guided by preflight output.",
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

      // Resolve target .o
      let targetObj = fn ? targetObjPath(repoRoot, fn) : null;
      if (!targetObj) {
        const addr = addrFromName(functionName);
        if (addr) targetObj = path.join(repoRoot, "build", "asm", `asm_${addr}.s.o`);
      }

      if (!targetObj || !fs.existsSync(targetObj)) {
        const pf = preflightFunction(repoRoot, fn, functionName);
        const guidance =
          pf.conversionMode === "included_stub"
            ? "This is an included asm stub, so a full build will not create the flattened build/asm target object. Pick a standalone_tu candidate for the normal autonomous workflow."
            : pf.conversionMode === "standalone_tu"
              ? "This is a standalone TU, but the comparison object is missing. Run one clean Docker build to populate build/asm objects, then retry compile_and_view_asm."
              : "The build layout is unknown for this function. Run preflight_candidate and pick a standalone_tu candidate before iterating.";
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
      const fn = findFn(db, functionName);
      const pf = preflightFunction(repoRoot, fn, functionName);
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
      const fn = findFn(db, functionName);
      const pf = preflightFunction(repoRoot, fn, functionName);

      const m2cPy = path.join(MIZUCHI_ROOT, "vendor/m2c/m2c.py");
      // setup-m2c.sh creates venv at vendor/m2c/.venv
      const venvPython = path.join(MIZUCHI_ROOT, "vendor/m2c/.venv/bin/python3");

      // Check setup
      if (!fs.existsSync(m2cPy)) {
        return {
          content: [
            {
              type: "text",
              text:
                "m2c not set up. To enable:\n" +
                "  cd /Users/kurt/Developer/mizuchi\n" +
                "  git clone https://github.com/matt-kempster/m2c.git vendor/m2c\n" +
                "  ./scripts/setup-m2c.sh\n\n" +
                "Then reload the extension with /reload.",
            },
          ],
          details: { error: "m2c_not_setup", preflight: pf },
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
      "Default conversionMode is standalone_tu, which excludes included asm stubs that break the generic linker-swap workflow.\n" +
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
          [Type.Literal("standalone_tu"), Type.Literal("included_stub"), Type.Literal("all")],
          { description: "Filter by safe conversion workflow (default: standalone_tu)" },
        ),
      ),
    }),
    async execute(_id, { count = 10, strategy = "smallest", family, conversionMode = "standalone_tu" }, _signal, _onUpdate, ctx) {
      const repoRoot = findRepoRoot(ctx.cwd);
      const db = loadDb(repoRoot);
      if (!db) {
        return {
          content: [{ type: "text", text: "mizuchi-db.json not found. Run: cd /Users/kurt/Developer/mizuchi && npm start -- index-codebase --config /path/to/mizuchi.yaml" }],
          details: { error: "db_missing" },
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

      // Must be a real function (has thumb_func_start or arm_func_start in asm)
      candidates = candidates.filter((f) =>
        f.asmCode && (
          f.asmCode.includes("thumb_func_start") ||
          f.asmCode.includes("arm_func_start")
        ),
      );

      const preflights = new Map();
      const getPreflight = (f) => {
        if (!preflights.has(f.name)) preflights.set(f.name, preflightFunction(repoRoot, f, f.name));
        return preflights.get(f.name);
      };

      // Conversion-mode filter. Autonomous chunks should use standalone_tu only.
      if (conversionMode !== "all") {
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

      const lines = [
        `Found ${candidates.length} ${conversionMode} unmatched function candidates. Showing top ${top.length} by "${strategy}"${family ? ` in module "${family}"` : ""}:\n`,
      ];

      for (const fn of top) {
        const pf = getPreflight(fn);
        const addr = functionAddr(fn, fn.name);
        const asmLines = fn.asmCode?.split("\n").length ?? 0;
        const asmSrc = asmRelPath(fn) ?? "?";
        const targetObj = addr ? `build/asm/asm_${addr}.s.o` : "?";
        const calledMatched = (fn.callsFunctions ?? []).filter(
          (n) => allFns.find((f) => f.name === n)?.cCode?.trim(),
        ).length;
        lines.push(
          `• ${fn.name}  [${asmLines} asm lines]\n` +
          `  workflow: ${pf.conversionMode}${pf.safeForAutonomous ? " (safe)" : " (skip/default-unsafe)"}\n` +
          `  asm: ${asmSrc}\n` +
          `  target: ${targetObj}${pf.targetObjectExists ? " (exists)" : " (not built/found)"}\n` +
          `  included-by: ${pf.includingSource ?? "none"}\n` +
          `  linker: ${pf.linkerEntry ? `line ${pf.linkerEntry.lineNumber}` : "none"}\n` +
          `  matched-calls: ${calledMatched}/${(fn.callsFunctions ?? []).length}`,
        );
      }

      return {
        content: [{ type: "text", text: lines.join("\n") }],
        details: {
          totalCandidates: candidates.length,
          strategy,
          family: family ?? null,
          conversionMode,
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
    label: "Apply Standalone Decomp Conversion",
    description:
      "Mechanically applies a verified standalone-TU conversion: writes src/decomp/asm_xxxxxxxx.c, swaps the linker entry, moves the original asm to asm/converted, and optionally runs a clean Docker ROM build. " +
      "It refuses included_stub candidates and auto-restores files if verification fails.",
    parameters: Type.Object({
      functionName: Type.String({
        description: "Function name that preflight_candidate classified as standalone_tu",
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
      const fn = findFn(db, functionName);
      const pf = preflightFunction(repoRoot, fn, functionName);

      if (pf.conversionMode !== "standalone_tu") {
        return {
          content: [
            {
              type: "text",
              text:
                `Refusing to apply conversion for ${functionName}: workflow is ${pf.conversionMode}, not standalone_tu.\n\n` +
                formatPreflight(pf),
            },
          ],
          details: { error: "unsafe_conversion_mode", preflight: pf },
        };
      }

      const addr = pf.addr;
      const asmRel = pf.asmRel;
      const targetObjRel = pf.targetObjRel;
      const decompRel = pf.decompRel;
      const convertedRel = pf.convertedAsmRel;
      const decompObjRel = `build/src/decomp/asm_${addr}.c.o`;

      const plan = [
        `write ${decompRel}`,
        `replace ${targetObjRel} -> ${decompObjRel} in wariowareinc.ld`,
        `move ${asmRel} -> ${convertedRel}`,
        verify ? "run clean Docker build and require wariowareinc.gba: OK" : "skip build verification",
      ];

      if (dryRun) {
        return {
          content: [{ type: "text", text: `Dry-run apply_conversion plan for ${functionName}:\n` + plan.map((x) => `  - ${x}`).join("\n") + "\n\n" + formatPreflight(pf) }],
          details: { dryRun: true, plan, preflight: pf },
        };
      }

      const backups = [];
      const backup = (rel) => {
        const abs = path.join(repoRoot, rel);
        backups.push([rel, fs.existsSync(abs) ? fs.readFileSync(abs, "utf8") : null]);
      };

      try {
        backup(decompRel);
        backup("wariowareinc.ld");
        backup(asmRel);
        backup(convertedRel);

        fs.mkdirSync(path.dirname(path.join(repoRoot, decompRel)), { recursive: true });
        fs.writeFileSync(path.join(repoRoot, decompRel), cCode.endsWith("\n") ? cCode : cCode + "\n", "utf8");

        const ldPath = path.join(repoRoot, "wariowareinc.ld");
        const ldText = fs.readFileSync(ldPath, "utf8");
        if (!ldText.includes(targetObjRel)) {
          throw new Error(`linker entry disappeared before edit: ${targetObjRel}`);
        }
        fs.writeFileSync(ldPath, ldText.replace(targetObjRel, decompObjRel), "utf8");

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
        }

        return {
          content: [
            {
              type: "text",
              text:
                `✅ Applied standalone conversion for ${functionName}.\n` +
                plan.map((x) => `  - ${x}`).join("\n") +
                (verify ? `\n\nBuild tail:\n${buildOutput.split("\n").slice(-6).join("\n")}` : ""),
            },
          ],
          details: { ok: true, plan, preflight: pf, buildOutputTail: buildOutput.split("\n").slice(-20).join("\n") },
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
          details: { error: err.message, stderr: err.stderr?.toString?.() ?? "", restored: true, preflight: pf },
        };
      }
    },
  });
}

// ── Command: /decomp-setup ────────────────────────────────────────────────────

function registerSetupCommand(pi) {
  pi.registerCommand("decomp-setup", {
    description: "Check WarioWare decomp tool prerequisites and show setup instructions",
    handler: async (_args, ctx) => {
      const repoRoot = findRepoRoot(ctx.cwd);
      const checks = [];

      const check = (label, ok, fix) => {
        checks.push({ label, ok, fix });
        return ok;
      };

      // Docker
      let dockerOk = false;
      try {
        execSync("docker info", { stdio: "pipe", timeout: 10_000 });
        dockerOk = true;
      } catch {}
      check("Docker running", dockerOk, "Start Docker Desktop");

      // compile-in-docker.sh
      check(
        "tools/mizuchi/compile-in-docker.sh",
        fs.existsSync(path.join(repoRoot, "tools/mizuchi/compile-in-docker.sh")),
        "Missing — check git status",
      );

      // get-context.sh
      check(
        "tools/mizuchi/get-context.sh",
        fs.existsSync(path.join(repoRoot, "tools/mizuchi/get-context.sh")),
        "Missing — check git status",
      );

      // objdiff-cli
      check(
        "tools/objdiff-cli (executable)",
        fs.existsSync(path.join(repoRoot, "tools/objdiff-cli")) &&
          (fs.statSync(path.join(repoRoot, "tools/objdiff-cli")).mode & 0o111) !== 0,
        "chmod +x tools/objdiff-cli",
      );

      // mizuchi-db.json
      check(
        "mizuchi-db.json",
        fs.existsSync(path.join(repoRoot, DB_FILE)),
        "cd /Users/kurt/Developer/mizuchi && npm start -- index-codebase --config wariowareinc/mizuchi.yaml",
      );

      // build/asm objects
      const buildAsmDir = path.join(repoRoot, "build/asm");
      const hasBuildObjs =
        fs.existsSync(buildAsmDir) && fs.readdirSync(buildAsmDir).some((f) => f.endsWith(".s.o"));
      check(
        "build/asm/*.s.o objects (required for standalone_tu compile_and_view_asm)",
        hasBuildObjs,
        "Run Docker build: docker run --rm -v \"$PWD:/workspace\" -w /workspace devkitpro/devkitarm:latest bash -lc 'make -j4'",
      );

      check(
        "m2c (optional — GBA target skeletons)",
        fs.existsSync(path.join(MIZUCHI_ROOT, "vendor/m2c/m2c.py")),
        "cd /Users/kurt/Developer/mizuchi && git clone https://github.com/matt-kempster/m2c.git vendor/m2c && ./scripts/setup-m2c.sh",
      );

      const all = checks.every((c) => c.ok);
      const lines = [
        all ? "✅ All decomp tools ready." : "⚠️  Some decomp tools need setup:\n",
        ...checks.map((c) => `  ${c.ok ? "✅" : "❌"} ${c.label}${!c.ok ? "\n      Fix: " + c.fix : ""}`),
      ];

      ctx.ui.notify(lines.join("\n"), all ? "info" : "warning");
    },
  });
}

// ── Export ────────────────────────────────────────────────────────────────────

export default function wariowareDecompTools(pi) {
  registerCompileAndViewAsm(pi);
  registerGetFunctionContext(pi);
  registerPreflightCandidate(pi);
  registerM2cDecompile(pi);
  registerQueryCandidates(pi);
  registerApplyConversion(pi);
  registerSetupCommand(pi);
}
