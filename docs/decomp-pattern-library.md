# Decomp pattern library

## Build / verification rule (critical)
- Always verify with Docker before committing: `docker run --rm -v $(pwd):/workspace devkitpro/devkitarm:latest /bin/bash -c "cd /workspace && make -j4"`
- The local `tools/agbcc/bin/agbcc` is not a reliable macOS host-native path; Docker is the only supported local verification path for this repo
- A chunk is not done until `wariowareinc.gba: OK` is confirmed in the Docker build output
- `make report` must be run (via Docker) after the ROM build before completing the chunk — this regenerates `build/report.json` which is needed for accurate matched-function / matched-code tracking
- Convenience commands: `/decomp-verify` runs clean Docker build + Docker report + objdiff refresh; `/decomp-report` runs Docker report + objdiff refresh
- `python3 tools/gen_objdiff.py` tracks **linked** unit coverage only; `included_stub` conversions do not increase linked C TU counts
- count `src/decomp/*.c` separately when you want total decompiled function-file coverage (`standalone_tu` + `included_stub`)
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
- conditional-call wrappers: `if (func_08011698()) callee()` — void function with CMP+BEQ+BL pattern
- const-arg sequential calls: `func_0800C7A4(8); func_0800C7A4(9)` — each call resets R0 before BL
- two sequential void calls: `callee1(); callee2();` with PUSH {LR}/POP {R0};BX R0
- gGraphicsBuffer.DISPCNT bit-OR-set: `gGraphicsBuffer.DISPCNT |= (0x80 << 1) << arg0` — generates LDR+MOVS+LSLS+LSLS+LDRH+ORRS+STRH

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
- **GraphicsTable pointer-advance loop**: forward-loop through `GraphicsTable` entries looking for `src == NULL` terminator, then tail-call. Use register-pinned pointer (`register char *r2 asm("r2")`) with goto labels to preserve `ADDS R2,#0xC` instruction order before the load/compare. The pattern is:
  ```c
  register char *r2 asm("r2") = arg0;
  goto start;
loop:
  r2 += 0xC;
start:
  if (*(void **)r2 != NULL)
      goto loop;
  func_0800247C(r2);
  ```
- `gCurrentSceneData` halfword add / shift / store helpers
- `sprite_id_delete(gSpriteHandler, *(u32*)(gCSV + offset))` siblings

## **NEVER decompile callers of already-converted C functions**
A 100% isolated compile match with `compile_and_view_asm` does **NOT** guarantee a final ROM match when the callee has already been converted to C. The isolated test compares your new C code against the **original asm callee**, but the linked ROM uses the **converted C callee** with potentially different register allocation for the call.

Example trap: `func_08002514` calls `func_080024D0`. Both were originally asm. `func_080024D0` was converted to C. Isolated testing of `func_08002514` used the original asm `func_080024D0`, producing a 100% match. But linking with the C `func_080024D0` changed the register allocation, causing ROM mismatch.

**Mitigation**: Use `decomp_siblings` with `strategy: callees` to check if any callees are already converted. If so, either:
1. Choose a different standalone function whose callees are all in asm
2. Or accept that the conversion requires manual verification beyond isolated testing

## Code-shaping rules that have proven important
- `#include "global.h"` in every decomp TU
- add `#include "types.h"` when touching g-symbols declared there
- add `#include "scenes.h"` when touching `gCurrentSceneData`
- use a local pointer variable when `((u32*)&gGlobal)[N]` would otherwise collapse into the wrong literal-pool expression
- `gCurrentSceneVariable` is a `struct BeatscriptLocalData *`, so raw `gCurrentSceneVariable + N` scales by `sizeof(struct BeatscriptLocalData)`; use `(u8 *)gCurrentSceneVariable + off` for byte offsets, or an intentional typed index like `((u32 *)gCurrentSceneVariable)[N]`
- **gCurrentSceneData shift-offset loads**: `ptr = (u8 *)gCurrentSceneData; return ptr[N << shift];` reproduces the LDR+MOVS+LSLS+ADDS+LDR[BH] sequence. The shift constant (e.g. `0xBA << 1` = 0x174) encodes as `MOVS R1, #0xBA; LSLS R1, R1, #1`. Use `u8 *` cast for byte loads, `*(u16 *)(ptr + offset)` for halfword loads.
- **Load-base-first trick**: When the original has LDR before LSLS (loading a global base, then shifting the index), assign the base to a local first: `u32 *base = D_0300400C; ptr = (u16 *)((u8 *)base + (arg0 << 2));`. Without the local, agbcc may emit LSLS before LDR.
- **Load-base-first for literal-pool offset stores**: When storing to a large offset from a global struct (e.g., `gBeatscriptScene.unk1C58` at offset 0x1C58), declare `u8 *base = (u8 *)&gSymbol;` first, then compute the destination: `u16 *dest = (u16 *)(base + 0x1C58);`. This matches the original's instruction sequence of `LDR R2,=gSymbol; LDR R3,=offset; ADDS R0,R2,R3; STRH R1,[R0]` rather than having the compiler fold the offset into a single literal-pool address.
- **Pointer-advance for zero-init/struct-init**: When the original does `ADDS R0, #0xC` then stores at [R0], [R0+4], [R0+8], reassign the pointer: `a0 = a0 + 3; a0[1]=0; a0[0]=0; a0[2]=0;`. Using absolute offsets `a0[3]=0; a0[4]=0; a0[5]=0;` generates offset-from-original instead of pointer advance.
- For RSBS-mask-clear pattern on a global struct: declare `u8 *p = (u8*)&gSymbol;` then use `p[offset]` to keep the global base in one register and the offset in the instruction; do NOT use `((u8*)&gSymbol)[offset]` directly (agbcc combines into a literal+relocation with zero offset).
- `s16-indexed byte-store` pattern: use `a0 = (u32)(s16)a0; a1 += 0x80; a1 += a0; *a1 = value;` to match LSLS/ASRS, ADDS R1 #imm, ADDS R1 R0 instruction order. Using `a1[(s16)a0 + 0x80] = value` or `*(a1 + 0x80 + (s16)a0) = value` generates ADD-to-R0 not ADD-to-R1.
- for post-BL stores, declare locals before statements and assign after the BL if needed to preserve C89 compliance
- when the original reuses the same base load for multiple stores, mirror that with a local pointer for the first stores and only fall back to the raw global on the final store if needed
- For GBA BIOS SVC wrappers: use `s32 result = a0; asm volatile("svc #6" : "+r"(result) : "r"(a1)); return result;` to keep R0=R0 and R1=a1 for the SWI call
- For hardware register writes: use `*(volatile u16 *)0x4XXXXXXX = val;` to match literal-pool address generation and store
- For IWRAM absolute-address byte store with non-zero offset: use a local struct typedef with fields laid out to put the target byte at the correct offset, then cast the address: `(*(StructType *)0x0300XXXX).field = val;`. Using `((u8 *)0x0300XXXX)[offset]` causes agbcc to fold the offset into the literal pool address, producing a different object. Also use `s32` parameter type instead of `u8` to avoid the compiler inserting `LSLS R0, #24; LSR R0, #24` u8 masking.
- For sequential byte writes: use `u8 *p = *a0; *p = byte0; p++; *p = byte1; p++; *a0 = p;` — pointer increment `p++` pattern generates `ADDS R2, #1` matching the original asm, while array indexing `p[0]`/`p[1]` uses different offset forms
- D_ symbols used in C files need entries in both `include/undefined_syms.inc` (for asm preprocessing) AND `undefined_syms.ld` (for linker resolution from C objects). When converting from asm to C, any D_ symbol referenced in the C code must be added to `undefined_syms.ld` or the linker will fail / produce a different ROM.

## Known traps
### Instruction ordering / code generation traps
- `a0 = (u32)(s16)a0` **before** the pointer constant-add forces the sign-extension instruction first (LSLS/ASRS before ADDS R1, #0x80). Writing `a1 += 0x80; a1 += (s16)a0` reverses the order even though it reads naturally in C.
- For global byte access via RSBS-mask-clear, use a local pointer `u8 *p = (u8*)&gSymbol` and then `p[offset]` to get `[R2, #offset]` addressing. Writing `((u8*)&gSymbol)[offset]` directly may produce a combined literal with the offset baked in, giving `[R2, #0]` instead.
- gGraphicsBuffer indexed halfword stores like `*(u16*)((u8*)&gGraphicsBuffer + 0x54 + (u16)a0 * 2)` can produce LDR-before-LSLS or LSLS-before-LDR depending on C form; the exact interleaved original asm pattern is hard to reproduce — leave as asm if the instruction ORDER differs.

### Header / include traps
- `gSpriteHandler` is declared in `src/lib_sprite.h` — include as `"src/lib_sprite.h"` (relative to repo root), not `"lib_sprite.h"`
- Never add a bare `extern void sprite_id_delete(u32, u32)` — it conflicts with the real `struct SpriteHandler *` signature; let lib_sprite.h provide it
- `gCurrentSceneVariable` is in `types.h`; `gGraphicsBuffer` is in `graphics.h`
- **Don't duplicate extern declarations** that already exist via transitively-included headers. E.g. `memory.h` includes `gameplay.h` which declares `func_08003FB8` and `func_08007EAC` — so `main_menu.c` already has them via `#include "src/memory.h"`. Adding your own extern in the decomp file causes conflicting types.
- **Included-stub extern type consistency**: When converting an included_stub function that uses symbols already declared in other included_stub files in the same host TU, the extern declarations must match exactly. E.g., `func_0800C7A4` was declared as `extern void func_0800C7A4(s32)` in asm_080117fc.c — a new file declaring it as `extern void func_0800C7A4(u32)` causes a compilation error in the host TU.
- **Implicit-register-argument trap**: Some functions like `func_08015A88` take an argument in R0 implicitly (reading R5=R0 in their prologue), but existing decomp files declare them as `void func(void)`. If you need to pass R0 explicitly for a new conversion, changing the extern to `void func(u32)` breaks existing callers that pass no argument. The compiler generates `MOVS R0, #0; BL func` instead of just `BL func`, causing ROM mismatch. **Mitigation**: Leave such functions for later when all callers can be updated simultaneously; do not patch the call with a naked asm wrapper.
- **Forward declarations needed** when an included_stub function is used before its include point in the host C file. Add `extern void func_XXXX(void);` before the first use.
- **s16 callee-signature sign-extension trap**: When a function passes a sign-extended s16 value to a callee declared as `s32 sprite_is_invalid(void*, s16)`, the compiler adds extra `LSLS R1, #16; ASRS R1, #16` before the BL because it re-sign-extends the s16 argument per the callee's formal parameter type. The original asm doesn't have this — the value is already sign-extended in a register. **Mitigation**: First try declaring the callee with a matching s32 parameter type in your decomp file. If that causes conflicts with other decomp files in the same TU, try a narrow `asm volatile("bl callee")` call-site shim, but do not wrap the whole function in naked asm. Example legacy files: `sprite_set_x`, `sprite_set_y` should be revisited. - **CMP#1/BGE vs CMP#0/BGT optimization trap**: The compiler (agbcc) always transforms `val >= 1` into `val > 0`, which generates `CMP R0, #0; BGT/BLE` instead of `CMP R0, #1; BGE/BLT`. This is an optimization since comparing against 0 is cheaper (no immediate needed for CMP). When the original asm uses `CMP R0, #1; BGE` (e.g., in a switch-like function where case 1-3 returns one value), pure C cannot reproduce this exact comparison pattern. **Mitigation**: Mark the candidate blocked or leave the legacy file untouched until a real-C workaround is found. Example legacy file: `func_0800BEC0`.
- **CMP#1/BLO vs CMP#0/BEQ optimization trap**: The compiler transforms `val < 1u` (unsigned) into `val == 0u`, generating `CMP R0, #0; BEQ/BNE` instead of `CMP R0, #1; BLO/BHS`. Same root cause as CMP#1/BGE — comparing against 0 is cheaper. When the original asm uses `CMP R0, #1; BLO` (checking if arg is 0 in a switch-like pattern), pure C cannot reproduce this exact sequence. **Mitigation**: Mark the candidate blocked. Example blocked function: `func_08011864`.
- **Register allocation mismatch with 5th stack arg + u16 truncation**: Functions that take a 5th stack argument AND truncate one of the register arguments to u16 (LSLS/LSRS) are extremely difficult to match with pure C. The compiler may swap which register gets the truncation (R0 vs R1) and which callee-save register holds the 5th arg vs the zero constant (R5 vs R6). Additionally, the compiler may place the stack buffer at different offsets (sp[0] vs sp[1]) causing all subsequent store offsets to shift. **Mitigation**: First try local stack structs/arrays, non-void return shaping, and register pinning to control which registers hold which values; `func_0800C110` proved this family can match in real C. If it still misses, keep shaping or choose another candidate — do not add naked asm. Example legacy file: `func_0800C080` should be revisited.
- **__divsi3 argument reorder trap**: When computing `__divsi3(0x10000, angle)` (where R0=0x10000 immediate and R1=angle variable), the compiler may reorder to `MOV R0, R1` instead of `MOVS R0, #0x80; LSLS R0, #9`. The compiler sees that R1 already holds the angle and simply copies it to R0 for the call, losing the immediate computation. **Mitigation**: Use `asm volatile("bl __divsi3" :: "r"(r0), "r"(r1) : clobbers)` to force the exact register assignment at the call site. Example: `func_08001C74`.
- **MOV R4,R2 collapse into LSL R4,R2,#0x10**: The compiler combines `MOV R4, R2; LSLS R4, R4, #0x10` into `LSLS R4, R2, #0x10` (3-operand form). This is semantically identical but byte-different. **Mitigation**: Use `asm volatile("mov r4, r2" : "+r"(r4) : "r"(r2))` to force the 2-instruction sequence. Example: `func_08001C74`.
- **agbcc won't push R7 callee-save register**: When a function uses R7 as a loop variable or general-purpose register, the compiler may not push/pop R7 even though the original asm does. The compiler only pushes callee-save registers that it actually clobbers across function calls, and since R7 is callee-saved by convention, the compiler assumes BL calls preserve it. If R7 is only used between BL calls, the compiler doesn't save it. **Mitigation**: No reliable pure-C workaround found. `asm volatile` barriers and clobbers don't force the push. Blocked example: `func_08012BB8`.
- **ORRS value-propagation into LSLS collapse**: After a register is assigned a small constant (e.g., `R2 = 4` from `MOVS R2, #4; ORRS R0, R2`), if the same register is later reassigned as `MOVS R2, #0x80; LSLS R2, #1`, the compiler may optimize this to `ADD R2, #0xFC` (adding 0xFC to the existing value 4 to get 0x100). This is semantically identical but byte-different. **Mitigation**: Insert `asm volatile("" : "+r"(r2))` barrier after the ORRS to break the value-propagation. Example: `func_08013EC0`.
- **s16/s8 callee-signature register-reuse in LDRSH**: When calling a function like `sprite_set_base_palette(struct SpriteHandler*, s16, s8)` where R2 is used for both the LDRSH offset and the third argument, the compiler swaps R2 and R3 — putting the LDRSH offset in R3 and the constant in R2. The original uses R2 for the offset. **Mitigation**: Use `asm volatile("bl sprite_set_base_palette")` to control register allocation. Example: `func_08014810`.
- **asm volatile BL clobber scope**: When using `asm volatile("bl callee")` with clobbers, including callee-save registers (R4-R7) in the clobber list tells the compiler they are destroyed, causing it to reassign them for later use. If you need the same register values after the BL, do NOT clobber them — the compiler will save/restore callee-save registers itself. Example: `func_08013388` needed R5 for LDRSH offsets after the BL, so R4/R5 were NOT clobbered in the asm volatile.
- **schedule_function_call u16 first-arg re-truncation**: When calling `schedule_function_call(u16 memID, ...)`, the compiler emits extra `LSLS R0, #0x10; LSRS R0, #0x10` after the register args are set up, even when R0 already holds a truncated u16 value from a prior truncation. **Mitigation**: Use `asm volatile("bl schedule_function_call")` to avoid the extra truncation. Example: `func_0800A0C4`.
- **Compiler hoisting literal pool loads before BL**: After a BL call, the compiler may hoist subsequent `LDR Rn, [PC, #offset]` (literal pool loads) above the BL if there are no data dependencies. The original asm loads them after the BL. **Mitigation**: Insert `asm volatile("" : "+r"(r0))` after the return value truncation to prevent the compiler from reordering. Example: `func_0800A0C4`.
- **mem_heap_dealloc(u32) vs mem_heap_dealloc(void*) type conflict**: `src/memory_heap.h` declares `void mem_heap_dealloc(void *)` but existing decomp files in main_menu.c use `void mem_heap_dealloc(u32)`. Including `memory_heap.h` in a new decomp file included in main_menu.c causes a conflicting types error. **Mitigation**: Don't include `memory_heap.h`; declare `extern void mem_heap_dealloc(u32)` to match existing same-TU extern declarations. Example: `func_0801522C`.
- **MOVS+RSBS constant-folding into SUB**: When code has `r0 = 0xF; r0 = -r0;` and r0 previously held a different value (e.g., 7), the compiler folds this into `SUB R0, #0x16` (computing `7 + (-0xF) = 7 - 0x16`). This produces `SUB` instead of `MOVS; RSBS`. No pure C workaround found — even asm volatile barriers and separate register variables don't prevent the fold. The compiler's constant propagation is too aggressive. **However**, if R0 previously held an unknown value (e.g., address of a global loaded from literal pool), the compiler CANNOT fold, and `MOVS R0, #N; RSBS R0, R0, #0` is emitted correctly. Blocked example: `scene_set_current_thread` (R0=7 was known). Working example: `func_08012658` (R0=gCurrentSceneData address was unknown).
- **Symbol address vs numeric constant in literal pool**: When a function stores a ROM address constant (e.g., `0x083A8588`) via `LDR R0, [PC, #offset]; STR R0, [R1]`, using `r0 = 0x083A8588` produces `.word 0x83a8588` in the literal pool (numeric constant). The original uses `.word D_083A8588` (symbol relocation). These have the same value but different object file entries, causing objdiff mismatches. **Mitigation**: Declare `extern u8 D_083A8588;` and use `r0 = (u32)&D_083A8588` instead of the numeric constant. Example: `func_0800DE24`.
- **CMP#1;BLO vs CMP#0;BEQ codegen**: The original compiler emits `CMP R0, #1; BLO` for `if (x < 1)` in switch-case lowering. Modern agbcc optimizes this to `CMP R0, #0; BEQ` since `x < 1` for unsigned is `x == 0`. These are semantically identical but byte-different. No pure C workaround found — the compiler always folds `CMP #1; BLO` into `CMP #0; BEQ`. Blocked example: `func_08011864`.
- **IP/R12 register not usable in pure C**: When the original asm uses `MOV IP, R0` to save a value in the IP register (R12), pure C cannot replicate this. The compiler uses R6 or another callee-save register instead, changing the PUSH/POP frame and register allocation. Blocked example: `func_08014740`.

### `_call_via_r1` indirect call trap

When the original asm loads a function pointer into R1 via `LDR R1, =func_XXXX` and then calls `BL _call_via_r1`, pure C generates a direct `BL func_XXXX` instead. The `_call_via_r1` pattern uses R1 for the callee address and R0 for the argument, producing `LDR R1, [PC, #offset]; MOV R0, R2; BL _call_via_r1`. A direct C call produces just `MOV R0, R4; BL func_XXXX` which is different object code.

**Current status**: This is a legacy hard case — no pure C spelling has been found for the indirect R1 call shape. Existing naked files with this pattern may remain quarantined until a real-C workaround is found, but this pattern does not justify adding new naked asm progress. Example legacy file: `func_080EE830`.

### s32 casts for ASR in signed multiply-shift
When computing `s32_result = (s16_val1 * s16_val2) >> 8`, the compiler may generate LSR instead of ASR if the multiply operands are in u32 register variables. Use explicit s32 casts to force ASR:
```c
register u32 r0 asm("r0");
r0 = (u32)*(s16 *)(r3 + r5);
r0 = (u32)((s32)r2 * (s32)r0);  // MUL with signed operands
r0 = (u32)((s32)r0 >> 8);        // generates ASR #8, not LSR #8
```
Without the s32 casts, the compiler treats the MUL result as unsigned and generates LSR. Used in func_08001BA4 and func_08001C08 (rotation matrix builders).

### Register / return-shape traps
- `POP {R1}; BX R1` epilogue is generated by agbcc for **non-void** functions with `-mthumb-interwork` (the return value is in R0, so it uses R1 for the pop). **Void** functions with `-mthumb-interwork` generate `POP {R0}; BX R0`. Without `-mthumb-interwork`, both generate `POP {PC}`.
- BL + STRH patterns may keep the wrong register live and produce `POP {R0}; BX R0` instead of `POP {R1}; BX R1` — check if the function should be non-void
- `s8`/`s16` return types can force extra sign-extension in the epilogue (`LSLS`/`ASRS`) even when the original just moves the register through. If that happens, try a wider intermediate and confirm whether the sign-extension is truly required or just a compiler artifact.
- `start_new_task` wrappers with on-stack task-arg structs often need to return the call result (`return start_new_task(...)`) to preserve the non-void epilogue (`POP {R1}; BX R1`) while still keeping the stack block in place.
- semantically identical C can still miss due to register allocation differences
- Zero-init store order depends on C source statement order: `a0[1]=0; a0[0]=0; a0[2]=0;` generates `STR [R0,#4]; STR [R0]; STR [R0,#8]` (matching the original's non-sequential pattern), while `a0[0]=0; a0[1]=0; a0[2]=0;` generates sequential stores
- Included stubs in the same TU don't need extern declarations for functions already defined in the host C file
- **Instruction ORDER is critical for byte-identical matching**, not just instruction choice. `.syntax divided` makes `mov`/`neg`/`and` encode identically to `movs`/`rsbs`/`ands`, but the compiler may reorder instructions differently from the original. For example, `MOVS R0,#3; RSBS R0,R0,#0; ANDS R0,R1` and `LDRB R1; MOVS R0,#3; RSBS R0,R0,#0; ANDS R0,R1` produce different bytes even though the same instructions are present — the LDRB position in the instruction stream matters.
- **Register-reuse between LDRSH offset and BL argument**: When the original uses R2 for both a LDRSH offset (e.g., `MOVS R2, #2; LDRSH R1, [R1, R2]`) and then immediately reuses R2 for a BL argument (`MOVS R2, #1; BL callee`), pure C with register pinning often fails. The compiler sees that R2 is needed for the BL argument and moves the LDRSH offset to a different register (R3), producing `MOVS R2, #1; MOVS R3, #2; LDRSH R1, [R1, R3]` which is functionally identical but byte-different. **Mitigation**: Try register pinning and statement reshaping; if it still cannot match, leave the legacy file untouched or mark the candidate blocked instead of adding naked asm. Example legacy file: `func_08011774`.

### Bitfield / mask traps
- do not replace bitfield extraction with AND masks when the original is a shift-pair
- `byte &= ~N` is dangerous when `(~N & 0xFF)` fits in 8 bits; agbcc tends to emit `MOVS #imm8; ANDS` instead of `MOVS #N; RSBS; ANDS`
- `-1` / other negative immediates can pick `NEGS`, `RSBS`, or literal-pool forms differently than expected
- some OR/bit-clear forms need a very specific spelling to avoid extra `ADDS`
- **register-pinning workaround**: when `&= ~3` must emit `MOVS R0,#3; RSBS R0,R0,#0; ANDS R0,R1` but agbcc prefers `MOVS #0xFC; ANDS`, pin the mask variable to R0: `register u32 m asm("r0"); val = ptr[7]; m = 3; m = -m; m = val & m; ptr[7] = m;`. **CRITICAL**: load the byte into a separate `val` local BEFORE creating the mask, so the compiler puts LDRB before MOVS/RSBS. Without this, the compiler may put MOVS/NEG before LDRB, producing different byte order.
- **Shift-OR-set instruction interleaving**: for `ptr[N] = (ptr[N] & 0x7F) | (arg0 << 7)`, declare `u32 shifted` as a local variable BEFORE `ptr`, and compute `shifted = arg0 << 7` after loading `ptr`. This forces the compiler to interleave `LSLS R0, #7` between `LDR R3` and `LDRB R2`, matching the original. Writing `arg0 = arg0 << 7` before loading `ptr` puts LSLS before LDR, which is wrong.
- **Forward loop with pointer increment pattern**: for loops that advance a pointer before checking (e.g., iterating through GraphicsTable entries), use `register char *r2 asm("r2")` to pin the pointer register, combined with `goto` labels for loop control. Example:
  ```c
  void func_080024E4(void *arg0) {
      register char *r2 asm("r2") = arg0;
      goto start;
  loop:
      r2 += 0xC;
  start:
      if (*(void **)r2 != NULL)
          goto loop;
      func_0800247C(r2);
  }
  ```
  This preserves the original instruction order (`ADDS R2,#0xC` before `LDR R0,[R2]`) and the specific register allocation the original asm uses.
- **BLS vs BLE for loop conditions**: `u32 i; while (i <= 2)` generates `BLS` (unsigned lower-or-same), while `s32 i; while (i <= 2)` generates `BLE` (signed less-or-equal). Check the original's branch type to determine the correct counter type. Using the wrong type produces different bytes even though the loop semantics are identical for non-negative values.
- **LSLS sign-bit test pattern**: When the original tests a specific bit using `LSLS R0, #0x1D; CMP R0, #0; BGE`, write `s32 val = ptr[N]; val = val << 0x1D; if (val >= 0) return; callee();`. Do NOT use `if (val & 4)` — that generates `MOVS R1, #4; ANDS; CMP; BEQ` which is different code.
- **Literal-pool AND mask** — `gGraphicsBuffer.DISPCNT &= 0xEFFF` compound assignment produces the right register allocation. Using a local `u16 val; val = ...; val &= ...; gGraphicsBuffer.DISPCNT = val;` produces wrong register allocation (AND result in R1 instead of R0).
- **Const bit-OR-set**: `gGraphicsBuffer.DISPCNT |= 0x1000` needs a local variable `u16 val; val = gGraphicsBuffer.DISPCNT; val |= 0x80 << 5; gGraphicsBuffer.DISPCNT = val;` to get the right register allocation (OR into R1, not R0).

### Addressing / literal-pool traps
- D_ absolute-address access is known-good mainly at offset `0`; non-zero offsets often drift
- large byte offsets may need pointer shaping so Thumb uses the same split (`+8` then `[+0x1E]`, etc.)
- `((u32*)&gGlobal)[N]` may compile to a symbol+offset literal relocation instead of base-load + offset addressing
- raw relocatable object bytes can mislead; final linked ROM is the real gate
- **Isolated agbcc assembly match ≠ ROM match**: Register allocation differences (e.g., R1 vs R3 for a loop pointer, R0 vs R1 for a STRH target) produce semantically identical but byte-different machine code. The linked ROM is the only reliable gate. Always verify with a full Docker build, not just agbcc output comparison.
- **Literal-pool AND+OR instruction ordering**: For `gGraphicsBuffer.DISPCNT = (DISPCNT & mask) | val`, the C declaration order matters. `loaded = base[0]; mask = 0xFFF8` produces `LDRH R2; LDR R1,=mask` (halfword first then mask), while `mask = 0xFFF8; loaded = base[0]` produces `LDR R1,=mask; LDRH R2` (mask first then halfword). The original typically loads the halfword first, so declare `loaded` before `mask`.
- **LSLS-before-LDR**: For `base + (arg0 << N)`, declare `shifted = arg0 << N` as a local BEFORE loading the base pointer to get `LSLS; LDR` ordering. Loading base first produces `LDR; LSLS`.
- **BICS pattern**: `result = 1; result &= ~val` generates `MOVS R0,#1; BICS R0,R1`. Writing `1 & ~val` or `~val & 1` generates `MVN; AND` instead.
- **Pointer-to-global vs global-value**: `base = &gCurrentSceneData` generates `LDR R4, .literal` (loads address into R4), while `base = gCurrentSceneData` generates `LDR R0, .literal; LDR R4, [R0]` (loads value into R4). Use `&gCurrentSceneData` when the original keeps the global's address in a register and dereferences it multiple times.

### Repo integration traps
- grouping multiple functions in one C file breaks matches; for batches, still create one `src/decomp/asm_xxxxxxxx.c` per function
- forgetting to move the old asm file causes duplicate / wildcard collisions
- forgetting the linker-script swap breaks standalone TU conversions
- included asm stubs inside an existing C TU (e.g. a mid-file `#include` in `graphics_table.c`) may need the host TU split before a standalone conversion will preserve ROM order
- bare `extern` declarations for g-symbols can fight existing repo types
- C89 declaration ordering still matters in this repo
- **Dirty-worktree false mismatches**: A manual edit left over from a failed attempt can make every later `apply_conversion` look like a candidate ROM mismatch. Before concluding callee-risk, run `git status --short` and revert unrelated edits. Batch 109 confirmed this: a lingering `func_08015A88` signature/call edit in `src/decomp/asm_08012c64.c` caused several false mismatches until it was reverted.
- **Included-stub linked mini-batches can be safe**: when a caller depends on a small callee, convert the callee first, verify ROM OK, then convert the caller in the same chunk. Batch 109 converted `func_08014E88` before its caller `func_080152A0`; both used naked inline asm and preserved ROM identity.

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

- **LSRS vs ASRS (logical vs arithmetic shift right)**: To force `LSRS` instead of `ASRS`, cast the intermediate to `u32` before shifting. For example, `(u32)val << 0x17` followed by `>> 0x19` generates `LSLS R0, #0x17; LSRS R3, R0, #0x19` while omitting the cast produces `ASRS`.
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
- **naked asm with .ltorg**: For `__attribute__((naked))` included_stub functions that reference literal pool constants (e.g., `ldr r1, =D_083A4A80`), add `.balign 4, 0` and `.ltorg` after the epilogue to match the original's literal pool placement. Without `.ltorg`, the assembler may defer the literal pool to the next function, changing the PC-relative offset in the `ldr` instruction and causing a ROM mismatch. Example: `func_0800C080`.

### noreturn trap
- **Do NOT use `__attribute__((noreturn))`** to suppress compiler-generated epilogues — it changes caller codegen and causes cascading ROM mismatch. Even if the function body is pure inline asm with `bx lr`, use plain `void` return type
- For simple BX LR leaf functions, `void func(void) {}` produces `BX LR + NOP` which matches in the final linked ROM even when the original object has `BX LR + .short 0x0000` — the linker resolves the alignment padding correctly
- Object-level NOP differences (`0xC046` vs `0x0000`) do NOT cause ROM-level mismatches for simple leaf functions

### ⛔ New naked inline asm conversions are banned

**Do not add new `__attribute__((naked))` + whole-function inline asm wrappers.** Naked asm wrappers are not real decompilation — they are just the original assembly wrapped in a C function shell, providing zero readability improvement. The goal of this project is to recover readable C, not to re-encode assembly.

**Mandatory C-shaping toolbox.** Keep iterating with these techniques, or switch to another real-C candidate. Do not pivot from a near-miss to naked asm:

1. **Pure C** with correct types, statement ordering, and local variables
2. **Register-pinned variables** (`register type asm("rN")`) to control register allocation
3. **asm volatile barriers** (`asm volatile("" : "+r"(x))`) to prevent instruction reordering or register swaps
4. **asm volatile clobbers** (`asm volatile("" ::: "r1")`) to force specific register choices
5. **Statement reordering** — declaration order affects instruction order in agbcc
6. **Type shaping** — `u16` vs `s32`, `(u32)` casts for LSRS vs ASRS, non-void return type for POP{R1};BX R1 epilogue
7. **Load-base-first trick** — assign global base to a local pointer before computing offsets
8. **Pointer arithmetic shaping** — `(u8 *)base + offset` vs `base[N]` vs pointer-advance patterns
9. **goto labels** for loop control when `for`/`while` generate wrong branch types

**Legacy hard cases that may stay naked until a real-C spelling is found:**
- **`_call_via_r1` / indirect call via register** — C always generates a direct BL; no pure C way has been found to emit `LDR R1,=func; BL _call_via_r1`
- **R2 register-reuse between LDRSH offset and BL argument** — the compiler moves the offset to a different register, which is functionally identical but byte-different
- **CMP#1/BGE vs CMP#0/BGT** — the compiler transforms `>= 1` to `> 0`; no C spelling has produced `CMP #1`
- **s16 callee-signature sign-extension** — when the callee's formal parameter type forces extra sign-extension that the original doesn't have, and the callee is in a different TU that can't be changed

These reasons justify leaving an existing legacy naked file untouched or marking a candidate blocked. They do not justify creating new naked asm progress.

**NOT valid reasons for naked asm:**
- "Register allocation is different" — try register pinning, asm volatile barriers, and statement reordering first
- "I tried for a while and got tired" — there is no attempt-count escape hatch; keep shaping C, pick another real-C candidate, or mark it blocked
- "It's 99% close" — close is not a reason to re-encode the function as naked asm; keep shaping or block with the exact mismatch
- "It's faster" — speed is not the goal; readable C is the goal
- "The function has a 5th stack argument" — use `u32 sp[1]` or similar array-based stack args in pure C

**Converting existing naked asm to real C:** Existing naked asm decomp files should be revisited and converted to real C whenever possible. Priority: (1) functions where naked asm was used only for register allocation differences, (2) functions where naked asm was used for instruction ordering fixable with statement reordering, (3) functions where naked asm was used for a trap that now has a documented C workaround. Each converted function must still achieve 100% match with `compile_and_view_asm` before applying.

**Included-stub helper typedef collision trap:** each `src/decomp/*.c` file is included into the host TU, so generic helper typedef names like `TaskArgs` can collide across files. Prefer unique typedef names or anonymous local structs inside each decomp file.


### Legacy naked-wrapper cleanup wins
- `func_08002468`: use explicit `u32` shift temporaries (`result = *ptr; result <<= 31; result >>= 31;`) instead of `& 1` to force the original `LDRB; LSLS #31; LSRS #31` sequence.
- `func_0800C080`: same family as `func_0800C110`; a local task-argument struct plus non-void `return start_new_task(...)` preserves the stack block and `POP {R1}; BX R1` epilogue without naked asm.
- `func_08001D5C`: register-pinned C can preserve the four halfword matrix stores once the 5th stack arg is loaded into a normal C parameter and all args are shaped as `s32` before explicit `<< 16` / `>> 16` truncation.
- `sprite_set_x`, `sprite_set_y`, `sprite_set_x_y`: use `s32` value parameters to prevent early caller-side truncation, preserve handler in `r5` before truncating value args, and use ordinary C stores after the `sprite_is_invalid` guard. For `sprite_set_x_y`, let the y temporary be an unpinned local so agbcc naturally saves/restores `r7`.
- `func_080CD564`: register-pin the destination pointer to `r3`; use separate pinned `r2`/`r1` temporaries so the two word copies match `LDR R2/STR R2` then `LDR R1/STR R1`.
- `func_080EE830`: a normal C function pointer local (`func = func_080efc88; func((void *)r2)`) can reproduce `LDR R1, =func_080efc88; BL _call_via_r1`; use `r0 = r3 + r0` for the command-skip pointer add to preserve operand order.
- `sprite_anim_get_cel_total` / `sprite_get_anim_duration`: declare the helpers as `u32` to avoid callee return sign-extension, then preserve the original caller code by spelling the cel-total assignment as `(sprite_anim_get_cel_total(anim) << 24) >> 24`.
- `func_08011774` / `func_08014E88`: agbcc will often lower `*(s16 *)(base + r2)` to an address add plus zero-offset `ldrsh`; a single narrow `asm volatile("ldrsh %0, [%1, %2]")` preserves the indexed `LDRSH` while keeping the rest of the function in C. Treat this as a last-resort narrow instruction workaround, not a whole-function asm escape hatch.


## Families still worth mining heavily
- conditional byte-check + BL wrappers
- shift-offset store wrappers
- remaining `sprite_id_delete` siblings (NOTE: loop-based variants like asm_08016d3c, asm_0806843c, asm_0806fe20 failed to match; their loop iteration patterns and register allocation don't align with simple C for-loops even when semantically correct)
- `scene_set_current_thread(1)` sibling families
- more MOVS R0, #const + BL wrapper families
- multi-BL wrappers whose good spellings are now documented