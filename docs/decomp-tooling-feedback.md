# Decomp tooling feedback log

Use this file to record where the current decomp tools helped, where they missed integration risk, and what manual workaround was needed. The goal is to improve future automation without reducing current verification rigor or tool scope.

### Related docs
- `docs/windows-tooling-notes.md` — Windows/MSYS2/Docker path issues and fixes (added 2025-06-26)
- `.pi/extensions/warioware-decomp-loop.js` — loop prompt includes a "Documentation discipline" section that instructs the AI to record tooling issues as they're encountered

## Round 33 — sprite, scene, music-table, and graphics fan-in (2026-08-05)
- m2c supplied ten compact standalone skeletons. The first screen found **5 exact / 4 near miss / 1 compile error**; register-bound variants recovered the three music-table siblings and the graphics-buffer store, producing a combined **10 exact** manifest.
- `func_0800C7A4`, `func_0800CE6C`, `func_08016688`, `func_08018534`, `func_08019644`, and `func_080C477C` matched directly through typed sprite/scene headers, explicit field offsets, or signed division. The music siblings required a base in `R4` and an offset/address accumulator in `R0`; `func_0804E290` required the same two-register accumulator to prevent folding `gGraphicsBuffer + 0x54` into the literal symbol.
- The rejected first spellings remain in `.nearmiss/` and `tools/attempts.tsv`; the best near misses show only the three-operand add or literal-folding differences. No instruction-bearing or volatile inline asm was added.
- The combined exact-only screen and one transactional full-ROM gate passed. Report progress is **1464 → 1474** matched functions, **1004 → 1014 C TUs**, and ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.

## Round 34 — local-label pure leaves (2026-08-05)
- The existing m2c/probe ledger supplied readable C for `func_080039EC` and `func_08008058`. A fresh one-container isolation pass reproduced their instruction streams, but linked-ELF objdiff inferred each target symbol only through its first internal local label, reporting **33.333332%** and **0.0%** symbol matches despite the candidate epilogues being present in the full object.
- This is distinct from an ordinary compiler near miss: the target assembly has `glabel` followed by local branch labels, so the comparison symbol size is truncated. The evidence is preserved in `.decomp-runs/20260805T-round-34-isolation.json`, `.nearmiss/`, and `tools/attempts.tsv`; no instruction-bearing or volatile inline asm was introduced.
- The rollback-capable `apply-batch --force` path was used as a narrow metadata exception, and the full-context gate—not the misleading inferred symbol score—decided acceptance. It passed `wariowareinc.gba: OK` with unchanged SHA-1 and moved the report **1474 → 1476** matched functions and **1014 → 1016** linked C TUs.
- Durable rule: keep normal standalone admission exact-only; only use the force path when the receipt identifies a known target-symbol-boundary defect, the full object bytes are explained, and the transactional full-ROM gate passes. Do not use it to waive a compiler-generated instruction difference.

## Round 35 — scene/runtime-buffer sibling screen (2026-08-05)
- m2c supplied eleven compact skeletons spanning scene-variable masks, scene-data setters, a runtime byte table, a graphics-buffer indexed transfer, and two scene wrappers. One Docker isolation invocation classified **3 exact / 5 near miss / 3 compile error**.
- The exact candidates were `func_0801002C`, `func_08010308`, and `func_080F3C60`. The two scene wrappers matched directly as ordinary C when their prototypes and absolute table arrays were made explicit; the four-byte runtime buffer setter matched with an `extern u8 D_030068F0[]` array. The linker preflight required the canonical `D_030068F0` assignment, which was added before the transaction.
- The rejected mask candidates demonstrate the existing constant-folding trap (`MOVS #0x3D; RSBS` or `MOVS #2; RSBS` becoming a literal mask), while the graphics indexed-transfer candidate changed load/add register allocation. Compile errors were missing `gCurrentSceneData` declarations in candidates that used the wrong header. All rejected hypotheses remain in `.nearmiss/`, `.decomp-runs/`, and `tools/attempts.tsv`; no non-exact source entered the ROM transaction.
- The exact-only subset passed one transactional full Docker ROM/report gate. Report progress is **1476 → 1479** matched functions and **1016 → 1019 C TUs**; ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.

## Rounds 36–44 — register-allocation follow-up (2026-08-05)
- The Round 35 near misses were revisited through eight small variant screens. Register-bound result masks closed `func_0801BEA8`, `func_0801AF18`, and `func_0801B3E4`; an ordered `variable`/`scene` pointer spelling closed `func_080DA0B0`; the `R0` offset/`R1` base accumulator closed `func_08082934`; and a two-operand `offset += base` form closed `func_080F1574`.
- `func_08016A60` and `func_08016A7C` were deliberately not forced. Their pointer/argument allocations can be reproduced, but after the argument's `MOVS #1; ANDS` sequence agbcc folds the later `mask = -2/-3` materialization into `SUB #3/#4`; all tested ordinary-C forms retained that mismatch. The near-miss receipts and candidate seeds remain durable, and no inline asm was added.
- One consolidated six-entry exact isolation receipt passed one transactional full Docker ROM/report gate. Report progress is **1479 → 1485** matched functions and **1019 → 1025 C TUs**; ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`. Receipts: `.decomp-runs/20260805T-round-36-isolation.json`, `.decomp-runs/20260805T-round-37-isolation.json`, `.decomp-runs/20260805T-round-38-isolation.json`, `.decomp-runs/20260805T-round-39-isolation.json`, `.decomp-runs/20260805T-round-40-isolation.json`, `.decomp-runs/20260805T-round-41-isolation.json`, `.decomp-runs/20260805T-round-42-isolation.json`, `.decomp-runs/20260805T-round-43-isolation.json`, `.decomp-runs/20260805T-round-44-isolation.json`, and `.decomp-runs/20260805T-round-44-apply.json`.

## Round 45 — wrapper/runtime fan-in and exact-only promotion (2026-08-05)
- m2c supplied ten compact candidates spanning five `gCurrentKeys` test wrappers, a scheduler callback, and the `D_030068E8` runtime-table family. The first one-container screen found **7 exact / 3 near miss**; a small register/order follow-up closed `func_080F0E14`, leaving **8 exact / 2 near miss** for the final manifest.
- The two-load runtime helper required more than direct pointer indexing: bind the table address to `R4`, load its first pointee into `R3` before shifting `R0`, then reload the pointee into `R1` after the first store. This reproduced both target loads and the literal-pool placement without instruction-bearing asm.
- The two retained bit setters reproduce the base load, scaled offset, argument mask, byte load, and OR exactly. agbcc still sees the preceding `R2 = 1` value and canonicalizes `mask = -5/-9` as `SUB R2,#6/#10`; the target instead materializes `MOVS R2,#5/#9; RSBS R2,R2,#0`. The near-miss diff and all candidate variants remain in `.nearmiss/`, `.decomp-runs/`, and `tools/attempts.tsv`.
- The exact-only eight-entry `apply-batch` ran one full Docker ROM/report gate and passed with SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. Report progress is **1485 → 1493** matched functions and **1025 → 1033 C TUs**. m2c plus the one-container lifecycle was sufficient for this wrapper-heavy batch; asmlift was not needed, and no non-exact candidate entered the transaction.

## Round 32 — scene-table and audio wrapper fan-in (2026-08-05)
- m2c supplied seven compact standalone skeletons. One Docker isolation invocation classified **6 exact / 1 near miss**; the exact subset was selected by candidate hash and reused for one transactional full-ROM gate.
- The three scene-table siblings matched as ordinary C when the candidates used `scenes.h`, raw `*(u32 *)((u8 *)gCurrentSceneData + 8)`, and `u8 D_083A98xx[]` table addresses. The audio siblings matched with an explicit non-void return for `func_0800C7FC`, a declared `u16` key read in `func_0801E918`, and a typed data-symbol pointer for `func_08024494`.
- The remaining `func_0800CDB0` hypothesis is retained in `.nearmiss/`: using a local absolute base preserved the target `+2` field offset, but agbcc still folded the target's `MOVS #3; RSBS` mask into `MOVS #0xFD`. No inline asm was added.
- Full-context integration caught three linker-map omissions before mutation (`D_083A98B8`, `D_083A98D8`, `D_083FC594`); adding canonical assignments mirrored from `include/undefined_syms.inc` allowed the six-entry transaction to pass. Report progress is **1458 → 1464** matched functions, **998 → 1004 C TUs**, and ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.

## Round 0805 — standalone wrapper fan-in (2026-08-05)
- m2c supplied usable C skeletons for eight short standalone candidates. One isolation Docker invocation found exact spellings for `func_0800D23C` and `func_08019A8C`; asmlift was useful as a diagnostic on the simpler data helpers but declined several project-global wrapper candidates. The six rejected candidates remain in `.nearmiss/`, `.decomp-runs/`, and `tools/attempts.tsv`; no non-exact C or assembly move entered the ROM transaction.
- The first apply attempt caught two lifecycle defects before source acceptance: `git status --porcelain` was parsed after a destructive whitespace trim, and `apply-batch` treated unrelated near misses in a larger receipt as batch failures. The fixes preserve the status column, select receipt results by candidate SHA-256, and add regression coverage; the tooling suite now passes 18 tests.
- The corrected run reused the exact screen receipt and applied only the two exact entries. One full Docker ROM/report gate passed with `wariowareinc.gba: OK`, SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`, and report progress from **1401** to **1403** matched functions. This validates the intended expensive-step budget: one isolation container for the candidate fan-in and one full build for the accepted subset.

## Round 30-b — standalone wrapper sibling fan-in (2026-08-05)
- m2c supplied useful skeletons for 14 small standalone functions. One Docker isolation invocation split them into **10 exact**, **3 near-miss**, and **1 compile-error** results. The exact subset was selected by candidate identity from the immutable receipt and applied without `--force`.
- The exact winners were ordinary C field/callback wrappers and unsigned loop siblings. `u32` loop counters reproduced the target `BLS` branches for `func_08017668` and `func_080A8A3C`; direct byte/halfword pointer offsets reproduced the scene-data and sprite-visible siblings. `func_080043A0` required the raw-object fallback because target symbol metadata prevented normalized linked comparison, but the full ROM gate confirmed it.
- The three near misses remain quarantined: `func_0805627C` adds an extra sign/zero-normalization round trip when the local is declared `s16`; `func_080A002C` loads the music-player global after argument normalization instead of before it; and `func_080F5FF4` models the callee's stack arguments as a five-parameter prototype, changing the stack frame and callee-saved registers. `func_08003228` did not compile because `src/lib_sprite.h` lacks the `sprite_handler_set_global_pause` declaration; the candidate was not applied.
- One transactional full Docker build passed `wariowareinc.gba: OK`, preserving SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. The report advanced from **1407** to **1417** matched functions. This confirms that one-container fan-in materially reduces the expensive iteration loop while retaining exact-only standalone acceptance.

## Round 30-c — near-miss shaping follow-up (2026-08-05)
- A focused seven-spelling screen recovered three previously rejected standalone candidates. `func_08003228` only needed the missing `sprite_handler_set_global_pause` prototype; `func_0805627C` needed a widened `s32` parameter followed by an explicit `(s16)` cast to produce the target `LSLS`/`ASRS` pair without a second normalization; and `func_080A002C` needed `register u16 value asm("r1")` so the music-player global loads before the compiler normalizes the argument.
- The old-style `func_080F56EC()` declaration improved `func_080F5FF4` from a 100% object-layout mismatch to a 10.5% near miss, but the remaining stack/callee-save difference is not accepted. It remains in `.nearmiss/` with the new evidence and no source change.
- The three exact candidates reused one isolation receipt and passed one transactional full Docker build. Report progress is **1417 → 1420** matched functions; ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.

## Round 31 — DMA, heap-copy, and sprite sibling fan-in (2026-08-05)
- The repo-local m2c adapter supplied compact skeletons for two related screens. The wrapper screen produced **8 exact / 4 near miss** results; the DMA/sprite screen produced **11 exact / 1 near miss**. A combined exact-only manifest kept all nineteen accepted entries tied to their candidate hashes and receipts.
- Register-bound C destinations recovered the two heap-copy siblings: a non-void no-return function must keep the allocation in `R0` while copying, otherwise agbcc moves the destination to `R2` to preserve a conceptual return value. A local byte-pointer base, rather than direct global-plus-offset syntax, preserved the target's `MOVS #0x1E; LDRSH` sequence in `func_0800CD94`.
- The DMA family matched directly through `src/code_08000f10.h`, but the first linker preflight found four `D_0300XXXX` symbols and `D_083FD264` missing from `undefined_syms.ld` even though `include/undefined_syms.inc` defined them. Adding those canonical absolute assignments let all eleven DMA/sprite candidates pass without force.
- `func_0801E6F8` remains evidence-only: the generated C semantics were correct, but the target's `MOVS #2; RSBS R0,R0,#0` mask shape was folded to a single `MOVS #0xFE`. No inline asm was added to rescue it. The accepted 19-entry transaction passed with report progress **1439 → 1458** and unchanged ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.

## Round 30-d — wrapper and sprite sibling fan-in (2026-08-05)
- m2c supplied the initial skeletons for nineteen short standalone wrappers, field stores, sprite-visible siblings, and one arithmetic helper. The first one-container screen found **11 exact**, **6 near misses**, and **2 compile errors**; all rejected candidates were recorded in `.nearmiss/`, `.decomp-runs/`, and `tools/attempts.tsv`.
- Small ordinary-C ABI variants closed the remaining gaps. `func_080042F4` keeps six incoming parameters but forwards only the four registers used by the target; `func_080043B8` models unused stack slots so its two loaded values land at the target's `SP+0x14`/`SP+0x18`; and non-void declarations without an explicit return reproduce the target `POP {R1}; BX R1` epilogue. A staged multiply in `func_080F2FFC` preserves the target's left-to-right register reuse, while register-bound locals preserve the global literal-load order in `func_0801E44C` and `func_08007FC0`. These are register-allocation declarations, not instruction-bearing asm.
- The two sprite-ID candidates initially failed only because `src/lib_sprite.h` did not declare `sprite_id_set_visible`; explicit matching prototypes fixed the compile errors. `func_0801E44C` also exposed the existing map asymmetry: `D_0300490E` was present in `include/undefined_syms.inc` but absent from `undefined_syms.ld`. The linker preflight caught this before the full build, and the canonical `0x0300490E` definition was added.
- The final exact-only 19-entry manifest passed one reused-receipt transaction and one clean full Docker ROM/report gate. Report progress is **1420 → 1439** matched functions, **979 C / 5708 asm-only** units, and the ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`. `asmlift` was not needed for this wrapper-heavy screen; m2c plus the automated receipt/variant loop was faster, while the asmlift adapter remains available for less direct semantic/control-flow candidates.

## Round 25-a — included-stub relocation false near misses (2026-08-05)
- m2c supplied usable semantic skeletons for four main-menu task wrappers: `func_080122FC`, `func_0801312C`, `func_080148EC`, and `func_08014C9C`. The candidates use ordinary C calls, callback pointers, and host-TU include guards; no instruction-bearing inline asm was added.
- Normalized linked-ELF isolation scored the four at **99.59–99.72%** instead of exact because external `BL` relocation records remained different in the isolated candidate object. Direct object disassembly showed the instructions and host-TU layout were exact, so the candidates were retained for a full-context research apply rather than recorded as ordinary exact winners.
- The forced transaction did not weaken the acceptance gate: it still ran the clean full Docker build/report step and passed `wariowareinc.gba: OK`, preserving SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. This is evidence that, for included stubs, final host-TU bytes and the full-ROM gate outrank relocation-only isolated scores; standalone candidates should remain exact-only.
- The same run exposed that standalone `D_XXXXXXXX` map validation is inappropriate for included stubs. `d7f9d29` now defers those references to the host TU and adds regression coverage; the lifecycle/tool test suite passed **21 tests**. The four conversions increased matched code but not matched-function count, so the 30% push should prioritize standalone linker entries.

## Round 0805b — linker preflight and rollback (2026-08-05)
- One eight-candidate isolation pass found six exact spellings. Four were safe to apply because they were pure helpers or called still-ASM callees; two exact callers of already-converted C helpers (`func_0800C9A4`, `func_080CAAEC`) were intentionally retained as evidence only.
- The first four-entry transaction failed at the final link, not at objdiff: `D_083A98D0` was present in `include/undefined_syms.inc` but absent from `undefined_syms.ld`. The transaction restored every source/ASM/linker path and the rollback verifier rebuilt the exact baseline SHA-1. Adding the canonical linker symbol and rerunning the immutable screen made the same four-entry transaction pass.
- `tools/decomp_cycle.py` now preflights candidate `D_XXXXXXXX` references against `undefined_syms.ld`, with a regression test, so this class of failure is reported before an expensive full-ROM build. The accepted rerun advanced the report from **1403** to **1407** matched functions and kept SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.

## Lifecycle hardening and included-stub fan-in (2026-08-05)
- The first included-stub probe found a real normalization gap: m2c dropped labels transported inside C-string asm (`_0800BFB6: \\n\\`), so it reported an undefined branch target. `tools/asmlift_warioware.py` now strips that transport suffix before matching labels/directives, with two unit regressions.
- Included-stub isolation also needs the host TU's section-relative symbol map. Without it, a correct standalone candidate produced artificial `BL 0` differences against the whole `bitmap_font.c.o`. `tools/decomp_cycle.py` now imports the target TU's text/data symbol offsets for the candidate link while keeping the entry function at the comparison origin.
- `tools/decomp_permute.py` ports Conker's safe fan-in shape: a directory of independent m2c/asmlift/manual spellings is compiled in one Docker isolation invocation, exact winners are listed by content hash, and `accept` delegates to the exact-only transactional ROM gate. The two-variant `func_0800BF7C` exercise found one exact m2c-shaped spelling and one 87.878784%-matching unsigned-coordinate near miss in one invocation; the earlier 99.393936% result was correctly identified as an unresolved-host-symbol artifact and disappeared after the host map fix.
- Receipts now record manifest and candidate/target SHA-256 identities. `.nearmiss/FUNC.json` retains a bounded eight-entry history instead of silently losing prior attempts when a better seed arrives. This keeps the Conker-style provenance trail without allowing diff payloads to grow unbounded.
- Apply transactions ignore only generated evidence paths (`.decomp-runs/`, `.nearmiss/`, `.mizuchi-tmp/`, and `tools/attempts.tsv`) when checking cleanliness; any unrelated source, linker, or tool edit remains a hard stop. This closes the loop between durable recording and the next exact apply without weakening the ROM gate.
- The Pi `apply_conversion` frontend was a bypass: it could mutate before isolated comparison and accept `verify:false`. It now hands a temporary manifest to `decomp_cycle.py apply`, so all frontends share exact-only isolation, candidate hashes, full-ROM verification, and rollback receipts.

## Included-stub full-context gate and provenance follow-up (2026-08-05)
- The first end-to-end `func_0800BF7C` acceptance caught an integration bug that isolated objdiff could not see: included-stub candidates are screened as standalone objects, but the normal Makefile also compiles `src/decomp/*.c` independently while the host TU includes the same file. Applying the raw candidate caused a duplicate definition. The full Docker gate failed, the transaction restored every touched path, and the rollback verifier rebuilt the exact baseline ROM.
- `decomp_cycle.py apply` now adds the repository's `#if __INCLUDE_LEVEL__ > 0` wrapper for included stubs and records `source_transform` in the receipt. The corrected candidate passed the same full-context gate with `wariowareinc.gba: OK` and unchanged SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- `decomp_permute.py screen` now snapshots all raw candidate files into `.decomp-runs/`; acceptance uses the immutable snapshot rather than an ignored scratch path. This makes a committed run receipt independently auditable.
- Acceptance also reuses a fresh, hash- and commit-validated isolation receipt, removing the redundant second isolation Docker invocation. Stale receipts fail closed, so this optimization does not weaken exact-only admission.

## Batch 180 lifecycle correction and pure-leaf screen (2026-08-05)
- The first ten-candidate screen exposed a false near-miss source: isolation compiled agbcc output without the Makefile's appended `.text` / `.align 2, 0` / `.note.GNU-stack` tail. For functions ending two bytes off a four-byte boundary, the linker supplied a Thumb NOP (`0xC046`) while the real build supplies zero padding. `tools/decomp_cycle.py` now reproduces the Makefile tail, and a regression test locks that contract in.
- After the correction, eight of ten candidates were exact in one Docker isolation invocation. The accepted set was `func_080F1F9C`, `func_080F28F8`, `func_080F2C50`, `func_08035ACC`, `func_08003014`, `func_0803F224`, `func_0803F26C`, and `func_0806754C`. The C candidates contain no instruction-bearing asm; `func_080F28F8` uses a register-bound C local (`asm("r0")`) for allocation shaping, not an asm instruction block.
- `func_08003014` has a legacy target object whose `glabel`/`.thumb_func` ordering leaves the linked target symbol as `NOTYPE`; objdiff cannot load that normalized ELF and reports a symbol-bounds error. The cycle now falls back to raw-object comparison only in that linked-ELF failure case, records `comparison: raw_object_fallback` and the loader error in the receipt, and still requires the transactional full-ROM gate. The raw comparison was byte-exact and the full ROM gate passed.
- `func_08008058` and `func_080039EC` were intentionally held back: objdiff/report identifies internal local labels as separate target functions, so a single C function would be semantically right but would not be a clean one-function acceptance. This is evidence for a future split-symbol-aware candidate mode, not permission to force the conversion.
- `apply-batch` then performed one full Docker ROM/report gate for the eight exact candidates; the result was `wariowareinc.gba: OK`, SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`, and report progress `1401 / 5960`. The isolation and apply receipts are committed with the batch.

## Automated candidate cycle and Git gates (2026-08-05)
- The old isolated frontend launched `tools/mizuchi/compile-in-docker.sh` once per candidate. `tools/decomp_cycle.py isolate` now accepts a manifest and compiles all candidates plus any temporary target objects in one `devkitpro/devkitarm:latest` container, then runs host-side objdiff for each result. This keeps the slow full build out of C-shape iteration while preserving the project compiler.
- The first two-entry exercise matched `func_080D74F4` exactly and classified the preserved `func_08002038` ordinary-C spelling as a near miss with one Docker invocation. The full diff is stored in `.decomp-runs/20260805T183342Z-isolation.json`; the near miss remains rejected and its prior full-ROM byte score is not compared with the isolated objdiff gap.
- `tools/decomp_cycle.py apply` is the guarded transaction layer: it refuses non-exact isolation in normal mode, moves standalone or included-stub files mechanically, runs a clean Docker ROM/report gate, and restores the exact pre-apply snapshot plus a fresh baseline build when the ROM differs. The forced audit of `func_08002038` produced `.decomp-runs/20260805T183726Z-apply-func_08002038.json`; rollback rebuilt SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3` and `cmp` passed.
- `.githooks/pre-commit` now performs only the fast policy and staged/unstaged ROM-scope checks; `.githooks/pre-push` checks the complete pushed range, rejects dirty ROM-affecting paths, and owns the Docker SHA gate. This removes one duplicate full compile from the normal commit-then-push loop without weakening the remote invariant. Tooling/docs-only pushes still skip Docker, while `tools/check_decomp_policy.py` independently blocks new non-empty inline asm and verification-gate edits. This complements, rather than replaces, `.pi/extensions/warioware-decomp-guard.js`.
- The remaining gap is candidate generation: the cycle evaluates explicitly supplied permutations but does not invent C AST variants. m2c/asmlift can continue producing those candidate files; the manifest is the safe fan-in point for testing them.
- A read-only Luna worker reproduced the same lifecycle from a fresh context: five unit tests passed, the exact/near-miss split was correct, the receipt reported one Docker invocation, and the worker made no repository edits. This is a suitable small validation task for future delegated workers.

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

## Conker tooling audit (2026-08-05)
- Ported the useful part of Conker's workflow: append-only attempt provenance plus keep-best near-miss seeds. The WarioWare recorder captures Git/ROM hashes and structured diff evidence while leaving source and acceptance state untouched.
- Deliberately did not port Conker's N64/IDO-specific harness, Transmuter, dashboard, or orchestrator. WarioWare already has a Docker/devkitARM gate and Mizuchi candidate tooling; those components would add unrelated compiler assumptions.

## First WarioWare near-miss record (2026-08-05)
- `func_08002038` was tested as ordinary C and failed the strict ROM gate with eight differing bytes in its 18-byte function window. The candidate was reverted, while `.nearmiss/func_08002038.json`, `.nearmiss/func_08002038.full.c`, and `tools/attempts.tsv` preserve the candidate, hashes, command, and localized diff.
- The restored baseline passed the clean Docker gate and matched SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.

## Batch 175 — normalized isolation and transactional batches (2026-08-05)
- The first paired graphics-buffer probe exposed a false raw-object near miss: both candidates emitted the exact 20 linked bytes, but the target assembly resolved `gGraphicsBuffer` through `gba.inc` while agbcc left the candidate literal as a relocation/common symbol. Raw objdiff therefore reported **88.888885%** and a missing literal-pool word even though the final linked bytes were identical.
- `tools/decomp_cycle.py isolate` now links the target and candidate objects at address zero inside the same Docker invocation. It extracts only the target object's absolute symbols as `--defsym` values, resolves candidate relocations/common symbols, and compares the resulting ELFs. Receipts identify this as `comparison: linked_elf`; raw-object comparison remains a fallback when normalization cannot be produced.
- `apply-batch` now applies every exact manifest entry under one rollback snapshot and runs one clean Docker ROM/report gate for the whole batch. The accepted `func_080A2524` + `func_080EE608` batch used one isolation container and one full build, passed the ROM SHA-1 gate, and advanced the report to **1381 / 5960**.
- `m2c` was useful for the semantic field-level hypothesis. `asmlift` correctly declined these Splat-disassembled GBA sources because its ARM frontend expects compiler-emitted GNU assembly; this is a tool-selection signal, not a candidate failure.
- The cycle unit suite now covers the linker normalization script and transaction path helpers (**7 tests**). Keep full ROM verification as the acceptance gate even when linked isolation is exact, because caller/callee and linker-order effects remain outside a single-function probe.

## Batch 176 — sibling batch throughput (2026-08-05)
- Candidate mining found four unmatched standalone functions sharing the Batch 175 graphics-buffer clear sequence. Three ended in `func_0800CDB0(1)` and one in `func_0800418C()`; existing C siblings supplied the exact prototypes and source spelling.
- One `isolate` invocation scored all four candidates at **100.0%** with `comparison: linked_elf`, and one `apply-batch` full-ROM transaction accepted all four. This confirms the new loop can safely amortize the Docker startup/build cost across a small linked family.
- The report advanced from **1381** to **1385** matched functions with the baseline ROM SHA-1 unchanged. Keep batches sibling-rich but semantically narrow; the four functions had identical clear ordering and only a known final call difference.

## Batch 177 — exact offset screen with preserved near misses (2026-08-05)
- A six-candidate isolation screen found two exact real-C candidates: `func_0808BD98` (large scene-variable halfword store) and `func_080AAA40` (indexed scene-variable halfword store). The other four remained isolated near misses and were not applied.
- `func_080AAA40` confirms that the manifest candidate can preserve a split `index << 1` plus `0x83 << 2` computation when the base load and additions are written as separate C statements.
- The exact pair passed one `apply-batch` transaction and moved the report from **1385** to **1387** matched functions. This is the intended loop behavior: collect several probes in one container, accept only exact results, and retain the rejected hypotheses for later shaping rather than mixing them into the ROM transaction.

## Batch 178 — near-miss shaping loop (2026-08-05)
- The preserved near-miss receipt localized four mismatches to codegen choices: wrong accumulator destination (`func_080C4A48`), repeated zero/literal handling (`func_080195E4`), register-role allocation (`func_080DF440`), and constant hoisting/reload collapse (`func_080EC308`).
- Register-pinned readable C solved `func_080C4A48` and `func_080EC308` in the next isolation pass. `func_080DF440` still has a near miss and `func_080195E4` still has literal-pool/code-order drift; neither was applied.
- The two exact variants passed one `apply-batch` transaction and advanced the report from **1387** to **1389** with the ROM SHA-1 unchanged. This validates the intended provenance loop: use the recorded localized diff to select the next C permutation, then re-score before any full build.

## Batch 179 — wrapper/reload shaping (2026-08-05)
- The next isolation manifest combined four focused permutations: a non-void callback wrapper, delayed zero initialization, pinned scene-variable reloads, and a call-then-clear wrapper. All four reached **100.0%** in one normalized linked-ELF pass.
- The full transaction accepted all four and advanced the report from **1389** to **1393** without ROM drift. The most useful feedback was the isolated epilogue mismatch on `func_0809C47C`: changing only the declaration from `void` to non-void corrected `POP {R0}` to the target `POP {R1}`.

## Batch 195 — runtime-table ordering screen (2026-08-05)
- The eight-entry screen amortized candidate compilation correctly: one Docker isolation invocation produced two exact results and six recorded near misses. The exact-only `apply-batch` reused the same receipt and paid for one full Docker ROM gate.
- `func_080F0DFC` and `func_080F2358` converted cleanly to real C and advanced the report from **1493** to **1495** with the baseline ROM SHA-1 unchanged. The source files use register-bound C locals only; there is no instruction-bearing or volatile inline asm.
- The six rejected runtime-table siblings exposed a reusable ordering gap that isolated objdiff made obvious: independent C locals allowed the `arg0+0x18` base load to move before the target's `LSLS/LSRS #24` normalization. `func_080F253C` also demonstrated the known constant-folding trap (`MOVS #2; RSBS` became `SUB #3`), and `func_080F2558` exposed a register-role mismatch around the field/literal loads.
- The cycle and provenance tooling helped with admission and evidence but did not synthesize a fix for the ordering gap. The next attempt should test only a small barrier/dependency permutation family, keep every non-exact result in `.nearmiss/`, and continue to reject instruction-bearing asm.

## Batch 196 — ordering repair follow-up (2026-08-05)
- A six-entry follow-up screen tested the smallest documented repair: an empty memory barrier after argument normalization, plus the R0/R3 register-role correction for the word-field sibling. Five candidates became exact in the same isolated Docker pass; `func_080F253C` remained a near miss solely because the compiler still canonicalizes the target `MOVS #2; RSBS` mask into `SUB #3`.
- The five-entry exact-only transaction passed one full Docker ROM gate and advanced the report from **1495** to **1500**. The cycle's receipt reuse avoided a second isolation container, while the final push hook still supplied the remote full-ROM invariant.
- This validates the workflow's role split: m2c/manual shaping supplied the semantic candidates, the isolated cycle localized the codegen gap, and the strict ROM gate admitted only the exact subgroup. The empty barriers are compiler metadata only; no instruction-bearing or volatile inline asm was added.

## Batch 197 — leaf screen and adapter comparison (2026-08-05)
- m2c supplied accurate semantic skeletons for `func_08003FB8`, `func_08006CC8`, `func_08006EE0`, `func_080F1B5C`, and `func_080F1FB4`. asmlift was useful as a comparison, but its generic pointer signatures were wrong for the project-global graphics/D03000528 candidates; it did not replace project-aware manual shaping.
- The isolated diff on `func_080F1FB4` directly exposed a control-flow-layout mismatch: the first candidate emitted `BHI` to the constant-return path, while the target uses `BLS` to a later constant-return block. Reordering the C early return closed that gap exactly.
- `func_080F1B5C` and `func_080F1FB4` passed one combined exact-only transaction and advanced the report **1500 → 1502**. The three global-buffer candidates remain evidence-only because their instruction bodies were accompanied by literal-pool width/symbol-boundary differences; strict admission correctly kept them out.

## Batch 198 — branch/epilogue shaping (2026-08-05)
- m2c/asmlift exposed three useful candidate skeletons. The isolated cycle then separated semantic correctness from codegen shape: `func_08016F60` needed the `!= 0` source branch, while `func_080F2C68` needed a widened return type to preserve the target's already-normalized counter register.
- `func_080F282C` matched after using the absolute `0x03000E78` address value instead of adding a new undefined-symbol map entry; the empty barrier kept input normalization before the global load and emits no instructions.
- The final three-entry exact-only apply passed the full Docker gate and advanced the report **1502 → 1505**. This was another case where m2c was directionally correct, asmlift was useful for comparison, and the isolated diff identified the minimal manual C-shape change.

## Batch 199 — register-copy and constant-folding follow-up (2026-08-05)
- The five-entry screen found exact initial spellings for `func_0801B174` and `func_08062488`; `func_0801C2D4` became exact after widening its byte argument to preserve raw ABI bits; and `func_0801F698` became exact after an empty `"+r"` output constraint prevented R3/R0 coalescing. The accepted four-entry transaction advanced the report **1505 → 1509**.
- Round 50 confirmed the RSBS blocker is not fixed by merely writing the first bit-mask as a direct expression: `func_080F253C`, `func_080F2598`, and `func_080F25B8` all still become `SUB` sequences. This is now a deliberate blocked family, not an unrecorded failed attempt.
- `func_080047D4` remains a useful literal-pool research seed: its instructions are close, but the candidate's absolute address pool and alignment shift the inferred symbol boundary. Strict admission correctly withheld it.

## Batch 200 — scene-state sibling sweep (2026-08-05)
- A five-entry m2c/manual screen for scene-state zero/setter helpers produced **5 exact / 0 near miss** results in one Docker isolation invocation. Existing register/offset patterns transferred directly, including the non-sequential reloads in `func_080D2768` and `func_080D286C`.
- The exact-only transaction passed one full Docker ROM gate and advanced the report **1509 → 1514**. This is a high-throughput family: the candidate generator needed no asmlift-specific repair and no inline instruction shim; the isolated cycle plus strict gate handled admission.

## Batch 201 — beatscript/runtime screen and symbol-boundary feedback (2026-08-05)
- Three isolated passes over the same five candidates were cheaper than full builds and localized the useful fixes: a destination-register reassignment, an empty output constraint that blocks a mask fold, and explicit absolute-symbol/operand-order shaping. The final screen classified **3 exact / 2 near miss**, and one exact-only full-ROM transaction accepted the three winners.
- The two retained misses demonstrate different limits. `func_08004770` has the right semantic byte-pair predicate but ordinary C does not retain its unusual leaf `PUSH {LR}` / `POP {R1}; BX R1` ABI shape. `func_08006148` has matching instruction/pool bytes through the return, but linked-ELF symbol inference counts the candidate's compiler literal pool inside the function while the target's local pool label ends the symbol earlier. The strict standalone path correctly withheld both rather than using `--force`.
- The runtime-table winner exposed a workflow detail: `include/undefined_syms.inc` can already know a `D_0300XXXX` address while `undefined_syms.ld` does not. The apply preflight caught this before mutation; adding the three canonical linker assignments and rerunning with the intentional map edit explicitly acknowledged let the full gate verify the integrated result.
- The accepted C contains no instruction-bearing or volatile inline asm. The only asm syntax is an empty `"+r"` compiler constraint, which contributes no bytes. m2c/manual shaping supplied the final candidates; asmlift was not needed for this small direct-wrapper family, and the isolated cycle plus ROM gate remained the deciding tools.

## Batch 202 — graphics/scene/DMA screen and metadata-only force (2026-08-05)
- m2c and asmlift both supplied useful semantic skeletons for the five candidates, but neither adapter was sufficient as the final source: project headers, absolute symbol declarations, register-bound locals, and manual statement ordering were required. The isolated diff then made the repairs cheap: moving `r1 = 0` before the first store fixed `func_080186AC`, while delaying the DMA table-symbol assignment until after the first store fixed the instruction order in `func_08002620` and `func_0800774C`.
- The final Round 54 evidence split was **3 exact / 2 near miss**. The two DMA near misses had instruction-identical bodies and fully accounted pool bytes; their only gap was compiler-versus-target function-symbol coverage over the literal pool. A first attempt to reuse the v2 receipt with `--force` was correctly rejected because receipt reuse requires exact entries. Re-running a fresh screen and then applying with `--force` admitted the documented metadata-only exception; the clean full Docker gate remained authoritative.
- One `apply-batch` transaction converted all five and passed `wariowareinc.gba: OK`, preserving ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. The sources contain no instruction-bearing or volatile inline asm; the DMA `volatile` uses are ordinary C memory accesses, not asm blocks. Receipts and near-miss candidates remain in `.decomp-runs/`, `.nearmiss/`, and `tools/attempts.tsv`.
- Fresh report: **1522 / 5956**, **25.554064%**, **6.9363704% matched code**, **1062 C / 5625 asm-only** units, and **1244** decomp files (`1043 standalone_tu` + `201 included_stub`).

## Round 55 — table-copy screen and byte-audited metadata force (2026-08-05)
- m2c supplied the table-copy and bounded-byte-copy skeletons; manual register-pinned pointer shaping transferred the proven `GraphicsTable` goto-loop pattern. The first screen compiled six candidates in one Docker invocation. Five had complete `.text` bodies but were reported as symbol-boundary near misses; `func_08007AD4` also had a genuine two-byte `R4`/`R5` prologue-order mismatch.
- A separate byte-audit extracted each candidate and target `.text` section with `arm-none-eabi-objcopy`. `func_08002FC0`, `func_08002FE8`, `func_08003028`, `func_08003040`, and `func_08003058` were equal at 40, 44, 24, 24, and 28 bytes respectively; the SHA-256 pairs are preserved in `.decomp-runs/20260805T-round-55-bytecheck.json`. `func_08007AD4` remained evidence-only because its two differing bytes were real instructions, not metadata.
- `func_08003058` initially failed compilation when its caller prototype narrowed incoming byte arguments. Widening the C form to `u32` and using an old-style declaration for the already-converted initializer preserved the target's untouched incoming R1–R3 registers. This is a useful ABI-preservation pattern for wrappers that forward inherited registers.
- The five-entry `apply-batch --force` transaction passed the clean Docker ROM/report gate with `wariowareinc.gba: OK`, ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`, and report progress **1522 → 1527** matched functions. The accepted sources contain no instruction-bearing or volatile inline asm; the round's `volatile`-free source-level register bindings and old-style call are ordinary C/compiler metadata.
- Fresh report: **1527 / 5951**, **25.659552%**, **6.9522724% matched code**, **1067 C / 5620 asm-only** units, and **1249** decomp files (`1048 standalone_tu` + `201 included_stub`).

## Round 56 — standalone global/scene screen (2026-08-05)
- The ld-backed inventory avoided a false queue: many low-address assembly files are already real C included stubs and do not increase linked coverage. Screening only objects still listed as `build/asm/*.s.o` selected four genuine standalone candidates.
- m2c/asmlift supplied useful semantic hypotheses, but the final exact spellings were project-aware manual C: `func_08024E34` needed a pointer-valued `D_083C8B64` declaration and `func_08030F9C` needed separate scene/table locals with staged `arg1 * 0xE + arg0` arithmetic. The screen classified **2 exact / 2 near miss** in one Docker invocation.
- `func_080020FC` demonstrates a new branch-layout seed: direct null-check C was semantically correct but emitted `BEQ` plus an extra zero-return block rather than the target's `BNE` fall-through. `func_08035FEC` demonstrates a register/zero-order gap: natural pointer-relative stores omitted the target's `MOVS #0` and `IP` preservation. Both were retained as evidence only.
- The apply preflight caught that `D_083C8B64` was present in `include/undefined_syms.inc` but absent from `undefined_syms.ld`; adding the one canonical linker assignment allowed the exact pair to pass the integrated gate. The full Docker ROM/report transaction advanced the report **1527 → 1529** and kept ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- No instruction-bearing or volatile inline asm was introduced. The cycle receipts and near-miss ledger record the four-way screen and exact-only apply; the post-apply Docker `make report` plus explicit `gen_objdiff.py` refresh produced **1069 C / 5618 asm-only** units.

## Round 57 — scene-table setter sibling (2026-08-05)
- The exact `func_08030F9C` getter transferred directly to `func_08030F7C` setter; the same scene/table locals and staged index arithmetic matched. m2c/asmlift recognized the family, while project-aware C spelling remained the deciding artifact.
- One isolated candidate scored exact and one `apply-batch` full Docker ROM gate accepted it. The report advanced **1529 → 1530**, matched code **69128 → 69160**, and ROM SHA-1 stayed `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- No instruction-bearing or volatile inline asm was introduced. Explicit post-apply Docker `make report` and `python3 tools/gen_objdiff.py` refreshed **1070 C / 5617 asm-only**.
