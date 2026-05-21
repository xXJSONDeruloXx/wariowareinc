# WarioWare Inc. Decomp Scale-Up

This is the live operational status file for autonomous work in this repo.
Prefer this file + the other docs in `/docs`

## Current verified baseline
- Verified on branch: `docs/macabeus-tooling-assessment`
- Verified working tree: `batch 69` — `func_0801667C` IWRAM byte load
- `matched_functions`: **1342 / 5954** = **22.546%**
- `matched_code_percent`: **6.4228%**
- `tools/gen_objdiff.py`: **888 C / 5801 asm-only units**
- ROM status: **`wariowareinc.gba: OK`**

## Goal
Reach at least **80% matched-function progress** while preserving byte-identical ROM output at every accepted milestone.

At the current `total_functions` count (`5956`), that means:
- target: **4765 / 5956** matched functions
- current gap: **3432** more matched functions

## What just landed
### Batch 69 — accepted
- Metric delta: **1341 → 1342 matched functions** (+1)
- Matched code: **6.4216% → 6.4228%**
- Commit: pending
- Accepted functions:
  - `asm_0801667C` — IWRAM byte load: returns byte at offset 6 of D_03006518 (struct+offset trick, s32 return)
- Notes: Sibling of func_08016670 (byte store at offset 5). Same struct+offset trick works for load variant.

### Batch 68 — accepted
- Metric delta: **1340 → 1341 matched functions** (+1)
- Matched code: **6.4204% → 6.4216%**
- Commit: pending
- Accepted functions:
  - `asm_08016670` — IWRAM byte store: writes arg0 to offset 5 of D_03006518 (s32 param to avoid u8 masking)
- Notes: Required struct+offset trick to prevent compiler from folding address+5 into literal pool; s32 param avoids u8 narrowing mask

### Batch 67 — accepted
- Metric delta: **1339 → 1340 matched functions** (+1)
- Matched code: **6.4190% → 6.4204%**
- Commit: pending
- Accepted functions:
  - `asm_08004A74` — zero-arg wrapper calling func_08004A84(arg0, arg1, 0, 0) (non-void return shape)
- Notes: Last of the zero-arg wrapper family (func_08004994, func_080049BC, func_08004A30, func_08004A74)

### Batch 66 — accepted
- Metric delta: **1338 → 1339 matched functions** (+1)
- Matched code: **6.4176% → 6.4190%**
- Commit: pending
- Accepted functions:
  - `asm_08004A30` — zero-arg wrapper calling func_08004A40(arg0, arg1, 0, 0) (non-void return shape)

### Batch 65 — accepted
- Metric delta: **1337 → 1338 matched functions** (+1)
- Matched code: **6.4162% → 6.4176%**
- Commit: pending
- Accepted functions:
  - `asm_080049BC` — zero-arg wrapper calling func_080049CC(arg0, arg1, 0, 0) (non-void return shape)

### Batch 64 — accepted
- Metric delta: **1336 → 1337 matched functions** (+1)
- Matched code: **6.4148% → 6.4162%**
- Commit: pending
- Accepted functions:
  - `asm_08004994` — zero-arg wrapper calling func_080049A4(arg0, arg1, 0, 0) with POP {R1};BX R1 epilogue (non-void return shape)

### Batch 63 — accepted
- Metric delta: **1335 → 1336 matched functions** (+1)
- Matched code: **6.4136% → 6.4148%**
- Commit: pending
- Accepted functions:
  - `asm_08003a00` — absolute value helper (CMP+BGE+NEGS pattern)

### Batch 62 — accepted
- Metric delta: **1334 → 1335 matched functions** (+1)
- Matched code: **6.4118% → 6.4136%**
- Commit: pending
- Accepted functions:
  - `asm_08002068` — conditional sound call wrapper (LSLS+LSRS+BL pattern)

### Batch 61 — accepted
- Metric delta: **1333 → 1334 matched functions** (+1)
- Matched code: **6.4098% → 6.4118%**
- Commit: pending
- Accepted functions:
  - `asm_080029d0` — byte `&= ~3` + halfword `&= 3` mask pair (register-pinned RSBS pattern)

### Batch 60 — accepted
- Metric delta: **1332 → 1333 matched functions** (**+1**)
- Matched code: **6.406549% → 6.4097586%**
- Commit: pending
- Accepted functions:
  - `asm_08006e94` — `gGraphicsBuffer.unk854_1 = arg0` bitfield wrapper

### Batch 58 — accepted
- Metric delta: **1331 → 1332 matched functions** (**+1**)
- Matched code: **6.4053407% → 6.406549%**
- Commit: `f36b7bd6`
- Accepted functions:
  - `asm_0800c610` — pointer-deref halfword store: `*(short *)((int *)a0[3]) = -1;`

### Batch 57 — accepted
- Metric delta: **1320 → 1324 matched functions** (**+4**)
- Matched code: **6.3896456% → 6.3964925%**
- Commit: `4c3f3d3d`
- Accepted functions:
  - `asm_0800ccb4` — `gBeatscriptScene` byte[2] RSBS-mask-clear (mask=2) via local pointer pattern
  - `asm_0801b194` — `gCurrentSceneVariable` deref byte[0x19] RSBS-mask-clear (mask=3)
  - `asm_08035194` — `a1[(s16)a0 + 0x80] = 1` via `a0=(u32)(s16)a0; a1+=0x80; a1+=a0` pattern
  - `asm_080351a4` — sibling, stores 3 instead of 1

### Batch 55 — exploration (no match)
- Exploration result: **BLOCKED** on loop-based patterns
- Commit: `c5cbc506` (docs update only)
- Attempted functions that failed:
  - `asm_0805d394` — loop with 3 iterations + 1 delete (loop variable ordering mismatch)
  - `asm_0806843c` — loop with 4 iterations + 1 byte-call + 1 delete (register allocation differs)
  - `asm_08016d3c` — loop with 2 iterations + 4 calls per iteration (loop unroll vs roll mismatch)
- Durable takeaways:
  - Loop-based sprite_id_delete functions with BLS/CMP patterns don't match simple C for-loops
  - Even semantically identical loops fail due to agbcc's register allocation and loop unrolling decisions
  - Remaining 6 unconverted sprite_id_delete functions are all loop-based; deprioritize this family
  - Focus shifted to conditional byte-check wrappers, shift-offset patterns, MOVS constant wrappers

### Batch 54 — accepted
- Metric delta: **1319 → 1320 matched functions** (**+1**)
- Matched code: **6.3811874% → 6.3896456%**
- Commit: `adc5930c`
- Accepted functions:
  - `asm_0804bc4c` — gGraphicsBuffer DISPCNT AND mask + 4 halfword clears, then sprite_id_delete at gCurrentSceneVariable+0xE4, then func_08001B28 sign-ext at gCurrentSceneVariable+0xCA

### Batch 53 — accepted
- Metric delta: **1318 → 1319 matched functions** (**+1**)
- Matched code: **6.3711314% → 6.3811874%**
- Commit: `80ba7f84`
- Accepted functions:
  - `asm_08067080` — conditional check on gCurrentSceneVariable + 0xE0, then four `sprite_id_delete` calls at (0xC4<<4), 0xC4C, 0xC48, 0xC44

### Batch 52 — accepted
- Metric delta: **1314 → 1318 matched functions** (**+4**)
- Matched code: **6.352630% → 6.3711314%**
- Commit: `12cf970c`
- Accepted functions:
  - `asm_080ba9d4` — `sprite_id_delete(gSpriteHandler, *(u32*)((u8*)gCurrentSceneVariable + (0x90 << 2)))`
  - `asm_0804c388` — dual delete at `(0xB0 << 1)` and `(0xB2 << 1)`
  - `asm_08056788` — dual delete at direct offsets `0xF4` and `0xF8`
  - `asm_0803e96c` — `func_08001B28` sign-ext byte at `+0xE4`, then `sprite_id_delete` at `+0xE0`, then `func_0800CDB0(1)`

### Batch 51 — accepted
- Metric delta: **1310 → 1314 matched functions** (**+4**)
- Matched code: **6.337337% → 6.352630%**
- Accepted functions:
  - `asm_08077174` — `sprite_id_delete(gSpriteHandler, *(u32*)((u8*)gCSV + (0xE6 << 1)))`
  - `asm_080b2bac` — `sprite_id_delete(gSpriteHandler, *(u32*)((u8*)gCSV + (0xB2 << 1)))`
  - `asm_080c9050` — `sprite_id_delete(gSpriteHandler, *(u32*)((u8*)gCSV + 0x574))`
  - `asm_0805ab2c` — two `sprite_id_delete` calls at byte offsets 0x94 and 0x98

### Batch 50 — accepted
- Metric delta: **1306 → 1310 matched functions** (**+4**)
- Matched code: **6.3184834% → 6.337337%**
- Commit: `3dacfb4c`
- Accepted functions:
  - `asm_08097fcc` — `sprite_id_delete(gSpriteHandler, *(u32*)((u8*)gCSV + 0x714))`
  - `asm_08016fb0` — `sprite_id_delete(gSpriteHandler, 1)` + `func_08001B70(1)`
  - `asm_0805f438` — `func_08001B28(*(s8*)(gCSV+0x46))` + `sprite_id_delete` at `gCSV + (0xAA<<2)`
  - `asm_080b0e80` — two `sprite_id_delete` + `gGraphicsBuffer.unk4C/0x4E` clear + `func_0800CDB0(1)`

### Batch 49 — accepted
- Metric delta: **1297 → 1306 matched functions** (**+9**)
- Matched code: **6.285469% → 6.3184834%**
- Accepted functions:
  - `asm_080749c4` — `sprite_id_delete` at `gCurrentSceneVariable + (0xE8 << 3)`
  - `asm_0807f078` — `sprite_id_delete` at `gCurrentSceneVariable + 0x444`
  - `asm_080840b4` — `sprite_id_delete` at `gCurrentSceneVariable + (0x89 << 3)`
  - `asm_08088564` — `sprite_id_delete` at `gCurrentSceneVariable + (0xE2 << 1)`
  - `asm_080a4424` — `sprite_id_delete` at `gCurrentSceneVariable + (0xCC << 4)`
  - `asm_080c4754` — `func_0800CDB0(1)` + `sprite_id_delete` at `gCurrentSceneVariable + (0x94 << 1)`
  - `asm_080d9b0c` — `sprite_id_delete` at `gCurrentSceneVariable + (0xC2 << 1)`
  - `asm_080e0fa8` — `sprite_id_delete` at `gCurrentSceneVariable + (0x92 << 1)`
  - `asm_080e4e0c` — `sprite_id_delete` at `gCurrentSceneVariable + (0xC4 << 1)`

### Recent momentum
| Batch | Commit | Δ matched | Main theme |
|---|---|---:|---|
| 62 | pending | +1 | `func_08002068` conditional sound call wrapper |
| 61 | pending | +1 | `func_080029D0` byte/halfword mask pair (RSBS register pin) |
| 60 | pending | +1 | `gGraphicsBuffer.unk854_1 = arg0` bitfield wrapper |
| 58 | `f36b7bd6` | +1 | pointer-deref halfword store (-1 wrapper) |
| 57 | `b3752d25` | +7 | Small wrapper sweep (SVC, div, HW reg, struct init) |
| 56 | `4c3f3d3d` | +4 | RSBS mask-clear + s16-indexed byte-store siblings |
| 55 | `c5cbc506` | +0 (explor.) | loop-based sprite_id_delete variant exploration blocked |
| 54 | `adc5930c` | +1 | gGraphicsBuffer clears + 2-call wrapper |
| 53 | `80ba7f84` | +1 | conditional 4-delete sprite_id_delete wrapper |
| 52 | `12cf970c` | +4 | `sprite_id_delete` single shift-2, dual shift-1, dual direct, sign-ext+delete+CDB0 |
| 51 | `c6a88977` | +4 | `sprite_id_delete` shift-1, direct, dual delete siblings |
| 50 | `3dacfb4c` | +4 | `sprite_id_delete` const-arg, sign-ext+delete, 2-delete+gGB clear |
| 49 | `d98c2b49` | +9 | `sprite_id_delete` byte-offset siblings |
| 48 | `0f121592` | +7 | pair-add, wrappers, gGraphicsBuffer, gCurrentSceneData |
| 47 | `49223873` | +4 | gCSV byte-- siblings, s8 sign-ext BL, multi-store reload |
| 46 | `10050330` | +7 | struct init, 4-call wrapper, gCSV byte/word ops, gCurrentSceneData |
| 45 | `f92ed4ac` | +8 | gGraphicsBuffer clears, BG_OFS setters, sound wrappers |
| 44 | `e0c0f888` | +7 | sprite helpers, s8 sign-ext, gCSV store families |

## Current proven strategy
- Mine sibling-rich families first.
- Keep batches small enough to binary-search quickly.
- Favor patterns already documented in `docs/decomp-pattern-library.md`.
- Treat docs updates as part of the accepted work, not optional follow-up.

## Active next candidate queue
1. More conditional byte-check + BL wrappers
2. More shift-offset + store wrappers
3. More `MOVS R0, #const` + BL wrapper families
4. Two-pointer call variants with alternate shift patterns
5. Functions that rely on `ADDS R0, R1, R2` three-register forms plus multiple BL calls
6. More `scene_set_current_thread(1)` + shift-store families
7. ~~More `sprite_id_delete(gSpriteHandler, *(u32*)(gCSV + offset))` siblings~~ — BLOCKED: remaining loop-based variants (asm_08016d3c, asm_0806843c, asm_0806fe20, asm_0805d394, asm_0805c550, asm_0806b99c) fail to match due to loop iteration register patterns not aligning with C for-loop code generation

## Highest-value reminders before selecting a batch
- `#include "types.h"` when touching g-symbols from `types.h`.
- `#include "scenes.h"` when touching `gCurrentSceneData`.
- `gCurrentSceneVariable` is a struct pointer: use `(u8 *)gCurrentSceneVariable + off` for byte offsets unless you intentionally want scaled indexing like `((u32 *)gCurrentSceneVariable)[N]`.
- Use local pointer shaping when literal-pool or offset form matters.
- Re-check any wrapper that returns with `POP {R1}; BX R1` — this is a frequent trap.
- Re-check any byte mask using `~N` — agbcc often collapses it to an 8-bit immediate and breaks the match.
- If a candidate is a mid-file asm include inside a larger C TU, expect standalone-TU conversion to perturb ROM order unless you split the host TU first.
- Keep C89 declaration ordering clean.

## What success looks like for the next autonomous pass
A good pass should:
1. choose a narrow candidate batch from the queue above,
2. verify with Docker,
3. binary-search immediately if mismatched,
4. update docs with any durable learning,
5. commit + push immediately if metrics improve.

If the pass cannot land code safely, it should still improve the docs: tighten the queue, record the failed pattern precisely, and leave the repo in a better state for the next `continue`.