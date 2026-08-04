# Decomp tooling feedback log

Use this file to record where the current decomp tools helped, where they missed integration risk, and what manual workaround was needed. The goal is to improve future automation without reducing current verification rigor or tool scope.

### Related docs
- `docs/windows-tooling-notes.md` — Windows/MSYS2/Docker path issues and fixes (added 2025-06-26)
- `.pi/extensions/warioware-decomp-loop.js` — loop prompt includes a "Documentation discipline" section that instructs the AI to record tooling issues as they're encountered

## m2c/asmlift adapter — strict re-hoist (2026-08-04)
- The restored `tools/asmlift_warioware.py` adapter finds an unconverted Splat assembly source, normalizes its address-commented Thumb syntax, runs the sibling Mizuchi `m2c`, and can pass an optional candidate through the globally installed `asmlift` executable.
- `tools/asmlift-compile.sh` supplies project headers and reuses the Docker compiler bridge, so generated candidates can be tested in the same agbcc context as normal decompilation units.
- On `func_08003D28`, m2c provided the correct mask-operation skeleton and asmlift was useful as a second attempted synthesis path, but asmlift declined the candidate because its generated type context did not compile against this project. The accepted spelling still required manual register pins and an empty barrier for the `MOVS`/`LSLS`/`RSBS` instruction shape.
- On `func_0800DAD8`, asmlift produced the exact accepted C spelling on its first project-header-aware attempt, including the signed index cast and 48-byte stride. The adapter therefore moved this function from source discovery through candidate synthesis quickly; the strict build and isolated object diff remained the acceptance gates.
- On `func_08015F80`, m2c's compact condition and asmlift's nested early-return spelling were both useful, with the latter matching the original branch layout exactly. This is a good adapter-assisted pattern for small control-flow leaves: compare both generators, then retain the candidate whose structure preserves the target branches.
- On `func_0803FED0`, the adapter exposed the global-halfword semantics but initially folded the non-zero offset into the literal address. An empty barrier on a local absolute pointer recovered the original base literal plus `[base, #2]` load; isolated objdiff and the strict ROM build both confirmed the workaround.
- On `func_080F26BC`, m2c correctly identified the raw record offsets, while the first C spelling lost the target's three-register add and loaded the base too late. Register pins plus `r3 = (u8 *)((u32)r1 + (u32)r3)` recovered `ADDS R3,R1,R3`; this was exact in isolation and in the strict ROM.
- On `func_080F2F68`, m2c surfaced a non-obvious ABI gap: the function uses its third incoming argument from `R2`, so a two-parameter C spelling sign-extended `R1` instead. Modeling the unused middle parameter made the call setup exact without inline instruction shims.
- On `func_080F2F78`, the same adapter insight transferred directly to the signed-byte sibling: only the shift width changed from 16 to 24, and the strict object/ROM gates confirmed the family spelling.
- On `func_08073650`, m2c identified the two-call wrapper but the first isolated C object carried an alignment NOP after the function instead of the source's explicit zero halfword. A `.text`-section `u16` padding symbol fixed the bytes; objdiff then matched all five instructions and the strict ROM stayed identical. Standalone reports count that padding symbol as an additional matched text symbol.
- On `func_08002024`, the full target and candidate `.text` sections were identical, but objdiff inferred the target function size only through the first internal local label and reported a misleading 40% symbol match. When target asm lacks an explicit function end before a local branch label, compare the complete section bytes and require the strict linked ROM gate.
- On `func_08007E8C`, the m2c wrapper was straightforward and the compiler naturally reproduced the source's alignment halfword before the `0x7FFFFFFF` literal. The full section and strict ROM matched; the report's total-function recount shifted by one because of the target literal-pool/local-symbol layout, so metrics must be copied from the fresh report rather than inferred by arithmetic.
- Strict workflow lesson: adapters are candidate generators and diagnostics only. Every adopted function still needs an isolated object diff plus a clean `NONMATCHING=0` Docker build with `wariowareinc.gba: OK`; this first re-hoist preserved the baseline ROM SHA-1 exactly.

## Guard/loop hardening — non-empty inline asm ban and unstoppable loop
- Tools that helped:
  - Recent chunks proved the naked-asm guard works, but also showed weaker agents can still hide meaningful instruction sequences inside non-empty `asm volatile` blocks.
  - The loop already had a deprecated no-progress compatibility field on `decomp_chunk_done`, but no-progress/text-marker/no-signal paths were not equally robust about keeping the stream moving.
- Tooling implemented:
  - `.pi/extensions/warioware-decomp-guard.js` now blocks new non-empty statement-form inline asm in `src/decomp/*.c` payloads and file edits. Empty asm barriers/clobbers remain allowed; register-pinning declarations like `register u32 r0 asm("r0")` are not treated as inline asm statements.
  - `.pi/extensions/warioware-decomp-loop.js` now has no blocked loop state: no-progress chunks, the legacy text marker, and missing completion signals all compact and advance immediately. No-progress means "try a different candidate next," not "stop the stream."
- Follow-up idea:
  - If accepted legacy non-empty inline asm files are edited later, convert them to pure C/empty-barrier C instead of preserving the instruction shim.

## Batch 147 — same-TU unprototyped callee call shaping
- Tools that helped:
  - `compile_and_view_asm` made the last-mile differences explicit: first the `sp+0xA` pointer was in `r3` instead of `r2`, then the generated coordinate load was `LDRH; LSLS; ASRS` instead of indexed `LDRSH`.
  - `apply_conversion` handled the bitmap_font include-shim and clean Docker ROM verification once the isolated match was perfect.
- Tooling/workflow gap:
  - The candidate did not surface that the already-converted same-TU callee prototype for `func_0800C110` would influence caller-side truncation/sign-extension.
- Manual workaround:
  - Use narrow asm only for the helper-call setup and two indexed `LDRSH` loads, keep the rest of the wrapper in C, and declare the callee old-style (`extern void *func_0800C110();`) so agbcc does not re-truncate already-shaped register arguments.
- Desired tooling improvement:
  - For included-stub callers of converted same-TU functions, show both the current callee prototype and whether an old-style no-prototype declaration may be needed to preserve original register argument shaping.

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
