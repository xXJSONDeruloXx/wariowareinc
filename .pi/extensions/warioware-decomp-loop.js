import fs from "node:fs";
import path from "node:path";

const WIDGET_KEY = "warioware-decomp-loop";
const REFRESH_INTERVAL_MS = 3000;
const STATE_FILE = path.join(".pi", "decomp-loop-state.json");
const REPO_SENTINEL = "wariowareinc.ld";
const DONE_MARKER = "<DECOMP_CHUNK_DONE>";
const BLOCKED_MARKER = "<DECOMP_BLOCKED>";

// ── Module-level state (persists via jiti module cache across sessions) ──────
let latestCtx;    // updated from session_start / agent_end (ExtensionContext)
let timer;        // shared setInterval handle

/**
 * Stored newSession function, set from command/withSession contexts
 * (ExtensionCommandContext).  This is the KEY fix: we never call
 * pi.sendUserMessage("/advance") (which sends text to the LLM). Instead we
 * store ctx.newSession here and call it directly from the timer.
 *
 * Lifecycle:
 *   - Set when /decomp-loop start or /decomp-next runs (command ctx).
 *   - Re-set inside withSession callback (ReplacedSessionContext) so the next
 *     session can chain forward.
 *   - Cleared to null on session_shutdown so stale refs are never used.
 */
let newSessionFn = null;

// ── Helpers ──────────────────────────────────────────────────────────────────

function findRepoRoot(startDir) {
  let current = path.resolve(startDir);
  while (true) {
    if (fs.existsSync(path.join(current, REPO_SENTINEL))) return current;
    const parent = path.dirname(current);
    if (parent === current) return path.resolve(startDir);
    current = parent;
  }
}

function getStatePath(repoRoot) {
  return path.join(repoRoot, STATE_FILE);
}

function defaultState() {
  return {
    enabled: false,
    chunk: 0,
    advanceRequested: false,
    lastStatus: "idle",
    lastBlockedReason: "",
    updatedAt: new Date().toISOString(),
  };
}

function loadState(repoRoot) {
  try {
    const p = getStatePath(repoRoot);
    if (!fs.existsSync(p)) return defaultState();
    return { ...defaultState(), ...JSON.parse(fs.readFileSync(p, "utf8")) };
  } catch {
    return defaultState();
  }
}

function saveState(repoRoot, state) {
  const p = getStatePath(repoRoot);
  fs.mkdirSync(path.dirname(p), { recursive: true });
  fs.writeFileSync(
    p,
    JSON.stringify({ ...state, updatedAt: new Date().toISOString() }, null, 2),
    "utf8",
  );
}

function extractAssistantText(messages) {
  for (let i = messages.length - 1; i >= 0; i--) {
    const msg = messages[i];
    if (msg?.role !== "assistant" || !Array.isArray(msg.content)) continue;
    return msg.content
      .filter((part) => part?.type === "text")
      .map((part) => part.text)
      .join("\n");
  }
  return "";
}

function buildChunkPrompt(chunk) {
  return [
    `You are running WarioWare autonomous decomp chunk ${chunk}.`,
    "",
    "## Step 1 — Read canonical docs",
    "- docs/README.md",
    "- docs/wariowareinc-decomp-scaleup.md",
    "- docs/decomp-pattern-library.md",
    "- docs/decomp-batch-history.md",
    "",
    "## Step 2 — Select a target function",
    "Use the `query_candidates` tool to find the best next unmatched function.",
    "Preferred strategy: 'smallest' or 'families'.",
    "One function per chunk.",
    "",
    "## Step 3 — Gather context",
    "Call `get_function_context` with the chosen function name.",
    "This returns preprocessed headers and type definitions — use them for accurate typing.",
    "",
    "## Step 4 — Get an initial C guess (optional but recommended)",
    "Call `m2c_decompile` for a rule-based starting skeleton.",
    "If m2c is not set up, write the C code from scratch using the asm in the candidate.",
    "",
    "## Step 5 — Iterate with isolated compile",
    "Call `compile_and_view_asm` with the function name and your current C code.",
    "This compiles with agbcc inside Docker and diffs against the target .o — NO full ROM build needed.",
    "Iterate until match_percent reaches 100% (PERFECT MATCH).",
    "Do NOT do a full Docker make build during iteration — only after achieving a match.",
    "",
    "## Step 6 — Full verification",
    "Only after compile_and_view_asm reports PERFECT MATCH:",
    "1. Place the C file at src/decomp/asm_XXXXXXXX.c",
    "2. Update wariowareinc.ld (swap build/asm/asm_XXXXXXXX.s.o → build/src/decomp/asm_XXXXXXXX.c.o)",
    "3. Move asm/asm_XXXXXXXX.s → asm/converted/asm_XXXXXXXX.s",
    "4. Run full Docker build: docker run --rm -v \"$PWD:/workspace\" -w /workspace devkitpro/devkitarm:latest bash -lc 'make -j4'",
    "5. Require 'wariowareinc.gba: OK'",
    "",
    "## Step 7 — Commit and report",
    "After successful full build:",
    "1. Run: source ~/.zshrc && make report",
    "2. Run: python3 tools/gen_objdiff.py",
    "3. Update docs (README.md, scaleup, batch-history, pattern-library if new pattern learned)",
    "4. Commit code + docs together and push",
    "",
    "## Rules",
    "- Do not create or rely on Ralph loops.",
    "- Do not ask the user to choose among reasonable next steps; pick the best default.",
    "- Use compile_and_view_asm for iteration — full builds only for final verification.",
    "- Preserve byte-identical ROM at every accepted milestone.",
    "- One function per chunk. If you cannot land any function, document why and stop.",
    "",
    "## Chunk completion markers",
    `When done, your final response must end with this exact marker on its own line: ${DONE_MARKER}`,
    `If truly blocked, end with: ${BLOCKED_MARKER} followed by one short reason line.`,
    "",
    "Do not continue indefinitely. One chunk, then stop.",
  ].join("\n");
}

function formatStatus(state) {
  let line = `decomp loop: ${state.enabled ? "on" : "off"} · chunk ${state.chunk} · ${state.lastStatus}`;
  if (state.advanceRequested) line += " · advance pending";
  if (state.lastStatus === "blocked" && state.lastBlockedReason)
    line += ` · ${state.lastBlockedReason}`;
  return line;
}

function applyWidget(ctx, repoRoot) {
  if (!ctx?.hasUI) return;
  const state = loadState(repoRoot);
  ctx.ui.setWidget(WIDGET_KEY, [formatStatus(state)]);
}

function ensureTimer() {
  if (timer) return;
  timer = setInterval(async () => {
    const ctx = latestCtx;
    if (!ctx) return;
    const repoRoot = findRepoRoot(ctx.cwd);
    const state = loadState(repoRoot);
    applyWidget(ctx, repoRoot);

    if (!state.enabled) return;
    if (!state.advanceRequested) return;
    if (!newSessionFn) return; // no session fn available yet
    if (!ctx.isIdle()) return;
    if (ctx.hasPendingMessages()) return;

    // Advance: clear request flag, then launch the next chunk directly
    state.advanceRequested = false;
    state.lastStatus = "advancing";
    saveState(repoRoot, state);
    applyWidget(ctx, repoRoot);

    try {
      await launchChunk({ repoRoot, enableLoop: true });
    } catch (err) {
      const reason = err instanceof Error ? err.message : String(err);
      const s = loadState(repoRoot);
      s.enabled = false;
      s.lastStatus = "error";
      s.lastBlockedReason = reason;
      saveState(repoRoot, s);
      applyWidget(ctx, repoRoot);
    }
  }, REFRESH_INTERVAL_MS);
}

/**
 * Launch one chunk in a fresh session.
 *
 * Uses the module-level `newSessionFn` which was stored from the most recent
 * command or withSession context.  Inside `withSession`, we immediately
 * re-set `newSessionFn` to the NEW session's ctx.newSession so the timer can
 * chain forward into session N+2, N+3, … without ever calling sendUserMessage.
 */
async function launchChunk({ repoRoot, enableLoop }) {
  const state = loadState(repoRoot);
  state.enabled = enableLoop;
  state.advanceRequested = false;
  state.chunk += 1;
  state.lastBlockedReason = "";
  state.lastStatus = "launching";
  saveState(repoRoot, state);
  applyWidget(latestCtx, repoRoot);

  const parentSession = latestCtx?.sessionManager?.getSessionFile?.();
  const prompt = buildChunkPrompt(state.chunk);
  const chunkNum = state.chunk;

  // Capture and clear so a double-fire can't happen
  const fn = newSessionFn;
  newSessionFn = null;

  await fn({
    parentSession,
    withSession: async (newCtx) => {
      // ── KEY: store newSession from the NEW session's context ──────────────
      // This lets the timer advance into session N+2 without sendUserMessage.
      // withSession runs after the new session has started (session_start has
      // already fired), so setting module-level state here is safe.
      newSessionFn = (opts) => newCtx.newSession(opts);
      // ─────────────────────────────────────────────────────────────────────

      newCtx.ui.notify(
        `Started decomp chunk ${chunkNum}${enableLoop ? " (loop)" : ""}`,
        "info",
      );
      await newCtx.sendUserMessage(prompt);
    },
  });
}

// ── Extension export ─────────────────────────────────────────────────────────

export default function wariowareDecompLoop(pi) {
  pi.registerCommand("decomp-next", {
    description: "Start one fresh-context WarioWare decomp chunk",
    handler: async (_args, ctx) => {
      await ctx.waitForIdle();
      const repoRoot = findRepoRoot(ctx.cwd);
      newSessionFn = (opts) => ctx.newSession(opts);
      await launchChunk({ repoRoot, enableLoop: false });
    },
  });

  pi.registerCommand("decomp-loop", {
    description: "Manage the fresh-context WarioWare decomp loop",
    handler: async (args, ctx) => {
      const repoRoot = findRepoRoot(ctx.cwd);
      const state = loadState(repoRoot);
      const sub = args.trim().toLowerCase();

      if (sub === "start") {
        await ctx.waitForIdle();
        newSessionFn = (opts) => ctx.newSession(opts);
        ensureTimer();
        await launchChunk({ repoRoot, enableLoop: true });
        return;
      }

      if (sub === "stop") {
        state.enabled = false;
        state.advanceRequested = false;
        state.lastStatus = "stopped";
        saveState(repoRoot, state);
        applyWidget(ctx, repoRoot);
        ctx.ui.notify("decomp loop: stopped", "info");
        return;
      }

      if (sub === "status" || !sub) {
        ctx.ui.notify(formatStatus(state), "info");
        return;
      }

      if (sub === "reset") {
        saveState(repoRoot, defaultState());
        applyWidget(ctx, repoRoot);
        ctx.ui.notify("decomp loop: state reset", "info");
        return;
      }

      ctx.ui.notify("Usage: /decomp-loop [start|stop|status|reset]", "warning");
    },
  });

  pi.on("session_start", async (_event, ctx) => {
    latestCtx = ctx;
    ensureTimer();
    applyWidget(ctx, findRepoRoot(ctx.cwd));
  });

  pi.on("agent_end", async (event, ctx) => {
    latestCtx = ctx;
    const repoRoot = findRepoRoot(ctx.cwd);
    const state = loadState(repoRoot);

    if (!state.enabled) {
      applyWidget(ctx, repoRoot);
      return;
    }

    const text = extractAssistantText(event.messages);

    if (text.includes(BLOCKED_MARKER)) {
      const idx = text.lastIndexOf(BLOCKED_MARKER);
      const tail =
        text.slice(idx + BLOCKED_MARKER.length).trim().split(/\r?\n/)[0] ?? "";
      state.enabled = false;
      state.advanceRequested = false;
      state.lastStatus = "blocked";
      state.lastBlockedReason = tail;
      saveState(repoRoot, state);
      applyWidget(ctx, repoRoot);
      if (ctx.hasUI)
        ctx.ui.notify(`decomp loop blocked${tail ? `: ${tail}` : ""}`, "warning");
      return;
    }

    if (text.includes(DONE_MARKER)) {
      state.advanceRequested = true;
      state.lastStatus = "waiting-to-advance";
      saveState(repoRoot, state);
      applyWidget(ctx, repoRoot);
      return;
    }

    // No marker found — keep loop enabled but flag the missing marker
    state.lastStatus = "waiting-for-marker";
    saveState(repoRoot, state);
    applyWidget(ctx, repoRoot);
  });

  pi.on("session_shutdown", async () => {
    latestCtx = undefined;
    // Clear stale newSessionFn so the timer doesn't call a dead reference.
    // withSession will re-set it after the new session starts if the loop
    // was advancing (withSession always runs after session_shutdown).
    newSessionFn = null;
  });
}
