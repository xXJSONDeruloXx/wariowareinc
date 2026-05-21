# Decomp pattern library

## Proven high-yield families
### Easy filler / utility
- standalone `BX LR` leaves
- simple tail-call wrappers
- simple void-call wrappers
- `MOVS R0, #const; BX LR` return-constant helpers

### gCurrentSceneVariable / gCSV families
- direct byte/word setters
- pointer-deref helpers
- shift-based byte setters
- `((u8*)gCurrentSceneVariable)[N]++` / `--` siblings
- shift-computed offset loads/stores like `(0xBD << 4)`
- signed-load helpers using explicit `(s8)` or `(s16)` shaping
- multi-store patterns that need a local pointer reload shape

### Wrapper families
- `scene_set_current_thread(1)` + byte/word store wrappers
- two-call and four-call R4-save wrappers
- wrappers that preserve `arg0` in `R4` across multiple BLs
- const-arg BL wrappers once literal-pool behavior is known-good
- two-pointer wrappers passing `p + off1`, `p + off2`

### Data-structure / arithmetic families
- raw-pointer struct-entry setters
- pair-add / arithmetic helpers
- small struct-init / zero-init functions
- gGraphicsBuffer small store pairs / clears
- `gCurrentSceneData` halfword add / shift / store helpers
- `sprite_id_delete(gSpriteHandler, *(u32*)(gCSV + offset))` siblings

## Code-shaping rules that have proven important
- `#include "global.h"` in every decomp TU
- add `#include "types.h"` when touching g-symbols declared there
- add `#include "scenes.h"` when touching `gCurrentSceneData`
- use a local pointer variable when `((u32*)&gGlobal)[N]` would otherwise collapse into the wrong literal-pool expression
- for post-BL stores, declare locals before statements and assign after the BL if needed to preserve C89 compliance
- when the original reuses the same base load for multiple stores, mirror that with a local pointer for the first stores and only fall back to the raw global on the final store if needed

## Known traps
### Register / return-shape traps
- `POP {R1}; BX R1` wrappers often do **not** match normal agbcc wrapper output
- BL + STRH patterns may keep the wrong register live and produce `POP {R0}; BX R0` instead of `POP {R1}; BX R1`
- semantically identical C can still miss due to register allocation differences

### Bitfield / mask traps
- do not replace bitfield extraction with AND masks when the original is a shift-pair
- `byte &= ~N` is dangerous when `(~N & 0xFF)` fits in 8 bits; agbcc tends to emit `MOVS #imm8; ANDS` instead of `MOVS #N; RSBS; ANDS`
- `-1` / other negative immediates can pick `NEGS`, `RSBS`, or literal-pool forms differently than expected
- some OR/bit-clear forms need a very specific spelling to avoid extra `ADDS`

### Addressing / literal-pool traps
- D_ absolute-address access is known-good mainly at offset `0`; non-zero offsets often drift
- large byte offsets may need pointer shaping so Thumb uses the same split (`+8` then `[+0x1E]`, etc.)
- `((u32*)&gGlobal)[N]` may compile to a symbol+offset literal relocation instead of base-load + offset addressing
- raw relocatable object bytes can mislead; final linked ROM is the real gate

### Repo integration traps
- grouping multiple functions in one C file breaks matches
- forgetting to move the old asm file causes duplicate / wildcard collisions
- forgetting the linker-script swap breaks standalone TU conversions
- bare `extern` declarations for g-symbols can fight existing repo types
- C89 declaration ordering still matters in this repo

## Specific accepted shaping examples
- gBeatscriptScene second-word access: use `u32 *p = (u32 *)&gBeatscriptScene; p[1]`
- byte decrement siblings: `((u8*)gCurrentSceneVariable)[0xBBA]--`
- multi-store-with-reload pattern:
  - `u8 *p = (u8*)gCurrentSceneVariable;`
  - `*(u16*)(p + X) = 0;`
  - `p[Y] = 1;`
  - `*(u16*)((u8*)gCurrentSceneVariable + Z) = arg0;`
- `gCurrentSceneData` helper pattern:
  - `*(u32*)((u8*)arg0 + off) += *(u16*)((u8*)gCurrentSceneData + 0x16);`

## Families still worth mining heavily
- conditional byte-check + BL wrappers
- shift-offset store wrappers
- remaining `sprite_id_delete` siblings
- `scene_set_current_thread(1)` sibling families
- multi-BL wrappers whose good spellings are now documented