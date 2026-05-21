# WarioWare Inc. Decomp Scale-Up

This is the live operational status file for autonomous work in this repo.
Prefer this file + the other docs in `/docs` over `.ralph/`.

## Current verified baseline
- Verified on branch: `docs/macabeus-tooling-assessment`
- Verified HEAD: `0f121592` — `feat: add batch 48 (pair-add, field++/2-BL, gGraphics stores, gCurrentSceneData, 3-BL return, 2-BL call)`
- `matched_functions`: **1297 / 5957** = **21.772705%**
- `matched_code_percent`: **6.285469%**
- `tools/gen_objdiff.py`: **845 C / 5842 asm-only units**
- ROM status: **`wariowareinc.gba: OK`**

## Goal
Reach at least **80% matched-function progress** while preserving byte-identical ROM output at every accepted milestone.

At the current `total_functions` count (`5957`), that means:
- target: **4766 / 5957** matched functions
- current gap: **3469** more matched functions

## What just landed
### Batch 48 — accepted
- Commit: `0f121592`
- Metric delta: **1290 → 1297 matched functions** (**+7**)
- Matched code: **6.2703905% → 6.285469%**
- Accepted functions:
  - `asm_0800cc9c` — 2-BL call using `func_0800A038()` result as arg0
  - `asm_0802e4ac` — field increment + 2 BL calls
  - `asm_08035fd8` — ordered halfword zero stores
  - `asm_08062dcc` — pair-add helper
  - `asm_080862dc` — `gGraphicsBuffer` store pair
  - `asm_080b7bb4` — 3-BL wrapper returning `u8`
  - `asm_080c9520` — `gCurrentSceneData` halfword >> 5 add

### Recent momentum
| Batch | Commit | Δ matched | Main theme |
|---|---|---:|---|
| 48 | `0f121592` | +7 | pair-add, wrappers, gGraphicsBuffer, gCurrentSceneData |
| 47 | `49223873` | +4 | gCSV byte-- siblings, s8 sign-ext BL, multi-store reload |
| 46 | `10050330` | +7 | struct init, 4-call wrapper, gCSV byte/word ops, gCurrentSceneData |
| 45 | `f92ed4ac` | +8 | gGraphicsBuffer clears, BG_OFS setters, sound wrappers |
| 44 | `e0c0f888` | +7 | sprite helpers, s8 sign-ext, gCSV store families |
| 43 | `36eed474` | +8 | 8-sibling `func_08026264(...); gCSV[4] |= M` family |

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