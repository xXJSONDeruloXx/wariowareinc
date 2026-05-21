# Accepted batch history

This is the migrated history from the Ralph task file plus the most recent session log work.
It is intentionally concise: keep the durable rules in `docs/decomp-pattern-library.md`, and use this file to remember what landed, when, and why it mattered.

## Repo-state cleanup note
- No new accepted batch landed in the repo-state cleanup pass.
- Placeholder / unverified WIP decomp stubs were reverted instead of being kept behind build exclusions.
- `src/decomp/` should only contain real build-participating TUs; keep rough experiments out of the build until they are ready.
- Converted asm is being normalized under `asm/converted/`.
- Latest accepted progress still ends at batch 50.

## Latest accepted batches
| Iteration / Batch | Commit | Δ matched | Summary |
|---|---|---:|---|
| 50 | `pending commit` | +4 | continued `sprite_id_delete` byte-offset siblings, plus helper functions |
| 49 | `d98c2b49` | +9 | `sprite_id_delete` byte-offset siblings, plus one `func_0800CDB0(1)` + delete wrapper |
| 48 | `0f121592` | +7 | pair-add, field++/2-BL, gGraphicsBuffer store pair, gCurrentSceneData add, 3-BL return, 2-BL call |
| 47 | `49223873` | +4 | gCSV byte-- siblings, BL+s8 sign-ext+BL, multi-store-with-reload |
| 46 | `10050330` | +7 | 5-arg struct init, 4-call wrapper, gCSV word++, gCurrentSceneData LDRH families |
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

## Iteration 50 details
- Result: match ✅ after binary search and pattern adaptation
- Report: **1310 / 5957**, **21.999498%**
- Accepted functions:
  - `asm_080c9050` — `sprite_id_delete` at `gCurrentSceneVariable + 0x574`
  - `asm_08016fb0` — `sprite_id_delete` and `func_08001B70` wrapper
  - `asm_08016d3c` — sprite deletion and memory cleanup loop
  - `asm_08056788` — `sprite_id_delete` at byte offsets `0xF4` and `0xF8`
  - `asm_0805ab2c` — `sprite_id_delete` at byte offsets `0x94` and `0x98`
- Durable takeaways:
  - `sprite_id_delete` byte-offset siblings remain productive
  - Simple wrappers with single BL calls are fast to convert
  - Loop-based cleanup functions can be translated with minimal C
  - The `gCurrentSceneVariable` pointer cast to `(u8 *)` is crucial for byte-offset access

## Iteration 49 details
- Result: match ✅ after immediate binary-search catch on a shaping bug
- Report: **1306 / 5957**, **21.923786%**
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

## Iteration 47-50 cluster
These recent batches have focused on `sprite_id_delete` helper functions, with 49 being a pure byte-offset sibling family and 50 expanding to more complex but still simple wrappers. The strategy of targeting small, well-documented families continues to yield steady progress.

## What this history says about the repo
- Reuse beats novelty.
- The best batches come from already-proven families, not isolated one-offs.
- A partial batch is acceptable if the binary search is done immediately and the trap is recorded.
- Doc quality directly affects autonomous throughput: every repeated mistake has historically come from a missing or stale rule.