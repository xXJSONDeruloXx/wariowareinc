/**
 * WarioWare Decomp Guard Extension
 *
 * This is the hard guardrail for the autonomous decomp loop. Prompt text is
 * not enough: this extension blocks tool calls that try to turn near-miss C
 * into naked/original asm wrappers.
 *
 * Policy:
 * - new naked asm / whole-function inline asm in src/decomp/*.c is banned
 * - existing legacy naked files may remain only while untouched
 * - editing a legacy naked file is allowed only if the result removes the asm
 * - asm/ and build/ remain generated/protected paths
 */

import fs from "node:fs";
import path from "node:path";
import { execFileSync } from "node:child_process";
import { Type } from "typebox";

const REPO_SENTINEL = "wariowareinc.ld";
const GUARD_NAME = "warioware-decomp-guard";

const SOURCE_EXT_RE = /\.(?:c|h|s)$/i;
const DECOMP_SOURCE_RE = /(^|\/)src\/decomp\/[^/]+\.c$/;

const LEGACY_NAKED_ASM_FILES = new Set([
  "src/decomp/asm_08001d5c.c",
  "src/decomp/asm_08002468.c",
  "src/decomp/asm_0800bec0.c",
  "src/decomp/asm_0800c080.c",
  "src/decomp/asm_080113ec.c",
  "src/decomp/asm_08011774.c",
  "src/decomp/asm_08014e88.c",
  "src/decomp/asm_080cd564.c",
  "src/decomp/asm_080ee830.c",
  "src/decomp/asm_080eed9c.c",
  "src/decomp/asm_080eedc0.c",
  "src/decomp/asm_080ef224.c",
  "src/decomp/asm_080ef264.c",
  "src/decomp/asm_080ef298.c",
]);

function findRepoRoot(startDir) {
  let current = path.resolve(startDir);
  while (true) {
    if (fs.existsSync(path.join(current, REPO_SENTINEL))) return current;
    const parent = path.dirname(current);
    if (parent === current) return path.resolve(startDir);
    current = parent;
  }
}

function relPath(repoRoot, p) {
  const abs = path.isAbsolute(p) ? p : path.join(repoRoot, p);
  return path.relative(repoRoot, abs).replaceAll(path.sep, "/");
}

function isManualOverrideEnabled() {
  return process.env.WARIOWARE_ALLOW_NAKED_ASM === "1";
}

function nakedAsmReason(text, context = "code") {
  if (!text) return "";

  if (/__attribute__\s*\(\s*\(\s*naked\s*\)\s*\)/.test(text)) {
    return `${context} contains __attribute__((naked))`;
  }
  if (/#\s*include\s+["<][^"<]*asm\/[^"<]*\.s[">]/.test(text)) {
    return `${context} includes an asm .s stub`;
  }
  if (/thumb_func_start\b/.test(text)) {
    return `${context} contains thumb_func_start/original asm stub text`;
  }

  const hasAsmBlock = /\basm\s+(?:volatile\s*)?\(/.test(text) || /\basm\s*\(/.test(text);
  const looksLikeWholeFunctionAsm = /\.syntax\s+(?:unified|divided)|\.ltorg\b|\.balign\b|\.word\b|\bmov\s+pc\b|\bpush\s*\{[^}]*\blr\b/i.test(text);
  if (hasAsmBlock && looksLikeWholeFunctionAsm) {
    return `${context} contains a whole-function inline asm block`;
  }

  return "";
}

function changedFiles(repoRoot, stagedOnly = false) {
  const args = stagedOnly ? ["diff", "--cached", "--name-only"] : ["status", "--porcelain"];
  const out = execFileSync("git", args, { cwd: repoRoot, encoding: "utf8" }).trim();
  if (!out) return [];
  if (stagedOnly) return out.split(/\n+/).filter(Boolean).map((p) => p.replaceAll(path.sep, "/"));
  return out.split(/\n+/)
    .map((line) => line.slice(3).replace(/^.* -> /, "").replaceAll(path.sep, "/"))
    .filter(Boolean);
}

function scanChangedSources(repoRoot, stagedOnly = false) {
  const violations = [];
  for (const rel of changedFiles(repoRoot, stagedOnly)) {
    if (!SOURCE_EXT_RE.test(rel)) continue;
    if (!DECOMP_SOURCE_RE.test(rel)) continue;
    const abs = path.join(repoRoot, rel);
    if (!fs.existsSync(abs)) continue;
    const text = fs.readFileSync(abs, "utf8");
    const reason = nakedAsmReason(text, rel);
    if (reason) violations.push({ path: rel, reason });
  }
  return violations;
}

function scanAllNakedAsm(repoRoot) {
  const dir = path.join(repoRoot, "src", "decomp");
  const violations = [];
  if (!fs.existsSync(dir)) return violations;
  for (const name of fs.readdirSync(dir)) {
    if (!name.endsWith(".c")) continue;
    const rel = `src/decomp/${name}`;
    const text = fs.readFileSync(path.join(dir, name), "utf8");
    const reason = nakedAsmReason(text, rel);
    if (!reason) continue;
    violations.push({ path: rel, reason, legacy: LEGACY_NAKED_ASM_FILES.has(rel) });
  }
  return violations;
}

function formatViolations(violations) {
  return violations.map((v) => `- ${v.path}: ${v.reason}`).join("\n");
}

function guardBlock(reason, ctx) {
  const message = `Blocked by ${GUARD_NAME}: ${reason}`;
  if (ctx?.hasUI) ctx.ui.notify(message, "warning");
  return { block: true, reason: message };
}

function inspectDirectCodeTool(event, ctx) {
  const input = event.input ?? {};
  const code = input.cCode ?? input.code ?? "";
  const reason = nakedAsmReason(String(code), event.toolName);
  if (reason) {
    return guardBlock(`${reason}. Naked asm wrappers are banned as decomp progress; keep iterating in real C or choose another real-C candidate.`, ctx);
  }
  return undefined;
}

function inspectWriteOrEdit(event, repoRoot, ctx) {
  const input = event.input ?? {};
  const rawPath = input.path;
  if (!rawPath) return undefined;
  const rel = relPath(repoRoot, String(rawPath));

  if (rel.startsWith("build/")) {
    return guardBlock(`build/ is generated output; do not edit ${rel} directly.`, ctx);
  }

  if (rel.startsWith("asm/") && !rel.startsWith("asm/converted/")) {
    return guardBlock(`asm/ contains original/generated stubs; use apply_conversion instead of editing ${rel} directly.`, ctx);
  }

  if (!DECOMP_SOURCE_RE.test(rel)) return undefined;

  if (event.toolName === "write") {
    const reason = nakedAsmReason(String(input.content ?? ""), rel);
    if (reason) return guardBlock(`${reason}. Write real C instead of naked asm.`, ctx);
  }

  if (event.toolName === "edit") {
    for (const edit of input.edits ?? []) {
      const reason = nakedAsmReason(String(edit?.newText ?? ""), rel);
      if (reason) return guardBlock(`${reason}. Edit must remove naked asm, not add or preserve it.`, ctx);
    }
  }

  return undefined;
}

function inspectBash(event, repoRoot, ctx) {
  const command = String(event.input?.command ?? "");

  if (/\bgit\s+commit\b[^\n;]*\s--no-verify\b/.test(command)) {
    return guardBlock("git commit --no-verify is not allowed; verification hooks must run.", ctx);
  }

  if (/\b(?:cp|mv|cat|tee)\b[\s\S]*(?:^|\s)asm\/[\w/.-]+\.s[\s\S]*src\/decomp\/[\w.-]+\.c/.test(command)) {
    return guardBlock("bash command appears to copy/include an asm stub into src/decomp. Write real C instead.", ctx);
  }

  if (/\bgit\s+(?:add|commit)\b/.test(command)) {
    const stagedOnly = /\bgit\s+commit\b/.test(command);
    const violations = scanChangedSources(repoRoot, stagedOnly);
    if (violations.length) {
      return guardBlock(`changed src/decomp file(s) still contain naked/original asm:\n${formatViolations(violations)}\nConvert to real C or leave the legacy file untouched.`, ctx);
    }
  }

  const writesDecomp = /src\/decomp\/[^\s'";]+\.c/.test(command);
  const suspicious = /__attribute__\s*\(\s*\(\s*naked\s*\)\s*\)|thumb_func_start\b|#\s*include\s+["<][^"<]*asm\/[^"<]*\.s[">]/.test(command);
  if (writesDecomp && suspicious) {
    return guardBlock("bash command appears to write naked/original asm into src/decomp. Use real C instead.", ctx);
  }

  return undefined;
}

export default function (pi) {
  pi.on("tool_call", async (event, ctx) => {
    if (isManualOverrideEnabled()) return undefined;

    const repoRoot = findRepoRoot(ctx.cwd ?? process.cwd());

    if (event.toolName === "apply_conversion" || event.toolName === "compile_and_view_asm") {
      return inspectDirectCodeTool(event, ctx);
    }

    if (event.toolName === "write" || event.toolName === "edit") {
      return inspectWriteOrEdit(event, repoRoot, ctx);
    }

    if (event.toolName === "bash") {
      return inspectBash(event, repoRoot, ctx);
    }

    return undefined;
  });

  pi.on("before_agent_start", async (event) => {
    return {
      systemPrompt: `${event.systemPrompt}\n\n## WarioWare Decomp Guard (tool-enforced)\n- New naked asm / whole-function inline asm wrappers in src/decomp/*.c are blocked by the project extension.\n- Existing legacy naked asm files may be left untouched or converted to real C; editing them while they still contain naked/original asm is blocked at commit time.\n- If a function does not match, keep iterating in real C or select another real-C candidate. Do not pivot to asm.\n- Use decomp_guard_check before committing chunks that touched src/decomp.`,
    };
  });

  pi.registerTool({
    name: "decomp_guard_check",
    label: "Decomp Guard Check",
    description: "Scan WarioWare decomp changes for banned naked/original asm wrappers.",
    parameters: Type.Object({
      stagedOnly: Type.Optional(Type.Boolean({ description: "Only scan staged files" })),
      includeLegacy: Type.Optional(Type.Boolean({ description: "Include pre-existing legacy naked asm files in the report" })),
    }),
    async execute(_toolCallId, params, _signal, _onUpdate, ctx) {
      const repoRoot = findRepoRoot(ctx.cwd ?? process.cwd());
      const violations = params?.includeLegacy
        ? scanAllNakedAsm(repoRoot).filter((v) => params.includeLegacy || !v.legacy)
        : scanChangedSources(repoRoot, Boolean(params?.stagedOnly));
      const active = violations.filter((v) => !v.legacy);
      const text = violations.length
        ? `${active.length ? "❌" : "ℹ️"} Decomp guard found ${violations.length} naked/original asm file(s):\n${violations.map((v) => `- ${v.path}${v.legacy ? " (legacy)" : ""}: ${v.reason}`).join("\n")}`
        : "✅ Decomp guard found no banned naked/original asm in changed src/decomp files.";
      return { content: [{ type: "text", text }], details: { violations, activeCount: active.length } };
    },
  });
}
