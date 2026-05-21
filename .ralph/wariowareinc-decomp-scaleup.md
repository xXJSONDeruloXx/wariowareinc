# WarioWare Inc. Decompilation Scale-Up (RESUMED from iteration 22)

## Goal
Reach **80% matched/decompiled-function progress** on the `docs/macabeus-tooling-assessment` branch, while preserving a **byte-identical ROM match** at every accepted milestone.
Target: **4768 / 5961 matched functions**

## Latest Verified Baseline (confirmed 2026-05-21, iterations 23-28)
- **Matched functions:** 1149 / 5961 = 19.275464%
- **Matched code percent:** 6.034865%
- **C units in linker graph:** 697 / 6687
- **ASM-only units in linker graph:** 5990
- **ROM status:** `wariowareinc.gba: OK`
- **Gap to target:** 1149 → 4768 = need +3619 more matched functions

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
- [ ] Select next 5-function candidate batch from proven sibling families
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
- empty BX LR stubs
- simple tail-call / void call wrappers
- `gCurrentSceneVariable` setters and pointer-deref helpers
- `scene_set_current_thread(1)` + byte/word store wrappers
- `D_03006520` compare-and-call guards (single and two-call variants)
- raw-pointer struct-entry setters
- pair-add / arithmetic helpers
- tiny sound wrappers with absolute-address casts
- bitfield extract (shift-pair, NOT AND), bit-clear, zero-init
- dec-counter, store-advance, mul-acc small-body functions

## Known Traps
- No AND masks for bitfield extraction (use shift-pair)
- No `~N` as `MOVS+RSBS` (compiles as immediate)
- No `POP {R1}; BX R1` wrappers
- No multi-function C files
- No bare extern for g-symbols
- Large byte offsets may need pointer shaping for Thumb immediate splitting

## Next Candidate Queue
1. More complex `D_03006520` wrappers that load gCurrentSceneVariable before BL call
2. Continue mining sibling-rich `gCurrentSceneVariable` families
3. BX LR empty stubs and return-constant functions as filler
4. Small-body functions (3-7 instructions, 0-1 branches, 0-2 BL calls)
5. `LDR global; LDR/LDRB/LDRH` getter chains