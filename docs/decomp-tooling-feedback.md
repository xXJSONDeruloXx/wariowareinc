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

## Legacy inline-asm reshaping pass (2026-08-04)
- The repo-local m2c/asmlift adapter was not needed for this maintenance pass; direct source/target instruction comparison plus the Docker agbcc build was the faster path for these already-understood wrappers. m2c/asmlift remains useful for discovering and synthesizing new functions, as recorded in the earlier re-hoist section.
- The repeatable solution was an ABI-shaping function-pointer typedef: widen `s16`/`s8`/`u16` helper parameters to `s32`/`u32` at the local call site so already-shaped ABI registers are not re-truncated or swapped. This converted the old non-empty BL shims to ordinary C calls across 25 included-stub files.
- Register-pinned C recovered the two indexed `LDRSH` loads and the exact operand order in `func_08012DCC`; ordinary C indirect dispatch reproduced `_call_via_r0`. A first `func_08012DCC` spelling missed by one byte, so the strict ROM gate caught and corrected the addition order.
- The same strict gate rejected pure-C attempts for the two-operand in-place `ADD` forms in `func_080141C8` and `func_08014DFC`, and the known `func_0800BEC0` compare trap remains. Those shims were restored rather than weakening the byte-matching rule.
- Metrics were unchanged at this pass's starting point because the 25 files were already C-linked included stubs: **1362 / 5960**, **6.4803877%** matched code, and **902 C / 5785 asm-only** objdiff units. The final clean Docker build still reported `wariowareinc.gba: OK`; the six legacy files listed there were then resolved by Batches 160–164 (real C) and Batch 165 (the honest asm-only SVC exception).

## Batch 160 — condition-code barrier for two-operand ADD (2026-08-04)
- A small isolated compiler probe showed that `asm volatile("" : "+r"(r2) : : "cc")` immediately before ordinary `r2 += 2` forces the exact Thumb `ADDS R2,#2`; the same empty barrier is allowed by the strengthened guard because it contains no instruction text.
- `func_080141C8` matched in the linked `main_menu.c` object and passed a clean `NONMATCHING=0` Docker build. This is a useful new shaping rule, while the analogous `R5 += 4` case remains unresolved.
- The maintenance count is now **26 reshaped files**, with five non-empty inline-asm files remaining.

## Batch 161 — callee-saved register ADD shaping (2026-08-04)
- The condition-code barrier pattern transferred unchanged from `R2` to callee-saved `R5`: ordinary `r5 += 4` emits `ADDS R5,#4` after `asm volatile("" : "+r"(r5) : : "cc")`.
- `func_08014DFC` passed the linked object check and clean Docker ROM gate. The maintenance count is now **27 reshaped files**, with four non-empty inline-asm files remaining.

## Batch 162 — compiler-generated STMIA (2026-08-04)
- A small pointer-loop probe showed that agbcc emits `STMIA` when the store pointer is a `u32 *` and the source uses `*r2++ = r1`; this was more effective than manually pinning an integer address and trying to model the store as a byte/word assignment.
- `func_08015A4C` matched through its literal pool and passed the clean Docker ROM gate. The maintenance count is now **28 reshaped files**, with three non-empty inline-asm files remaining.

## Batch 163 — pure-C generated-coordinate wrapper (2026-08-04)
- An isolated pure-C probe showed that non-volatile `s16` locals are important: `volatile` forces `LDRH` plus explicit sign-extension, while ordinary locals let agbcc select the target indexed `LDRSH` form. A typed `func_08006F84(s16, s16 *, s16 *)` declaration also reproduced the target `SB = SP+0xA` setup.
- `func_0800C15C` matched in the integrated bitmap-font object and passed the clean Docker ROM gate. The maintenance count is now **29 reshaped files**, with two non-empty inline-asm files remaining.

## Batch 164 — pure-C range-dispatch shaping (2026-08-04)
- The documented direct-comparison trap was not a fundamental blocker for a byte-valued switch. An ordinary `switch` with cases 1–3 and 4 still gave the wrong lower-bound-first tree; adding a redundant `case -10` that shares the default target makes agbcc retain the target upper-bound-first range dispatch without any non-empty inline asm. The case is unreachable because the switch value is loaded from a `u8` byte.
- `func_0800BEC0` matched byte-for-byte in the integrated bitmap-font object, including its literal-pool padding, and passed the clean Docker ROM gate. The maintenance count is now **30 reshaped files**, with only the BIOS `svc #6` wrapper still containing non-empty inline asm.
- This is a compiler-shaping workaround, so keep the value-domain justification beside the switch; do not generalize the redundant case to a signed value whose negative range is reachable.

## Batch 165 — standalone BIOS SVC exception (2026-08-04)
- The final legacy non-empty inline-asm file was `func_080EE61C`, whose exact target body is only `SVC #6; BX LR` (the GBA BIOS signed-division service).
- Direct C `a0 / a1` is not an exact substitute under the bundled agbcc: it emits a call to `__divsi3`. Probes for the usual ARM SVC/SWI builtins and attributes did not produce an SVC instruction; the installed devkitARM `libgba` has an exact `Div.o`, but that is also external assembly and would not turn this function into real C.
- m2c/asmlift therefore cannot solve this particular compiler/ISA boundary. The honest project form is a standalone `asm/asm_080ee61c.s` source with the original two instructions, while all `src/decomp` C files remain free of non-empty instruction asm.
- The clean Docker ROM gate passed with `wariowareinc.gba: OK`; both ROMs hash to `3f556448d290fa5406d6ed367fee16cc02387ad3`. The report moves from **1362 / 5960** to **1361 / 5960** and from **902 C / 5785 asm-only** to **901 C / 5786 asm-only** solely because objdiff no longer classifies this function as C-produced output.
- Durable rule: do not trade an honest asm-only BIOS exception for a fake C wrapper or a non-empty inline-asm shim merely to preserve the decompilation counter.

## Batch 166 — compiler-level BIOS SVC lowering (2026-08-04)
- The remaining wrapper is now real C. A target-specific `__builtin_swi_div()` was added to the bundled agbcc front end and expanded through a Thumb `define_insn` backend pattern; the source function itself contains no instruction-bearing inline asm.
- The builtin intentionally takes no source arguments: the BIOS service consumes the function's incoming `r0`/`r1` ABI values directly and returns its `r0` result. Passing the C parameters through an ordinary builtin call caused old agbcc to create stack home stores, so the fixed-ABI spelling is important.
- The isolated and integrated object both disassemble to exactly `SVC #6; BX LR` (`df06 4770`). The clean Docker ROM gate passed, with both ROMs at SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Reproducibility lesson: the compiler change is tracked in `tools/agbcc-swi.patch`, and `.github/workflows/report.yaml` applies it after cloning agbcc. This keeps the project C source honest without relying on a machine-local compiler binary or source-level asm escape.
- Tool assessment: m2c/asmlift correctly exposed the signed-division semantics, but the decisive step was extending the target compiler; adapters cannot repair a missing ISA lowering by themselves.

## Batch 167 — shifted scene-data accumulator siblings (2026-08-05)
- Candidate selection worked best by mining the report for tiny unmatched standalone objects and then grouping identical instruction families. `func_080B36B0`, `func_080C9BFC`, `func_080DA1A4`, and `func_080E1A80` share the same seven-instruction body with only the destination offset changing.
- The existing real-C pattern from `func_080C9520` transferred directly: cast both bases to `u8 *`, load the scene-data halfword at `0x16`, shift by three, and add it to the `u32` destination field. No register pins or barriers were needed.
- The first linker attempt exposed an important integration rule: replacing a linker-script entry is not enough while the old assembly file remains in `SFILES`; the original source must also move to `asm/converted/` so its object is no longer included in the global object list.
- All four linked C units matched 100%, and the clean Docker ROM gate passed with the baseline SHA-1 unchanged. This batch adds **4 matched functions**, moving the report to **1366 / 5960** and **22.919462%**.

## Batch 168 — large global-offset beatscript stores (2026-08-05)
- `func_0800CAA4` and `func_0800CAB8` were semantically simple halfword stores, but direct C pointer arithmetic caused agbcc to fold the large offset into the `gBeatscriptScene` relocation, producing only `LDR; STRH; BX` instead of the target’s separate global/offset loads and `ADDS`.
- Register-pinned C (`r1` base, `r2` offset) plus an empty compiler barrier restored the exact target instruction order and literal-pool layout. This remains source-level real C; the barrier contains no instruction text.
- The first full-build attempt also reinforced the integration rule that converted assembly must move to `asm/converted/`; leaving it in `SFILES` creates duplicate symbols even when its linker-script entry is replaced.
- Both linked units matched 100%, the strict ROM gate passed, and the report advanced to **1368 / 5960** and **22.953020%**.
## Batch 169 — graphics-buffer ordering (2026-08-05)
- m2c-style semantic translation was straightforward, but the first C spelling hoisted the literal load before the target address add in `func_0805CB5C`, causing a ROM mismatch despite similar isolated code.
- A statement-level empty barrier after `r0 += 0x54` restored the target order. The final four objects matched and the clean ROM gate passed.
- Lesson: for tiny register-sensitive stores, inspect the linked bytes—not just semantic output or a disassembly that omits literal-pool ordering.
## Batch 170 — nested scene-variable pointer siblings (2026-08-05)
- The two helpers share the same `gCurrentSceneVariable -> +0xC` pointer chain but differ in whether the final byte is zeroed or set to one.
- Register-pinned C pointers plus empty barriers reproduced both 20-byte objects exactly. This is a useful companion pattern for small state-field helpers; ordinary typed struct access is more likely to alter register allocation.
## Batch 171 — indexed large-offset store (2026-08-05)
- The earlier large-global-offset store pattern transferred to an indexed word store: pin the base in `r2`, shift the index before introducing the offset literal, then preserve `ADDS R2,R3` followed by `ADDS R0,R2`.
- The real-C object matched exactly and the clean ROM gate passed. This confirms the pattern extends beyond fixed halfword stores when the compiler-visible operand order is controlled.
## Batch 172 — explicit byte serialization (2026-08-05)
- Ordinary C pointer increments and explicit shifts were sufficient to reproduce both the unrolled writer and reader; no register pins or barriers were needed.
- The report confirms both new units at 100%, and the strict ROM gate passed. This is a useful easy family because the compiler's byte-width operations naturally select the target `STRB`/`LDRB` forms.
## Batch 173 — paired accumulator (2026-08-05)
- A direct C spelling of two offset-based `u32` accumulations reproduced the target's load/add/store sequence without pins or barriers.
- This is a low-risk family worth prioritizing when the source and destination offsets are regular and the compiler can retain the same base register.
## Batch 174 — scene-variable flag setter (2026-08-05)
- The established large-offset global pattern transferred directly with ordinary C pointer arithmetic; no register pins or barriers are needed for this setter.
- The object and full ROM matched exactly, confirming this is a productive family for nearby scene-state helpers.
