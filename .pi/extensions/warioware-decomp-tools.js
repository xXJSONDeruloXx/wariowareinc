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

// ── objdiff JSON parser ───────────────────────────────────────────────────────

function collectMatchPercents(obj, results = [], depth = 0) {
  if (depth > 15 || !obj || typeof obj !== "object") return results;
  if (Array.isArray(obj)) {
    for (const item of obj) collectMatchPercents(item, results, depth + 1);
    return results;
  }
  if ("match_percent" in obj) {
    results.push({
      name: obj.name ?? null,
      matchPercent: Number(obj.match_percent),
    });
  }
  for (const v of Object.values(obj)) {
    if (v && typeof v === "object") collectMatchPercents(v, results, depth + 1);
  }
  return results;
}

function collectInstrDiffs(obj, diffs = [], depth = 0) {
  if (depth > 15 || !obj || typeof obj !== "object") return diffs;
  if (Array.isArray(obj)) {
    for (const item of obj) collectInstrDiffs(item, diffs, depth + 1);
    return diffs;
  }
  // Instruction diff entry has diff_kind + at least one of left_instr/right_instr
  if (obj.diff_kind && obj.diff_kind !== "NONE" && obj.diff_kind !== 0) {
    diffs.push({
      kind: obj.diff_kind,
      left: obj.left_instr?.formatted ?? obj.base_instr ?? null,
      right: obj.right_instr?.formatted ?? obj.target_instr ?? null,
    });
  }
  for (const v of Object.values(obj)) {
    if (v && typeof v === "object") collectInstrDiffs(v, diffs, depth + 1);
  }
  return diffs;
}

function parseDiffJson(jsonStr, symbolName) {
  let data;
  try {
    data = JSON.parse(jsonStr);
  } catch (e) {
    return { ok: false, error: `JSON parse failed: ${e.message}`, raw: jsonStr.slice(0, 400) };
  }

  // Collect from the `right` side (our compiled code vs target)
  const rightSide = data.right ?? data;
  const allPercents = collectMatchPercents(rightSide);

  // Find the entry that matches our symbol name (THUMB +1 variant too)
  const hit =
    allPercents.find((p) => p.name === symbolName) ??
    allPercents.find((p) => p.name === symbolName + "+1") ??
    allPercents.find((p) => p.name?.toLowerCase() === symbolName.toLowerCase());

  const matchPercent = hit?.matchPercent ?? (allPercents.length ? Math.min(...allPercents.map((p) => p.matchPercent)) : null);

  const instrDiffs = collectInstrDiffs(rightSide).slice(0, 40);

  return {
    ok: true,
    matchPercent,
    isPerfectMatch: matchPercent != null && matchPercent >= 100.0,
    instrDiffs,
    symbolFound: !!hit,
  };
}

function formatDiffResult(result, functionName) {
  if (!result.ok) return `Diff parse error: ${result.error}`;

  const pct = result.matchPercent != null ? `${result.matchPercent.toFixed(1)}%` : "?%";
  const header = result.isPerfectMatch
    ? `✅ PERFECT MATCH (100%) — ${functionName}`
    : `❌ MISMATCH (${pct}) — ${functionName}`;

  const lines = [header];
  if (!result.isPerfectMatch && result.instrDiffs.length > 0) {
    lines.push(`\nInstruction differences (up to 40):`);
    for (const d of result.instrDiffs) {
      const left = d.left ?? "(none)";
      const right = d.right ?? "(none)";
      lines.push(`  [${d.kind}]  target: ${left}  |  compiled: ${right}`);
    }
  }
  if (!result.symbolFound) {
    lines.push(`\n(Note: symbol "${functionName}" not found by exact name in diff output; showing overall minimum match.)`);
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
      "Returns: match percentage, instruction diffs, and whether it is a perfect match.\n" +
      "Requires: Docker running, project previously built (build/asm/*.s.o must exist).",
    parameters: Type.Object({
      functionName: Type.String({
        description:
          "Function name as it appears in the asm file or mizuchi-db (e.g. func_08002468 or asm_08002468)",
      }),
      cCode: Type.String({
        description: "Complete C code to compile and test against the target",
      }),
    }),
    async execute({ functionName, cCode }, _signal, ctx) {
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
        return {
          content: [
            {
              type: "text",
              text:
                `ERROR: Target object not found: ${targetObj ?? "unknown"}\n` +
                `Run a Docker build first (make -j4 in devkitpro/devkitarm container) to produce build/asm/ objects.`,
            },
          ],
          details: { error: "target_not_found", targetObj },
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
    async execute({ functionName }, _signal, ctx) {
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
        return {
          content: [{ type: "text", text: `Context script failed:\n${stderr}` }],
          details: { error: stderr },
        };
      }
    },
  });
}

// ── Tool: m2c_decompile ───────────────────────────────────────────────────────

function registerM2cDecompile(pi) {
  pi.registerTool({
    name: "m2c_decompile",
    label: "m2c Decompile",
    description:
      "Generates an initial C skeleton from assembly using m2c (a rule-based ARM decompiler).\n" +
      "The result is usually not a perfect match but gives a useful starting point.\n" +
      "Requires: m2c submodule initialized in /Users/kurt/Developer/mizuchi/vendor/m2c/\n" +
      "Setup: cd /Users/kurt/Developer/mizuchi && git submodule update --init vendor/m2c && ./scripts/setup-m2c.sh",
    parameters: Type.Object({
      functionName: Type.String({
        description: "Function name (e.g. func_08002468 or asm_08002468)",
      }),
    }),
    async execute({ functionName }, _signal, ctx) {
      const repoRoot = findRepoRoot(ctx.cwd);
      const m2cPy = path.join(MIZUCHI_ROOT, "vendor/m2c/m2c.py");
      const venvPython = path.join(os.homedir(), ".cache/mizuchi/python-venv/bin/python3");

      // Check setup
      if (!fs.existsSync(m2cPy)) {
        return {
          content: [
            {
              type: "text",
              text:
                "m2c not set up. To enable:\n" +
                "  cd /Users/kurt/Developer/mizuchi\n" +
                "  git submodule update --init vendor/m2c\n" +
                "  ./scripts/setup-m2c.sh\n\n" +
                "Then reload the extension with /reload.",
            },
          ],
          details: { error: "m2c_not_setup" },
        };
      }

      const python = fs.existsSync(venvPython) ? venvPython : "python3";

      // Get asm for the function from mizuchi-db.json
      const db = loadDb(repoRoot);
      const fn = findFn(db, functionName);
      if (!fn) {
        return {
          content: [{ type: "text", text: `Function "${functionName}" not found in mizuchi-db.json. Run index-codebase first.` }],
          details: { error: "function_not_found" },
        };
      }

      const asmContent = fn.asmCode;
      if (!asmContent?.trim()) {
        return {
          content: [{ type: "text", text: `No asm content found for "${functionName}" in the database.` }],
          details: { error: "no_asm" },
        };
      }

      // Also try to find the actual .mizuchi-asm file for more accurate m2c input
      let asmForM2c = asmContent;
      const asmFile = fn.asmModulePath ? path.join(repoRoot, fn.asmModulePath) : null;
      if (asmFile && fs.existsSync(asmFile)) {
        asmForM2c = fs.readFileSync(asmFile, "utf8");
      }

      const tmpDir = fs.mkdtempSync(path.join(os.tmpdir(), "ww-m2c-"));
      const asmTmp = path.join(tmpDir, `${functionName}.s`);

      try {
        fs.writeFileSync(asmTmp, asmForM2c, "utf8");

        const output = execFileSync(python, [m2cPy, "--arch", "arm", asmTmp], {
          cwd: repoRoot,
          timeout: 30_000,
          stdio: "pipe",
        }).toString();

        return {
          content: [
            {
              type: "text",
              text: `m2c initial decompilation of ${functionName}:\n\n${output}\n\n` +
                `(This is a rule-based starting point. Use compile_and_view_asm to iterate toward a match.)`,
            },
          ],
          details: { generatedCode: output },
        };
      } catch (err) {
        const stderr = err.stderr?.toString?.() || err.message;
        return {
          content: [{ type: "text", text: `m2c failed:\n${stderr}` }],
          details: { error: stderr },
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
    }),
    async execute({ count = 10, strategy = "smallest", family }, _signal, ctx) {
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

      // Exclude pure data labels (no thumb/arm func marker)
      candidates = candidates.filter((f) =>
        f.asmCode && (
          f.asmCode.includes("thumb_func_start") ||
          f.asmCode.includes("arm_func_start") ||
          f.asmCode.includes("func_") ||
          f.asmCode.match(/^\s*push\s*\{/m)
        ),
      );

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
            const aAddr = parseInt(addrFromName(a.name) ?? "0", 16);
            const bAddr = parseInt(addrFromName(b.name) ?? "0", 16);
            return aAddr - bAddr;
          });
          break;
        case "random":
          candidates.sort(() => Math.random() - 0.5);
          break;
      }

      const top = candidates.slice(0, count);

      const lines = [
        `Found ${candidates.length} unmatched function candidates. Showing top ${top.length} by "${strategy}"${family ? ` in module "${family}"` : ""}:\n`,
      ];

      for (const fn of top) {
        const addr = addrFromName(fn.name);
        const asmLines = fn.asmCode?.split("\n").length ?? 0;
        const module = fn.asmModulePath?.replace(".mizuchi-asm/asm/", "").replace(/\/asm_[^/]+\.s$/, "") ?? "?";
        const calledMatched = (fn.callsFunctions ?? []).filter(
          (n) => allFns.find((f) => f.name === n)?.cCode?.trim(),
        ).length;
        const targetObj = addr ? `build/asm/asm_${addr}.s.o` : "?";
        lines.push(
          `• ${fn.name}  [${asmLines} asm lines | module: ${module} | matched-calls: ${calledMatched}/${(fn.callsFunctions ?? []).length}]\n` +
          `  ROM: 0x${addr?.toUpperCase() ?? "?"}  target: ${targetObj}`,
        );
      }

      return {
        content: [{ type: "text", text: lines.join("\n") }],
        details: {
          totalCandidates: candidates.length,
          strategy,
          family: family ?? null,
          candidates: top.map((f) => ({
            name: f.name,
            asmLines: f.asmCode?.split("\n").length ?? 0,
            module: f.asmModulePath?.replace(".mizuchi-asm/asm/", "").replace(/\/asm_[^/]+\.s$/, "") ?? null,
            asmCode: f.asmCode ?? null,
            callsFunctions: f.callsFunctions ?? [],
          })),
        },
      };
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
        "build/asm/*.s.o objects (required for compile_and_view_asm)",
        hasBuildObjs,
        "Run Docker build: docker run --rm -v \"$PWD:/workspace\" -w /workspace devkitpro/devkitarm:latest bash -lc 'make -j4'",
      );

      // m2c
      check(
        "m2c (optional — for initial C skeletons)",
        fs.existsSync(path.join(MIZUCHI_ROOT, "vendor/m2c/m2c.py")),
        "cd /Users/kurt/Developer/mizuchi && git submodule update --init vendor/m2c && ./scripts/setup-m2c.sh",
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
  registerM2cDecompile(pi);
  registerQueryCandidates(pi);
  registerSetupCommand(pi);
}
