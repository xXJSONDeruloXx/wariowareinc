# Decomp tooling feedback log

Use this file to record where the current decomp tools helped, where they missed integration risk, and what manual workaround was needed. The goal is to improve future automation without reducing current verification rigor or tool scope.

## Batch 109 — linked included-stub mini-batch
- Tools that helped:
  - `compile_and_view_asm` correctly proved isolated instruction matches for `func_08014E88` and `func_080152A0`.
  - `apply_conversion` safely performed the include-shim edits, asm moves, clean Docker builds, and ROM identity checks for each function.
- Tooling/workflow gap:
  - Chunk 29 initially misdiagnosed several candidates as callee-risk because the worktree had an unrelated manual edit in `src/decomp/asm_08012c64.c` from a failed `func_08014374` experiment. `apply_conversion` restored its own attempted edits, but it cannot know whether unrelated pre-existing manual edits are accidental.
  - `query_candidates` and `preflight_candidate` are useful but do not express linked-batch opportunities or dirty-worktree risk.
- Manual workaround:
  - Run `git status --short` before and after failed `apply_conversion` attempts.
  - Revert unrelated edits before retrying a 100% isolated match.
  - For linked included stubs, apply the callee first and verify ROM, then apply the caller.
- Desired tooling improvement:
  - Add a pre-apply clean-worktree warning or at least list unrelated dirty files before running the Docker build.
  - Add a candidate-family mode that suggests tiny callee-before-caller batches instead of only one-function candidates.

## Batch 133 — real-C-first / naked-asm follow-up
- Tools that helped:
  - `compile_and_view_asm` made it obvious when a candidate was only failing on register allocation or epilogue shape, which is exactly the point where a real-C pass is worth trying.
  - `make report` + `tools/gen_objdiff.py` confirmed the post-chunk baseline after the accepted `func_08001D5C` included-stub conversion.
- Tooling/workflow gap:
  - The loop prompt previously did not explicitly reserve time for revisiting existing naked asm files with a real-C attempt after the primary chunk work was done.
  - Some candidates that look like simple C rewrites still collapse into return-shape or scratch-register problems, so the prompt needs to steer agents toward "real C first, naked asm last" instead of treating naked asm as a default fallback.
- Manual workaround:
  - I updated `docs/decomp-agent-workflow.md`, `docs/decomp-pattern-library.md`, and the decomp-loop prompt to make the real-C-first rule explicit and to reserve spare capacity for revisting existing naked asm files.
- Desired tooling improvement:
  - Surface a lightweight reminder in chunk prompts when the candidate queue is thin: try one real-C conversion pass on an existing naked file before reaching for a new naked file or another manual asm wrapper.

## Batch 134 — task-wrapper real-C shape
- Tools that helped:
  - `compile_and_view_asm` confirmed the last-mile difference on `func_0800C110` was just epilogue shape and one missing return, not the stack layout.
  - `apply_conversion` preserved the host-TU include order correctly for the included-stub conversion.
- Tooling/workflow gap:
  - Task-launcher wrappers with on-stack structs are easy to under-specify when the return type is left as `void`; that hides the correct `POP {R1}; BX R1` epilogue.
- Manual workaround:
  - Use the `start_new_task` return value directly in the wrapper when the asm returns via `bx r1`, and keep the task arguments in a local struct so the compiler emits the same `strh` sequence.
- Desired tooling improvement:
  - Add a small reminder in the loop prompt for stack-struct wrappers: if the asm returns a value, try `return start_new_task(...)` before falling back to void/naked variants.

## Batch 135 — anti-ASM-cop-out guardrails
- Tools that helped:
  - `vcc_recall` surfaced the earlier prompt edits and the exact toxic failure mode: a near-miss compile result leading straight to naked asm.
  - The existing naked-asm real-C docs already had the right ingredients; the missing piece was a stricter loop prompt that treats naked asm as maintenance, not a fallback.
- Tooling/workflow gap:
  - The loop prompt still made the naked-asm maintenance pass feel optional and low-friction, which is too easy for weaker agents to abuse when a candidate is only a register-allocation mismatch away from perfect.
  - Included-stub real-C conversions can fail late on host-TU symbol collisions if helper typedef names are reused across files.
- Manual workaround:
  - Tightened the loop prompt and the workflow docs to say: keep iterating on C when a candidate is close, and only revisit naked asm with a concrete C-shaped plan after the main chunk is done or blocked.
  - Added a reminder to use unique helper typedef names (or anonymous structs) inside included-stub decomp files.
- Desired tooling improvement:
  - Add a stronger visible cue in the chunk loop UI when the agent is about to switch from a near-miss C candidate to a naked asm target.
  - Consider a lint/pass that flags duplicate helper typedef names across `src/decomp/*.c` includes before `apply_conversion` runs.

