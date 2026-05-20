# WarioWare Inc. Decompilation Scale-Up (RESUMED from iteration 22)

## Goal
Reach **80% matched/decompiled-function progress** on the `docs/macabeus-tooling-assessment` branch, while preserving a **byte-identical ROM match** at every accepted milestone.
Target: **4768 / 5961 matched functions**

## Latest Verified Baseline (confirmed 2026-05-21)
- **Matched functions:** 1121 / 5961 = 18.805569%
- **Matched code percent:** 5.986366%
- **C units in linker graph:** 669 / 6687
- **ASM-only units in linker graph:** 6018
- **ROM status:** `wariowareinc.gba: OK`
- **Gap to target:** 1126 → 4768 = need +3642 more matched functions

## Iteration 23 — accepted
- **Candidate set:** 5-function batch — `asm_0801e914` (BX LR stub), `asm_08005570` (D_03003FE8 byte setter), `asm_080656e4` (gCurrentSceneVariable decrement), `asm_080da190` (pair-add), `asm_080e5514` (pair-add sibling)
- **Result:** match (after fixing `asm_08005570` — `u8` param caused unwanted truncation; switched to `u32`)
- **Metric delta:** matched_functions 1121→1126 (+5), matched_code_percent 5.986366%→5.993805%, C units 669→674, asm-only 6018→6013
- **Commit:** `234220c1`, pushed
- **Learnings:** `u8` function params cause agbcc to emit `lsls/lsrs` truncation before `STRB`; use `u32` arg type when the original asm just does `STRB R0` without truncating

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