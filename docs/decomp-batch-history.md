# Accepted batch history

This is the migrated history from the Ralph task file plus the most recent session log work.
It is intentionally concise: keep the durable rules in `docs/decomp-pattern-library.md`, and use this file to remember what landed, when, and why it mattered.

## Latest accepted batches
| Iteration / Batch | Commit | Δ matched | Summary |
|---|---|---:|---|
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