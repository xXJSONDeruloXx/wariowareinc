import fs from "node:fs";
import path from "node:path";
import { truncateToWidth } from "@earendil-works/pi-tui";
import { Type } from "typebox";

const WIDGET_KEY = "warioware-decomp-loop";
const REFRESH_INTERVAL_MS = 3000;
const STATE_FILE = path.join(".pi", "decomp-loop-state.json");
const REPO_SENTINEL = "wariowareinc.ld";

// Text-based fallback markers (used if agent prints text instead of calling the tool)
const DONE_MARKER = "<DECOMP_CHUNK_DONE>";
const NO_PROGRESS_MARKER = "<DECOMP_BLOCKED>"; // legacy text marker name, treated as no-progress

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
    lastNoProgressReason: "",
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
    "- docs/windows-tooling-notes.md",
    "  (read this for known Windows/MSYS2/Docker issues; add new ones as you encounter them)",
    "",
    "## Step 2 — Select and preflight a target function",
    "Use `query_candidates` (strategy: 'smallest' or 'families'; default conversionMode: 'recommended').",
    "Recommended candidates include both standalone_tu and included_stub workflows that the tools can apply mechanically.",
    "Then call `preflight_candidate` for the chosen function.",
    "Proceed if preflight says safeForAutonomous=true. Supported workflows are standalone_tu and included_stub.",
    "If only unknown_skip/manual candidates remain, use conversionMode='all' for diagnostics, pick the most promising small candidate, and block only if no supported/manual path is reasonable.",
    "Keep one function per src/decomp file. A tiny linked batch is allowed when the dependency order is clear.",
    "",
    "## Step 3 — Gather context",
    "Call `get_function_context` with the chosen function name if it has an including C source; otherwise read the asm file and relevant include/ headers directly.",
    "Use returned or manually-read headers/typedefs for accurate typing.",
    "",
    "## Step 4 — Get an initial C guess (optional but recommended)",
    "Call `m2c_decompile` for a GBA-target rule-based starting skeleton, then cross-check against the asm.",
    "If m2c is unavailable or weak on the function, write C from the asm and documented patterns, then test it.",
    "",
    "## Step 4b — Use spare capacity to convert an existing naked asm file to real C only when it has a clear C-shaped plan",
    "This is a maintenance pass, not a fallback for a stubborn primary candidate. Only do it after the main chunk goal is done or the candidate queue yields no progress, and only if you can explain the exact C-shaping plan before you start.",
    "Use the documented effort checklist: pure C, register pinning, empty asm barriers/clobbers only, statement reordering, type shaping, load-base-first, pointer shaping, and goto loops.",
    "There is no attempt limit. If it does not match, keep iterating in real C or pick another real-C candidate. If the function is one of the documented hard cases, leave it untouched and record why; do not create a new naked asm wrapper.",
    "",
    "## Step 5 — Iterate with isolated compile",
    "Call `compile_and_view_asm` with your current C code.",
    "Repeat until match_percent = 100% (PERFECT MATCH). If the result is close but not exact, keep shaping C; do not pivot to naked asm as a shortcut.",
    "When you have multiple spellings or a small sibling batch, put the candidate paths in a JSON manifest and use `python3 tools/decomp_cycle.py isolate --manifest MANIFEST`; it compiles the batch in one Docker invocation and writes a .decomp-runs receipt.",
    "Do NOT run a full Docker make build during iteration.",
    "",
    "## Step 6 — Apply and verify (only after PERFECT MATCH)",
    "Prefer `python3 tools/decomp_cycle.py apply --manifest MANIFEST --function FUNC` after an exact manifest result; it records isolation, applies the source/linker/include/asm move, runs the clean Docker/report gate, and restores the candidate transaction plus a clean baseline on failure. `apply_conversion` remains the Pi frontend fallback.",
    "Require: wariowareinc.gba: OK.",
    "For included_stub candidates, apply_conversion preserves host-TU order by replacing the asm include with a guarded src/decomp C include.",
    "",
    "## Step 7 — Commit, report, and signal done",
    "1. docker run --rm -v \"$PWD:/workspace\" -w /workspace devkitpro/devkitarm:latest bash -lc 'make report'",
    "   On macOS do NOT run local make/make report for verification; use Docker because tools/agbcc in-repo is not a reliable host-native path.",
    "2. python3 tools/gen_objdiff.py",
    "3. Count src/decomp/*.c separately so docs keep standalone_tu vs included_stub progress distinct from linked C TU coverage",
    "4. Update docs (README, scaleup, batch-history, pattern-library if new patterns, windows-tooling-notes.md for tooling issues)",
    "5. git add -A && git commit -m 'feat: ...' && git push",
    "6. Call the `decomp_chunk_done` tool with a brief summary.",
    "   If no progress was made, still call `decomp_chunk_done` with a concise no-progress summary; the loop will immediately continue.",
    "",
    "## Documentation discipline — record tooling issues as you encounter them",
    "- When a tool fails with an unexpected error, the fix is NOT to work around it silently and move on.",
    "  Instead: reproduce the failure, find the root cause, fix the tool or script, and write a concise entry",
    "  to `docs/windows-tooling-notes.md` (Windows) or `docs/decomp-tooling-feedback.md` (non-Windows).",
    "- The entry should include: symptom, root cause, fix, and where it applies (which tool/script).",
    "- This prevents the same issue from being re-discovered in future chunks. The loop prompt reads this doc",
    "  in Step 1, so future chunks inherit the knowledge.",
    "",
    "## Rules",
    "- Do not create or rely on Ralph loops.",
    "- Do not ask the user to choose; pick the best default and proceed.",
    "- Use compile_and_view_asm or decomp_cycle.py isolate for iteration — full builds only inside decomp_cycle.py apply/verify or apply_conversion.",
    "- Install .githooks with tools/install-hooks.sh; do not use git commit --no-verify or push a ROM-affecting change without the Docker gate.",
    "- Do not add non-empty inline asm. Empty asm barriers/clobbers are allowed; instruction/call shims are not.",
    "- A 100% isolated compile match is not accepted progress unless preflight is standalone_tu and the clean ROM build is byte-identical.",
    "- Preserve byte-identical ROM at every accepted milestone.",
    "- If a tool/script fails with a novel error, fix it and document the issue in windows-tooling-notes.md or decomp-tooling-feedback.md before moving on.",
    "- You MUST call `decomp_chunk_done` when the chunk is finished or no-progress; after calling it, do not send a final explanatory response — let the loop advance.",
  ].join("\n");
}

function getLoopPower(state) {
  return state.enabled
    ? { label: "on", color: "success" }
    : { label: "off", color: "dim" };
}

function getChunkActivity(state) {
  if (state.lastStatus === "launching" || state.lastStatus === "running" || state.lastStatus === "advancing" || state.lastStatus === "compacting") {
    return { label: "in progress", color: "accent" };
  }
  if (state.advanceRequested || state.lastStatus === "waiting-to-advance") {
    return { label: "queued", color: "accent" };
  }
  return { label: "idle", color: "muted" };
}

function getLoopPhase(state) {
  if (state.lastStatus === "no-signal") return { label: "stale", color: "warning" };
  if (state.lastStatus === "advancing" || state.lastStatus === "compacting") {
    return { label: state.lastStatus, color: "accent" };
  }
  if (state.lastStatus === "done") return { label: "done", color: "success" };
  if (state.lastStatus === "running" || state.lastStatus === "launching") {
    return { label: state.lastStatus, color: "success" };
  }
  return { label: state.lastStatus || "idle", color: "muted" };
}

function formatStatus(state) {
  const power = getLoopPower(state);
  const activity = getChunkActivity(state);
  const phase = getLoopPhase(state);
  const bits = [`loop ${power.label}`, `chunk ${state.chunk}`, activity.label, `state ${phase.label}`];
  if (state.advanceRequested) bits.push("advance pending");
  if (state.lastStatus === "no-signal") bits.push("last run ended without decomp_chunk_done");
  return bits.join(" · ");
}

function buildLoopLines(theme, width, state) {
  const power = getLoopPower(state);
  const activity = getChunkActivity(state);
  const phase = getLoopPhase(state);
  const divider = theme.fg("borderMuted", "─".repeat(Math.max(0, width)));
  const header = [
    theme.fg("accent", theme.bold("↻ Decomp Loop")),
    theme.fg(power.color, power.label.toUpperCase()),
    theme.fg("muted", `chunk ${state.chunk}`),
    theme.fg(activity.color, activity.label.toUpperCase()),
  ];

  const lines = [
    divider,
    truncateToWidth(header.join(` ${theme.fg("dim", "·")} `), width),
  ];

  let detail = `state: ${phase.label}`;
  let detailColor = phase.color;
  if (state.advanceRequested) {
    detail += " · advance pending";
  }
  if (state.lastStatus === "no-signal") {
    detail += " · last run ended without decomp_chunk_done";
  }
  lines.push(truncateToWidth(theme.fg(detailColor, detail), width));


  lines.push(divider);
  return lines;
}

function applyWidget(ctx, repoRoot) {
  if (!ctx?.hasUI) return;
  const state = loadState(repoRoot);
  ctx.ui.setWidget(WIDGET_KEY, (_tui, theme) => ({
    render(width) {
      return buildLoopLines(theme, width, state);
    },
    invalidate() {},
  }));
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
      "Signal that this decomp chunk is complete or no-progress. " +
      "If the loop is enabled, prior context is compacted to a single summary line and the next " +
      "chunk starts immediately after, with essentially fresh context. " +
      "Do not send a final explanatory response after calling this tool.",
    parameters: Type.Object({
      summary: Type.String({
        description: "1-3 sentence summary: what was done, which function(s), current metrics",
      }),
      blocked: Type.Optional(
        Type.Boolean({
          description: "Deprecated compatibility flag for no-progress chunks. This never creates a blocked loop state; the next chunk starts automatically.",
        }),
      ),
      blockedReason: Type.Optional(
        Type.String({
          description: "Optional no-progress reason (e.g. 'no Docker', 'candidate mismatch')",
        }),
      ),
    }),
    async execute(_id, { summary, blocked = false, blockedReason }, _signal, _onUpdate, ctx) {
      const repoRoot = findRepoRoot(ctx.cwd);
      const state = loadState(repoRoot);
      const finalSummary = blocked && blockedReason
        ? `${summary} No-progress reason: ${blockedReason}`
        : summary;

      state.lastChunkSummary = finalSummary;
      state.lastNoProgressReason = blockedReason || "";

      if (!state.enabled) {
        // Single-shot /decomp-next — just mark done, no advancement.
        // Even no-progress chunks are "done" here; there is no blocked state.
        state.lastStatus = "done";
        state.advanceRequested = false;
        saveState(repoRoot, state);
        applyWidget(latestCtx, repoRoot);
        return {
          content: [{ type: "text", text: `Chunk ${state.chunk} done (loop off). Summary: ${finalSummary}` }],
          details: { chunkDone: true, chunkNum: state.chunk, willAdvance: false, noProgress: Boolean(blocked) },
        };
      }

      // Loop is on — compact then immediately send next chunk prompt.
      // No-progress chunks use the same path as successful chunks; the stream
      // should never pause in a special blocked state.
      const nextChunkNum = state.chunk + 1;
      const nextPrompt = buildChunkPrompt(nextChunkNum);

      state.lastStatus = "compacting";
      state.advanceRequested = false;
      saveState(repoRoot, state);
      applyWidget(latestCtx, repoRoot);

      if (blocked && latestCtx?.hasUI) {
        latestCtx.ui.notify(
          `decomp loop no-progress at chunk ${state.chunk}; compacting and advancing`,
          "warning",
        );
      }

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
          try { loopPi.sendUserMessage(nextPrompt, { deliverAs: "followUp" }); } catch {
            const retry = loadState(repoRoot);
            retry.advanceRequested = true;
            retry.lastStatus = "waiting-to-advance";
            saveState(repoRoot, retry);
          }
        }
      };

      ctx.compact({
        onComplete: doAdvance,
        onError: doAdvance,
      });

      return {
        content: [
          {
            type: "text",
            text:
              `Chunk ${state.chunk} complete. Compacting context, then starting chunk ${nextChunkNum}.
` +
              `Summary: ${finalSummary}`,
          },
        ],
        details: { chunkDone: true, chunkNum: state.chunk, willAdvance: true, nextChunkNum, noProgress: Boolean(blocked) },
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
    getArgumentCompletions: (prefix) => {
      const options = [
        { value: "start",  label: "start  — begin autonomous loop (fresh chunk each iteration)" },
        { value: "stop",   label: "stop   — halt loop after current chunk" },
        { value: "status", label: "status — show current loop state" },
        { value: "reset",  label: "reset  — clear loop state file" },
      ];
      const filtered = options.filter((o) => o.value.startsWith(prefix.toLowerCase()));
      return filtered.length > 0 ? filtered : options;
    },
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

  // Static compaction — bypass LLM summary entirely.
  // When compact fires (either from decomp_chunk_done or auto context-overflow),
  // return a fixed one-liner immediately. No API call, no delay, zero prior context
  // for the agent. Canonical docs are the project's memory, not chat history.
  pi.on("session_before_compact", (event, _ctx) => {
    return {
      compaction: {
        summary: "[prior work complete — re-read canonical docs for context]",
        firstKeptEntryId: event.preparation.firstKeptEntryId,
        tokensBefore: event.preparation.tokensBefore,
      },
    };
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

    // Already handled by decomp_chunk_done tool call → nothing to do here
    if (state.advanceRequested) {
      applyWidget(ctx, repoRoot);
      return;
    }

    // Fallback: detect text markers in case the agent printed them instead of calling the tool
    const text = extractAssistantText(event.messages);

    if (text.includes(NO_PROGRESS_MARKER)) {
      const idx = text.lastIndexOf(NO_PROGRESS_MARKER);
      const tail = text.slice(idx + NO_PROGRESS_MARKER.length).trim().split(/\r?\n/)[0] ?? "";
      state.advanceRequested = true;
      state.lastStatus = "waiting-to-advance";
      state.lastNoProgressReason = tail;
      saveState(repoRoot, state);
      applyWidget(ctx, repoRoot);
      if (ctx.hasUI) ctx.ui.notify(`decomp loop no-progress marker: ${tail || "(no reason)"}; advancing`, "warning");
      return;
    }

    if (text.includes(DONE_MARKER)) {
      state.advanceRequested = true;
      state.lastStatus = "waiting-to-advance";
      saveState(repoRoot, state);
      applyWidget(ctx, repoRoot);
      return;
    }

    // No signal at all — keep the stream moving anyway. A missing
    // decomp_chunk_done should not stall an unattended loop.
    state.advanceRequested = true;
    state.lastStatus = "waiting-to-advance";
    state.lastNoProgressReason = "last run ended without decomp_chunk_done";
    saveState(repoRoot, state);
    applyWidget(ctx, repoRoot);
    if (ctx.hasUI) ctx.ui.notify("decomp loop: no completion signal; advancing anyway", "warning");
  });

  pi.on("session_shutdown", async () => {
    latestCtx = undefined;
  });
}
