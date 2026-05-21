# Accepted batch history

This is the migrated history from the Ralph task file plus the most recent session log work.
It is intentionally concise: keep the durable rules in `docs/decomp-pattern-library.md`, and use this file to remember what landed, when, and why it mattered.

## Latest accepted batches
| Iteration / Batch | Commit | Δ matched | Summary |
|---|---|---:|---|
| 48 | `0f121592` | +7 | pair-add, field++/2-BL, gGraphicsBuffer store pair, gCurrentSceneData add, 3-BL return, 2-BL call |
| 47 | `49223873` | +4 | gCSV byte-- siblings, BL+s8 sign-ext+BL, multi-store-with-reload |
| 46 | `10050330` | +7 | 5-arg struct init, 4-call R4 wrapper, gCSV word++, gCurrentSceneData LDRH families |
| 45 | `f92ed4ac` | +8 | gGraphicsBuffer clears, BG_OFS setters, soundplayer_pitch siblings, DE30 wrapper |
| 44 | `e0c0f888` | +7 | sprite_set_enable_updates, shift deref + 2 BL, s8 sign-ext, gCSV store families |
| 43 | `36eed474` | +8 | 8-sibling `func_08026264(...); gCSV[4] |= M` family |
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