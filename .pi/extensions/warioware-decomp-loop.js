import fs from "node:fs";
import path from "node:path";
import { Type } from "typebox";

const WIDGET_KEY = "warioware-decomp-loop";
const REFRESH_INTERVAL_MS = 3000;
const STATE_FILE = path.join(".pi", "decomp-loop-state.json");
const REPO_SENTINEL = "wariowareinc.ld";

// Text-based fallback markers (used if agent prints text instead of calling the tool)
const DONE_MARKER = "<DECOMP_CHUNK_DONE>";
const BLOCKED_MARKER = "<DECOMP_BLOCKED>";

// ── Module-level (fresh per-session due to jiti moduleCache: false) ───────────
// These are intentionally session-scoped. The loop advances within a single
// session via pi.sendUserMessage(nextChunkPrompt) — no cross-session state needed.
let latestCtx;
let timer;
let loopPi; // set from the default export; used by timer to send messages

// ── Helpers ───────────────────────────────────────────────────────────────────

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
    lastChunkSummary: "",
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
      .filter((p) => p?.type === "text")
      .map((p) => p.text)
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
    "Use the `query_candidates` tool (strategy: 'smallest' or 'families').",
    "One function per chunk.",
    "",
    "## Step 3 — Gather context",
    "Call `get_function_context` with the chosen function name.",
    "Use the returned headers/typedefs for accurate typing.",
    "",
    "## Step 4 — Get an initial C guess (optional but recommended)",
    "Call `m2c_decompile` for a rule-based starting skeleton.",
    "If m2c is not set up, write C from scratch using the asm in the candidate.",
    "",
    "## Step 5 — Iterate with isolated compile",
    "Call `compile_and_view_asm` with your current C code.",
    "Repeat until match_percent = 100% (PERFECT MATCH).",
    "Do NOT run a full Docker make build during iteration.",
    "",
    "## Step 6 — Full verification (only after PERFECT MATCH)",
    "1. Place C file at src/decomp/asm_XXXXXXXX.c",
    "2. Update wariowareinc.ld (swap build/asm/asm_XXXXXXXX.s.o → build/src/decomp/asm_XXXXXXXX.c.o)",
    "3. Move asm/asm_XXXXXXXX.s → asm/converted/asm_XXXXXXXX.s",
    "4. Full Docker build: docker run --rm -v \"$PWD:/workspace\" -w /workspace devkitpro/devkitarm:latest bash -lc 'make -j4'",
    "5. Require: wariowareinc.gba: OK",
    "",
    "## Step 7 — Commit, report, and signal done",
    "1. source ~/.zshrc && make report",
    "2. python3 tools/gen_objdiff.py",
    "3. Update docs (README, scaleup, batch-history, pattern-library if new patterns)",
    "4. git add -A && git commit -m 'feat: ...' && git push",
    "5. Call the `decomp_chunk_done` tool with a brief summary.",
    "   If blocked at any step, call `decomp_chunk_done` with blocked=true and a reason.",
    "",
    "## Rules",
    "- Do not create or rely on Ralph loops.",
    "- Do not ask the user to choose; pick the best default and proceed.",
    "- Use compile_and_view_asm for iteration — full builds only for final verification.",
    "- Preserve byte-identical ROM at every accepted milestone.",
    "- You MUST call `decomp_chunk_done` before your final response, whether successful or blocked.",
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

// ── Advance the loop (same session, fresh prompt) ─────────────────────────────
//
// Why same-session instead of ctx.newSession():
//   jiti loads extensions with moduleCache:false, so every new session gets a
//   completely fresh module instance. Module-level state (like a stored newSession
//   function) cannot survive across session boundaries. pi.sendUserMessage()
//   within the current session is the only reliable advancement mechanism.

function advanceLoop(repoRoot) {
  const state = loadState(repoRoot);
  state.advanceRequested = false;
  state.chunk += 1;
  state.lastStatus = "advancing";
  saveState(repoRoot, state);
  applyWidget(latestCtx, repoRoot);

  const prompt = buildChunkPrompt(state.chunk);
  try {
    loopPi.sendUserMessage(prompt);
  } catch (err) {
    // If agent is mid-stream, queue it for after
    try {
      loopPi.sendUserMessage(prompt, { deliverAs: "followUp" });
    } catch {
      // Last resort: re-set the flag and let the timer retry
      const s = loadState(repoRoot);
      s.advanceRequested = true;
      s.chunk -= 1;
      s.lastStatus = "waiting-to-advance";
      saveState(repoRoot, s);
    }
  }
}

// ── Timer ─────────────────────────────────────────────────────────────────────

function ensureTimer() {
  if (timer) return;
  timer = setInterval(() => {
    const ctx = latestCtx;
    if (!ctx) return;
    const repoRoot = findRepoRoot(ctx.cwd);
    const state = loadState(repoRoot);
    applyWidget(ctx, repoRoot);

    // Primary path: decomp_chunk_done tool → compact → onComplete → sendUserMessage
    // This timer is fallback-only: fires when the text marker was detected in agent_end
    // (i.e. agent printed <DECOMP_CHUNK_DONE> instead of calling the tool).
    if (!state.enabled) return;
    if (!state.advanceRequested) return;
    if (state.lastStatus === "compacting") return; // compact already in progress
    if (!ctx.isIdle()) return;
    if (ctx.hasPendingMessages()) return;

    // Text-marker fallback: advance without compact (simpler, since we don't have
    // tool ctx here — just send the next prompt directly).
    advanceLoop(repoRoot);
  }, REFRESH_INTERVAL_MS);
}

// ── Extension export ──────────────────────────────────────────────────────────

export default function wariowareDecompLoop(pi) {
  loopPi = pi;

  // ── Tool: decomp_chunk_done ────────────────────────────────────────────────
  // The LLM calls this to signal end-of-chunk, replacing the text marker approach.
  // If enabled, the timer picks up advanceRequested and sends the next chunk prompt.

  pi.registerTool({
    name: "decomp_chunk_done",
    label: "Decomp Chunk Done",
    description:
      "Signal that this decomp chunk is complete. " +
      "Call this at the end of every chunk — whether work was done, partially done, or blocked. " +
      "If the loop is enabled, prior context is compacted to a single summary line and the next " +
      "chunk starts immediately after, with essentially fresh context.",
    parameters: Type.Object({
      summary: Type.String({
        description: "1-3 sentence summary: what was done, which function(s), current metrics",
      }),
      blocked: Type.Optional(
        Type.Boolean({
          description: "Set true if you could not make meaningful progress",
        }),
      ),
      blockedReason: Type.Optional(
        Type.String({
          description: "If blocked=true, brief reason (e.g. 'no Docker', 'repeated mismatch on all candidates')",
        }),
      ),
    }),
    async execute({ summary, blocked = false, blockedReason }, _signal, ctx) {
      const repoRoot = findRepoRoot(ctx.cwd);
      const state = loadState(repoRoot);

      if (blocked) {
        state.enabled = false;
        state.advanceRequested = false;
        state.lastStatus = "blocked";
        state.lastBlockedReason = blockedReason || summary;
        state.lastChunkSummary = summary;
        saveState(repoRoot, state);
        applyWidget(latestCtx, repoRoot);
        if (latestCtx?.hasUI) {
          latestCtx.ui.notify(
            `decomp loop blocked: ${blockedReason || summary}`,
            "warning",
          );
        }
        return {
          content: [
            {
              type: "text",
              text: `Chunk ${state.chunk} marked blocked. Loop stopped.\nReason: ${blockedReason || summary}`,
            },
          ],
          details: { blocked: true, reason: blockedReason || summary },
        };
      }

      state.lastChunkSummary = summary;

      if (!state.enabled) {
        // Single-shot /decomp-next — just mark done, no advancement
        state.lastStatus = "done";
        saveState(repoRoot, state);
        applyWidget(latestCtx, repoRoot);
        return {
          content: [{ type: "text", text: `Chunk ${state.chunk} done (loop off). Summary: ${summary}` }],
          details: { chunkDone: true, chunkNum: state.chunk, willAdvance: false },
        };
      }

      // Loop is on — compact then immediately send next chunk prompt.
      // compact() is fire-and-forget with onComplete/onError callbacks.
      // The next chunk prompt arrives AFTER compact finishes so the agent
      // sees only [one-line compact summary] + [fresh chunk N+1 prompt].
      const nextChunkNum = state.chunk + 1;
      const nextPrompt = buildChunkPrompt(nextChunkNum);

      state.lastStatus = "compacting";
      saveState(repoRoot, state);
      applyWidget(latestCtx, repoRoot);

      const doAdvance = () => {
        const s = loadState(repoRoot);
        s.chunk = nextChunkNum;
        s.advanceRequested = false;
        s.lastStatus = "running";
        saveState(repoRoot, s);
        applyWidget(latestCtx, repoRoot);
        try {
          loopPi.sendUserMessage(nextPrompt);
        } catch {
          try { loopPi.sendUserMessage(nextPrompt, { deliverAs: "followUp" }); } catch { /* give up */ }
        }
      };

      ctx.compact({
        // One-line summary so context is essentially empty after compaction.
        // replaceInstructions is not available here — customInstructions appends to default.
        // Keeping it tight enough that the model outputs a single line.
        customInstructions:
          "Output ONLY a single line in this exact format (no other text): " +
          `'Chunk ${state.chunk} done: ${summary.split(".")[0]}'`,
        onComplete: doAdvance,
        onError: doAdvance, // advance even if compact fails
      });

      return {
        content: [
          {
            type: "text",
            text:
              `Chunk ${state.chunk} complete. Compacting context, then starting chunk ${nextChunkNum}.\n` +
              `Summary: ${summary}`,
          },
        ],
        details: { chunkDone: true, chunkNum: state.chunk, willAdvance: true, nextChunkNum },
      };
    },
  });

  // ── Command: /decomp-next ──────────────────────────────────────────────────
  // Single chunk in a fresh session (one-shot, not part of the continuous loop).

  pi.registerCommand("decomp-next", {
    description: "Start one fresh-context WarioWare decomp chunk (no loop)",
    handler: async (_args, ctx) => {
      await ctx.waitForIdle();
      const repoRoot = findRepoRoot(ctx.cwd);
      const state = loadState(repoRoot);
      state.chunk += 1;
      state.enabled = false;
      state.advanceRequested = false;
      state.lastStatus = "launching";
      saveState(repoRoot, state);

      const chunkNum = state.chunk;
      const prompt = buildChunkPrompt(chunkNum);
      const parentSession = ctx.sessionManager.getSessionFile();

      await ctx.newSession({
        parentSession,
        withSession: async (newCtx) => {
          newCtx.ui.notify(`Started decomp chunk ${chunkNum} (single)`, "info");
          await newCtx.sendUserMessage(prompt);
        },
      });
    },
  });

  // ── Command: /decomp-loop ──────────────────────────────────────────────────

  pi.registerCommand("decomp-loop", {
    description: "Manage the WarioWare decomp loop [start|stop|status|reset]",
    handler: async (args, ctx) => {
      const repoRoot = findRepoRoot(ctx.cwd);
      const state = loadState(repoRoot);
      const sub = args.trim().toLowerCase();

      if (sub === "start") {
        await ctx.waitForIdle();
        ensureTimer();
        state.enabled = true;
        state.chunk += 1;
        state.advanceRequested = false;
        state.lastStatus = "launching";
        saveState(repoRoot, state);
        applyWidget(ctx, repoRoot);
        ctx.ui.notify(`Starting decomp loop at chunk ${state.chunk}`, "info");
        // First chunk in same session — subsequent chunks advance via timer
        loopPi.sendUserMessage(buildChunkPrompt(state.chunk));
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

  // ── Events ─────────────────────────────────────────────────────────────────

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

    // Already handled by decomp_chunk_done tool call → nothing to do here
    if (state.advanceRequested || state.lastStatus === "blocked") {
      applyWidget(ctx, repoRoot);
      return;
    }

    // Fallback: detect text markers in case the agent printed them instead of calling the tool
    const text = extractAssistantText(event.messages);

    if (text.includes(BLOCKED_MARKER)) {
      const idx = text.lastIndexOf(BLOCKED_MARKER);
      const tail = text.slice(idx + BLOCKED_MARKER.length).trim().split(/\r?\n/)[0] ?? "";
      state.enabled = false;
      state.lastStatus = "blocked";
      state.lastBlockedReason = tail;
      saveState(repoRoot, state);
      applyWidget(ctx, repoRoot);
      if (ctx.hasUI) ctx.ui.notify(`decomp loop blocked: ${tail || "(no reason)"}`, "warning");
      return;
    }

    if (text.includes(DONE_MARKER)) {
      state.advanceRequested = true;
      state.lastStatus = "waiting-to-advance";
      saveState(repoRoot, state);
      applyWidget(ctx, repoRoot);
      return;
    }

    // No signal at all — just update widget
    state.lastStatus = "no-signal";
    saveState(repoRoot, state);
    applyWidget(ctx, repoRoot);
  });

  pi.on("session_shutdown", async () => {
    latestCtx = undefined;
  });
}
