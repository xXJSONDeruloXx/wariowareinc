# WarioWare Inc. Decomp Scale-Up

This is the live operational status file for autonomous work in this repo.
Prefer this file + the other docs in `/docs` over `.ralph/`.

## Current verified baseline
- Verified on branch: `docs/macabeus-tooling-assessment`
- Verified working tree: `batch 51` candidate — `sprite_id_delete` byte-offset siblings (shift-1, direct, dual delete)
- `matched_functions`: **1314 / 5957** = **22.058083%**
- `matched_code_percent`: **6.352630%**
- `tools/gen_objdiff.py`: **862 C / 5825 asm-only units**
- ROM status: **`wariowareinc.gba: OK`**

## Goal
Reach at least **80% matched-function progress** while preserving byte-identical ROM output at every accepted milestone.

At the current `total_functions` count (`5957`), that means:
- target: **4766 / 5957** matched functions
- current gap: **3460** more matched functions

## What just landed
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
| 51 | (current) | +4 | `sprite_id_delete` shift-1, direct, dual delete siblings |
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
3. More `sprite_id_delete(gSpriteHandler, *(u32*)(gCSV + offset))` siblings
4. Two-pointer call variants with alternate shift patterns
5. Functions that rely on `ADDS R0, R1, R2` three-register forms plus multiple BL calls
6. More `scene_set_current_thread(1)` + shift-store families
7. More `MOVS R0, #const` + BL wrapper families

## Highest-value reminders before selecting a batch
- `#include "types.h"` when touching g-symbols from `types.h`.
- `#include "scenes.h"` when touching `gCurrentSceneData`.
- `gCurrentSceneVariable` is a struct pointer: use `(u8 *)gCurrentSceneVariable + off` for byte offsets unless you intentionally want scaled indexing like `((u32 *)gCurrentSceneVariable)[N]`.
- Use local pointer shaping when literal-pool or offset form matters.
- Re-check any wrapper that returns with `POP {R1}; BX R1` — this is a frequent trap.
- Re-check any byte mask using `~N` — agbcc often collapses it to an 8-bit immediate and breaks the match.
- Keep C89 declaration ordering clean.

## What success looks like for the next autonomous pass
A good pass should:
1. choose a narrow candidate batch from the queue above,
2. verify with Docker,
3. binary-search immediately if mismatched,
4. update docs with any durable learning,
5. commit + push immediately if metrics improve.

If the pass cannot land code safely, it should still improve the docs: tighten the queue, record the failed pattern precisely, and leave the repo in a better state for the next `continue`.