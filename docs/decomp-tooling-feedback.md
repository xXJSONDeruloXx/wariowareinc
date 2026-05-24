# Decomp tooling feedback log

Use this file to record where the current decomp tools helped, where they missed integration risk, and what manual workaround was needed. The goal is to improve future automation without reducing current verification rigor or tool scope.

## Batch 146 — same-TU callee prototype shaping
- Tools that helped:
  - `compile_and_view_asm` quickly showed `func_0800C0BC` was a perfect match if `func_0800C080`'s second parameter was treated as signed, and that the only pure-C miss under the old prototype was `LDRH` vs `LDRSH`.
  - `apply_conversion` handled the include-shim, asm move, clean Docker build, and ROM identity check after the manual same-TU prototype adjustment.
- Tooling/workflow gap:
  - `preflight_candidate` did not flag that `func_0800C0BC` calls an already-converted included-stub callee in the same TU, so the necessary prototype compatibility check was manual.
- Manual workaround:
  - Update the already-converted callee signature from `u16` to `s16` only after preserving its own byte shape with an explicit `(u16)` cast in the callee body, then apply the caller and let the clean ROM build prove the linked result.
- Desired tooling improvement:
  - When an included-stub candidate calls a converted same-TU function, surface the current callee prototype and note whether signedness may affect caller load instructions (`LDRH` vs `LDRSH`).

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
  - Tightened the loop prompt and workflow docs to say: keep iterating on C when a candidate is close; never turn a near-miss into naked asm progress. Existing naked files are maintenance-only targets for removal.
  - Added a reminder to use unique helper typedef names (or anonymous structs) inside included-stub decomp files.
- Tooling implemented:
  - Added `.pi/extensions/warioware-decomp-guard.js` to block `compile_and_view_asm` / `apply_conversion` C payloads, `write` / `edit` changes, suspicious `bash` writes, and `git add` / `git commit` attempts that introduce or preserve naked/original asm in changed `src/decomp/*.c` files.
  - Added `decomp_guard_check` so agents can explicitly scan changed `src/decomp` files before committing.
- Desired tooling improvement:
  - Consider a lint/pass that flags duplicate helper typedef names across `src/decomp/*.c` includes before `apply_conversion` runs.

## Batch 135 cleanup — legacy wrapper burn-down
- Tools that helped:
  - The guard forced the right behavior: edited legacy files had to become real C before commit, and `decomp_guard_check` verified no changed `src/decomp` files still contained naked/original asm.
  - Full Docker ROM builds caught semantically-correct but byte-different rewrites (`& 1` vs shift pair, unpinned copy registers, header return-type ripple).
- Tooling/workflow gap:
  - Already-converted legacy files often cannot use `compile_and_view_asm` because their target asm object has moved; for those, clean Docker ROM builds remain the reliable verifier.
  - `make -j4` can pick up stale dependency state after failed include-shim experiments; removing `build/` before verification avoids false missing-source errors.
- Manual workaround:
  - Burned down 7 legacy wrappers to real C and shrank the guard exception list to the 7 wrappers that still need real-C work.
- Desired tooling improvement:
  - Add first-class support for comparing an already-converted `src/decomp/*.c` against its `asm/converted/*.s` target so legacy wrapper cleanup can iterate without full ROM builds.

## Batch 136 cleanup — reducing legacy quarantine to one file
- Tools that helped:
  - Full ROM verification caught operand-order differences (`r0 + r3` vs `r3 + r0`) and caller-side codegen ripples from changing helper return prototypes.
  - The guard's changed-file scan stayed useful while shrinking the legacy allowlist; converted files now need no legacy exception.
- Remaining blocker:
  - `func_0800BEC0` is the only naked/whole-function wrapper left. Pure C still hits the documented `CMP #1/BGE` vs `CMP #0/BGT` optimizer trap.
- Follow-up idea:
  - Decide whether a very small branch/compare inline-asm workaround is acceptable for `func_0800BEC0`, or keep it quarantined until a pure-C spelling is found.

