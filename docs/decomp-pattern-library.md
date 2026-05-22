# Decomp pattern library

## Build / verification rule (critical)
- Always verify with Docker before committing: `docker run --rm -v $(pwd):/workspace devkitpro/devkitarm:latest /bin/bash -c "cd /workspace && make -j4"`
- The local `tools/agbcc/bin/agbcc` is a Linux aarch64 binary and will not run on macOS; Docker is the only valid local build path
- A chunk is not done until `wariowareinc.gba: OK` is confirmed in the Docker build output
- `make report` must be run (via Docker) after the ROM build before completing the chunk — this regenerates `build/report.json` which is needed for accurate metric tracking
- **agbcc requires `-mthumb-interwork`** to generate interwork-safe epilogues (`POP {R0}; BX R0` or `POP {R1}; BX R1`). Without it, agbcc generates `POP {PC}` which is NOT interwork-safe and does NOT match the original. The Makefile passes `-mthumb-interwork` via `CFLAGS`. When doing isolated agbcc testing, always include this flag.

## Proven high-yield families
### Easy filler / utility
- standalone `BX LR` leaves (simple `void func(void) {}` matches in final linked ROM despite object-level NOP padding diff 0x0000 vs 0xC046)
- simple tail-call wrappers
- 3-word struct stores: `a0[0] = a1; a0[1] = a2; a0[2] = a3;` — agbcc generates separate STR instructions (matching the original), while devkitARM gcc uses STMIA. **Must verify with the agbcc toolchain or the full Docker build, not devkitARM gcc alone**
- simple void-call wrappers
- `MOVS R0, #const; BX LR` return-constant helpers
- zero-arg wrappers: `func(arg0, arg1, 0, 0)` with non-void return type to get `POP {R1}; BX R1` epilogue instead of `POP {R0}; BX R0`
- pointer-deref halfword store: `*(short *)((int *)a0[3]) = -1;` (generates LDR R1,[R0,#0xC]; MOVS R2,#1; RSBS R2,R2,#0; ADDS R0,R2,#0; STRH R0,[R1]; BX LR)

### gCurrentSceneVariable / gCSV families
- direct byte/word setters
- pointer-deref helpers
- shift-based byte setters
- `((u8*)gCurrentSceneVariable)[N]++` / `--` siblings
- shift-computed offset loads/stores like `(0xBD << 4)`
- `sprite_id_delete(gSpriteHandler, *(u32 *)((u8 *)gCurrentSceneVariable + off))` siblings
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
- `gGraphicsBuffer` 1-bit field writes like `gGraphicsBuffer.unk854_1 = arg0`
- `gCurrentSceneData` halfword add / shift / store helpers
- `sprite_id_delete(gSpriteHandler, *(u32*)(gCSV + offset))` siblings

## Code-shaping rules that have proven important
- `#include "global.h"` in every decomp TU
- add `#include "types.h"` when touching g-symbols declared there
- add `#include "scenes.h"` when touching `gCurrentSceneData`
- use a local pointer variable when `((u32*)&gGlobal)[N]` would otherwise collapse into the wrong literal-pool expression
- `gCurrentSceneVariable` is a `struct BeatscriptLocalData *`, so raw `gCurrentSceneVariable + N` scales by `sizeof(struct BeatscriptLocalData)`; use `(u8 *)gCurrentSceneVariable + off` for byte offsets, or an intentional typed index like `((u32 *)gCurrentSceneVariable)[N]`
- For RSBS-mask-clear pattern on a global struct: declare `u8 *p = (u8*)&gSymbol;` then use `p[offset]` to keep the global base in one register and the offset in the instruction; do NOT use `((u8*)&gSymbol)[offset]` directly (agbcc combines into a literal+relocation with zero offset).
- `s16-indexed byte-store` pattern: use `a0 = (u32)(s16)a0; a1 += 0x80; a1 += a0; *a1 = value;` to match LSLS/ASRS, ADDS R1 #imm, ADDS R1 R0 instruction order. Using `a1[(s16)a0 + 0x80] = value` or `*(a1 + 0x80 + (s16)a0) = value` generates ADD-to-R0 not ADD-to-R1.
- for post-BL stores, declare locals before statements and assign after the BL if needed to preserve C89 compliance
- when the original reuses the same base load for multiple stores, mirror that with a local pointer for the first stores and only fall back to the raw global on the final store if needed
- For GBA BIOS SVC wrappers: use `s32 result = a0; asm volatile("svc #6" : "+r"(result) : "r"(a1)); return result;` to keep R0=R0 and R1=a1 for the SWI call
- For hardware register writes: use `*(volatile u16 *)0x4XXXXXXX = val;` to match literal-pool address generation and store
- For IWRAM absolute-address byte store with non-zero offset: use a local struct typedef with fields laid out to put the target byte at the correct offset, then cast the address: `(*(StructType *)0x0300XXXX).field = val;`. Using `((u8 *)0x0300XXXX)[offset]` causes agbcc to fold the offset into the literal pool address, producing a different object. Also use `s32` parameter type instead of `u8` to avoid the compiler inserting `LSLS R0, #24; LSR R0, #24` u8 masking.
- For sequential byte writes: use `u8 *p = *a0; *p = byte0; p++; *p = byte1; p++; *a0 = p;` — pointer increment `p++` pattern generates `ADDS R2, #1` matching the original asm, while array indexing `p[0]`/`p[1]` uses different offset forms
- D_ symbols used in C files need entries in both `include/undefined_syms.inc` (for asm preprocessing) AND `undefined_syms.ld` (for linker resolution from C objects)

## Known traps
### Instruction ordering / code generation traps
- `a0 = (u32)(s16)a0` **before** the pointer constant-add forces the sign-extension instruction first (LSLS/ASRS before ADDS R1, #0x80). Writing `a1 += 0x80; a1 += (s16)a0` reverses the order even though it reads naturally in C.
- For global byte access via RSBS-mask-clear, use a local pointer `u8 *p = (u8*)&gSymbol` and then `p[offset]` to get `[R2, #offset]` addressing. Writing `((u8*)&gSymbol)[offset]` directly may produce a combined literal with the offset baked in, giving `[R2, #0]` instead.
- gGraphicsBuffer indexed halfword stores like `*(u16*)((u8*)&gGraphicsBuffer + 0x54 + (u16)a0 * 2)` can produce LDR-before-LSLS or LSLS-before-LDR depending on C form; the exact interleaved original asm pattern is hard to reproduce — leave as asm if the instruction ORDER differs.

### Header / include traps
- `gSpriteHandler` is declared in `src/lib_sprite.h` — include as `"src/lib_sprite.h"` (relative to repo root), not `"lib_sprite.h"`
- Never add a bare `extern void sprite_id_delete(u32, u32)` — it conflicts with the real `struct SpriteHandler *` signature; let lib_sprite.h provide it
- `gCurrentSceneVariable` is in `types.h`; `gGraphicsBuffer` is in `graphics.h`

### Register / return-shape traps
- `POP {R1}; BX R1` epilogue is generated by agbcc for **non-void** functions with `-mthumb-interwork` (the return value is in R0, so it uses R1 for the pop). **Void** functions with `-mthumb-interwork` generate `POP {R0}; BX R0`. Without `-mthumb-interwork`, both generate `POP {PC}`.
- BL + STRH patterns may keep the wrong register live and produce `POP {R0}; BX R0` instead of `POP {R1}; BX R1` — check if the function should be non-void
- semantically identical C can still miss due to register allocation differences
- Zero-init store order depends on C source statement order: `a0[1]=0; a0[0]=0; a0[2]=0;` generates `STR [R0,#4]; STR [R0]; STR [R0,#8]` (matching the original's non-sequential pattern), while `a0[0]=0; a0[1]=0; a0[2]=0;` generates sequential stores
- Included stubs in the same TU don't need extern declarations for functions already defined in the host C file

### Bitfield / mask traps
- do not replace bitfield extraction with AND masks when the original is a shift-pair
- `byte &= ~N` is dangerous when `(~N & 0xFF)` fits in 8 bits; agbcc tends to emit `MOVS #imm8; ANDS` instead of `MOVS #N; RSBS; ANDS`
- `-1` / other negative immediates can pick `NEGS`, `RSBS`, or literal-pool forms differently than expected
- some OR/bit-clear forms need a very specific spelling to avoid extra `ADDS`
- **register-pinning workaround**: when `&= ~3` must emit `MOVS R1,#3; RSBS R1,R1,#0; ANDS R1,R2` but agbcc prefers `MOVS R1,#0xFC; ANDS`, pin the mask variable to a register: `register u8 m asm("r1"); m = 3; m = -m; m = v & m;`

### Addressing / literal-pool traps
- D_ absolute-address access is known-good mainly at offset `0`; non-zero offsets often drift
- large byte offsets may need pointer shaping so Thumb uses the same split (`+8` then `[+0x1E]`, etc.)
- `((u32*)&gGlobal)[N]` may compile to a symbol+offset literal relocation instead of base-load + offset addressing
- raw relocatable object bytes can mislead; final linked ROM is the real gate

### Repo integration traps
- grouping multiple functions in one C file breaks matches
- forgetting to move the old asm file causes duplicate / wildcard collisions
- forgetting the linker-script swap breaks standalone TU conversions
- included asm stubs inside an existing C TU (e.g. a mid-file `#include` in `graphics_table.c`) may need the host TU split before a standalone conversion will preserve ROM order
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
- `sprite_id_delete` byte-offset helper pattern:
  - `sprite_id_delete(gSpriteHandler, *(u32 *)((u8 *)gCurrentSceneVariable + off));`
  - if the original has a pre-call like `func_0800CDB0(1)`, keep it before the delete and keep the byte-cast on the offset load

- bit-extract via LSLS+LSRS pair: when the original uses `LSLS R0, R0, #N; LSRS R0, R0, #N` to extract a single bit, C `& 1` or `(x << N) >> N` may generate AND or ASRS instead of LSRS. Use inline asm with `.syntax unified` to match exactly:
  ```c
  s32 result;
  asm volatile(
      ".syntax unified\n"
      "ldrb %0, [%1]\n"
      "lsls %0, %0, #31\n"
      "lsrs %0, %0, #31\n"
      ".syntax divided\n"
      : "=r"(result) : "r"(ptr)
  );
  return result;
  ```

### included_stub asm format traps
- included_stub asm files that use `.syntax unified` embedded in a C `asm()` string cannot be assembled by the `compile_and_view_asm` tool (it writes the raw string to a temp `.s` file which the assembler rejects)
- Workaround: manually assemble a standalone `.s` version of the target, compare with `objdump -d` and `objcopy -O binary` + `cmp`, then use `apply_conversion` directly after manual verification
- **include path depth**: for included_stub conversions where the host C file is in a subdirectory (e.g. `src/scenes/main_menu.c`), the include-shim path must use `../decomp/asm_XXXX.c` instead of `decomp/asm_XXXX.c`. The `apply_conversion` tool doesn't account for this and generates wrong include paths for subdirectory host files

### noreturn trap
- **Do NOT use `__attribute__((noreturn))`** to suppress compiler-generated epilogues — it changes caller codegen and causes cascading ROM mismatch. Even if the function body is pure inline asm with `bx lr`, use plain `void` return type
- For simple BX LR leaf functions, `void func(void) {}` produces `BX LR + NOP` which matches in the final linked ROM even when the original object has `BX LR + .short 0x0000` — the linker resolves the alignment padding correctly
- Object-level NOP differences (`0xC046` vs `0x0000`) do NOT cause ROM-level mismatches for simple leaf functions

## Families still worth mining heavily
- conditional byte-check + BL wrappers
- shift-offset store wrappers
- remaining `sprite_id_delete` siblings (NOTE: loop-based variants like asm_08016d3c, asm_0806843c, asm_0806fe20 failed to match; their loop iteration patterns and register allocation don't align with simple C for-loops even when semantically correct)
- `scene_set_current_thread(1)` sibling families
- more MOVS R0, #const + BL wrapper families
- multi-BL wrappers whose good spellings are now documented