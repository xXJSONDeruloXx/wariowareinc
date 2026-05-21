# WarioWare Inc. Decompilation Scale-Up (RESUMED from iteration 22)

## Goal
Reach **80% matched/decompiled-function progress** on the `docs/macabeus-tooling-assessment` branch, while preserving a **byte-identical ROM match** at every accepted milestone.
Target: **4768 / 5961 matched functions**

## Latest Verified Baseline (confirmed 2026-05-20, iterations 45-47)
- **Matched functions:** 1290 / 5957 = 21.65%
- **Matched code percent:** 6.2704%
- **ROM status:** `wariowareinc.gba: OK`
- **Gap to target:** 1290 → 4768 = need +3478 more matched functions

## Iteration 47 — accepted (partial: 4/8)
- **Candidate set:** 8 attempted — 4 accepted: `asm_08062260`/`asm_08062278` (gCSV[0xBBA/BBB]--), `asm_0801c758` (BL+s8 sign-ext+BL), `asm_0808a56c` (local-ptr multi-store with reload)
- **Reverted:** `asm_0801af18`/`asm_0801bea8`/`asm_0801b3e4` (bitfield clear ~N trap), `asm_080d3a60`/`asm_080d3a60` (POP register mismatch)
- **Result:** match ✅ (4-for-8 after binary search)
- **Metric delta:** matched_functions 1286→1290 (+4), matched_code_percent 6.2599%→6.2704%
- **Commit:** `49223873`, pushed
- **Learnings:** (1) `byte &= ~N` where `(~N & 0xFF)` fits in 8 bits: agbcc optimizes to `MOVS #(~N&0xFF); ANDS` instead of `MOVS #N; RSBS; ANDS` — cannot match (2) BL+gCSV+shift+STRH pattern may have POP register mismatch (R0 vs R1) — check carefully (3) Multi-store with reload requires local ptr var for first two stores if the original reused same gCSV load: `u8 *p = gCSV; *(u16*)(p+X)=0; p[Y]=1; *(u16*)(gCSV+Z)=arg0` (4) `((u8*)gCSV)[0xBBA]--` generates LDR+LDRB+SUBS#1+STRB from literal pool correctly

## Iteration 46 — accepted
- **Candidate set:** 7-function batch — `asm_08006790` (5-arg struct init), `asm_0802b078` (4-call R4 save wrapper), `asm_080623e4` (gCSV word++), `asm_08088f8c` (gCurrentSceneData LDRH+=), `asm_0808ed64` (byte store+BL), `asm_080b27d8` (gCSV byte clear large offset), `asm_080cd358` (gCurrentSceneData LDRH+shift+store)
- **Result:** match ✅ (7-for-7)
- **Metric delta:** matched_functions 1279→1286 (+7), matched_code_percent 6.245104%→6.2599%
- **Commit:** `10050330`, pushed
- **Learnings:** (1) 5-arg struct init with stack-passed arg4 (`LDR R4, [SP, #8]`) matches `void func(void*, u32, u32, u32, u32)` directly (2) 4-call R4-save wrapper: agbcc saves arg0 in R4 before first BL, restores with `ADDS R0, R4, #0` before each subsequent call (3) `(*(u32*)((u8*)gCSV + (0xBD << 4)))++` generates LDR/ADDS#1/STR correctly (4) `gCurrentSceneData` half-word add/shift patterns match cleanly with `#include "scenes.h"`

## Iteration 45 — accepted
- **Candidate set:** 8-function batch — `asm_080862cc`/`asm_080bf188`/`asm_080e4454` (gGraphicsBuffer.unk4C=0 siblings), `asm_0801e42c`/`asm_0801e43c` (BG_OFS[0].y setters), `asm_080df2c4`/`asm_080df2d8` (LDRH-SUB-STRH-LDRSH-set_soundplayer_pitch siblings), `asm_0808de30` (two-const-BL wrapper)
- **Result:** match ✅ (8-for-8)
- **Metric delta:** matched_functions 1271→1279 (+8), matched_code_percent 6.231208%→6.245104%
- **Commit:** `f92ed4ac`, pushed
- **Learnings:** (1) `gGraphicsBuffer.unk4C = 0` cleanly matches 3 sibling functions (2) `*(s16*)arg1` after `*arg1 -= N` generates MOVS R2, #0 + LDRSH [R1, R2] (3) Two-const-BL wrapper with POP {R0}/BX R0 matches `void func_X(){func_A(3); func_B(0);}`

## Iteration 44 — accepted
- **Candidate set:** 7-function batch — `asm_0807e1ac`/`asm_0807e350` (sprite_set_enable_updates + LDRSH), `asm_08062430` (shift deref + two BL), `asm_080597a8` (s8 sign-ext + two BL), `asm_0807b044` (gCSV increment + D_ ROM call), `asm_08073660` (func(0) + gCSV large-offset store), `asm_08024480` (gBeatscriptScene[1] with local ptr var)
- **Result:** match ✅ after fixing asm_08024480 (use local `u32 *p` instead of inline array indexing)
- **Metric delta:** matched_functions 1264→1271 (+7), matched_code_percent 6.209873%→6.231208%
- **Commit:** `e0c0f888`, pushed
- **Learnings:** (1) `((u32*)&gBeatscriptScene)[1]` generates wrong code — use `u32 *p = (u32*)&gBeatscriptScene; p[1]` instead (local var forces base-address load, enables [R, #4] offset form) (2) LDRSH [R1, R2] uses register-offset form — `*(s16*)(p + N)` generates MOVS R2, #N + LDRSH [R1, R2] ✓ (3) `((u8*)gCSV)[N]++` generates ADDS/LDRB/ADDS/STRB correctly

## Iteration 43 — accepted
- **Candidate set:** 8-sibling family — `func_08026264(N, arg0); gCSV[4] |= M` pattern
- **Result:** match ✅ (8-for-8)
- **Metric delta:** matched_functions 1256→1264 (+8), matched_code_percent 6.184095%→6.209873%
- **Commit:** `36eed474`, pushed
- **Learnings:** `ADDS R1, R0, #0` (copy arg to different position) generated when const is first arg and variable is second arg in BL call

## Iteration 42 — accepted
- **Candidate set:** 8-function batch — sct(1) patterns, conditional checks, BL+store combos
- **Result:** match ✅ after fixing two C89 declaration-before-statement issues
- **Metric delta:** matched_functions 1248→1256 (+8), matched_code_percent 6.160734%→6.184095%
- **Commit:** `4218ab64`, pushed
- **Learnings:** C89 requires all variable declarations before statements; use `u8 *p; BL; p = gCSV;` pattern for post-BL gCSV access

## Iteration 41 — accepted
- **Candidate set:** 6-function batch — `asm_0804e6c4` (sprite_id_delete shift 0xD6<<1), `asm_0809cf30` (sct+shift word store), `asm_080b0608` (two-pointer LDR/LDR), `asm_080d28ec` (two-pointer LDR/LDRH), `asm_0805c2d0` (shift-load ASRS call), `asm_080c992c` (two-address call gCSV+0xF4, gCSV+shift)
- **Result:** match ✅ (6-for-6)
- **Metric delta:** matched_functions 1242→1248 (+6), matched_code_percent 6.141803%→6.160734%
- **Commit:** `40aa7e61`, pushed
- **Learnings:** (1) `*(s32*)(p + shift) >> 9` generates ASRS R0, R0, #9 correctly (2) two-pointer LDR/LDRH calls with large shift offsets work using ADDS R0, R1, R2 form (3) sprite_id_delete with shift offsets matches ✓ (4) ADDS R2, #N between first and second arg computation correctly captured by compiler

## Iteration 40 — accepted
- **Candidate set:** 27 BX LR stubs
- **Result:** match ✅
- **Metric delta:** matched_functions 1215→1242 (+27), matched_code_percent 6.136365%→6.141803%
- **Commit:** `702b14cc`, pushed

## Iteration 39 — accepted
- **Candidate set:** 15 BX LR stubs
- **Result:** match ✅
- **Metric delta:** matched_functions 1200→1215 (+15), matched_code_percent 6.133345%→6.136365%
- **Commit:** `be5cdc3f`, pushed

## Iteration 38 — accepted
- **Candidate set:** 12 BX LR stubs
- **Result:** match ✅
- **Metric delta:** matched_functions 1188→1200 (+12) — crossed 1200 milestone!
- **Commit:** `75dd08ae`, pushed

## Iteration 37 — accepted
- **Candidate set:** 6-function batch — `asm_080aac74` (sprite_id_delete direct offset 0x1C), `asm_080ac524` (sprite_id_delete ADDS 0xE0), `asm_080cc884` (sprite_id_delete ADDS 0xFC), `asm_080adac0` (conditional byte check), `asm_080d9dd4` (two-call R4 wrapper), `asm_08058a5c` (sct+BL+gCSV store)
- **Result:** match ✅ (6-for-6)
- **Metric delta:** matched_functions 1182→1188 (+6), matched_code_percent 6.112803%→6.130928%
- **Commit:** `44d082bc`, pushed

## Iteration 36 — accepted
- **Candidate set:** 2-function batch — `asm_0804e594` (sprite_id_delete gCSV+0x84), `asm_080d4fe8` (gCSV+8 ptr + void call + byte store)
- **Result:** match ✅ after fixing offset (0x1C→0x84) and signature issues
- **Metric delta:** matched_functions 1182→1182 (batch 35 already in; +2 vs batch 35 baseline)
- **Commit:** `c1cacf3f`, pushed
- **Learnings:** (1) sprite_id_delete takes `struct SpriteHandler *` not u32 cast (2) When gCSV+N > 124, agbcc uses ADDS form; for small offsets [R, #N] form (3) `func_080D2F10()` takes void — callers pass no args (4) `u8 *p = (u8*)gCSV + 8` keeps R4=gCSV+8 for subsequent STRB [R4, #offset]

## Iteration 35 — accepted
- **Candidate set:** 4-function batch — `asm_0809ced4` (shift-offset deref+call siblings), `asm_080cf804`/`asm_080cf820` (gCSV offset call siblings), `asm_080de130` (conditional byte-check call)
- **Result:** match ✅ (committed as `30f3b829`)
- **Metric delta:** matched_functions 1176→1182 (+6 combined with iteration 36)

## Iteration 30 — accepted
- **Candidate set:** 3-function batch — `asm_0800c764` (LDRSH dealloc), `asm_08016cb0` (R2-arg sound wrapper), `asm_0800418c` (D_ clear+call)
- **Result:** match ✅
- **Metric delta:** matched_functions 1153→1156 (+3), matched_code_percent 6.043122%→6.048760%
- **Commit:** `9c7de76e`, pushed

## Iteration 31 — accepted
- **Candidate set:** 4-function batch — `asm_080f30e0`/`asm_080f30f0`/`asm_080f3100` (shift-extract siblings), `asm_08024298` (gBeatscriptScene deref wrapper)
- **Result:** match after reverting asm_08016670 (struct access linker error)
- **Metric delta:** matched_functions 1156→1160 (+4), matched_code_percent 6.048760%→6.055608%
- **Commit:** `9a35ff76`, pushed

## Iteration 32 — accepted
- **Candidate set:** 5-function batch — `asm_08023350`/`asm_08024024` (gCSV deref+call), `asm_0801b250`/`asm_0801b268` (gCSV offset deref+call), `asm_08061034` (gCSV s8 sign-ext call)
- **Result:** match ✅ (5-for-5 on first try!)
- **Metric delta:** matched_functions 1160→1165 (+5), matched_code_percent 6.055608%→6.067691%
- **Commit:** `45a3ea2e`, pushed

## Iteration 33 — accepted
- **Candidate set:** 5-function batch — `asm_08019aa4`/`asm_08019ac0` (gCSV shift-offset deref+call), `asm_08088b9c`/`asm_0808949c` (gCSV shift-offset+call), `asm_080c9534` (gCSV two-pointer wrapper)
- **Result:** match ✅ (5-for-5 on first try!)
- **Metric delta:** matched_functions 1165→1170 (+5), matched_code_percent 6.067691%→6.080581%
- **Commit:** `448b02bd`, pushed
- **Learnings:** (1) gCSV deref+call wrapper siblings are the most productive family — once one spelling is validated, siblings are nearly free wins (2) Shift-computed offsets in call wrappers match cleanly (3) Two-pointer wrappers passing `p+offset1, p+offset2` also match (4) `(s8)p[N]` produces LSLS/ASRS sign-extension correctly

## Iteration 34 — accepted
- **Candidate set:** 6-function batch — `asm_08075e8c`/`asm_080df478` (gCSV word+halfword pair calls), `asm_080c98ec` (two-pointer wrapper), `asm_080da130` (shift-offset call), `asm_08038f6c`/`asm_080421e8` (s8 sign-ext call siblings)
- **Result:** match ✅
- **Metric delta:** matched_functions 1170→1176 (+6), matched_code_percent 6.080581%→6.095886%
- **Commit:** `e40387e4`, pushed
- **Learnings:** (1) gCSV word+halfword pair calls like `func(*(u32*)(p+off1), *(u16*)(p+off2))` match cleanly (2) More s8 sign-ext call siblings confirmed productive

## Iteration 29 — accepted
- **Candidate set:** 5-function batch — `asm_0800d3b8` (two-call R4 wrapper), `asm_0802b4d4` (two-call R4 wrapper sibling), `asm_080cc920` (two-call R4 wrapper sibling), `asm_08005fa0` (double dealloc wrapper), `asm_0800c764` (LDRSH dealloc — REVERTED)
- **Result:** match after reverting asm_0800c764 (LDR vs LDRSH type mismatch)
- **Metric delta:** matched_functions 1149→1153 (+4), matched_code_percent 6.034865%→6.043122%, C units 697→701, asm-only 5990→5986
- **Commit:** `ae0fc72f`, pushed
- **Learnings:** (1) Two-call R4-save wrappers of the form `func1(arg0); func2(arg0)` match cleanly across siblings (2) `arg0[0]` with `u32*` produces LDR, not LDRSH — need `*(s16 *)arg0` for halfword loads (3) Double-dealloc wrappers (`mem_heap_dealloc(arg0[0]); mem_heap_dealloc(arg0)`) match cleanly

## Iteration 23 — accepted
- **Candidate set:** 5-function batch — `asm_0801e914` (BX LR stub), `asm_08005570` (D_03003FE8 byte setter), `asm_080656e4` (gCurrentSceneVariable decrement), `asm_080da190` (pair-add), `asm_080e5514` (pair-add sibling)
- **Result:** match (after fixing `asm_08005570` — `u8` param caused unwanted truncation; switched to `u32`)
- **Metric delta:** matched_functions 1121→1126 (+5), matched_code_percent 5.986366%→5.993805%, C units 669→674, asm-only 6018→6013
- **Commit:** `234220c1`, pushed
- **Learnings:** `u8` function params cause agbcc to emit `lsls/lsrs` truncation before `STRB`; use `u32` arg type when the original asm just does `STRB R0` without truncating

## Iteration 24 — accepted
- **Candidate set:** 5-function batch — `asm_08025160` (D_03006524 halfword store pair), `asm_080c9eb0` (struct init with zero-fill), `asm_080041a0` (D_ byte setter pair), `asm_080039c0` (load-word-pair from pointer), `asm_080f0e30` (D_030068E8 table byte store)
- **Result:** match after reverting 3 mismatching candidates (asm_080dd8a4 register alloc diff, asm_080f1574 LSRS vs ASRS, asm_0801f698 extra ADDS)
- **Metric delta:** matched_functions 1126→1131 (+5), matched_code_percent 5.993805%→6.003258% (**crossed 6%!**), C units 674→679, asm-only 6013→6008
- **Commit:** `fc4f684b`, pushed
- **Learnings:** (1) register allocation differences between C and asm still cause ROM mismatch even for semantically identical code (2) `(u32)(b << 31) >> 31` still generates ASRS not LSRS — need unsigned cast pattern (3) `p[0] |= 0x8000` generates extra ADDS — need `p[0] = p[0] | 0x8000` form or verify exact codegen (4) D_ absolute address pointers work for simple byte/halfword stores

## Iteration 25 — accepted
- **Candidate set:** 6-function batch — `asm_0801d2d0` (gCSV pointer-deref byte store), `asm_08040a2c` (gCSV word add), `asm_08052538` (gCSV word increment by 0x50), `asm_08076378` (gCSV word clear + byte set), `asm_080f1588` (D_03006570 word setter), `asm_080f1594` (D_03006888 byte setter)
- **Result:** match after reverting asm_08005914 (addressing mode diff) and asm_0801d4a0 (register alloc diff)
- **Metric delta:** matched_functions 1131→1137 (+6), matched_code_percent 6.003258%→6.013328%, C units 679→685, asm-only 6008→6002
- **Commit:** `9796ba11`, pushed
- **Learnings:** (1) D_ absolute address stores work when the offset is 0 (direct STRB/STRH/STR) — C and asm both load the final address (2) D_ with non-zero offsets (e.g., STR R0,[R1,#4]) produce different machine code than C with *(type *)(base+4) because agbcc loads the computed address while asm loads the base and uses an offset instruction (3) gCurrentSceneVariable simple deref + store patterns match well when C uses `(u8 *)gCurrentSceneVariable` pointer arithmetic

## Iteration 26 — accepted
- **Candidate set:** 5-function batch — `asm_080623fc` (gCSV shift-offset word clear), `asm_080266f0` (offset tail-call), `asm_0801bd30` (two-call wrapper: scene_set_current_thread + func), `asm_080c4368` (one-call+store wrapper), `asm_080c4a48` (gCSV s16 add — REVERTED)
- **Result:** match after reverting asm_080c4a48 (missing sign-extension LSLS/ASRS pair)
- **Metric delta:** matched_functions 1137→1141 (+4), matched_code_percent 6.013328%→6.020377%, C units 685→689, asm-only 6002→5998
- **Commit:** `ceba332e`, pushed
- **Learnings:** (1) gCSV shift-computed offsets like `(0xBD << 4)` can match when agbcc emits the same MOVS+LSLS pair (2) `scene_set_current_thread` wrappers with one-BL calls still match cleanly (3) `(s16)arg0` doesn't produce LSLS/ASRS sign-extension in agbcc — need explicit sign extension C spelling

## Iteration 27 — accepted
- **Candidate set:** 5-function batch — `asm_08007ea0` (D_ word clear), `asm_080a9360` (sct+gCSV byte clear), `asm_080d409c` (sct+tail-call wrapper), `asm_080f2e74` (one-call+store wrapper), `asm_08020198` (const-arg one-BL wrapper)
- **Result:** match ✅
- **Metric delta:** matched_functions 1141→1146 (+5), matched_code_percent 6.020377%→6.029842%, C units 689→694, asm-only 5998→5993
- **Commit:** `b00387e7`, pushed
- **Learnings:** (1) D_ zero-offset word clears work with `*(volatile u32 *)addr = 0` (2) `scene_set_current_thread(1); *(u8 *)gCurrentSceneVariable = 0` matches (3) const-arg wrappers like `func_0800CE1C((void *)0x083BBCDC)` match when the constant address goes to literal pool

## Iteration 28 — accepted (partial)
- **Candidate set:** 8-function attempt — 3 accepted, 5 reverted
- **Accepted:** `asm_0802eca0` (increment-and-call), `asm_080aaa2c` (const-arg pair call), `asm_080359b4` (offset-deref tail-call)
- **Reverted:** asm_08004994/080049bc/08004a30/08004a74 (POP {R1}/BX R1 trap), asm_0803dda4 (MOVS+NEGS vs MOVS+RSBS), asm_0801667c (D_ offset addressing), asm_08007e8c (wrong arg slot)
- **Metric delta:** matched_functions 1146→1149 (+3), matched_code_percent 6.029842%→6.034865%, C units 694→697, asm-only 5993→5990
- **Commit:** `84964974`, pushed
- **Learnings:** (1) POP {R1}/BX R1 trap still catches zero-pad wrapper siblings — need to check POP pattern before attempting conversion (2) -1 in C compiles as MOVS+NEGS not MOVS+RSBS — another known trap reconfirmed (3) D_ with non-zero offsets like `[R0,#6]` produce different machine code than C absolute address + [R0,#0]

## Checklist (next batch priorities)
- [ ] Select next 6-8 function candidate batch from proven sibling families
- [ ] Preflight candidates at object-file level
- [ ] Run clean Docker build, verify `wariowareinc.gba: OK`
- [ ] Run `make report` and `gen_objdiff.py`, compare vs baseline
- [ ] If match: commit code+docs, push, update baseline
- [ ] If mismatch: binary-search, fix/revert, document trap

## Key Rules
1. One function per C file in `src/decomp/`
2. Move converted `.s` to `asm/converted/`
3. Update `wariowareinc.ld` for every TU conversion
4. Every batch must pass clean Docker build with `wariowareinc.gba: OK`
5. Every batch must rerun `make report`
6. Commit and push immediately on verified progress
7. Documentation is mandatory for every measurable improvement

## Docker Build Commands
```bash
# Clean Docker build
docker run --rm -v "$PWD:/workspace" -w /workspace devkitpro/devkitarm:latest \
  bash -lc 'set -euo pipefail; make clean >/dev/null 2>&1; make -j4 2>&1 | tail -n 3'

# Refresh report
docker run --rm -v "$PWD:/workspace" -w /workspace devkitpro/devkitarm:latest \
  bash -lc 'set -euo pipefail; make report 2>&1 | tail -n 3'

# Unit coverage
python3 tools/gen_objdiff.py
```

## Proven Pattern Families (reuse aggressively)
- empty BX LR stubs (all 58 converted!)
- simple tail-call / void call wrappers
- `gCurrentSceneVariable` setters and pointer-deref helpers
- `scene_set_current_thread(1)` + byte/word store wrappers
- `D_03006520` compare-and-call guards (single and two-call variants)
- raw-pointer struct-entry setters
- pair-add / arithmetic helpers
- tiny sound wrappers with absolute-address casts
- bitfield extract (shift-pair, NOT AND), bit-clear, zero-init
- dec-counter, store-advance, mul-acc small-body functions
- sprite_id_delete(gSpriteHandler, *(u32*)(gCSV + offset)) siblings (ADDS for offset>124, LDR[N] for small)
- two-pointer calls: (*(u32*)(p+shift), *(u32/u16*)(p+shift+N)) — both large/small offset forms
- gCSV + shift offset + ASRS/>> shift for signed values
- `u8 *p = (u8*)gCSV + N` + void-arg call + store at p[M]

## Known Traps
- No AND masks for bitfield extraction (use shift-pair)
- No `~N` as `MOVS+RSBS` (compiles as immediate)
- No `POP {R1}; BX R1` wrappers
- No multi-function C files
- No bare extern for g-symbols
- Large byte offsets may need pointer shaping for Thumb immediate splitting
- `((u32*)&gGlobal)[N]` generates `gGlobal+N*4` in literal pool (wrong!) — use local ptr var instead
- C89: no declarations after statements in same block level
- **NEW: `byte &= ~N` optimization trap** — when `(~N & 0xFF)` fits in u8, agbcc uses `MOVS #(~N&0xFF); ANDS` not `MOVS #N; RSBS; ANDS`. Affects ~1→0xFE, ~2→0xFD, ~0x3C→0xC3, etc.
- **NEW: BL+STRH POP register** — after BL+store pattern, agbcc may pick POP {R0}/BX R0 but original has POP {R1}/BX R1. Check register liveness carefully.

## Next Candidate Queue
1. More conditional check patterns (byte compare + BL call)
2. More shift-offset + store wrappers
3. More sprite_id_delete siblings from the remaining list
4. Two-pointer call variants with different shift patterns
5. Functions with ADDS R0, R1, R2 three-reg form + multiple BL calls
6. Functions with sct(1) + shift-store (proven family)
7. Search for MOVS R0/#const + BL wrappers (const-arg call patterns)