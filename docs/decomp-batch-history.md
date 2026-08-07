# Accepted batch history

This is the migrated history from the Ralph task file plus the most recent session log work.
It is intentionally concise: keep the durable rules in `docs/decomp-pattern-library.md`, and use this file to remember what landed, when, and why it mattered.

## Tooling hardening follow-up — Conker provenance and source quality (2026-08-07)
- Compared the sister Conker workflow and retained the useful parts: hash-identified candidate inputs, durable near-miss receipts, and an uncompromised whole-ROM gate. Conker's legacy source model still permits inline ASM and volatile shaping, so those are not copied into WarioWare's admission policy.
- Tightened WarioWare's source audit and transactional cycle to reject non-mapped `volatile` and to count numeric accesses made through a scalar-pointer alias after its declaration. Named overlays and small explicit layout evidence remain allowed and visible.
- The 31-test source-quality/cycle suite passes (**33** tests across the tools suite). This pass changes no ROM bytes or progress metrics; Round 86's exact `func_080178C4` screen result remains eligible, while `func_08017930` and `func_0801776C` stay in the near-miss ledger.

## Batch 235 — strict title-scene wrapper (2026-08-07)
- Converted `func_080178C4` to standalone ordinary C. The function uses a named offset-zero `SceneVariableRoot` overlay for the loader result and contains no asm, compiler register pin, barrier, non-mapped volatile, or opaque offset blob.
- The hardened Round 86 screen classified **1 exact / 2 near miss**. Only the exact candidate entered the transactional apply; the full Docker gate emitted `wariowareinc.gba: OK`, advancing **1682 → 1683** matched functions, **1222 → 1223** linked C units, and **1406 → 1407** decomp files with unchanged ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- `func_08017930` remains a frame-size near miss (`SP,#0x54` target versus `SP,#8` ordinary-C candidate) and `func_0801776C` remains a table-base/register-order near miss. Neither was padded or force-applied. The isolation, source-audit, apply, near-miss, and attempt-ledger receipts are retained under the Round 86 paths.

## Batch 236 — strict named-overlay leaf/wrapper batch (2026-08-07)
- Converted `func_0807DC6C`, `func_0808EF04`, `func_080D6C30`, `func_080526EC`, `func_08086970`, `func_08025174`, `func_080D70EC`, and `func_0808828C` to standalone ordinary C. The accepted sources use named field overlays with explicit preserved gaps where the project-wide record type is not recovered; they contain no wrapped asm, instruction asm, compiler register pin, empty barrier, non-mapped volatile, or opaque scalar-pointer layout blob.
- Round 87 screened **12** fresh candidates in one isolation pass and classified **8 exact / 4 near miss**. m2c supplied the semantic skeletons; manual C shaping selected the named-field forms, and no asmlift-generated output was admitted. The exact-only transaction passed the clean Docker gate with `wariowareinc.gba: OK`, advancing **1683 → 1691** matched functions, **1223 → 1231** linked C TUs, and **1407 → 1415** decomp files.
- The evidence-only near misses are `func_080D906C` (**39.9375**), `func_08040AAC` (**8.066666**), `func_080B2450` (**7.117645**), and `func_08082BB0` (**0.125**). Their full C seeds and normalized results remain under the corresponding `.nearmiss/func_080*.full.c/json` files plus `.decomp-runs/round-87-isolation-v1.json`; the four attempt rows are in `tools/attempts.tsv`.
- `round-87-source-audit.json` reports zero instruction asm, barriers, register pins, volatile accesses, raw pointer accesses, and numeric pointer-offset lines in the eight accepted files. The ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.

## Batch 237 — strict included-stub bitmap wrapper (2026-08-07)
- Converted `func_0800C4E0` in `src/bitmap_font.c` from its asm include to a guarded ordinary-C source file. The accepted source uses typed `func_08006F84`/`func_0800C430` declarations, an explicit `u16` fifth-argument temporary, and a semantically honest `void *` return; no wrapped asm, instruction asm, register pin, barrier, non-mapped volatile, or opaque layout blob was used.
- Round 88 screened two real-C spellings. The first was a **91.7%** near miss from stack-argument truncation order; the second was exact against the original host object. After apply, the return-type refinement was compared against that exact current host object and passed a fresh clean Docker verification with `wariowareinc.gba: OK` and unchanged ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- This included-stub conversion changes source coverage but not linked report metrics: **1691 / 5934** matched functions, **75984 / 993820** matched code, and **1231 C / 5456 asm-only** linked units remain unchanged. Decompiled files advance **1415 → 1416**, with included stubs **203 → 204**.
- `func_08011864` remains evidence-only after five strict real-C variants reached a best **0.74074** isolated gap. The compiler's `CMP #1; BLO` → `CMP #0; BEQ` optimization is documented and the candidate was not force-applied. Round 88 receipts, `.nearmiss/func_08011864.*`, `.nearmiss/func_0800C4E0.*`, and the attempt ledger preserve the provenance.

## Batch 234 — strict ordinary-C scene/graphics wrappers (2026-08-07)
- Converted `func_08017054`, `func_0801709C`, `func_0801720C`, `func_0801743C`, `func_080179A8`, and `func_080179E4` to standalone ordinary C. The graphics initializer and scene-variable store use named local overlays; no opaque offset-heavy pointer blob, instruction asm, barrier, or register pin was admitted.
- Round 85's final screen classified **6 exact / 9 near miss**. m2c supplied the useful semantic skeletons. asmlift was used diagnostically, but its stack/prototype/project-compile failures produced no admitted source. The DMA flag siblings remain near-miss evidence because agbcc chooses a signed branch and different register/literal layout; `func_080174A4` remains the MOVS+RSBS mask-folding near miss.
- The exact-only apply passed `wariowareinc.gba: OK`, advancing **1676 → 1682** matched functions, **1216 → 1222** linked C TUs, and **1400 → 1406** decomp files. Fresh report is **1682 / 5934** (**28.345129%**), **75634 / 993802** matched code (**7.6105704%**), with **1203 standalone_tu / 203 included_stub**. ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Evidence: `.decomp-runs/round-85-manifest.json`, `round-85-isolation-v1.json` through `round-85-isolation-v14.json`, `round-85-exact-manifest.json`, `round-85-source-audit.json`, and `round-85-apply.json`; rejected seeds are retained in `.nearmiss/` and `tools/attempts.tsv`.

## Batch 233 — named scene/graphics overlays (2026-08-07)
- Converted `func_080165D4` and `func_08016BF0` to standalone ordinary C. The first uses a named packed `SceneState` overlay for the title-scene dispatcher; the second uses a named `GraphicsMenuRegisters` overlay for the graphics register setup.
- Round 84's broad screen produced **2 exact / 3 near miss** candidates. The exact-only manifest was re-screened after the new opaque-layout policy and reached **2 exact / 0 rejected**. The first exact `func_080165D4` raw-byte-pointer spelling was reshaped to the named overlay and stayed exact; asmlift's generic BF0 pointer skeleton was rejected as a candidate.
- The exact-only apply passed `wariowareinc.gba: OK`, advancing **1674 → 1676** matched functions, **1214 → 1216** linked C TUs, and **1398 → 1400** decomp files. Fresh report is **1676 / 5934** (**28.244019%**), **75264 / 993780** matched code (**7.573507%**), with **1197 standalone_tu / 203 included_stub**. ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- The accepted sources have zero instruction asm, barriers, register pins, raw pointer accesses, and numeric pointer offsets under `round-84-source-audit.json`. Evidence includes `.decomp-runs/round-84-manifest.json`, `round-84-isolation-v1.json` through `round-84-isolation-v5.json`, and `round-84-apply.json`; the three rejected candidates remain in `.nearmiss/` and `tools/attempts.tsv`.

## Batch 232 — scene/main-menu helpers (2026-08-07)
- Converted `func_080167D4`, `func_08016CBC`, `func_08016808`, and `func_08016C60` to standalone ordinary C. The four helpers cover the sound-stop/scene-thread flag leaf, beatscript scene bootstrap, main-menu state dispatch, and graphics-buffer scene update.
- m2c supplied the semantic skeletons. asmlift was used diagnostically but stopped at the project-header boundary; no generated lift was admitted. Round 83 v1 recorded two header-context compile errors, v2 repaired those to three exact plus one stack-size near miss, and v3 reached **4 exact / 0 rejected** by modeling the bootstrap's four-pointer local array in ordinary C.
- The strict source audit reports zero instruction asm, empty barriers, and compiler register pins in all four accepted sources. Only `func_08016808`'s current-scene `+0x3A` read and `func_08016C60`'s five packed scene-field reads are reported as raw-layout evidence; neither uses inline asm or a register pin.
- The exact-only apply passed `wariowareinc.gba: OK`, advancing **1670 → 1674** matched functions, **1210 → 1214** linked C TUs, and **1394 → 1398** decomp files. Fresh report is **1674 / 5934** (**28.210312%**), **75056 / 993772** matched code (**7.552638%**), and **1195 standalone_tu / 203 included_stub**. ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Evidence is in `.decomp-runs/round-83-manifest.json`, `round-83-isolation-v1.json` through `round-83-isolation-v3.json`, `round-83-source-audit.json`, and `round-83-apply.json`; the v2 stack-size near miss remains in `.nearmiss/func_08016CBC.*` and `tools/attempts.tsv`.

## Batch 229 — main-menu update/input helpers (2026-08-07)
- Converted `func_08016D00`, `func_08016DB8`, `func_08016798`, and `func_08016850` to standalone ordinary C. The four-function group covers the main-menu update fan-in, soft-reset cleanup, input-triggered sound/state transition, and the paired scene/graphics readiness predicate.
- m2c supplied the semantic skeletons. asmlift was run as a comparison path but reached its project-header compile boundary for this Splat-shaped neighborhood; no asmlift output was admitted. The first screen recorded three header-related compile errors and then three exact candidates; the repaired screen reached **4 exact / 0 rejected** after an ordinary-C `BNE` fall-through rewrite for `func_08016D00`.
- The strict source audit reports zero instruction asm, empty barriers, and compiler register pins in every accepted source. Raw scene memory accesses are explicit audit evidence (`+8`, `+0x3A`, and the current-scene byte), not inline assembly or register-pinned code.
- The standalone linker preflight caught that canonical `D_083FBB44` was present in `include/undefined_syms.inc` but absent from `undefined_syms.ld`; `fc3419a6` added that one symbol assignment and passed its own full Docker SHA gate. The exact-only batch transaction then passed the same gate with `wariowareinc.gba: OK`, advancing **1662 → 1666** matched functions, **1202 → 1206** linked C TUs, and **1386 → 1390** decomp files. ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Durable evidence is in `.decomp-runs/round-80-isolation-v1.json` through `round-80-isolation-v5.json`, `round-80-linker-verify.json`, and `round-80-apply.json`; the first `func_08016D00` near miss remains in `.nearmiss/` and `tools/attempts.tsv`.

## Batch 230 — main-menu callback/sound wrappers (2026-08-07)
- Converted `func_08016B4C`, `func_08016B88`, and `func_08016BC4` to standalone ordinary C. The sibling trio plays callback sounds, normalizes the sprite ID from the three-argument callback ABI, sets callback cels 7/0x11/-1, and installs the next callback/data pair for the first two wrappers.
- m2c recovered the three-argument callback shape. asmlift was run as a comparison path but stopped at its project-header compile boundary; no generated lift was admitted. One warning-only compile failure in each callback-data candidate was fixed by matching `sprite_set_callback`'s integer data argument with `(u32)&D_083FF654/67C`, yielding **3 exact / 0 rejected**.
- The strict source audit reports zero instruction asm, empty barriers, and compiler register pins in all three accepted sources. The callback data appears as named symbols, not magic numeric ROM addresses or function-body asm.
- The canonical linker map needed `D_083FF654` and `D_083FF67C`; `75c4148d` added both and passed a separate full Docker SHA gate before the batch transaction. The exact-only apply passed `wariowareinc.gba: OK`, advancing **1666 → 1669** matched functions, **1206 → 1209** linked C TUs, and **1390 → 1393** decomp files. ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Evidence is in `.decomp-runs/round-81-isolation-v1.json` through `round-81-isolation-v3.json`, `round-81-linker-verify.json`, and `round-81-apply.json`.

## Batch 231 — scene initializer (2026-08-07)
- Converted `func_08016F14` to standalone ordinary C. It allocates the scene sprite, stores the ID at current-scene offset 0, installs `D_083AD81C` through `func_08005538`, runs `func_08016EF8`, and clears the scene byte at offset `+4`.
- m2c supplied the direct initialization skeleton. asmlift was used diagnostically but reached its project-header boundary; no generated lift was admitted. A three-entry isolation screen classified **1 exact / 2 near miss**. `func_08016DE0` and `func_08016C24` remain evidence-only because their state-machine branch ladder and global-base register roles do not match ordinary C codegen.
- The strict source audit found zero instruction asm, empty barriers, or compiler register pins in the accepted initializer. `afd59602` added the canonical `D_083AD81C` linker assignment and passed its own full Docker SHA gate before apply. The exact-only batch transaction passed `wariowareinc.gba: OK`, advancing **1669 → 1670** matched functions, **1209 → 1210** linked C TUs, and **1393 → 1394** decomp files. ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Evidence is in `.decomp-runs/round-82-isolation-v1.json`, `round-82-isolation-v2.json`, `round-82-linker-verify.json`, and `round-82-apply.json`; near-miss seeds are in `.nearmiss/` and `tools/attempts.tsv`.

## Workflow engineering pass — 2026-08-05

Paused the 30% expansion target to re-engineer the candidate lifecycle around
the Conker evidence model. Added `tools/decomp_cycle.py` for one-container
isolated batches, structured `.decomp-runs/` receipts, score-kind-aware
near-miss recording, transactional full-ROM application, and automatic clean
baseline restoration after a failed candidate. Added repository pre-commit and
pre-push hooks that run the same Docker SHA gate only for ROM-affecting changes,
plus a standalone policy scan for new instruction-bearing inline asm. The
two-entry exercise (`func_080D74F4` exact + `func_08002038` near miss) used one
Docker isolation invocation; forced full application of the near miss failed
the ROM gate and restored a byte-identical baseline. No decomp function was
accepted in that initial tooling-only pass. The follow-up graphics-buffer batch
validated the improved lifecycle end to end.

The follow-up hardening pass added host-TU symbol normalization for included
stubs, embedded-C-string label parsing for the m2c/asmlift adapter, SHA-256
input identities in receipts, bounded near-miss history, and
`tools/decomp_permute.py` for one-container variant fan-in. A two-variant
`func_0800BF7C` screen selected one exact spelling and preserved the other as a
near-miss; no source was accepted until the transactional full-ROM step.
The first full-context apply then caught the missing `__INCLUDE_LEVEL__` guard
for an included stub, rolled back cleanly, and rebuilt the exact baseline. The
apply path now adds that wrapper, snapshots candidate inputs under
`.decomp-runs/`, and reuses a fresh hash-validated screen receipt instead of
paying for a second isolation container.

The next standalone wrapper screen also exposed two receipt/guard edge cases:
the porcelain status parser had stripped the leading worktree column, and a
batch apply reused the full screen receipt instead of selecting only its
requested exact candidates. Both are covered by unit tests and fixed before
the accepted batch below. The durable near-miss C snapshots and lifecycle
receipts are committed so later runs can revisit the rejected spellings.

## Latest accepted batches
| Iteration / Batch | Commit | Δ matched | Summary |
|---|---|---:|---|
| 223 | current commit | +9 report matched / +9 standalone_tu decomp files / +9 linked C TUs | Converted nine compact wrapper/scene-state functions to ordinary C. Round 74 repaired an initial **5 exact / 2 compile error / 2 near miss** screen to **9 exact**, and the full Docker gate passed at **1621 / 5934**, **7.267425%** matched code, `wariowareinc.gba: OK`, and SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. |
| 222 | current commit | +2 report matched / +2 standalone_tu decomp files / +2 linked C TUs | Converted `func_08016B14` and `func_080241E8` to ordinary C scene-data wrappers. Round 73 reached **2 exact / 1 literal-pool near miss**; the exact-only full-context transaction passed at **1612 / 5934**, **7.236317%** matched code, `wariowareinc.gba: OK`, and SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. |
| 221 | current commit | +3 report matched / +3 standalone_tu decomp files / +3 linked C TUs | Converted `func_08003DE0`, `func_080E1F48`, and `func_08023494` to ordinary C. Round 72 reached **2 exact / 1 linked-objdiff boundary near miss**; the complete `func_08003DE0` `.text` section audit proved 20/20 bytes equal before the narrow metadata-only force transaction. The clean Docker gate passed at **1610 / 5934**, **7.229920%** matched code, `wariowareinc.gba: OK`, and SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. |
| 220 | current commit | +2 report matched / +2 standalone_tu decomp files / +2 linked C TUs | Converted `set_soundplayer_pitch` and `set_soundplayer_volume` to ordinary C. Their complete `.text` sections were independently byte-audited after linked objdiff exposed the known local-label boundary artifact; the strict transaction passed at **1607 / 5935**, **7.222674%** matched code, `wariowareinc.gba: OK`, and SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. |
| 219 | current commit | +2 report matched / +2 standalone_tu decomp files / +2 linked C TUs | Converted `func_0800E800` and `func_08004EAC` to ordinary C. Round 71 reached **2 exact / 3 near miss** after repair; the exact-only transaction passed at **1605 / 5937**, **7.2174697%** matched code, `wariowareinc.gba: OK`, and SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. |
| 218 | current commit | +5 report matched / +5 standalone_tu decomp files / +5 linked C TUs | Converted `func_0805C5D8`, `func_0806EC7C`, `func_08088B80`, `func_08089648`, and `func_0809C0C0` to ordinary C. Round 70 reached **5 exact / 0 rejected** after branch-polarity and register-order repairs; the clean Docker ROM gate passed at **1603 / 5937**, **7.2094917%** matched code, `wariowareinc.gba: OK`, and SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. |
| 209 | pending | +4 report matched / +4 standalone_tu decomp files / +4 linked C TUs | Converted `func_08002038`, `func_0800207C`, `func_080020E0`, and `func_08006148` to ordinary C. The soundplayer wrappers and bootstrap record were all linked-byte audited at 20, 20, 28, and 44 bytes; the metadata-only force transaction passed the clean Docker ROM/report gate. Fresh report **1545 / 5937**, matched code **7.029306%**, **1085 C / 5602 asm-only**. |
| 208 | pending | +3 report matched / +3 standalone_tu decomp files / +3 linked C TUs | Converted `func_0800200C`, `func_080041B4`, and `func_08005F64` to ordinary C. The two wrapper sections were linked-byte audited at 24 bytes each; the heap-record allocator was audited at 60 bytes. The narrow metadata-only force path plus a clean Docker ROM/report gate preserved `wariowareinc.gba: OK`, SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. Fresh report **1541 / 5941**, matched code **7.0184655%**, **1081 C / 5606 asm-only**. |
| 207 | pending | +6 report matched / +6 standalone_tu decomp files / +6 linked C TUs | Converted six task-pool scan/mutation siblings with normalized linked-text byte audits and a strict-ROM force transaction. Fresh report **1538 / 5943**, matched code **7.0077977%**, **1078 C / 5609 asm-only**. |
| 206 | pending | +2 report matched / +2 standalone_tu decomp files / +2 linked C TUs | Converted two task-pool state/cancel siblings through the same evidence-backed transaction. Fresh report **1532 / 5949**, matched code **6.973608%**, **1072 C / 5615 asm-only**. |
| 190 | pending | +10 report matched / +10 standalone_tu decomp files / +10 linked C TUs | Converted `func_0800C7A4`, `func_0800CE6C`, `func_08016688`, `func_08018534`, `func_0801911C`, `func_0801913C`, `func_0801915C`, `func_08019644`, `func_0804E290`, and `func_080C477C` to ordinary C. A combined ten-entry m2c screen was exact and the exact-only transaction passed one clean full Docker gate. Added canonical `D_083AE428`, `D_083AE430`, and `D_083AE438` linker definitions. Report **1474 / 5960**, matched code **6.7847543%**, `wariowareinc.gba: OK`, SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. |
| 191 | pending | +2 report matched / +2 standalone_tu decomp files / +2 linked C TUs | Converted `func_080039EC` and `func_08008058` to real C pure-leaf helpers. Their legacy local labels made linked-ELF symbol bounds truncate before the epilogues, so the rollback-capable force path was used only with the documented metadata exception; the clean full Docker gate still required exact ROM bytes and passed. Fresh report: **1476 / 5958**, **1016 C / 5671 asm-only**, `wariowareinc.gba: OK`, SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. Receipts: `.decomp-runs/20260805T-round-34-isolation.json` and `.decomp-runs/20260805T-round-34-apply.json`. |
| 192 | pending | +3 report matched / +3 standalone_tu decomp files / +3 linked C TUs | Converted `func_0801002C`, `func_08010308`, and `func_080F3C60` to ordinary C scene/runtime-buffer helpers. An eleven-entry m2c screen found three exact winners; the exact subset passed one transactional full Docker gate after adding canonical `D_030068F0`. Fresh report: **1479 / 5958**, **1019 C / 5668 asm-only**, `wariowareinc.gba: OK`, SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. Receipts: `.decomp-runs/20260805T-round-35-isolation.json` and `.decomp-runs/20260805T-round-35-apply.json`. |
| 193 | pending | +6 report matched / +6 standalone_tu decomp files / +6 linked C TUs | Converted `func_080DA0B0`, `func_08082934`, `func_0801BEA8`, `func_0801AF18`, `func_0801B3E4`, and `func_080F1574` to ordinary C. Rounds 36–44 screened ordered pointer loads, register-bound masks, and accumulator variants; six exact candidates passed one consolidated transactional full Docker gate after adding canonical `D_030068E8`. The `func_08016A60`/`func_08016A7C` `MOVS`/`RSBS` masks remain evidence-only. Fresh report: **1485 / 5958**, **1025 C / 5662 asm-only**, `wariowareinc.gba: OK`, SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. Receipts: `.decomp-runs/20260805T-round-44-isolation.json` and `.decomp-runs/20260805T-round-44-apply.json`. |
| 194 | current commit | +8 report matched / +8 standalone_tu decomp files / +8 linked C TUs | Converted `func_0801B780`, `func_080203F8`, `func_080227F0`, `func_0801CCC0`, `func_0801CDDC`, `func_08016EF8`, `func_080F0DE0`, and `func_080F0E14` to ordinary C. One ten-entry Round 45 screen narrowed to eight exact candidates; the two runtime bit setters remain recorded near misses because agbcc emits `SUB #6/#10` instead of the target `MOVS #5/#9; RSBS`. The exact-only eight-entry transaction passed one clean full Docker gate. Fresh report: **1493 / 5958**, **1033 C / 5654 asm-only**, `wariowareinc.gba: OK`, SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. Receipts: `.decomp-runs/20260805T-round-45-exact-isolation.json` and `.decomp-runs/20260805T-round-45-apply.json`. |
| 189 | pending | +6 report matched / +6 standalone_tu decomp files / +6 linked C TUs | Converted `func_0800C7FC`, `func_0801004C`, `func_080102A4`, `func_08010328`, `func_0801E918`, and `func_08024494` to ordinary C. A seven-entry m2c isolation screen found six exact candidates and one near miss; the exact-only six-entry transaction passed one clean full Docker gate. Added canonical `D_083A98B8`, `D_083A98D8`, and `D_083FC594` linker definitions. Report **1464 / 5960**, matched code **6.752255%**, `wariowareinc.gba: OK`, SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. |
| 188 | pending | +19 report matched / +19 standalone_tu decomp files / +19 linked C TUs | Converted nineteen DMA, heap-copy, sprite, and scene-state siblings to ordinary C. Two m2c fan-in screens produced eight and eleven exact candidates; the combined exact-only manifest passed one transactional full Docker gate. Added five canonical linker definitions mirrored from `include/undefined_syms.inc`. Report **1458 / 5960**, matched code **6.733334%**, `wariowareinc.gba: OK`, SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. |
| 187 | pending | +19 report matched / +19 standalone_tu decomp files / +19 linked C TUs | Converted nineteen wrapper/sprite sibling functions to ordinary C. A 19-entry m2c screen plus ABI-focused variants found an exact candidate for every selected function; the final exact-only manifest passed one transactional full Docker gate. Added the canonical `D_0300490E` linker definition required by `func_0801E44C`. Report **1439 / 5960**, matched code **6.6739535%**, `wariowareinc.gba: OK`, SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. |
| 186 | pending | +3 report matched / +3 standalone_tu decomp files / +3 linked C TUs | Recovered `func_08003228`, `func_0805627C`, and `func_080A002C` from recorded near misses using an explicit prototype, widened signed input/cast shaping, and an `r1` argument pin. A seven-spelling follow-up screen found all three exact; the exact subset passed one transactional full Docker gate. Report **1420 / 5960**, matched code **6.6157527%**, `wariowareinc.gba: OK`, SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. |
| 185 | pending | +10 report matched / +10 standalone_tu decomp files / +10 linked C TUs | Converted `func_080043A0`, `func_08017668`, `func_0801A7D8`, `func_0801A7F4`, `func_0801A994`, `func_0801B61C`, `func_080223E0`, `func_0808967C`, `func_080A8A3C`, and `func_080ED734` to ordinary C. A 14-candidate m2c screen found ten exact winners, three near misses, and one compile error; the exact subset passed one transactional full Docker gate. Report **1417 / 5960**, matched code **6.609311%**, `wariowareinc.gba: OK`, SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. |
| 184 | pending | +0 report matched / +4 included_stub decomp files / +0 linked C TUs | Converted main-menu task wrappers `func_080122FC`, `func_0801312C`, `func_080148EC`, and `func_08014C9C` to ordinary C. Their normalized isolation scores were 99.59–99.72% because of external-call relocation records, while direct object disassembly and host-TU layout matched; a forced research transaction still passed the strict full-ROM gate. Report **1407 / 5960**, matched code **6.584173%**, `wariowareinc.gba: OK`, SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. |
| 183 | `82a85b07` | +4 report matched / +4 standalone_tu decomp files / +4 linked C TUs | Converted `func_0800EA44`, `func_08038694`, `func_080102C4`, and `func_08072C20` to ordinary C. An eight-candidate isolation screen found six exact spellings; two exact callers of converted C helpers stayed evidence-only and two candidates were near misses. The first full-context attempt rolled back on the missing `D_083A98D0` linker map entry; after adding it, the four-entry transaction passed one full Docker gate. Report **1407 / 5960**, `wariowareinc.gba: OK`, SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. |
| 182 | `ffa7eb5f` | +2 report matched / +2 standalone_tu decomp files / +2 linked C TUs | Converted `func_0800D23C` and `func_08019A8C` to ordinary C wrappers. An eight-candidate isolation screen found two exact spellings and retained six near misses. The exact subset reused the receipt and passed one transactional full Docker gate; report **1403 / 5960**, `wariowareinc.gba: OK`, SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. Receipts: `.decomp-runs/20260805T-round-0805-isolation-v3.json` and `.decomp-runs/20260805T-round-0805-apply-v2.json`. |
| 181 | `8e13321d` | +0 report matched / +1 included_stub decomp file / +0 linked C TUs | Converted `func_0800BF7C` in `bitmap_font.c` to real C. A two-variant permutation screen found one exact m2c spelling and one 87.878784%-matching unsigned-coordinate near miss. The first full-context apply exposed the missing include-level guard, rolled back to the exact ROM, and the corrected transactional apply passed `wariowareinc.gba: OK`; SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`. |
| 180 | pending | +8 report matched / +8 standalone_tu decomp files / +8 linked C TUs | Converted `func_080F1F9C`, `func_080F28F8`, `func_080F2C50`, `func_08035ACC`, `func_08003014`, `func_0803F224`, `func_0803F26C`, and `func_0806754C` to real C. A ten-candidate pure-leaf isolation screen found eight exact candidates after isolation was aligned with the Makefile's zero-filled `.text` tail; one malformed target symbol used the explicitly recorded raw-object fallback. One transactional full Docker gate preserved SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. Fresh report: **1401 / 5960**, **941 C / 5746 asm-only**. Receipt: `.decomp-runs/20260805T2008-pure-leaves-exact.json`. |
| 179 | pending | +4 report matched / +4 standalone_tu decomp files / +4 linked C TUs | Converted `func_0809C47C`, `func_080195E4`, `func_080DF440`, and `func_080DCD54` to real-C wrapper/reload helpers. One normalized linked-ELF isolation pass scored all four at 100%; one transactional full Docker gate preserved SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. Fresh report: **1393 / 5960**, **933 C / 5754 asm-only**. |
| 178 | pending | +2 report matched / +2 standalone_tu decomp files / +2 linked C TUs | Solved two preserved scene-state near misses with readable register-shaped C: `func_080C4A48` and `func_080EC308`. Both normalized isolation comparisons reached 100%; one transactional full Docker gate preserved SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. Fresh report: **1389 / 5960**, **929 C / 5758 asm-only**. |
| 177 | pending | +2 report matched / +2 standalone_tu decomp files / +2 linked C TUs | Converted `func_0808BD98` and `func_080AAA40` to real-C scene-variable halfword stores. A six-candidate isolation screen found these two exact results; one transactional full Docker gate preserved SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. Fresh report: **1387 / 5960**, **927 C / 5760 asm-only**. |
| 176 | pending | +4 report matched / +4 standalone_tu decomp files / +4 linked C TUs | Converted `func_0808EBF8`, `func_0809CE64`, `func_080DF420`, and `func_080E9B60` to real-C graphics-buffer clear/call siblings. One normalized linked-ELF isolation pass scored all four at 100%; one transactional full Docker gate preserved SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. Fresh report: **1385 / 5960**, **925 C / 5762 asm-only**. |
| 175 | pending | +2 report matched / +2 standalone_tu decomp files / +2 linked C TUs | Converted `func_080A2524` and `func_080EE608` to real C adjacent `gGraphicsBuffer` clears. One normalized linked-ELF isolation pass scored both at 100%; one transactional full Docker gate reported `wariowareinc.gba: OK` and preserved SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. Fresh report: **1381 / 5960**, **921 C / 5766 asm-only**. |
| 174 | pending | +1 report matched / +1 standalone_tu decomp file / +1 linked C TU | Converted `func_080D74F4` to a real-C scene-variable flag setter. Clean Docker ROM: `wariowareinc.gba: OK`; SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`. Fresh report: **1379 / 5960**, **919 C / 5768 asm-only**. |
| 173 | pending | +1 report matched / +1 standalone_tu decomp file / +1 linked C TU | Converted `func_080E1A6C` to a real-C paired accumulator. Clean Docker ROM: `wariowareinc.gba: OK`; SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`. Fresh report: **1378 / 5960**, **918 C / 5769 asm-only**. |
| 172 | pending | +2 report matched / +2 standalone_tu decomp files / +2 linked C TUs | Converted `func_08003998` and `func_080039D0` to real C serialization/deserialization helpers. Clean Docker ROM: `wariowareinc.gba: OK`; SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`. Fresh report: **1377 / 5960**, **917 C / 5770 asm-only**. |
| 171 | pending | +1 report matched / +1 standalone_tu decomp file / +1 linked C TU | Converted `func_0800D224` to a real C indexed store into `gBeatscriptScene + 0x1C5C`. Clean Docker ROM: `wariowareinc.gba: OK`; SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`. Fresh report: **1375 / 5960**, **915 C / 5772 asm-only**. |
| 170 | pending | +2 report matched / +2 standalone_tu decomp files / +2 linked C TUs | Converted `func_0801D4A0` and `func_0801D4B4` to real C scene-variable field helpers. Clean Docker ROM: `wariowareinc.gba: OK`; SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`. Fresh report: **1374 / 5960**, **914 C / 5773 asm-only**. |
| 169 | pending | +4 report matched / +4 standalone_tu decomp files / +4 linked C TUs | Converted four `gGraphicsBuffer` helpers to real C with register pins and empty barriers preserving exact Thumb ordering. Clean Docker ROM: `wariowareinc.gba: OK`; SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`. Fresh report: **1372 / 5960**, **912 C / 5775 asm-only**. |
| 168 | current commit | +2 report matched / +2 standalone_tu decomp files / +2 linked C TUs | Converted `func_0800CAA4` and `func_0800CAB8` to real C stores at `gBeatscriptScene + 0x1C32` and `+0x1C30`. Register-pinned C plus an empty barrier preserved the target’s separate global/offset literal loads. Both units report 100%; clean Docker ROM: `wariowareinc.gba: OK`; SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`. Fresh report: **1368 / 5960**, **908 C / 5779 asm-only**. |
| 167 | current commit | +4 report matched / +4 standalone_tu decomp files / +4 linked C TUs | Converted the sibling shift-accumulator wrappers `func_080B36B0`, `func_080C9BFC`, `func_080DA1A4`, and `func_080E1A80` to real C. Each uses the shared `gCurrentSceneData + 0x16` halfword, right-shifted by three, with a different destination field offset. All four linked C units report 100%; clean Docker ROM: `wariowareinc.gba: OK`; SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`. Fresh report: **1366 / 5960**, **906 C / 5781 asm-only**. |
| 166 | current commit | +1 report matched / +1 standalone_tu decomp file / +1 linked C TU | Converted `func_080EE61C` from standalone BIOS assembly to real C using the reproducible `tools/agbcc-swi.patch` target-specific `__builtin_swi_div()` lowering. The generated object is exactly `SVC #6; BX LR` (`df06 4770`); source inline-asm audit is empty. Clean Docker ROM: `wariowareinc.gba: OK`; SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`. Fresh report: **1362 / 5960**, **902 C / 5785 asm-only**. |
| 165 | current commit | +0 report matched / -1 C decomp file / +1 asm-only unit | Honest BIOS SVC exception: moved `func_080EE61C` from the legacy inline-asm C shim back to `asm/asm_080ee61c.s`; the linker still places exact `SVC #6; BX LR` bytes at the target address. Clean Docker ROM: `wariowareinc.gba: OK`; SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`. The report changes to **1361 / 5960** and **901 C / 5786 asm-only** because this function is no longer presented as C output. |
| 164 | current commit | +0 report matched / +1 legacy file reshaped | `func_0800BEC0` now uses an ordinary C `switch` over the loaded byte. A redundant unreachable `case -10`, sharing the default result, preserves agbcc's range-dispatch `CMP #3; BGT` / `CMP #1; BGE` sequence; cases 1–3 return 1, case 4 returns 2, and other byte values return 0. The integrated bitmap-font object and clean Docker ROM both match; `wariowareinc.gba: OK`, with the baseline ROM SHA-1 unchanged. Remaining non-empty inline-asm file: `asm_080ee61c.c`. |
| 163 | current commit | +0 report matched / +1 legacy file reshaped | `func_0800C15C` now uses ordinary non-volatile `s16` stack locals and a typed `func_08006F84` call. agbcc emits the target `SB = SP+0xA` setup, both indexed `LDRSH` loads, epilogue, and padding without the former narrow instruction blocks. The integrated bitmap-font object and clean Docker ROM both match; `wariowareinc.gba: OK`, with the baseline ROM SHA-1 unchanged. Remaining non-empty inline-asm files: `asm_0800bec0.c` and `asm_080ee61c.c`. |
| 162 | current commit | +0 report matched / +1 legacy file reshaped | `func_08015A4C` now keeps its fill pointer as `u32 *` and uses ordinary `*r2++ = r1`, which emits the target `STMIA R2!,{R1}` and original loop branch. The isolated main-menu object sequence and clean Docker ROM both match; `wariowareinc.gba: OK`, with the baseline ROM SHA-1 unchanged. Remaining non-empty inline-asm files: `asm_0800bec0.c`, `asm_0800c15c.c`, and `asm_080ee61c.c`. |
| 161 | current commit | +0 report matched / +1 legacy file reshaped | `func_08014DFC` now uses an empty condition-code barrier plus ordinary `r5 += 4`, which emits the target two-operand `ADDS R5,#4`. The isolated main-menu object sequence and clean Docker ROM both match; `wariowareinc.gba: OK`, with the baseline ROM SHA-1 unchanged. Remaining non-empty inline-asm files: `asm_0800bec0.c`, `asm_0800c15c.c`, `asm_08015a4c.c`, and `asm_080ee61c.c`. |
| 160 | current commit | +0 report matched / +1 legacy file reshaped | `func_080141C8` now uses an empty condition-code barrier plus ordinary `r2 += 2`, which emits the target two-operand `ADDS R2,#2`. The isolated main-menu object sequence and clean Docker ROM both match; `wariowareinc.gba: OK`, with the baseline ROM SHA-1 unchanged. Remaining non-empty inline-asm files: `asm_0800bec0.c`, `asm_0800c15c.c`, `asm_08014dfc.c`, `asm_08015a4c.c`, and `asm_080ee61c.c`. |
| 159 | current commit | +0 report matched / +25 legacy files reshaped | Strict legacy maintenance pass: replaced non-empty inline-asm call/load shims with real C in 25 already C-linked included stubs across bitmap/font, main-menu, and sprite-library families. ABI-shaping function-pointer typedefs and register-pinned C preserved the target bytes. Clean Docker build: `wariowareinc.gba: OK`; ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`. Remaining non-empty inline-asm files: `asm_0800bec0.c`, `asm_0800c15c.c`, `asm_080141c8.c`, `asm_08014dfc.c`, `asm_08015a4c.c`, and `asm_080ee61c.c`. |
| 158 | pending | +1 report matched / total-function recount -1 | Strict literal-pool re-hoist: `func_08007E8C` forwards two arguments to `func_08007E18` with `0x7FFFFFFF` and zero. The complete 20-byte target/candidate `.text` sections are identical, including the zero alignment halfword and literal pool. Clean Docker build: `wariowareinc.gba: OK`; ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`. |
| 157 | pending | +1 report matched / +1 standalone_tu decomp file | Strict local-label-aware re-hoist: `func_08002024` conditionally calls `func_080F2F04` or `func_080F2F34`. The complete 20-byte target/candidate `.text` sections are identical; objdiff's inferred target symbol stops at an internal local label and reports 40% for the symbol, so section bytes plus the strict ROM gate are authoritative. Clean Docker build: `wariowareinc.gba: OK`; ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`. |
| 156 | pending | +2 report symbols / +1 real standalone function | Strict padding-aware re-hoist: `func_08073650` calls `func_08072048` and `func_08073540`, returning the latter's value. A `.text`-section zero padding word reproduces the target `0x0000` halfword; without it the object emitted an alignment NOP. Isolated comparison is **100.0% / 0 diffs** across 5 instructions and the clean Docker build reports `wariowareinc.gba: OK`; ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`. |
| 155 | pending | +1 report matched / +1 standalone_tu decomp file | Strict ABI-gap sibling re-hoist: `func_080F2F78` is the 8-bit sign-extension counterpart to `func_080F2F68`, forwarding arg0[1] and the third ABI argument to `func_080F26D8`. The unused middle parameter preserves the `R2` source. Isolated comparison is **100.0% / 0 diffs** across 7 instructions and the clean Docker build reports `wariowareinc.gba: OK`; ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`. |
| 154 | pending | +1 report matched / +1 standalone_tu decomp file | Strict adapter-assisted re-hoist: `func_080F2F68` loads arg0[1], sign-extends the third ABI argument, and forwards both to `func_080F2704`. m2c exposed the `R2` argument position; an unused middle C parameter reproduces it. Isolated comparison is **100.0% / 0 diffs** across 7 instructions and the clean Docker build reports `wariowareinc.gba: OK`; ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`. |
| 153 | pending | +1 report matched / +1 standalone_tu decomp file | Strict adapter-assisted re-hoist: `func_080F26BC` normalizes a byte, computes a 32-byte record offset, and writes record offsets `0x1D`/`0x1E`. m2c supplied the raw structure layout; register-pinned C plus an explicit three-register pointer expression preserved `ADDS R3,R1,R3`. Isolated comparison is **100.0% / 0 diffs** across 10 instructions and the clean Docker build reports `wariowareinc.gba: OK`; ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`. |
| 152 | pending | +1 report matched / +1 standalone_tu decomp file | Strict leaf re-hoist: `func_0803FED0` reads `0x086F277C + 2`, adds 20, and returns the signed halfword. A local absolute pointer plus an empty barrier preserves the target base literal and `[base, #2]` load. Isolated comparison is **100.0% / 0 diffs** across 7 instructions and the clean Docker build reports `wariowareinc.gba: OK`; ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`. |
| 151 | pending | +1 report matched / +1 standalone_tu decomp file | Strict adapter-assisted re-hoist: `func_08015F80` unlocks stage 9 after stages 2, 3, and 5 are complete and returns `0x200` on success. m2c supplied the condition; asmlift supplied the nested early-return spelling. Isolated comparison is **100.0% / 0 diffs** across 25 instructions and the clean Docker build reports `wariowareinc.gba: OK`; ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`. |
| 150 | pending | +1 report matched / +1 standalone_tu decomp file | Strict adapter-assisted re-hoist: `func_0800DAD8` generated an accepted project-compatible C candidate from asmlift (`*(s16 *)((s16)a1 * 48 + a0[20] + 0)`). Isolated object comparison is **100.0% / 0 diffs** and the clean Docker build reports `wariowareinc.gba: OK`; ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`. |
| 149 | pending | +0 report matched / +1 standalone_tu decomp file | Strict re-hoist after restoring the last byte-identical baseline: `func_08003D28` now compiles to an isolated **100.0% / 0-diff** object and the clean Docker build reports `wariowareinc.gba: OK`. m2c supplied the semantic skeleton; the restored `tools/asmlift_warioware.py` / `tools/asmlift-compile.sh` adapter was exercised, but asmlift declined this candidate's project type context. Final C uses register pins plus an empty compiler barrier for the original `MOVS`/`LSLS`/`RSBS` shape. |
| 148 | current commit | +1 report matched / +1 included_stub decomp file | Real C: `func_0800C128` (bitmap_font font-size lookup: `gCurrentSceneData+0x193 == 1` guard, `func_08006F84` call, `D_03006518.unk51[2]` check, `func_0800C15C` wrapper call with 4 args). Key: `s32 bitmap_font_get_text_width(u32, u32, u32, u32)` return type to match original `POP {R1}; BX R1` epilogue; `extern void func_0800C15C(u32, u32, u32, u32)` to match caller-side 4-arg push. Blocked: `func_080020FC` (agbcc conditional-return MOV-before-B trap — compiler places default return `MOV R0, #0` before the `B done`, original places it after. No pure C workaround found).
| 147 | current commit | +1 report matched / +1 included_stub decomp file | Real C: `func_0800C15C` (bitmap_font generated-coordinate wrapper: preserves four halfword args, calls `func_08006F84`, then forwards signed generated coordinates plus signed original args to `func_0800C110`). Key pattern: narrow asm only for call setup/indexed `LDRSH` loads, with an old-style `extern void *func_0800C110();` to avoid same-TU prototype re-truncation without changing the callee. |
| 146 | `5b9ea820` | +0 report matched / +1 included_stub decomp file | Real C: `func_0800C0BC` (bitmap_font coordinate task wrapper: `func_08006F84` fills two signed stack halfwords, then `func_0800C080` launches the task with generated coords and signed copies of arg1/arg2). Key pattern: adjust same-TU callee prototype from `u16` to `s16` only when the callee remains byte-identical via an explicit `(u16)` cast, allowing the caller to emit `LDRSH` for a signed stack halfword without a conflicting extern. |
| 145 | current commit | +0 report matched / +0 included_stub decomp files | Blocked: copy propagation merges sign-extension + register copy (func_0800DE84 — 49 ROM bytes differ). agbcc folds `LSLS R5, #16; ASRS R5, #16; ADDS R3, R5, #0` into `ASRS R3, R5, #16` regardless of asm volatile barriers. Also TU-wide literal pool layout mismatch continues to block func_080119EC and func_0801216C. New pattern documented: copy propagation merges sign-ext + copy.
| 144 | current commit | +0 report matched / +0 included_stub decomp files | Blocked: all attempted included_stub conversions hit TU-wide literal pool layout mismatch — isolated compile shows 0 instruction differences but full ROM build fails to match. The compiler merges/reorders literal pool entries across the entire TU when compiling C instead of including asm stubs, changing binary layout. Attempted: func_0801216C (32 instrs match, 2 literal pool entries), func_080119EC (130 instrs match, 6+ literal pool entries, also type conflicts with gSpriteHandler/gCurrentSceneSpritePool/D_083AA294 that were fixed), func_0800A2D8 (38 instrs match from prior chunk). Also blocked: func_08011614 (IP/R12 register), func_08011698 (already matched natively in main_menu.c), sprite_set_z (R7 push), func_08011864 (CMP#1/BLO), func_08014740 (IP/R12), func_08011DFC (_call_via_r1), func_08002124 (_call_via_r1 + multiple RSBS), func_080136F4 (R7 push + RSBS). New pattern documented: TU-wide literal pool layout mismatch — functions with multiple literal pool entries in large TUs fail ROM matching despite isolated compile matching perfectly; functions with fewer entries in smaller TUs may still work (func_08013E64 worked with 1 entry). Possible mitigation: target functions in smaller TUs or with minimal literal pool entries. |
| 143 | current commit | +0 report matched / +1 included_stub decomp file | Real C: func_08013E64 (main_menu scene update: gPressedKeys & 0xF1 check, gCurrentSceneData+0xF1 byte check, conditional func_08011584 call based on +0xDD bit 0, conditional func_08013A4C call based on +0xDD bit 7, func_08011698 return check → func_080137B0, then func_080139D4 — callee-risk from func_08011698 was NOT an issue since func_08011698 is still asm stub, so the compiler treats the BL as opaque). Blocked: func_08012D7C (task launcher — function pointer literal `func_08012D3C+1` emitted as `.hword` instead of `.word` in literal pool, causing ROM mismatch; also same-TU callee-risk for get_current_mem_id/start_new_task), func_0800A2D8 (same `.hword` vs `.word` literal pool issue + same-TU callee-risk — isolated compile matches all 38 instructions but full build ROM doesn't match), sprite_set_z (agbcc won't push R7 callee-save — uses {R4-R6,LR} instead of {R4-R7,LR}), func_080136F4 (R7 push + RSBS constant-fold into SUB), func_08011864 (CMP#1/BLO→CMP#0/BEQ fold), func_08014740 (IP/R12). Key patterns: callee-risk warning for asm-stub callees is a false positive — compiler treats BL to asm stub as opaque; function pointer literal `func+1` in literal pool gets `.hword` packing from assembler even though address is 32-bit, causing ROM mismatch — no known workaround in pure C; must clean stale build deps (rm build/src/*.d) after failed apply_conversion. |
| 142 | current commit | +0 report matched / +3 included_stub decomp files | Real C: func_0800DE24 (name_select: conditional func_08016CBC + func_08016D00 check, func_080007C0(2) with fallback path storing gCurrentScene=2, D_03003848=0, D_03003628=D_083A8588 address symbol, D_03003634=0 — key: D_083A8588 must be referenced as `(u32)&D_083A8588` not numeric `0x083A8588` so compiler emits symbol relocation in literal pool), func_08014F38 (main_menu scene init with loop: scene_set_current_thread(0), func_08014E88 via asm volatile BL (same-TU callee-risk), sprite_set_visible loop over gCurrentSceneData+0xCA array entries, RSBS mask-clear 0x41 on gCurrentSceneData+0xDE — R0 held unknown gCurrentSceneData address so MOVS+RSBS not folded), func_08012658 (main_menu scene update: scene_set_current_thread(0), D_03006518 byte into D_083AA0C4 array offset, sprite_set_x_y via asm volatile BL (same-TU s16 callee-signature trap), conditional func_08015C38/func_08012C18/func_08015A88 calls, RSBS mask-clear ~2 on gCurrentSceneData+0xDD). Key patterns: symbol address literal pool entries must use `&symbol` not numeric constants; RSBS after LDR of unknown address doesn't get constant-folded; D_083AA0C4 declared as u8[] per existing decomp files; func_08012C18 is u32(u32) per existing decomp files. Blocked: func_08011864 (CMP#1/BLO — compiler always emits CMP#0;BEQ instead), func_08014740 (IP/R12 register usage — compiler uses R6 instead). |
| 141 | current commit | +0 report matched / +3 included_stub decomp files | Real C: func_0800A0C4 (beatscript: gCurrentSceneData+0x17A halfword check, conditional gCurrentSceneData+0x178 store or schedule_function_call with func_0800C974 + gCurrentSceneData+0x27E halfword — asm volatile BL for schedule_function_call to avoid u16 first-arg re-truncation; asm volatile barrier after LSLS/LSRS to prevent compiler hoisting literal-pool loads before get_current_mem_id BL), func_0801522C (main_menu scene cleanup: func_0800A240 with D_083A4A2C + gCurrentSceneData+0x17C, func_08005600 with gSpriteHandler + offsets 0xCA/0xCE, 3x mem_heap_dealloc(u32) to match existing extern in asm_08014fa8.c — not memory_heap.h void*, OR 0x40 into gCurrentSceneData+0xDE), scene_set_current_thread (attempted, blocked: compiler folds MOVS R0,#0xF;RSBS R0,R0,#0 into SUB R0,#0x16 — no pure C workaround found for this constant-propagation optimization; literal pool .hword vs .word layout also differs). Key patterns: schedule_function_call u16 first-arg causes extra LSLS/LSRS truncation — use asm volatile BL; compiler hoists literal pool loads before BL — use asm volatile barrier after truncation; mem_heap_dealloc(u32) vs mem_heap_dealloc(void*) — must match existing same-TU extern declarations, not header. Blocked: sprite_set_z (agbcc won't push R7 callee-save), scene_set_current_thread (MOVS+RSBS constant fold). |
| 140 | current commit | +0 report matched / +4 included_stub decomp files | Real C: func_08013EC0 (main_menu scene init: scene_set_current_thread(0), RSBS mask-clear on gCurrentSceneData+0xDE, ORs 4, byte-zero at +0xFE, halfword stores at offsets 0x100/0x102/0x104, calls func_0800BF0C(2) — asm volatile barrier after ORRS to prevent compiler value-propagation that collapses MOVS R2,#0x80;LSLS R2,#1 into ADD R2,#0xFC), func_08013388 (main_menu scene update: scene_set_current_thread(0), D_03006518 byte into D_083AA294 table offset, sprite_set_x_y via asm volatile BL, calls func_080135E8/func_08015A88/func_08012E04, RSBS mask-clear on gCurrentSceneData+0xDD), func_08015760 (similar to func_08013388: reads gCurrentSceneData+0x1C4 halfword, D_083AA294 offset, sprite_set_x_y with LDRH+ADD+sign-extend position values, RSBS mask-clear on +0xDD), func_08014810 (sprite_set_base_palette multi-call: 2 unconditional calls with palette 6, then conditional palette 0xC based on gCurrentSceneData+0x14C byte — asm volatile BL for all calls to avoid s16/s8 callee-signature register-reuse trap). Key patterns: asm volatile barrier after ORRS to break register value-propagation; don't clobber R4/R5 in asm volatile BL if they're needed after; D_03006518 must be struct Unk03006518 type per existing decomp files; title.h conflicts with other stubs — use direct extern declarations instead. |
| 139 | current commit | +0 report matched / +3 included_stub decomp files | Real C: func_08014D6C (main_menu scene init: func_0800A240 with D_083A4A2C + gCurrentSceneData+0x16C, func_0800C77C(0), func_08005600 with gSpriteHandler + D_083AB394 + gCurrentSceneSpritePool, OR 0x20 into gCurrentSceneData+0xDE — sibling of func_080149BC with different offset/mask), scene_change_music (beatscript: conditional stop_soundplayer + play_sound + func_0800A430 + update_beatscript_tempo + scene_update_music_pitch + set_soundplayer_volume with gBeatscriptScene+0x1C58 volume — u32 return type for POP {R1};BX R1 epilogue, proper struct SongHeader*/SoundPlayer* types from audio.h). Key patterns: u32 return type for non-void epilogue even when semantically void; audio.h struct types must be used exactly to avoid type conflicts in same-TU decomp files. Blocked: func_080122FC (stack allocation via asm volatile STR doesn't emit SUB SP), func_08012BB8 (agbcc won't push R7 callee-save register), func_0800BF7C (R8 register + 3 stack args too complex). |
| 138 | current commit | +0 report matched / +4 included_stub decomp files | Real C: func_08014440 (main_menu scene wrapper: scene_set_current_thread(0), tests gCurrentSceneData+0x14C byte, conditional D_03006518.unk1=4 or =9 + set_pause_beatscript_scene(0) + func_0800C7A4(0), register-pinned locals for gCurrentSceneData base reuse in R5), func_080117A8 (main_menu sprite setup: calls func_08011774, loads sprite ID from gCurrentSceneSpritePool table, calls sprite_set_anim_cel and sprite_set_x_y via asm volatile BL for s16 callee-signature trap, loads position from D_083A9CE0 table, calls func_0800C77C), func_080149BC (main_menu scene init: calls func_0800A240 with D_083A4A2C and gCurrentSceneData+0x140, calls func_08005600 with gSpriteHandler + gCurrentSceneData+4 + D_083AB35C + gCurrentSceneSpritePool, ORs 0x10 into gCurrentSceneData+0xDE), func_08001C74 (code_08001a70 rotation matrix with __divsi3: computes angle/0x10000 via asm volatile BL __divsi3 to prevent compiler argument reordering, then builds 2x2 rotation matrix from gCosineTable/gSineTable using s32 casts for ASR). Fixed extern type conflicts in asm_080117fc.c and asm_080118e0.c (func_080117A8 u8→s32). |
| 137 | current commit | +0 report matched / +5 included_stub decomp files | Real C: func_0800C298 (bitmap_font task launcher with 6 halfword args, D_083A4AB0, unique Func0800C298TaskArgs typedef to avoid collision with func_0800C110's TaskArgs), func_08014E38 (main_menu sprite palette loop — iterates gCurrentSceneData sprite pool, calls sprite_set_base_palette via asm volatile BL for R2 register-reuse between LDRSH offset and BL argument, register-pinned locals for gCurrentSceneData pointer reuse in R5), func_0800C1C0 (bitmap_font task launcher with 7 halfword args, D_083A4AA0, Func0800C1C0TaskArgs typedef), func_0800C344 (bitmap_font task launcher with 9 halfword/byte args, D_083A4AC0, Func0800C344TaskArgs with u8 at offset 2 + pad3), func_0800C548 (bitmap_font task launcher with 7 halfword args, D_083A4AE0, Func0800C548TaskArgs typedef). Fixed extern type conflict in asm_08014e88.c (void func_08014E38(s32) → void func_08014E38(void)). Blocked: func_08011864 (CMP#1/BLO vs CMP#0/BEQ optimization trap), func_080139D4 (MUL operand reorder: MOV R0,R1; MUL R0,R4 vs MOV R0,R4; MUL R0,R1). |
| 136 | current commit | +0 report matched / +0 decomp files / -6 legacy asm wrappers | Cleanup: converted `func_080113EC`, `func_08011774`, `func_08014E88`, `func_080EE830`, `sprite_anim_get_cel_total`, and `sprite_get_anim_duration` out of naked/whole-function asm wrappers. `func_080EE830` matches as real C with an indirect function-pointer call that agbcc lowers to `_call_via_r1`; the animation helpers use `u32` return declarations plus the original caller-side shift/mask spelling. `func_08011774`/`func_08014E88` still require one narrow `ldrsh` inline-asm instruction for the indexed halfword load, but no longer use naked/whole-function asm wrappers. |
| 135 | current commit | +0 report matched / +0 decomp files / -7 legacy asm wrappers | Real C cleanup: converted `func_08001D5C`, `func_08002468`, `func_0800C080`, `func_080CD564`, `sprite_set_x_y`, `sprite_set_x`, and `sprite_set_y` from naked/whole-function asm wrappers to real C. Key: stack-struct `start_new_task` wrapper shape also fixes `func_0800C080`; `u32` shift temporaries reproduce byte bit extraction; lib_sprite setters match with s32 position args, register-pinned locals, and barrier after handler preservation. |
| 134 | current commit | +0 report matched / +1 decomp file | Real C: func_0800C110 (bitmap_font task launcher wrapper with six halfword stack args, a `start_new_task` return, and a local task-arg struct to preserve stack layout). Key: returning the call result keeps the `POP {R1}; BX R1` epilogue while the stack struct preserves the original stores and 5th/6th stack args. |
| 133 | current commit | +0 report matched / +1 decomp file | Naked asm: func_08001D5C (code_08001a70 matrix writer for four halfword stores into D_03000010; real-C attempts were blocked by R3 scratch-register lifetime and epilogue shape, so this one stays as a naked included_stub for now. Key: the compiler wants to reorder the arg truncations and adds extra save/restore when trying to keep arg3 live across the global base load. |
| 132 | current commit | +0 report matched / +1 decomp file | Naked asm: func_080EE830 (lib_sprite animation command reader — reads halfword command from struct, stores at offset 9, advances pointer by 2, resets counter at offset 8, conditionally jumps pointer by (count-1)*6 for negative mode at offset 0xC, then calls func_080efc88 via _call_via_r1). Key: pure C generates direct BL instead of LDR R1,=func/BL _call_via_r1 indirect call pattern. Naked inline asm with .ltorg required. |
| 131 | current commit | +0 report matched / +1 decomp file | Naked asm: func_0800BEC0 (bitmap_font scene data state reader — reads byte at gCurrentSceneData+0x195, returns 0/1/2 based on switch-like value). Key: pure C blocked by CMP#1/BGE vs CMP#0/BGT optimization trap (compiler always transforms `>= 1` to `> 0`, producing CMP#0/BLE instead of CMP#1/BGE). Naked inline asm required. |
| 130 | current commit | +0 report matched / +1 decomp file | Naked asm: sprite_set_x_y (lib_sprite combined x/y position setter with sprite_is_invalid guard). Key: same pattern as sprite_set_x/sprite_set_y but stores both x at offset +2 and y at offset +4, uses R6 for x value and R7 for y value. |
| 129 | current commit | +0 report matched / +1 decomp file | Naked asm: func_0800C080 (bitmap_font task launcher with 5th stack arg, start_new_task). Key: pure C failed due to R5↔R6 and R0↔R1 register swap + sp offset shift; naked inline asm with .ltorg + .balign 4,0 required. Also added D_083A4A80 to undefined_syms.ld. |
| 128 | current commit | +0 report matched / +2 decomp files / +0.0008% code | Real C: func_08012D3C (scene thread setup with bit-mask clear), func_0800BBCC (scene data init with 5th stack arg inline asm). Key: match existing extern declarations across decomp files. |
| 127 | current commit | +0 report matched / +3 decomp files / +0.0000% code | Real C: func_08001B28 (matrix identity init with R6 callee-save barrier), sprite_delete (sprite dealloc with s32 arg1 trick), func_080EF358 (anim progress with __udivsi3). Key: s32 arg1 prevents early truncation, barriers for R6 push. Blocked: sprite_set_z/x_y by R7 push issue. |
| 126 | current commit | +0 report matched / +6 decomp files / +0.0002% code | Real C: func_080159FC (counter+lookup copy), func_0800C038 (gGraphicsBuffer AND/OR mask), func_08001AC0 (slot allocator), func_08001A70 (array init loop), func_08001BA4 (rotation matrix with ASR), func_08001C08 (2D rotation matrix). Key: s32 casts for ASR, triple asm volatile barrier for MOVS/LSLS/MOV ordering. |
| 125 | current commit | +0 report matched / +5 decomp files / +0.0006% code | Real C: func_08012C18 (stage lookup), func_08011920 (scene thread setup with bit-test), func_0800BFF0 (gGraphicsBuffer position AND/OR mask), func_0800894C (struct entry init with RSBS bit-clear), func_0800898C (linked-list append with 0xFF-sentinel loop). New: asm volatile barrier to prevent LDR+MOV collapse and SUBS#3 optimization. |
| 124 | current commit | +0 report matched / +5 decomp files | Real C: sprite_remove_z_link (linked-list removal), func_08012700 (scene init+callback), func_08013764 (struct init with AND/OR masks), func_080136A4 (scene thread setup), func_08011584 (sprite position set). New: asm volatile barrier for instruction ordering. |
| 123 | current commit | +0 report matched / +8 decomp files | Real C: func_08012058 (scene init+fp), func_08013A4C (flag handler), func_08013A94 (cursor scroll), func_0800BB74 (bitmap init), func_08001DA4 (loop copy), sprite_handler_dealloc_id (id dealloc), func_0800BC10 (sprite show), func_0800BC50 (sprite hide). New: (s32) cast for BGE/BHS, asm volatile BL for type conflicts. |
| 122 | current commit | +1 report matched / +9 decomp files | Real C: func_08012768/func_08012798/func_080127C8/func_080127F8 (stage finder family offsets 4-7), func_08012DCC (sprite visibility loop), func_080118E0 (scene init+sound), func_080166AC (intro check), func_08014FA8 (scene cleanup with _call_via_r0), func_080EF31C (sprite field getter). New: r1 clobber for reordering, unique _padding names. |
| 121 | current commit | +0 report matched / +8 decomp files | Real C: func_080126C8/func_08013428/func_080143F0 (identical scene inits), func_080025BC (DMA copy with sp[] for stack arg), func_08016E6C (language_select with extern symbol), func_08001B70 (task finder), func_08001E20 (task counter), func_08015A4C (STM buffer fill). New: inline STM, sp[] for stack args, extern symbol for literal pool. |
| 120 | current commit | +1 report matched / +6 decomp files | Real C: func_080119B8 (scene init mode 4), func_08016D88 (soft_reset check), func_08014DFC (game data setup), func_08013660 (stage select init), func_080141C8 (scene flag setup with asm volatile clobber), func_08002514 (graphics_table find-empty). New patterns: register clobber, inline add. |
| 119 | current commit | +0 report matched / +2 decomp files | Real C: func_08014374 (language-indexed scene data), func_080135E8 (stage-unlocked string lookup). asm volatile BL pattern for type conflicts. |
| 118 | current commit | +0 report matched / +2 decomp files | Real C: func_0800247C (graphics_table copy-entries), func_080024A4 (copy with count). Fixed forward-decl conflicts in func_080024E4/func_080024FC. |
| 117 | current commit | +1 report matched / +2 decomp files | Real C: sprite_handler_alloc_id (free-list allocator, u32=0xFFFF literal pool trick), func_080EFC50 (sprite count by unk30 match). Both use id*56 offset pattern + padding byte. |
| 116 | current commit | +1 report matched / +3 decomp files | Real C: func_080EFC20 (animation count loop with sentinel -1, padding byte). Naked asm: sprite_set_x, sprite_set_y (lib_sprite x/y setters — naked needed because sprite_is_invalid(void*, s16) callee signature adds extra sign-extension before BL). |
| 115 | current commit | +0 report matched / +3 decomp files | Real C included_stub: `func_08014878` (scene_set_current_thread + func_08014810 + 5x func_0800C77C + RSBS mask-clear 0x11), `func_08015590` (scene_set_current_thread + gCurrentSceneData shift-offset word load + func_080065C0 + AND mask 0x7F + function pointer call via _call_via_r0). Also `func_08011774` (sprite anim loop) converted using naked inline asm due to R2 register reuse between LDRSH offset and BL arg that pure C couldn't replicate. |
| 114 | current commit | +0 report matched / +2 decomp files | Real C included_stub: `func_08011824` (4 sequential func_0800C7A4 calls + sprite_set_anim_cel + func_0800C77C), `func_0801197C` (scene_set_current_thread + D_03006518 write + multi-call + RSBS mask-clear). Key: func_08011824 matches with simple `sprite_set_anim_cel(gSpriteHandler, gCurrentSceneSpritePool[6], 0)` — no register pinning needed. Attempted `func_08014374` but blocked by `func_08015A88` signature conflict (takes R0 implicitly but declared void in other decomp files). |
| 113 | current commit | +0 report matched / +3 decomp files | Real C included_stub: `func_08014C34` (scene wrapper + RSBS mask + function pointer call), `func_08011730` (gGraphicsBuffer conditional write + func_0800A000), `load_gfx_table` (graphics table loader with stack buffer + polling loop). Also fixed `func_080021C8` type mismatch in asm_08002584.c. |
| 112 | current commit | +0 report matched / +3 decomp files | Real C included_stub: `func_080116D4` (RSBS mask + bit-test), `func_080143BC` (scene init wrapper), `func_080133EC` (multi-call + D_03006518 write). All register-pinned real C. |
| 110 | `7a3ce013` | +0 report matched / +12 refactor | Refactored 12 naked inline-asm files to real C with register-pinned variables. 4 remain as naked asm. |
| 109 | current commit | +0 report matched / +2 decomp files | Linked main_menu mini-batch: `func_08014E88` palette helper + `func_080152A0` caller. Both use naked inline asm. Confirms linked batches can move included_stub coverage safely when the callee is converted first and ROM is checked after each apply. |
| 108 | `918f30c7` | +0 report matched / +1 decomp file | `func_08014C6C` main_menu scene wrapper: scene_set_current_thread(0), RSBS mask-clear bits 0,5 (mask=0x21) at gCurrentSceneData+0xDE, calls function pointer at gCurrentSceneData+0x170. Naked inline asm. |
| 104 | pending | +1 | `func_0800A430` beatscript table lookup: searches D_083A4BF0 table for matching entry. Forward-loop with 8-byte struct entries. Naked inline asm with `.syntax unified` |
| 103 | pending | +1 | `func_0800A3FC` beatscript texture load wrapper: R4/R5 arg preservation, get_current_mem_id(), func_0800430C(D_083ADADC, ...), func_0800D23C(). Naked inline asm |
| 102 | pending | +1 | `func_0800A240` beatscript task launcher: R4/R5/R6/R8 save, get_current_mem_id(), tail-call start_new_task with stack arg. Naked inline asm |
| 101 | pending | +1 | `func_080148BC` main_menu wrapper: scene_set_current_thread(0), RSBS-mask-clear bits 0,1,4 at gCurrentSceneData+0xDE (mask=0x11), then call function pointer at gCurrentSceneData+0x144. Naked inline asm. Sibling to func_080144BC |
| 100 | pending | +1 | `func_08013628` main_menu byte lookup: indexes D_083AAD70 via D_03006518.unk0, reads byte at offset ((unk3 * 4 + unk4) * 8). Naked inline asm to match LDR/LDRB/LSLS/ADDS instruction ordering |
| 99 | pending | +1 | `sprite_anim_get_cel_total` lib_sprite helper: counts animation cels by iterating through Animation array until NULL cel. Sibling to sprite_get_anim_duration. Uses naked inline asm with `.syntax unified` + `.short 0x0000` padding for byte-identical match |
| 98 | pending | +1 | `func_080115DC` main_menu dma3_set conditional wrapper: checks gCurrentSceneData[0xDC], calls dma3_set with source/dest from offsets 0xD4/0xD8. Naked inline asm for exact constant generation (MOVS+LSLS for 0x500 and 0x100) |
| 97 | `8f02974e` | +1 | `func_08001DFC` array counter loop: counts non-zero bytes in D_03000118[0..0x1F]. Uses `u32 i` loop counter to get `BLS` (unsigned) branch instead of `BLE` (signed). Pattern: BLS vs BLE depends on counter signedness |
| 96 | `pending` | +1 | `func_080113EC` main_menu conditional bit-test wrapper: tests bits 1,3 in gCurrentSceneData[0xDD] for early return, tests bit 2 to call func_080122FC and clear bits 0+2, tests bit 4 to call func_08013188 and clear bits 0+4. Naked inline asm for exact instruction-level match |
| 95 | `pending` | +1 | `func_08014490` main_menu wrapper: scene_set_current_thread(0), write 1 to gCurrentSceneData->field_0x38, set_pause_beatscript_scene(0), clear byte at offset 8, call func_0800C7A4(0). Used naked inline asm for exact byte-identical match |
| 94 | `pending` | +1 | `sprite_get_anim_duration` lib_sprite helper - loops through Animation array summing durations until NULL cel. Inline asm with `__attribute__((naked))` and `.short 0x0000` padding for byte-identical match. Pattern: some loop-based patterns resist pure C due to register allocation - naked inline asm is viable alternative |
| 93 | `pending` | +1 | `func_080123F4` main_menu data processing: extracts bitfield from gCurrentSceneData[0x88], shifts (LSLS #0x17 then LSRS #0x19), caps at 0x20, calls func_08006CE8. Key pattern: `(u32)val << 0x17` forces unsigned shift (LSRS not ASRS) |
| 91 | `pending` | +1 | `func_08014DC4` main_menu conditional key-check wrapper: checks `gPressedKeys & 3` (DPAD_RIGHT/LEFT mask), calls `func_08014D6C()`, then plays sound `D_083FBBBC`. Uses proper header chain through `src/audio.h` and `src/scenes/gameplay.h` for symbol declarations |
| 90 | `pending` | +1 | `func_0800A3D0` texture loader callback setup: starts texture loader task then schedules callback via `run_func_after_task(task, func_0800A3BC + 1, 0)`. Uses proper TaskFinalFunc cast for callback. Sets bit in gCurrentSceneData[7] after setup |
| 89 | `pending` | +1 | `func_0800A000` soundplayer volume setter: stores arg0 at gBeatscriptScene offset 0x1C58, then calls `set_soundplayer_volume(gBeatscriptScene.musicPlayer, arg0)`. Uses load-base-first pattern to match LDR R2/R3 literal-pool sequence |
| 88 | `pending` | +1 | `start_load_gfx_table_task` graphics table task launcher: stack-allocated array pattern for passing args to `start_new_task`. Array-based layout matches original `sub sp, #0xc` + sequential stores |
| 87 | `pending` | +1 | `func_080118A0` switch(arg0) dispatcher with 3 cases (0/1/2). Simple switch statement matches perfectly, unlike if-else chain |
| 86 | `pending` | +1 | `func_080EF998` sprite field increment with overflow guard (0x100 cap). Trailing padding uses `__attribute__((section(".text")))` to avoid NOPs |
| 85 | `pending` | +1 | `func_080109EC` main_menu scene setup wrapper: scene_set_current_thread(0), texture loader, run_func_after_task callback. Pattern: common scene init sequence with interwork-safe epilogue |
| 81 | `ac30fda5` | +1 | `func_080024E4` graphics_table loop wrapper - forward-loop through GraphicsTable entries (while(ptr->src != NULL) ptr += 0xC), then tail-call func_0800247C. Key: register `asm("r2")` + goto loop/start pattern preserves the `ADDS R2,#0xC` in original instruction order |
| 80 | `pending` | +1 | 1 standalone_tu: field copy func_080CD564 (ADDS R3,R0,#0; LDR/STR pairs at 0x28/0x2C, inline asm) |
| 79 | `pending` | +1 | 1 included_stub: GBA virtual→physical address dereference (0800210C) |
| 78 | `pending` | +5 | 5 included_stubs: BICS pattern (08002584), conditional struct-store (08001B04), conditional indexed-return (08001DE0), literal-pool AND+OR gGraphicsBuffer (0800BEF4), literal-pool AND+OR D_03004004 (0800BF60) |
| 77 | `pending` | +11 | 11 included_stubs: 3 RSBS mask-clear+call (080109CC/080144BC/08014A0C), conditional-call D_03006518 (08012C64), bit-test+call (0800BC90), switch-2 (080118C4), conditional-return (0801274C), for-loop+C7A4 (08014354), D_03004004 hw-write (0800BF44), 5+2-call (080113BC), do-while+BLE (080117FC) |
| 83 | `fddec90f` | +1 | `func_0800BCAC` bitmap_font bit-field setter - sets bit 2 in gCurrentSceneData[7] using RSBS mask pattern. Key: "__asm__("" : "+r"(mask))" prevents constant folding for "MOVS R1,#5; RSBS R1,R1,#0" instruction sequence |
| 84 | BLOCKED | - | `func_08002514` graphics_table loop wrapper - ROM mismatch when applied. Root cause: calls already-converted func_080024D0; declaration conflicts and potential register allocation mismatch between C callee (converted) and asm caller cause ROM divergence. Lesson: avoid decompiling functions that call already-converted C functions unless both use consistent register allocation. |
| 83 | `fddec90f` | +1 | `func_0800BCAC` bitmap_font bit-field setter - sets bit 2 in gCurrentSceneData[7] using RSBS mask pattern. Key: "__asm__("" : "+r"(mask))" prevents constant folding for "MOVS R1,#5; RSBS R1,R1,#0" instruction sequence |
| 82 | `16cf338b` | +1 | `func_080024FC` graphics_table loop wrapper sibling - same pattern as func_080024E4 (register `asm("r3")` + goto loop/start, forward-loop through GraphicsTable entries, tail-call func_080024A4) |
| 76 | `pending` | +16 | 16 included_stubs: 3 gGraphicsBuffer bit-ops (0800BF20/BFC8/BFDC), 3 RSBS mask-clears (0800A3BC/080121B8/08013114), 2 shift-OR-set (0800A200/0800A3A4), beatscript indexed bit-set (0800A280), D_03006518 zero-clear (080109B4), scene_set+store (08014428), 4-const-arg (08013E44), alloc+init (08002568), byte+2-call (080143A0), scene_paused (080114E4), literal-offset load (0800A050) |
| 75 | `pending` | +17 | 17 included_stubs: 6 conditional-call wrappers (08012CB4/0801364C/08014B44/08014DE8/080153E0/08015930), 4 gCurrentSceneData shift-offset loads (0800A024/0800A138/0800A14C/0800A390), scene_stop+const-arg+2-call+lookup (08011764/08013AE0/0800A228/0800BBB4), bit-OR+hw-pair+struct-init (0800BF0C/0800BF34/080024D0) |
| 74 | `pending` | +8 | 8 included_stubs: struct zero-inits (08002470/08002600/08002614), gBeatscriptScene getters (0800A038/0800A044), const-arg+2-call wrappers (08013B88/0800A128/0800A218) |
| 73 | `pending` | +5 | `func_080025F8`/`0800260C` 3-word struct stores + 3 BX LR leaves (08013184/08013624/08014FF4) |
| 72 | `pending` | +1 | `func_08012274` BX LR leaf (included_stub, noreturn trap, subdir include path) |
| 71 | `pending` | +1 | `func_08002468` bit-extract (LSLS+LSRS, inline asm, first included_stub) |
| 70 | `pending` | +2 | `func_0803DDA4` wrapper call with -1 + .short padding |
| 69 | `pending` | +1 | `func_0801667C` IWRAM byte load (struct+offset trick) |
| 68 | `pending` | +1 | `func_08016670` IWRAM byte store (struct+offset trick) |
| 67 | `pending` | +1 | `func_08004A74` zero-arg wrapper (last of family) |
| 66 | `pending` | +1 | `func_08004A30` zero-arg wrapper (same family) |
| 65 | `pending` | +1 | `func_080049BC` zero-arg wrapper (same family as batch 64) |
| 64 | `pending` | +1 | `func_08004994` zero-arg wrapper (MOVS R2/R3 #0 + BL, non-void epilogue) |
| 63 | `pending` | +1 | `func_08003A00` absolute value helper (CMP+BGE+NEGS) |
| 62 | `pending` | +1 | `func_08002068` conditional sound call wrapper (LSLS+LSRS+BL) |
| 61 | `pending` | +1 | `func_080029D0` byte `&= ~3` + halfword `&= 3` mask pair (RSBS register pin) |
| 60 | `7fab3891` | +1 | `gGraphicsBuffer.unk854_1 = arg0` bitfield wrapper |
| 58 | `f36b7bd6` | +1 | Pointer-deref halfword store (-1 wrapper) |
| 57 | `b3752d25` | +7 | Small wrapper sweep (SVC, div, arithmetic, D_ store, byte-write, HW reg, struct init) |
| 56 | `4c3f3d3d` | +4 | RSBS mask-clear + s16-indexed byte-store siblings |
| 55 | `c5cbc506` | +0 (exploration) | loop-based sprite_id_delete variants failed to match; identified register allocation mismatch blocker |
| 54 | `adc5930c` | +1 | gGraphicsBuffer clears + 2-call wrapper |
| 53 | `80ba7f84` | +1 | conditional 4-delete sprite_id_delete wrapper |
| 52 | `12cf970c` | +4 | `sprite_id_delete` single, dual shift-1, dual direct, sign-ext+delete+CDB0 |
| 51 | `c6a88977` | +4 | `sprite_id_delete` byte-offset siblings (shift-1, direct, and dual delete) |
| 50 | `3dacfb4c` | +4 | `sprite_id_delete` const-arg, sign-ext+delete, 2-delete+gGraphicsBuffer clear |
| 49 | `d98c2b49` | +9 | `sprite_id_delete` byte-offset siblings, plus one `func_0800CDB0(1)` + delete wrapper |
| 48 | `0f121592` | +7 | pair-add, field++/2-BL, gGraphicsBuffer store pair, gCurrentSceneData add, 3-BL return, 2-BL call |
| 47 | `49223873` | +4 | gCSV byte-- siblings, BL+s8 sign-ext+BL, multi-store-with-reload |
| 46 | `10050330` | +7 | 5-arg struct init, 4-call R4 wrapper, gCSV word++, gCurrentSceneData LDRH families |
| 45 | `f92ed4ac` | +8 | gGraphicsBuffer clears, BG_OFS setters, soundplayer_pitch siblings, DE30 wrapper |
| 44 | `e0c0f888` | +7 | sprite_set_enable_updates, shift deref + 2 BL, s8 sign-ext, gCSV store families |
| 42 | `4218ab64` | +8 | `scene_set_current_thread(1)` patterns, conditionals, BL+store combos |
| 41 | `40aa7e61` | +6 | sprite_id_delete shift offsets, two-pointer wrappers, ASRS signed load helper |
| 40 | `702b14cc` | +27 | BX LR stub sweep |
| 39 | `be5cdc3f` | +15 | BX LR stub sweep |
| 38 | `75dd08ae` | +12 | BX LR stub sweep; crossed 1200 |
| 37 | `44d082bc` | +6 | sprite_id_delete siblings, conditional byte check, 2-call R4 wrapper |
| 36 | `c1cacf3f` | +0 matched / +2 accepted units | sprite_id_delete gCSV+0x84, gCSV+8 ptr + void call + byte store |
| 35 | `30f3b829` | +6 combined with 36 | shift-offset deref+call siblings, gCSV offset call siblings |
| 34 | `e40387e4` | +6 | gCSV word+halfword pair calls, 2-pointer wrapper, s8 sign-ext siblings |
| 33 | `448b02bd` | +5 | gCSV shift-offset deref+call siblings, 2-pointer wrapper |
| 32 | `45a3ea2e` | +5 | gCSV deref+call, gCSV offset deref+call, s8 sign-ext call |
| 31 | `9a35ff76` | +4 | shift-extract siblings, gBeatscriptScene deref wrapper |
| 30 | `9c7de76e` | +3 | LDRSH dealloc, sound wrapper, D_ clear+call |
| 29 | `ae0fc72f` | +4 | two-call R4 wrappers, double dealloc wrapper |
| 28 | `84964974` | +3 | partial batch: increment-and-call, const-arg pair call, offset-deref tail-call |
| 27 | `b00387e7` | +5 | D_ word clear, sct+gCSV byte clear, tail-call wrapper, one-call+store |
| 26 | `ceba332e` | +4 | gCSV shift-offset word clear, offset tail-call, one-BL wrappers |
| 25 | `9796ba11` | +6 | gCSV pointer-deref byte store, gCSV word add/increment, D_ setters |
| 24 | `fc4f684b` | +5 | D_ stores, struct init, load-word-pair, crossed 6% matched code |
| 23 | `234220c1` | +5 | BX LR stub, D_ byte setter, gCurrentSceneVariable decrement, pair-add |

## Iteration 57 details
- Result: match ✅ all 7 accepted
- Report: **1331 / 5956**, **6.4053407%**
- Commit: `b3752d25`
- Accepted functions:
  - `asm_080ee61c` — `SVC #6` BIOS wrapper via inline asm with register constraints
  - `asm_08089614` — `__divsi3(a2 << 8, a1)` wrapper
  - `asm_080baef4` — arithmetic: `a0[2] = (a0[1] * a1 >> 8) + a2`
  - `asm_08005914` — D_03000698[1] store (required adding symbol to `undefined_syms.ld`)
  - `asm_08003988` — double byte write with pointer increment (`p++` pattern)
  - `asm_080069F4` — hardware register clear (DISPCNT, BG0HOFS) via volatile pointers
  - `asm_080e5a18` — 6-field struct init (stores 4 args then zeros 3 fields)
- Durable takeaways:
  - GBA BIOS SVC calls work with: `s32 r = a0; asm("svc #6" : "+r"(r) : "r"(a1)); return r;`
  - D_ symbols in C files also need `undefined_syms.ld` entries (not just `include/undefined_syms.inc`)
  - Pointer increment `p++` pattern matches where array `p[1]` fails for byte-write sequencing
  - Hardware registers match cleanly with `*(volatile u16 *)0x4XXXXXX = val;`
  - Struct init with order-sensitive zeros matches when store order matches the asm

## Iteration 56 details
- Result: match ✅ all 4 accepted
- Report: **1324 / 5957**, **6.3964925%**
- Commit: `4c3f3d3d`
- Accepted functions:
  - `asm_0800ccb4` — `gBeatscriptScene` byte[2] RSBS-mask-clear (mask=2). Key: use local `u8 *p = (u8*)&gBeatscriptScene` then `p[2]` to avoid combined literal.
  - `asm_0801b194` — `gCurrentSceneVariable` deref byte[0x19] RSBS-mask-clear (mask=3). Same local-pointer pattern but with double-deref.
  - `asm_08035194` — s16-indexed byte-store, value=1. Key: `a0=(u32)(s16)a0; a1+=0x80; a1+=a0; *a1=1` forces LSLS/ASRS before ADDS R1,#0x80.
  - `asm_080351a4` — sibling, value=3. Same pattern.
- Attempted but deferred:
  - `asm_0804e290` — gGraphicsBuffer indexed halfword store: `bgPalette[0][(u16)a0]` gets close but LDR comes before LSLS in compiled vs after in original. Instruction scheduling mismatch, leave as asm.
- Durable takeaways:
  - RSBS-mask-clear via local pointer: `u8 *p = (u8*)&gSymbol; p[N]` avoids combined literal and gives `[R2, #N]` addressing.
  - s16-indexed byte-store: must reassign `a0 = (u32)(s16)a0` explicitly to force sign-ext before pointer constant-add.
  - Instruction ORDER within a function matters for exact byte match; agbcc can reorder independent statements.

## Iteration 55 details
- Result: exploration/blocked ❌ (no matches)
- Report: remains **1320 / 5957**, **6.3896456%** (unchanged)
- Commit: `c5cbc506` (docs/pattern-library update only)
- Attempted functions that failed to match:
  - `asm_0805d394` — loop from 0 to 2, byte-load at gCurrentSceneVariable + (i<<5) + 0x4FB with func_08001B28, then 1 sprite_id_delete
  - `asm_0806843c` — loop from 0 to 3, byte-load at gCurrentSceneVariable + (i<<5) + 0x7B with func_08001B28, then 1 more byte-load + 1 sprite_id_delete
  - `asm_08016d3c` — two func calls (func_08000F74, func_08003E64), then loop from 1 to 2 with sprite_id_delete + func_08001B70 + task_pool_force_cancel_id + mem_heap_dealloc_with_id
- Durable takeaways:
  - Loop-based patterns with BLS/CMP exit conditions fundamentally mismatch agbcc's loop code generation
  - Even semantically identical C loops fail due to register allocation and loop unroll/fold heuristics
  - The CMP + BLS (while <= N) pattern doesn't align with standard for-loop code generation
  - Remaining 6 unconverted sprite_id_delete functions are all loop-based; unlikely to match with simple C patterns
  - Deprioritize sprite_id_delete family entirely; focus on other higher-yield families

## Iteration 54 details
- Result: match ✅ all 1 accepted
- Report: **1320 / 5957**, **6.3896456%**
- Commit: `adc5930c`
- Accepted functions:
  - `asm_0804bc4c` — gGraphicsBuffer DISPCNT AND 0xDFFF, clear 4 halfwords at 0x46/0x44/0x3C/0x40, then sprite_id_delete at gCurrentSceneVariable+0xE4, then func_08001B28 with sign-ext at gCurrentSceneVariable+0xCA
- Durable takeaways:
  - gGraphicsBuffer halfword clears with byte-offset casting match cleanly
  - The pattern of struct clear + deref-load + BL chain remains productive
  - 6 remaining sprite_id_delete asm files after this pass (one failed to match due to loop optimization)

## Iteration 53 details
- Result: match ✅ all 1 accepted
- Report: **1319 / 5957**, **6.3811874%**
- Commit: `80ba7f84`
- Accepted functions:
  - `asm_08067080` — conditional check at `gCurrentSceneVariable + 0xE0`, then four sequential `sprite_id_delete` calls at offsets (0xC4<<4), 0xC4C, 0xC48, 0xC44
- Durable takeaways:
  - Remaining sprite_id_delete functions are increasingly complex (loops, multi-call patterns)
  - Only 7 sprite_id_delete asm files remain unconverted
  - Conditional-skip-then-multi-delete pattern continues to match cleanly

## Iteration 52 details
- Result: match ✅ all 4 accepted
- Report: **1318 / 5957**, **6.3711314%**
- Commit: `12cf970c`
- Accepted functions:
  - `asm_080ba9d4` — `sprite_id_delete(gSpriteHandler, *(u32*)((u8*)gCurrentSceneVariable + (0x90 << 2)))` (PUSH {LR} single-call variant)
  - `asm_0804c388` — dual delete at `(0xB0 << 1)` and `(0xB2 << 1)` (R4/R5 dual-call pattern)
  - `asm_08056788` — dual delete at direct byte offsets `0xF4` and `0xF8`
  - `asm_0803e96c` — `func_08001B28(*(s8*)((u8*)gCSV+0xE4))` then `sprite_id_delete` at `gCSV+0xE0` then `func_0800CDB0(1)`
- Durable takeaways:
  - PUSH {LR} / POP {R0}; BX R0 single-call wrappers continue to match reliably for sprite_id_delete
  - sign-ext byte → func_08001B28 → sprite_id_delete → func_0800CDB0(1) three-call pattern works cleanly
  - Only 8 sprite_id_delete asm files remain unconverted in the queue

## Iteration 51 details
- Result: match ✅ all 4 accepted
- Report: **1314 / 5957**, **6.352630%**
- Commit: (to be committed)
- Accepted functions:
  - `asm_08077174` — `sprite_id_delete(gSpriteHandler, *(u32 *)((u8 *)gCurrentSceneVariable + (0xE6 << 1)))`
  - `asm_080b2bac` — `sprite_id_delete(gSpriteHandler, *(u32 *)((u8 *)gCurrentSceneVariable + (0xB2 << 1)))`
  - `asm_080c9050` — `sprite_id_delete(gSpriteHandler, *(u32 *)((u8 *)gCurrentSceneVariable + 0x574))`
  - `asm_0805ab2c` — two `sprite_id_delete` calls at byte offsets 0x94 and 0x98
- Durable takeaways:
  - Single-call `sprite_id_delete` wrappers with shift-1 offsets are proven safe
  - Direct offsets like 0x574 (no shift) also match cleanly
  - Dual-delete wrappers (R4/R5 preserve, two sequential deletes) follow the same proven pattern as batch 50's `asm_080b0e80`

## Iteration 50 details
- Result: match ✅ all 4 accepted
- Report: **1310 / 5957**, **6.337337%**
- Commit: `3dacfb4c`
- Accepted functions:
  - `asm_08097fcc` — single `sprite_id_delete` at byte offset 0x714
  - `asm_08016fb0` — `sprite_id_delete(gSpriteHandler, 1)` then `func_08001B70(1)` (const-arg pattern)
  - `asm_0805f438` — `func_08001B28` sign-ext byte load from gCSV+0x46, then `sprite_id_delete` at `gCSV+(0xAA<<2)`
  - `asm_080b0e80` — two deletes + `gGraphicsBuffer.unk4C = 0` + `*(u16*)((u8*)&gGraphicsBuffer+0x4E) = 0` + `func_0800CDB0(1)`
- Durable takeaways:
  - `gSpriteHandler` is in `src/lib_sprite.h`; include as `"src/lib_sprite.h"` (not `"lib_sprite.h"`)
  - Never redeclare `sprite_id_delete` with a raw `u32` arg — conflicts with the real `struct SpriteHandler *` signature in lib_sprite.h
  - `gGraphicsBuffer.unk4C` covers offset 0x4C; `*(u16*)((u8*)&gGraphicsBuffer + 0x4E)` covers the pad field immediately after
  - Docker build via `docker run --rm -v $(pwd):/workspace devkitpro/devkitarm:latest /bin/bash -c "cd /workspace && make -j4"` is the correct local verification path; must complete with `wariowareinc.gba: OK` before any commit

## Iteration 49 details
- Result: match ✅ after an immediate binary-search catch on a shaping bug
- Report: **1306 / 5957**, **6.3184834%**
- Accepted functions:
  - `asm_080749c4`
  - `asm_0807f078`
  - `asm_080840b4`
  - `asm_08088564`
  - `asm_080a4424`
  - `asm_080c4754`
  - `asm_080d9b0c`
  - `asm_080e0fa8`
  - `asm_080e4e0c`
- Durable takeaways:
  - `sprite_id_delete` byte-offset siblings remain a strong family
  - for `gCurrentSceneVariable`, missing the `(u8 *)` byte-cast is an easy self-inflicted mismatch because `gCurrentSceneVariable + N` scales by the local-data struct size
  - the `func_0800CDB0(1)` pre-call variant is also safe when the delete load keeps the byte-cast spelling

## Iteration 48 details
- Result: match ✅ after initial mismatch and binary search
- Report: **1297 / 5957**, **6.285469%**
- Accepted functions:
  - `asm_0800cc9c`
  - `asm_0802e4ac`
  - `asm_08035fd8`
  - `asm_08062dcc`
  - `asm_080862dc`
  - `asm_080b7bb4`
  - `asm_080c9520`
- Durable takeaway: the current documented wrapper/arithmetic families are still productive; recent momentum is coming from mixing a few family types in the same batch once each spelling is already low-risk.

## Iteration 47 details
- Result: partial match ✅ (4 accepted out of 8 attempted)
- Report: **1290 / 5957**, **6.2703905%**
- Accepted:
  - `asm_08062260`
  - `asm_08062278`
  - `asm_0801c758`
  - `asm_0808a56c`
- Reverted due to traps:
  - `asm_0801af18`
  - `asm_0801bea8`
  - `asm_0801b3e4`
  - `asm_080d3a60`
- Durable takeaways:
  - `byte &= ~N` is still a trap
  - BL+STRH wrappers can fail on return-register shape
  - local pointer reload shaping works for the accepted multi-store family

## Iterations 45-47 cluster
This cluster established the current active playbook:
- `gCurrentSceneData` halfword add/shift helpers are dependable
- `gGraphicsBuffer` small store families are dependable
- R4-save multi-BL wrappers remain productive
- gCSV byte increment/decrement siblings are cheap wins
- not all bitfield-clear siblings are safe even when semantically trivial

## Earlier arc worth remembering
### Batches 38-40
Large BX LR stub sweeps were still worth doing and created a major step-up in matched functions.

### Batches 41-44
The work shifted from filler into richer families:
- `sprite_id_delete`
- two-pointer wrappers
- signed-load helpers
- `scene_set_current_thread(1)` + store patterns

### Batches 23-37
This period built the reusable base library of patterns:
- D_ setters / clears
- pair-add helpers
- gCSV deref + call wrappers
- shift-offset families
- raw-pointer struct-entry setters
- sound wrappers
- early `gCurrentSceneVariable` store families

## What this history says about the repo
- Reuse beats novelty.
- The best batches come from already-proven families, not isolated one-offs.
- A partial batch is acceptable if the binary search is done immediately and the trap is recorded.
- Doc quality directly affects autonomous throughput: every repeated mistake has historically come from a missing or stale rule.

## Iteration 58 details
- Result: match ✅
- Report: **1332 / 5956**, **6.406549%**
- Commit: `f36b7bd6`
- Accepted functions:
  - `asm_0800c610` — pointer-deref halfword store: `*(short *)((int *)a0[3]) = -1;`
- Durable takeaways:
  - Simple pointer-deref + halfword store pattern matches perfectly when written as `*(short *)((int *)a0[3]) = -1;`
  - The pattern `MOVS R2, #1; RSBS R2, R2, #0` (load -1 via negation) is generated by using `-1` directly as a literal in C
## Iteration 59 details
- Result: blocked exploration ❌ (no accepted progress)
- Candidate: `func_08002468`
- Outcome: the isolated C spelling matched the target asm, but moving it out of the mid-file include in `src/graphics_table.c` changed ROM ordering; the safe ROM-preserving shape is to keep it as an include-shim until the host TU can be split.
- Durable takeaway:
  - mid-file asm includes inside a larger C TU are not always safe standalone conversions, even when `compile_and_view_asm` reports a perfect local match

## Iteration 62 details
- Result: match ✅
- Report: **1335 / 5955**, **6.4136%**
- Commit: pending
- Accepted functions:
  - `asm_08002068` — conditional sound call wrapper
- Durable takeaways:
  - Standalone TU asm objects with size=0 symbols cause objdiff to truncate comparisons at BL boundaries; code is still correct and ROM matches
  - `apply_conversion` accepts these despite partial objdiff match since the ROM build verifies byte-identity
  - Tooling fix: `asmForStandaloneObject` now handles C-asm string format (`asm("...")`) used by included stubs; also added `.thumb_func` and `glabel` to the query_candidates filter
  - Reverted an included_stub conversion attempt — the build system tracks `.s` file deps via C preprocessing, making included_stub conversions fragile without proper dep-file handling

## Iteration 61 details
- Result: match ✅
- Report: **1334 / 5956**, **6.4118%**
- Commit: pending
- Accepted functions:
  - `asm_080029d0` — byte `&= ~3` + halfword `&= 3` mask pair
- Durable takeaways:
  - agbcc folds `&= ~3` into `mov r1, #0xfc` instead of `mov r1, #3; neg r1, r1`; register-pinning forces the RSBS form
  - Register-pinning with `register type asm("rN")` is a viable strategy for exact instruction matching on small functions
  - Tooling fix: added mtime-based db cache invalidation and synthetic fn entries from function names so preflight works for functions not in mizuchi-db
  - mizuchi-db.json was expanded with 4434 entries from filesystem scan and stripped of asmCode to keep file small

## Iteration 60 details
- Result: match ✅
- Report: **1333 / 5956**, **6.4097586%**
- Commit: `7fab3891`
- Accepted functions:
  - `asm_08006E94` — `gGraphicsBuffer.unk854_1 = arg0` bitfield wrapper
- Durable takeaways:
  - 1-bit `gGraphicsBuffer` bitfield writes can match cleanly with direct assignment when the original just masks to bit 0 and stores it back
  - The byte-level shape at offset `0x854` is a good low-risk filler family when nearby `gGraphicsBuffer` patterns are already proven

## Batch 195 — accepted (runtime-table bitfield siblings)
- Result: match ✅
- Report: **1495 / 5958**, **25.092312%**, **6.840671% matched code**
- Accepted functions: `func_080F0DFC` and `func_080F2358`
- One eight-entry isolation receipt produced two exact candidates and six near misses. The exact-only two-entry transaction reused the receipt and passed one clean Docker ROM gate with SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- The six rejected siblings all exposed the same compiler-ordering trap: the base load was hoisted before the required `LSLS/LSRS #24` input normalization. `func_080F253C` additionally folded the target's RSBS mask into `SUB #3`; `func_080F2558` chose the wrong load/literal register roles. No non-empty inline asm was introduced; all candidate and diff evidence remains in `.nearmiss/`, `.decomp-runs/`, and `tools/attempts.tsv`.

## Batch 196 — accepted (runtime-table ordering repair)
- Result: match ✅
- Report: **1500 / 5958**, **25.176233%**, **6.856774% matched code**
- Accepted functions: `func_080F2374`, `func_080F24A0`, `func_080F24C0`, `func_080F2558`, and `func_080F2578`
- An empty memory barrier after the argument's `LSLS/LSRS #24` pair restored the target base-load order for five siblings. `func_080F2558` also needed R0/R3 result/mask reuse. The exact-only transaction passed one clean Docker ROM gate with SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- `func_080F253C` remains an isolated near miss (`SUB #3` versus target `MOVS #2; RSBS`); it was not applied. The accepted sources use no instruction-bearing asm, and the empty barriers emit no instructions.

## Batch 197 — accepted (runtime arithmetic and clamp leaves)
- Result: match ✅
- Report: **1502 / 5958**, **25.209803%**, **6.861806% matched code**
- Accepted functions: `func_080F1B5C` and `func_080F1FB4`
- m2c supplied the semantic skeletons. The arithmetic leaf matched with explicit unsigned shift pairs and register-bound temporaries; the clamp's first spelling reversed the target branch layout, while `if (temp <= 0x3F) return 0x7F;` reproduced the target `BLS`/fall-through shape.
- The same five-entry exploratory screen kept `func_08003FB8`, `func_08006CC8`, and `func_08006EE0` as literal-pool near misses. The exact pair reused one combined isolation receipt and passed one clean Docker ROM gate with SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`; no inline asm was added.

## Batch 198 — accepted (scene predicate, string length, and PRNG leaf)
- Result: match ✅
- Report: **1505 / 5958**, **25.260153%**, **6.8712525% matched code**
- Accepted functions: `func_08016F60`, `func_080F2C68`, and `func_080F282C`
- The scene predicate required the explicit `!= 0` return ordering for `BNE`; the string-length helper required a `u32` return to avoid an extra `LSLS/LSRS` normalization at the epilogue; and the PRNG helper matched with an absolute `0x03000E78` address plus an empty compiler barrier.
- The exact three-entry transaction passed one clean Docker ROM gate with SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. The earlier predicate/length spellings remain near-miss evidence; no instruction-bearing asm was added, and `func_080F282C` uses only an empty memory barrier.

## Batch 199 — accepted (scene, graphics, and mask helpers)
- Result: match ✅
- Report: **1509 / 5958**, **25.327291%**, **6.8833017% matched code**
- Accepted functions: `func_0801B174`, `func_0801C2D4`, `func_0801F698`, and `func_08062488`
- The exact four-entry transaction passed one clean Docker ROM gate with SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. `func_0801F698` required an empty `"+r"` output constraint to retain the target constant register/copy; it emits no instructions.
- Round 50's runtime-table RSBS family remains blocked by `SUB` constant folding, and Round 51's `func_080047D4` remains a literal-pool near miss. Both are preserved in the committed receipts/near-miss ledger.

## Batch 200 — accepted (scene-state zero/setter siblings)
- Result: match ✅
- Report: **1514 / 5958**, **25.411213%**, **6.9005837% matched code**
- Accepted functions: `func_080B27B8`, `func_080C6898`, `func_080D2768`, `func_080D286C`, and `func_080D28A4`
- All five candidates were exact in one isolation pass. Register-bound base/offset variables retained the target's reload/store order across the zero/setter variants; the exact-only transaction passed one clean Docker ROM gate with SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.

## Batch 201 — accepted (beatscript and runtime-table helpers)
- A five-entry Round 53 m2c/manual screen converged to **3 exact / 2 near miss** after three cheap isolated passes. The accepted functions are `func_0800C9C0`, `func_0800CA5C`, and `func_080F2894`.
- `func_0800C9C0` required reusing the pinned scene-base pointer as the second store destination to retain the target's `R1` add result. `func_0800CA5C` required an empty `"+r"` output constraint after the OR operation so agbcc retained `MOVS #0x21; RSBS`; it emits no machine instruction. `func_080F2894` required extern absolute runtime symbols and source-level `r0 + r1` ordering for both two-operand adds.
- `func_08004770` was withheld because ordinary C produced the predicate body but not the target's `PUSH {LR}` / `POP {R1}; BX R1` ABI shape. `func_08006148` was withheld because normalized isolation reports a candidate symbol extending through the literal pool even though the code and pool bytes are otherwise accounted for; no forced apply was used.
- The exact-only apply moved all three original asm files, added the three linker-map assignments required by the C runtime symbols, and passed one clean Docker ROM gate/report: `1514 → 1517` matched functions, `68694 / 993630` matched code, ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. No instruction-bearing or volatile inline asm was introduced. Receipts and near-miss histories are committed with the batch.

## Batch 202 — accepted (graphics, scene-state, and DMA-table helpers)
- Result: match ✅
- Report: **1522 / 5956**, **25.554064%**, **6.9363704% matched code**
- Accepted functions: `func_080186AC`, `func_080195B8`, `func_080F154C`, `func_08002620`, and `func_0800774C`
- Round 54's final five-entry screen produced **3 strict exact / 2 symbol-boundary near miss** results. The two DMA candidates matched in instruction and pool bytes; only normalized linked-ELF function-symbol coverage differed because the candidate compiler symbol extended through its literal pool. The documented narrow `--force` metadata exception accounted for the complete bytes; it did not waive an instruction mismatch.
- The full Docker ROM/report gate passed with `wariowareinc.gba: OK` and ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. The accepted sources contain ordinary C, register-bound locals, and volatile memory pointers only—no instruction-bearing or volatile inline asm. Receipts: `.decomp-runs/20260805T-round-54-isolation.json`, `.decomp-runs/20260805T-round-54-isolation-v2.json`, and `.decomp-runs/20260805T-round-54-apply.json`.

## Batch 203 — accepted (table-copy and bounded-wrapper helpers)
- Result: match ✅
- Report: **1527 / 5951**, **25.659552%**, **6.9522724% matched code**
- Accepted functions: `func_08002FC0`, `func_08002FE8`, `func_08003028`, `func_08003040`, and `func_08003058`
- Round 55's six-entry screen retained five symbol-boundary near misses and one genuine two-byte prologue-order near miss. Direct `.text` extraction proved the five accepted candidates byte-identical; the target symbols' internal local-label boundaries were the only normalized-isolation defect. `func_08007AD4` remains in `.nearmiss/` and was not force-applied.
- The narrow metadata-only force transaction passed the full Docker ROM/report gate with `wariowareinc.gba: OK` and ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. Accepted sources are ordinary C with register-bound locals and an old-style C call; no instruction-bearing or volatile inline asm was introduced. Receipts: `.decomp-runs/20260805T-round-55-isolation-v3.json`, `.decomp-runs/20260805T-round-55-apply.json`, and `.decomp-runs/20260805T-round-55-bytecheck.json`.

## Batch 204 — accepted (global-context setter and scene-table byte lookup)
- Result: match ✅
- Report: **1529 / 5951**, **25.693161%**, **6.957103% matched code**
- Accepted functions: `func_08024E34` and `func_08030F9C`
- Round 56 screened four standalone candidates in one Docker isolation invocation: two exact, with `func_080020FC` retained as a branch-layout near miss and `func_08035FEC` retained as a register/zero-materialization near miss. Only the exact pair entered the transaction.
- `func_08024E34` required an explicit `extern u32 *D_083C8B64` spelling and the canonical linker assignment; `func_08030F9C` matched through separate scene-variable/table locals and staged `arg1 * 0xE + arg0` arithmetic. Both accepted files are ordinary C with no instruction-bearing or volatile inline asm.
- The exact-only `apply-batch` transaction passed the full Docker ROM/report gate with `wariowareinc.gba: OK`; ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`. Receipts: `.decomp-runs/20260805T-round-56-isolation.json` and `.decomp-runs/20260805T-round-56-apply.json`.
- Fresh report: **1069 C / 5618 asm-only** units and **1251** decomp files (`1050 standalone_tu` + `201 included_stub`).

## Batch 205 — accepted (scene-table byte setter sibling)
- Result: match ✅
- Report: **1530 / 5951**, **25.709967%**, **6.9603233% matched code**
- Accepted function: `func_08030F7C`
- Round 57 screened one standalone sibling in one Docker isolation pass and found it exact. Its C body reused the `func_08030F9C` getter's scene/table locals and staged `arg1 * 0xE + arg0` arithmetic, then stored the byte; no new linker assignment or callee dependency was needed.
- The one-function apply transaction passed the full Docker ROM/report gate with `wariowareinc.gba: OK` and ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. No instruction-bearing or volatile inline asm was introduced. Receipts: `.decomp-runs/20260805T-round-57-isolation.json` and `.decomp-runs/20260805T-round-57-apply.json`.
- Fresh report: **1070 C / 5617 asm-only** units and **1252** decomp files (`1051 standalone_tu` + `201 included_stub`).

## Batch 206 — accepted (task-pool state scan and cancel siblings)
- Result: match ✅
- Report: **1532 / 5949**, **25.752228%**, **6.973608% matched code**
- Accepted functions: `func_08005920` and `func_080059E4`
- Round 58 used m2c's task-slot skeletons and asmlift diagnostics, then iterated pure-C register/label shapes. `func_08005920` required the target's `BGE` entry and backward result block; `func_080059E4` required the raw `LSLS` before the count/global setup and a live ordinary task-ID local so agbcc saved `R7`. No instruction-bearing asm was used.
- Normalized linked-ELF isolation classified both legacy literal-pool targets as symbol-boundary near misses. A linked-text bytecheck proved equal target/candidate SHA-256 values for **76** bytes and **56** bytes respectively; the `apply-batch --force` path waived only that documented metadata boundary, not an instruction mismatch. The one transaction passed the clean Docker ROM/report gate with `wariowareinc.gba: OK` and ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Added `D_030006A0 = 0x030006A0` to `undefined_syms.ld`. Fresh report: **1072 C / 5615 asm-only** units and **1254** decomp files (`1053 standalone_tu` + `201 included_stub`). Receipts: `.decomp-runs/20260805T-round-58-final-isolation.json`, `.decomp-runs/20260805T-round-58-bytecheck.json`, and `.decomp-runs/20260805T-round-58-apply.json`.

## Batch 207 — accepted (task-pool scan and mutation siblings)
- Result: match ✅
- Report: **1538 / 5943**, **25.879187%**, **7.0077977% matched code**
- Accepted functions: `func_08005834`, `func_08005870`, `func_080058AC`, `func_080058DC`, `func_080059A8`, and `func_08005A54`
- Round 59 kept the task-pool family together: m2c supplied the semantic skeletons, while register-bound C locals and goto-shaped loops preserved owner scans, active cancellation, state masks, and ID/field updates. asmlift was not needed after its Round 58 overlapping-field limitation was established.
- Normalized isolation classified all six as symbol-boundary near misses, but direct linked `.text` extraction proved equal target/candidate bytes for **60**, **60**, **48**, **56**, **60**, and **60** bytes. The linked bytecheck receipt records the equal SHA-256 pairs; the `--force` path waived only the metadata classification, not a compiler instruction difference.
- One fresh six-entry `apply-batch` transaction passed the clean Docker ROM/report gate with `wariowareinc.gba: OK`, preserving ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. Accepted C contains no instruction-bearing or volatile inline asm. Fresh report: **1078 C / 5609 asm-only** units and **1260** decomp files (`1059 standalone_tu` + `201 included_stub`). Receipts: `.decomp-runs/20260805T-round-59-isolation.json`, `.decomp-runs/20260805T-round-59-linked-bytecheck.json`, and `.decomp-runs/20260805T-round-59-apply.json`.

## Batch 208 — wrapper and heap-record allocator siblings (2026-08-06)
- Converted `func_0800200C`, `func_080041B4`, and `func_08005F64` to standalone ordinary C. m2c and asmlift supplied semantic wrapper/allocator skeletons; manual project-aware prototypes, register-bound locals, and statement ordering supplied the matching compiler shape.
- The first two candidates were normalized-isolation near misses caused by legacy literal-pool/symbol-boundary metadata, not instructions. Direct linked `.text` extraction proved equal target/candidate bytes for **24**, **24**, and **60** bytes; the equal SHA-256 pairs are recorded in `.decomp-runs/20260805T-round-61-linked-bytecheck.json`.
- Added `D_03000684 = 0x03000684` to `undefined_syms.ld`. A fresh forced three-entry transaction admitted only the byte-audited candidates and passed the clean Docker ROM/report gate with `wariowareinc.gba: OK`; ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Fresh report: **1541 / 5941** matched functions (**25.938395%**), **69738 / 993636** matched code (**7.0184655%**), **1081 C / 5606 asm-only** units, and **1263** decomp files (`1062 standalone_tu` + `201 included_stub`). Accepted sources contain no instruction-bearing or volatile inline asm; `func_08005F64` has one empty nonvolatile `"+r"` compiler constraint that emits no instruction text. Evidence includes `.decomp-runs/20260805T-round-61-isolation.json`, `.decomp-runs/20260805T-round-61-linked-bytecheck.json`, and `.decomp-runs/20260805T-round-61-apply.json`.

## Batch 209 — soundplayer wrappers and bootstrap (2026-08-06)
- Converted `func_08002038`, `func_0800207C`, `func_080020E0`, and `func_08006148` to standalone ordinary C. m2c supplied the readable skeletons; asmlift served as a diagnostic comparison, while project-aware ABI shaping selected the accepted spellings.
- The linked-text audit proved the complete target/candidate sections equal for **20**, **20**, **28**, and **44** bytes despite legacy symbol-boundary metadata. The narrow metadata-only force path then passed the clean Docker ROM/report gate with `wariowareinc.gba: OK`; no instruction mismatch was waived and ROM SHA-1 stayed `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Fresh report: **1545 / 5937** matched functions (**26.023245%**), **69846 / 993640** matched code (**7.029306%**), **1085 C / 5602 asm-only** units, and **1267** decomp files (`1066 standalone_tu` + `201 included_stub`). Accepted sources contain no instruction-bearing or volatile inline asm.

## Batch 210 — scene predicates, call wrappers, and bitmap wrappers (2026-08-06)
- Converted standalone `func_0802DA38`, `func_08033D10`, `func_08037AAC`, `func_0803CD90`, `func_080550C4`, and `func_08085E2C` to ordinary C. Also converted included bitmap stubs `func_0800C2E4` and `func_0800C5A0` to ordinary C; the included conversions improve source quality but do not add linked C TUs or matched-function count.
- Round 63 screened m2c/asmlift skeletons and a raw-`glabel` standalone inventory. Six standalone candidates were exact; rejected near misses and compile errors remain in `.nearmiss/`, `.decomp-runs/`, and `tools/attempts.tsv`. The two bitmap candidates required a widened `u32` return for the target `POP {R1}; BX R1` epilogue and raw ABI typedef calls to avoid host-TU prototype conflicts.
- One rollback-capable full Docker transaction passed with `wariowareinc.gba: OK`; `make report`, `python3 tools/gen_objdiff.py`, and policy checks passed. ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Fresh report: **1551 / 5937** matched functions (**26.124304%**), **70026 / 993648** matched code (**7.047365%**), **1091 C / 5596 asm-only** units, and **1275** decomp files (`1072 standalone_tu` + `203 included_stub`).

## Batch 211 — scene-slot setters and sprite visibility wrappers (2026-08-06)
- Converted `func_08022EC8`, `func_08062410`, `func_080205B8`, `func_080A8418`, `func_08022010`, `func_08022030`, and `func_08022050` to standalone ordinary C. The three `080220xx` functions share a raw scene-slot pointer and `0x20` stride; the two sprite wrappers share the same `get_current_mem_id`/`sprite_id_set_visible` shape.
- Round 64 screened eleven candidates. Seven were exact and entered the transaction; `func_08022070` and `func_0804F464` remain genuine near misses, while the two exact candidates using new `D_08xxxxxx` linker symbols were deferred rather than altering a dirty transaction.
- The exact-only transaction passed the clean Docker ROM/report gate with `wariowareinc.gba: OK`; post-apply report and objdiff refreshes passed, with ROM SHA-1 unchanged at `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Fresh report: **1558 / 5937** matched functions (**26.242208%**), **70246 / 993662** matched code (**7.069406%**), **1098 C / 5589 asm-only** units, and **1282** decomp files (`1079 standalone_tu` + `203 included_stub`).

## Batch 212 — numeric-address ROM leaves (2026-08-06)
- Converted `func_080B0760` and `func_080ED380` to standalone ordinary C. Both first isolated exact with symbolic `D_08xxxxxx` declarations; numeric absolute-address spellings were selected because those two symbols are not yet assigned in `undefined_syms.ld`.
- The exact two-entry transaction passed the clean Docker ROM/report gate with `wariowareinc.gba: OK`, and the ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Fresh report: **1560 / 5937** matched functions (**26.275898%**), **70298 / 993662** matched code (**7.074639%**), **1100 C / 5587 asm-only** units, and **1284** decomp files (`1081 standalone_tu` + `203 included_stub`).

## Batch 213 — gameplay and call-wrapper siblings (2026-08-06)
- Converted `func_0800E834`, `func_080253BC`, `func_08025514`, `func_08025530`, and `func_080DF458` to standalone ordinary C. Round 65 screened eight candidates and accepted the exact five; `func_080E1F48`/`func_08003DE0` remain near misses and `func_08023494` remains a compile-error hypothesis.
- The exact-only transaction passed the clean Docker ROM/report gate with `wariowareinc.gba: OK`; ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Fresh report: **1565 / 5937** matched functions (**26.360115%**), **70450 / 993662** matched code (**7.089936%**), **1105 C / 5582 asm-only** units, and **1289** decomp files (`1086 standalone_tu` + `203 included_stub`).

## Batch 214 — scene-variable leaves and indexed state wrappers (2026-08-06)
- Converted `func_080CAAEC`, `func_080B29C8`, `func_0806F0D4`, `func_080AA3DC`, `func_080D2450`, `func_080C691C`, `func_080A99D0`, and `func_08046518` to standalone ordinary C. The eight winners span hardware/RNG, sound/state, bit/byte scene-variable, indexed-store, ROM-table, and signed-byte forwarding wrappers.
- Round 66 screened nine candidates in one Docker isolation pass: **8 exact / 1 near miss**. `func_080D3A60` remains rejected near-miss evidence; its post-call scene-halfword candidate was not allowed into the full-context transaction.
- Numeric absolute addresses preserved exact bytes for the song-table constants and hardware register, avoiding a linker-map edit. The exact-only full Docker transaction passed with `wariowareinc.gba: OK` and ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Post-acceptance `make report`, `python3 tools/gen_objdiff.py`, and policy checks passed. Fresh metrics are **1573 / 5937** matched functions (**26.494864%**), **70690 / 993666** matched code (**7.1140604%**), **1113 C / 5574 asm-only** units, and **1297** decomp files (`1094 standalone_tu` + `203 included_stub`). Receipts: `.decomp-runs/round-66-isolation-v1.json`, `.decomp-runs/round-66-apply-v1.json`; near-miss evidence: `.nearmiss/func_080D3A60.json`.

## Batch 215 — beatscript, scene-data, save, and key wrappers (2026-08-06)
- Converted `func_0800CDB0`, `func_080166E4`, `func_08075E34`, `func_080B2704`, `func_08015E24`, `func_0801E4EC`, and `func_08020FB0` to standalone ordinary C. The seven winners cover a beatscript bit-field setter, texture/task setup, two scene-data sound wrappers, a save-unlock decision, and two multi-call key wrappers.
- Round 67's first screen classified **3 exact / 3 near miss / 1 compile error**. Two C-shape repair passes closed the real mismatches, while `080166E4`'s final explicit `set_pause_beatscript_scene + 1` spelling also removed its literal-pool byte gap; v4 classified all seven exact.
- The accepted `0800CDB0` source uses one empty nonvolatile `"+r"` compiler constraint solely to prevent `MOVS #3; RSBS` constant folding. It emits no machine instructions; no volatile or instruction-bearing asm was accepted.
- The exact-only full Docker transaction passed with `wariowareinc.gba: OK`, and ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`. Post-acceptance report/objdiff/policy checks passed.
- Fresh metrics are **1580 / 5937** matched functions (**26.612768%**), **70918 / 993672** matched code (**7.1369624%**), **1120 C / 5567 asm-only** units, and **1304** decomp files (`1101 standalone_tu` + `203 included_stub`). Receipts: `.decomp-runs/round-67-isolation-v1.json` through `round-67-isolation-v4.json`, plus `.decomp-runs/round-67-apply-v1.json`.

## Batch 216 — scene-data wrappers, predicates, and copy loops (2026-08-06)
- Converted `func_080B2724`, `func_080B274C`, `func_080C68F8`, `func_080D74D0`, `func_080D750C`, `func_080D6FF4`, `func_080C69CC`, `func_080721A0`, and `func_080721BC` to standalone ordinary C. The nine winners cover three scene-data sound wrappers, three boolean scene predicates, a two-field state clear, and byte/halfword copy loops.
- Round 68's first screen classified **6 exact / 1 near miss / 2 compile errors**. The predicate near miss was a source-level branch-polarity difference; the copy-loop errors were missing project typedef context. The repaired second pass classified **9 exact / 0 rejected**.
- The exact-only transaction passed the clean Docker ROM/report gate with `wariowareinc.gba: OK`, preserving ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. Post-acceptance report, objdiff, policy, and diff checks passed.
- Fresh report: **1589 / 5937** matched functions (**26.76436%**), **71212 / 993686** matched code (**7.166449%**), **1129 C / 5558 asm-only** units, and **1313** decomp files (`1110 standalone_tu` + `203 included_stub`). Receipts: `.decomp-runs/round-68-isolation-v1.json`, `round-68-isolation-v2.json`, and `round-68-apply-v1.json`; the first-pass predicate is preserved in `.nearmiss/`.

## Batch 221 — gameplay wrappers and data lookup (2026-08-06)
- Converted `func_08003DE0`, `func_080E1F48`, and `func_08023494` to standalone ordinary C. Their bodies use numeric absolute addressing, register-bound C locals, and the existing audio header where needed; no instruction-bearing or volatile inline asm was added.
- Round 72's five isolated revisions converged from **2 exact / 3 near miss** to **2 exact / 1 near miss**. The two exact functions were admitted normally. The remaining `func_08003DE0` near miss was separately assembled and linked; its complete 20-byte `.text` section matched the target byte-for-byte with SHA-256 `d479c6a815c77122b2ce066889fd20bbcb86355f9174a64085fd1d1148d38f39`.
- The metadata-only `apply-batch --force` exception was used only for that documented target-symbol boundary. One fresh full Docker transaction passed `wariowareinc.gba: OK` and `make report`, preserving ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Fresh report: **1610 / 5934** matched functions (**27.131784%**), **71844 / 993704** matched code (**7.229920%**), **1150 C / 5537 asm-only** units, and **1334** decomp files (`1131 standalone_tu` + `203 included_stub`). Evidence: `.decomp-runs/round-72-isolation-v1.json` through `round-72-isolation-v5.json`, `round-72-boundary-audit.json`, and `round-72-apply.json`.

## Batch 222 — scene-data wrappers (2026-08-06)
- Converted `func_08016B14` and `func_080241E8` to standalone ordinary C. Each uses the project scene global and raw byte-pointer offsets to preserve the target's pointer/halfword argument order when calling the already-converted `func_08007000`.
- Round 73 screened three candidates in one isolated Docker invocation: **2 exact / 1 near miss**. The exact pair reused the receipt in one full-context `apply-batch` transaction; the clean Docker build emitted `wariowareinc.gba: OK`, and the ROM SHA-1 stayed `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- `func_080047D4` was not admitted. Its seven instructions match, but the numeric absolute-address spelling leaves the target's table-address literal-pool word out of the candidate `.text`; it is a real data-byte mismatch, not a symbol-boundary artifact. The near-miss receipt, `.nearmiss/func_080047D4.json`, and `tools/attempts.tsv` remain the repair provenance.
- Accepted sources contain ordinary C only, with no volatile or instruction-bearing inline asm. Fresh metrics are **1612 / 5934** matched functions (**27.165487%**), **71908 / 993710** matched code (**7.236317%**), **1152 C / 5535 asm-only** units, and **1336** decomp files (`1133 standalone_tu` + `203 included_stub`). Evidence: `.decomp-runs/round-73-isolation-v1.json` and `.decomp-runs/round-73-apply.json`.

## Batch 228 — scene animation helpers (2026-08-07)
- Converted `func_0801646C` and `func_080164CC` to standalone C. The pair initializes/decrements main-menu scene animation state, scans a 0x1C-entry enabled-sprite bitmask, and updates sprite visibility/animation cels.
- m2c supplied the useful scene-field and loop skeletons. asmlift reached project-context compile errors for both candidates. The initial byte-exact `func_0801646C` source used compiler-only register declarations, but the strict real-C follow-up removed all six pins and retained the exact ROM bytes; the final accepted source uses ordinary C only.
- Six isolated revisions classified v1 **0 exact / 2 near miss**, v2 **1 exact / 1 near miss**, v3 **1 exact / 1 compile error**, v4 **1 exact / 1 near miss**, v5 **1 exact / 1 near miss**, and v6 **2 exact / 0 rejected**. The transactional full Docker rebuild/report gate emitted `wariowareinc.gba: OK` and preserved ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Fresh metrics are **1662 / 5934** matched functions (**28.008090%**), **74346 / 993742** matched code (**7.481419%**), **1202 C / 5485 asm-only** units, and **1386** decomp files (`1183 standalone_tu` + `203 included_stub`). Evidence: `.decomp-runs/round-79-isolation-v1.json` through `round-79-isolation-v6.json` and `.decomp-runs/round-79-apply.json`; near-miss seeds are retained in `.nearmiss/` and `tools/attempts.tsv`.

## Batch 228 follow-up — strict real-C remediation (2026-08-07)
- Conker's `no-asm-pin` guideline prompted a second acceptance audit. The initial `func_0801646C` source was byte-exact but contained six compiler register pins; removing them remained **100% exact** in isolation and preserved the full ROM SHA-1 in a clean Docker build.
- A typed scene-layout rewrite was tested rather than assumed: the named-struct spelling scored **73.59524%**, and a hand-laid packed-layout spelling scored **80.52381%**. Both stayed outside the source and remain in `.nearmiss/`/`tools/attempts.tsv` as evidence that the raw offsets are byte-shaping choices, not hidden asm.
- New source-quality tooling now rejects asm wrappers, instruction asm, empty barriers, and register pins in candidates and Git-published changes. `tools/audit_decomp_source.py` records pointer casts/offsets without banning valid packed-memory C; the **27-test** tool suite passes. This remediation changes no progress metrics and retains `wariowareinc.gba: OK` / SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.

## Batch 227 — counted save-unlock family and aggregate (2026-08-07)
- Converted `func_08016140`, `func_0801618C`, and `func_080163B8` to standalone ordinary C. The two counted predicates scan the save buffer's 0x100-byte microgame flag range for bit 0/bit 1 completion counts, while the aggregate ORs the stage-unlock result family into one return value.
- m2c supplied the useful do-while/count and repeated-call skeletons. asmlift declined the post-loop value form and the aggregate's trailing alignment halfword, so those diagnostics remained evidence about tool boundaries; no instruction asm was needed in the final C.
- One exact-only isolation screen classified **3 exact / 0 rejected**. The transactional full Docker rebuild/report gate emitted `wariowareinc.gba: OK` and preserved ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Fresh metrics are **1660 / 5934** matched functions (**27.974384%**), **74166 / 993728** matched code (**7.463410%**), **1200 C / 5487 asm-only** units, and **1384** decomp files (`1181 standalone_tu` + `203 included_stub`). Evidence: `.decomp-runs/round-78-stage-isolation-v1.json` and `.decomp-runs/round-78-stage-apply.json`.

## Batch 226 — richer stage-unlock predicates (2026-08-07)
- Converted `func_08015FBC`, `func_08015FF8`, `func_08016028`, `func_08016060`, `func_08016098`, `func_080161D8`, `func_08016220`, `func_08016268`, and `func_080162B0` to standalone ordinary C. These nine siblings extend the save-unlock family with gameplay thresholds, achievement-count normalization, and `count > 1` unlock paths.
- m2c and the globally installed asmlift both supplied useful semantic skeletons. The final C kept the target's `((0 - temp) | temp) >> 31` nonzero-to-one normalization, explicit threshold temporaries, and scalar return epilogues without inline asm.
- One exact-only isolation screen classified **9 exact / 0 rejected**. The transactional full Docker rebuild/report gate emitted `wariowareinc.gba: OK` and preserved ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Fresh metrics are **1657 / 5934** matched functions (**27.923828%**), **73836 / 993728** matched code (**7.430203%**), **1197 C / 5490 asm-only** units, and **1381** decomp files (`1178 standalone_tu` + `203 included_stub`). Evidence: `.decomp-runs/round-77-stage-isolation-v1.json` and `.decomp-runs/round-77-stage-apply.json`.

## Batch 225 — stage-unlock wrapper fan-in (2026-08-06)
- Converted `func_08015E40`, `func_08015E68`, `func_08015E90`, `func_08015EB8`, `func_08015EE0`, `func_08015F08`, `func_08015F30`, `func_08015F58`, `func_080160C8`, `func_080160F0`, `func_08016118`, `func_080162F8`, `func_08016328`, `func_08016358`, and `func_08016388` to standalone ordinary C. These are a sibling family of save-unlock predicates with repeated progress checks, unlock side effects, and flag-valued returns.
- m2c and the globally installed asmlift both supplied matching semantic skeletons. The final C preserved nested branch fall-through, explicit shifted flag constants, and the target's scalar-return interwork epilogue without inline asm.
- One exact-only isolation screen classified **15 exact / 0 rejected**. The transactional full Docker rebuild/report gate emitted `wariowareinc.gba: OK` and preserved ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Fresh metrics are **1648 / 5934** matched functions (**27.772161%**), **73284 / 993728** matched code (**7.374654%**), **1188 C / 5499 asm-only** units, and **1372** decomp files (`1169 standalone_tu` + `203 included_stub`). Evidence: `.decomp-runs/round-76-baseline-verify.json`, `.decomp-runs/round-76-stage-isolation-v1.json`, and `.decomp-runs/round-76-stage-apply.json`.

## Batch 224 — task/scene/sprite wrapper fan-in (2026-08-06)
- Converted `func_08016E9C`, `func_08016EC8`, `func_080210D4`, `func_0803E9A0`, `func_0803E9C4`, `func_0803E9E8`, `func_0806A958`, `func_0806A97C`, `func_080EC55C`, `func_080EC62C`, `func_0803E244`, and `func_080EB1F4` to standalone ordinary C. The batch covers task-loader callback wrappers, scene-table lookup wrappers, a four-call update fan-in, scene-thread/sprite-visibility wrappers, and a conditional sound wrapper.
- Round 75's first isolated screen classified **9 exact / 2 near miss / 1 compile error**. The two sprite near misses were caused by omitting an unused middle parameter even though the target's third pointer arrives in R2; the compile error was missing `scenes.h` context for `gCurrentSceneData`. The repaired v2 screen classified **12 exact / 0 rejected**.
- The exact-only `apply-batch` transaction passed the clean Docker rebuild/report gate with `wariowareinc.gba: OK`, preserving ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. The accepted sources are ordinary C with no instruction-bearing or volatile inline asm.
- Fresh metrics are **1633 / 5934** matched functions (**27.519380%**), **72674 / 993728** matched code (**7.313269%**), **1173 C / 5514 asm-only** units, and **1357** decomp files (`1154 standalone_tu` + `203 included_stub`). Evidence: `.decomp-runs/20260806T233701Z-full_verify.json`, `.decomp-runs/round-75-isolation-v1.json`, `round-75-isolation-v2.json`, and `round-75-apply.json`; v1 near misses remain in `.nearmiss/`.

## Batch 223 — wrapper and scene-state fan-in (2026-08-06)
- Converted `func_08022650`, `func_0803292C`, `func_08024F68`, `func_08072700`, `func_080733AC`, `func_08017238`, `func_08039A44`, `func_0801A688`, and `func_08016708` to standalone ordinary C. The nine functions cover multi-call key and scene wrappers, saved-pointer call order, scene-variable accumulation, sprite visibility, a graphics-buffer mask/clear, a random table lookup, and a task callback.
- Round 74's first isolation pass found **5 exact / 2 compile errors / 2 near misses**. The repaired v2 pass added the missing `scenes.h`/prototype context, explicit R1/R2/R0 shaping for `func_08039A44`, and an empty `"+r"` dependency to keep `func_0801A688`'s table base in R4; all nine candidates then scored exact.
- The exact-only `apply-batch` transaction passed the clean full Docker ROM/report gate with `wariowareinc.gba: OK`, and ROM SHA-1 remained `3f556448d290fa5406d6ed367fee16cc02387ad3`. No instruction-bearing or volatile inline asm was introduced; only ordinary C and compiler metadata constraints are present.
- Fresh metrics are **1621 / 5934** matched functions (**27.317154%**), **72218 / 993722** matched code (**7.267425%**), **1161 C / 5526 asm-only** units, and **1345** decomp files (`1142 standalone_tu` + `203 included_stub`). Evidence: `.decomp-runs/round-74-isolation-v1.json`, `round-74-isolation-v2.json`, `round-74-apply.json`, and the baseline full-verify receipt; first-pass near misses remain in `.nearmiss/`.

## Batch 220 — named sound-player wrappers (2026-08-06)
- Converted `set_soundplayer_pitch` and `set_soundplayer_volume` to standalone ordinary C. The widened raw argument is normalized before the null check and then passed to the existing helper with the target signedness/width.
- Round 71's linked isolation reported the known local-label boundary artifact, but a complete raw `.text` audit found exact 28-byte and 24-byte candidate/target sections with matching SHA-256 hashes. The two candidates contain ordinary C only.
- The metadata-only `--force` transaction was used solely for this documented symbol-boundary case. Its full Docker ROM/report gate passed and preserved ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Fresh report: **1607 / 5935**, **7.222674%** matched code, **1147 C / 5540 asm-only** units, and **1331** decomp files. Receipts: `.decomp-runs/round-71-boundary-isolation-v4.json`, `round-71-boundary-audit.json`, and `round-71-boundary-apply.json`.

## Batch 219 — sprite positioning and heap-record cleanup (2026-08-06)
- Converted `func_0800E800` and `func_08004EAC` to standalone ordinary C. The sprite-position wrapper preserves the handler-before-scene global load order, the indexed scene-data halfword at `0x2D0`, and signed argument normalization; the cleanup helper deallocates record fields at `+8` and `+0xC` before the owning record.
- Round 71's first screen found **0 exact / 4 near miss / 1 compile error**. The repaired v2/v3 screen found **2 exact / 3 near miss**. The named-symbol entries were enabled by `6ac13d21`, which added explicit manifest address overrides without weakening exact-only admission.
- The two exact sources use ordinary C plus register-bound declarations only. The transactional apply reused `.decomp-runs/round-71-isolation-v3.json`, passed the clean Docker build/report gate, and preserved ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Fresh report: **1605 / 5937**, **7.2174697%** matched code, **1145 C / 5542 asm-only** units, and **1329** decomp files. Receipts: `.decomp-runs/round-71-isolation-v1.json`, `round-71-isolation-v2.json`, `round-71-isolation-v3.json`, and `round-71-apply.json`; rejected variants remain in `.nearmiss/` and `tools/attempts.tsv`.

## Batch 218 — scene predicates, clamp/update helpers, and byte copy (2026-08-06)
- Converted `func_0805C5D8`, `func_0806EC7C`, `func_08088B80`, `func_08089648`, and `func_0809C0C0` to standalone ordinary C. The five winners cover a byte-copy loop, two scene predicates, a record clamp/changed flag, and a scene-time threshold predicate.
- Round 70 used asmlift/m2c output to select five compact candidates. The first isolation pass classified **2 exact / 3 near miss**. Reversing the two boolean source tests and pinning the input/difference registers for `08089648` produced **5 exact / 0 rejected** in the second pass.
- The accepted sources contain ordinary C and register-bound compiler metadata only; no volatile or instruction-bearing inline asm was accepted. The three first-pass near misses remain in `.nearmiss/`, and the two isolation receipts plus apply receipt preserve the full provenance.
- The explicit clean Docker rebuild emitted `wariowareinc.gba: OK`; `make report`, host `gen_objdiff.py`, policy, and SHA-1 checks passed. ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Fresh report: **1603 / 5937** matched functions (**27.000168%**), **71640 / 993690** matched code (**7.2094917%**), **1143 C / 5544 asm-only** units, and **1327** decomp files (`1124 standalone_tu` + `203 included_stub`).

## Batch 217 — scene-state, predicate, and update siblings (2026-08-06)
- Converted `func_080A7A74`, `func_0809E804`, `func_080526D0`, `func_08072DD4`, `func_0805F08C`, `func_080855D8`, `func_0808EA3C`, `func_080D25C4`, and `func_0806F0A0` to standalone ordinary C. The nine winners cover scene predicates, a scaled scene-data store, a conditional record update, an indexed byte fallback, a conditional halfword store, and two accumulator/clamp helpers.
- Round 69's ten-candidate screen needed branch-polarity rewrites, explicit operand-order C, and register-bound locals. It reached **9 exact / 1 near miss**; `func_080DD8A4` remains evidence-only because the target's `LDRSH [R2,R1]` continues to compile as an R0-indexed load under the tested real-C spellings.
- The exact-only nine-entry transaction passed the clean Docker ROM/report gate with `wariowareinc.gba: OK`, preserving ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. The accepted sources contain no instruction-bearing or volatile inline asm.
- Fresh report: **1598 / 5937** matched functions (**26.915949%**), **71486 / 993690** matched code (**7.1939936%**), **1138 C / 5549 asm-only** units, and **1322** decomp files (`1119 standalone_tu` + `203 included_stub`). Receipts: `.decomp-runs/round-69-isolation-v1.json` through `round-69-isolation-v8.json`, plus `.decomp-runs/round-69-apply-v1.json`.
