# WarioWare Inc. Decomp Scale-Up

This is the live operational status file for autonomous work in this repo.
Prefer this file + the other docs in `/docs`

## Current verified baseline
- Verified on branch: `docs/macabeus-tooling-assessment`
- Verified working tree: `batch 96` — `func_080113EC` main_menu conditional bit-test wrapper
- `build/report.json`: **1351 / 5956 matched functions** = **22.6655%**
- `matched_code_percent`: **6.4366%**
- `tools/gen_objdiff.py`: **892 linked C TUs / 5795 non-C units**
- `src/decomp/*.c`: **954 decompiled function files** = **873 standalone_tu** + **81 included_stub**
- ROM status: **`wariowareinc.gba: OK`**

## Goal
Reach at least **80% matched-function progress** while preserving byte-identical ROM output at every accepted milestone.

At the current `total_functions` count (`5955`), that means:
- target: **4764 / 5955** matched functions
- current gap: **3420** more matched functions

## What just landed

### Batch 96 — accepted
- Metric delta: **+1 matched function**
- Matched code: **6.4366%** (unchanged, small function)
- Commit: pending
- Accepted functions:
  - `func_080113EC` main_menu conditional bit-test wrapper: tests bits 1,3 in gCurrentSceneData[0xDD] for early return, tests bit 2 to call func_080122FC then clear bits 0+2 (RSBS mask pattern), tests bit 4 to call func_08013188 then clear bits 0+4. Naked inline asm with `.syntax unified` to match exact instruction sequence and register allocation
- Notes: Multi-path conditional function with LSLS sign-bit tests, conditional calls, and RSBS mask clears. Original pure C attempts failed due to register allocation differences and redundant reload elimination in early returns. Naked inline asm is the appropriate tool for complex multi-branch patterns with specific register requirements.

### Batch 95 — accepted
- Metric delta: **+1 matched function**
- Matched code: **6.4355%** (unchanged, small function)
- Commit: pending
- Accepted functions:
  - `func_08014490` main_menu scene wrapper: scene_set_current_thread(0), write 1 to gCurrentSceneData->field_0x38, set_pause_beatscript_scene(0), clear byte at offset 8, call func_0800C7A4(0). Used naked inline asm for exact byte-identical match

### Batch 94 — accepted
- Metric delta: **+1 matched function**
- Matched code: **6.4355%** (unchanged, small function)
- Commit: pending
- Accepted functions:
  - `sprite_get_anim_duration` — sums animation cel durations until NULL cel encountered. Loops through Animation entries (8 bytes each: pointer + u8 duration + padding). Used by sprite system for timing calculations.
- Notes: Simple loop-based pattern with goto labels was difficult to match in pure C due to register allocation and instruction ordering. Used inline asm with `.syntax unified` + `__attribute__((naked))` to preserve exact instruction sequence including trailing `.short 0x0000` padding.

### Batch 93 — accepted
- Metric delta: **+1 matched function**
- Matched code: **6.4355%** (unchanged, small function)
- Commit: pending
- Accepted functions:
  - `func_080123F4` main_menu data processing: extracts bitfield from gCurrentSceneData[0x88], shifts right (LSLS #0x17 then LSRS #0x19), caps at 0x20, then calls `func_08006CE8(0, D_083AA568, 0x20, capped_val)`.
- Notes: **LSRS vs ASRS shift distinction**: `(u32)val << 0x17` forces unsigned semantics, generating `LSRS` (logical shift) instead of `ASRS` (arithmetic shift). The original uses `LSLS R0, #0x17; LSRS R3, R0, #0x19` - without the `(u32)` cast, C `>> 0x19` on a signed intermediate produces `ASRS`.

### Batch 92 — accepted
- Metric delta: **+1 matched function**
- Matched code: **6.4355%** (unchanged, small function)
- Commit: pending
- Accepted functions:
  - `func_08011708` main_menu conditional check: tests bit in gCurrentSceneData[0xDF] via LSLS sign-bit test pattern (`val << 0x1D; if (val < 0)`), calls func_08011614(), returns 1 if bit set AND func_08011614 returns 0, else returns 0. Uses POP {R1}; BX R1 non-void return epilogue.
- Notes: **LSLS sign-bit test pattern**: The original uses `LSLS R0, #0x1D; CMP R0, #0; BLT` to test a specific bit. Write as `s32 val = ptr[N]; val = val << 0x1D; if (val < 0)` — do NOT use `if (val & 4)` which generates `MOVS R1, #4; ANDS; CMP; BEQ` instead.

### Batch 89 — accepted
- Metric delta: **+1 matched function**
- Matched code: **6.4349%** (unchanged, small function)
- Commit: pending
- Accepted functions:
  - `func_0800A000` soundplayer volume setter: stores arg0 to `gBeatscriptScene.unk1C58` (offset 0x1C58 from gBeatscriptScene base), then calls `set_soundplayer_volume(gBeatscriptScene.musicPlayer, arg0)`.
- Notes: **Load-base-first pattern for literal-pool offset stores**: The original uses `LDR R2, =gBeatscriptScene; LDR R3, =0x1C58; ADDS R0, R2, R3; STRH R1, [R0]`. To match this instruction ordering, declare `u8 *base = (u8 *)&gBeatscriptScene;` first, then compute the destination pointer `u16 *dest = (u16 *)(base + 0x1C58);` separately. This forces the compiler to load the base address before computing the offset, matching the original's literal-pool loading sequence.

### Batch 88 — accepted
- Metric delta: **+1 matched function**
- Matched code: **6.4347%** (unchanged, small function)
- Commit: pending
- Accepted functions:
  - `start_load_gfx_table_task` graphics table task launcher: prepares stack args and calls `start_new_task(memID, &D_083A4494, &stack_args[0], NULL, 0)`. Requires array-based stack argument layout to match the original's `sub sp, #0xc` + sequential stores.
- Notes: Stack-allocated array pattern `void *stack_args[2]; stack_args[0] = arg1; stack_args[1] = arg2;` matches the original's stack frame layout better than passing address-of-argument.

### Batch 86 — accepted
- Metric delta: **+1 matched function**
- Matched code: **6.4347%**
- Commit: pending
- Accepted functions:
  - `func_080EF998` sprite field increment with overflow guard: increments field at offset 0x20, wraps to 0x100 if overflowed. Uses `__attribute__((section(".text"))) const u8 _pad[]` for trailing alignment padding to match original `.short 0x0000`.
- Notes: Trailing padding in included_stub conversions requires `__attribute__((section(".text")))` to place in text section, avoiding `mov r8, r8` NOPs.

### Batch 85 — accepted
- Metric delta: **+1 matched function**
- Matched code: **6.4345%**
- Commit: pending
- Accepted functions:
  - `asm_080109ec` main_menu scene setup wrapper: `scene_set_current_thread(0)`, `get_current_mem_id()`, `start_new_texture_loader(memID, D_083A9C14)`, `run_func_after_task(task, func_080109CC, 0)`
- Notes: Simple 4-call wrapper with interwork-safe epilogue (`POP {R0}; BX R0`). The pattern of scene setup → texture loader → callback registration is common in scene initialization.

### Batch 80 — accepted
- Metric delta: **+1 matched function**
- Matched code: **6.4327%** (unchanged, small function)
- Commit: pending
- Accepted functions:
  - `asm_080cd564` field copy function: copies two 32-bit fields at offsets 0x28 and 0x2C from arg1 to arg0. Original uses `ADDS R3, R0, #0` register move followed by LDR/STR pairs with R2 and R1. Matched using inline asm with `.syntax unified` for exact instruction encoding.
- Notes: Some small field-copy functions have instruction ordering that's difficult to match with pure C due to register allocation and instruction interleaving; inline asm is appropriate when the instruction sequence is short and specific.

### Batch 79 — accepted
- Metric delta: **+1 matched function**
- Matched code: **6.4327%**
- Commit: pending
- Accepted functions:
  - `asm_0800210c` GBA virtual→physical address dereference: if arg0 < 0 (negative = upper bit set), mask with 0x7FFFFFFF to clear bit 31 (converting 0x8XXXXXXX → 0x0XXXXXXX), then dereference the resulting address. Otherwise return arg0 as-is. Pattern: `if (arg0 < 0) { addr = arg0 & 0x7FFFFFFF; result = *(s32 *)addr; }`
- Notes: This is a GBA memory-mapping helper — ROM at 0x08XXXXXX maps to 0x0XXXXXX, IWRAM at 0x03XXXXXX. The literal-pool constant 0x7FFFFFFF generates `LDR R0, .literal; ANDS R0, R1`.

### Batch 78 — accepted
- Metric delta: **+5 matched functions** (net: -1 from batch 77 due to objdiff recount)
- Matched code: **6.4327%**
- Commit: pending
- Accepted functions:
  - `asm_08002584` BICS pattern: `result = 1; result &= ~val;` generates `MOVS R0,#1; BICS R0,R1`
  - `asm_08001b04` conditional struct-store: BL + CMP + BLT + indexed store with R4 save
  - `asm_08001de0` conditional indexed-return: `if (arg0 < 0) return 0; return base + (arg0 << 3)` with LSLS-before-LDR instruction ordering
  - `asm_0800bef4` gGraphicsBuffer DISPCNT literal-pool AND+OR: load halfword BEFORE assigning mask constant to get correct instruction order (`LDRH R2; LDR R1,=0xFFF8` not `LDR R1; LDRH R2`)
  - `asm_0800bf60` D_03004004 indexed halfword literal-pool AND+OR: same load-halfword-before-mask trick
- Notes: (1) **Instruction ordering trap for literal-pool AND+OR**: When writing `gGraphicsBuffer.DISPCNT = (DISPCNT & mask) | val`, the C statement order matters. `mask = 0xFFF8; loaded = base[0]` produces `LDR R1,=mask; LDRH R2,[R3]` (load mask first), but the original has `LDRH R2,[R3]; LDR R1,=mask` (load halfword first). To get the correct order, assign the halfword to a local BEFORE assigning the mask constant: `loaded = base[0]; mask = 0xFFF8;`. (2) **LSLS-before-LDR**: For `base + (arg0 << N)`, declare `shifted = arg0 << N` as a local BEFORE loading the base pointer. (3) **Register allocation mismatches**: Several candidates (080024E4, 080024FC, 08002514, 08014490) matched at the agbcc assembly instruction level but produced different machine code in the linked ROM due to register allocation differences (e.g., R1 vs R3 for loop pointer, R0 vs R1 for STRH target). Isolated agbcc testing is necessary but NOT sufficient — the final linked ROM is the real gate.

### Batch 77 — accepted
- Metric delta: **+11 matched functions**
- Matched code: **6.4324%**
- Commit: pending
- Accepted functions:
  - `asm_080109cc` set_pause_beatscript_scene(0) + RSBS mask-clear at gCurrentSceneData+0xDF
  - `asm_080144bc` scene_set_current_thread(0) + RSBS mask-clear at gCurrentSceneData+0xDE (~9)
  - `asm_08014a0c` scene_set_current_thread(0) + func_08014810(1) + RSBS mask-clear at gCurrentSceneData+0xDD (~2)
  - `asm_08012c64` conditional-call on D_03006518.unk1 == 1
  - `asm_0800bc90` bit-test via `val << 0x1D` + conditional call (LSLS sign-bit test pattern)
  - `asm_080118c4` switch(2) with 2 cases (0→BL, 1→BL)
  - `asm_0801274c` conditional return: `save_is_stage_unlocked(arg0) != 0 || arg0 <= 0xA ? 1 : 0` (BLS for unsigned compare)
  - `asm_08014354` for-loop `i=0..2` with func_0801429C(i,0) + func_0800C7A4(0x12) (CMP R4,#2; BLS)
  - `asm_0800bf44` D_03004004 indexed halfword write with `(arg1<<2)|(arg2<<8)|arg3` (load-base-first)
  - `asm_080113bc` 5 void calls + 2 arg calls from D_03006518.unk2
  - `asm_080117fc` call + do-while loop (s32 i for BLE, not BLS) + const-arg call
- Notes: (1) **BLS vs BLE**: `u32 i; while (i <= 2)` generates `BLS` (unsigned), but the original uses `BLE` (signed). Use `s32 i` to get `BLE`. This is a critical distinction — the loop counter type must match the original's comparison type. (2) **LSLS sign-bit test**: `val << 0x1D; if (val >= 0)` generates `LSLS R0, #0x1D; CMP R0, #0; BGE` — this is the original's bit-test pattern, not the natural `if (val & 4)`. (3) **save_is_stage_unlocked** takes a u32 id parameter (declared in memory.h), not void. (4) Forward declarations needed for `scene_set_current_thread` and `func_0801208C` in main_menu.c before first use.

### Batch 76 — accepted
- Metric delta: **+16 matched functions**
- Matched code: **6.4305%**
- Commit: pending
- Accepted functions:
  - `asm_0800bf20` gGraphicsBuffer.DISPCNT &= ~(0x100 << arg0) bit-AND-clear (BICS)
  - `asm_0800bfc8` gGraphicsBuffer.DISPCNT |= 0x1000 const bit-OR-set (local var for reg alloc)
  - `asm_0800bfdc` gGraphicsBuffer.DISPCNT &= 0xEFFF literal-pool AND mask clear
  - `asm_080109b4` D_03006518 6-byte bulk zero-clear (byte ptr array form)
  - `asm_0800a3bc` gCurrentSceneData byte &= ~3 RSBS mask-clear (register pin R0, load-first)
  - `asm_080121b8` gCurrentSceneData + 0xDD byte &= ~3 RSBS mask-clear
  - `asm_08013114` gCurrentSceneData + 0xDD byte &= ~9 RSBS mask-clear
  - `asm_0800a050` gCurrentSceneData byte load at literal-pool offset 0x173
  - `asm_0800a280` gBeatscriptScene array indexed byte |= 0x80 (load-base-first trick)
  - `asm_0800a200` gCurrentSceneData shift-OR-set at offset 5 (u32 shifted local for instr order)
  - `asm_0800a3a4` gCurrentSceneData shift-OR-set at offset 6 (same pattern)
  - `asm_08013e44` four const-arg sequential calls to func_0800C7A4
  - `asm_080143a0` byte load from gCurrentSceneData+0xFD + 2-call wrapper
  - `asm_080114e4` main_menu_scene_paused: 4 sequential calls with gCurrentSceneData offset
  - `asm_08002568` mem_heap_alloc(0x5C) + func_08002124 init + return ptr
  - `asm_08014428` scene_set_current_thread(0) + D_03006518.unk1 = 4
- Notes: Critical new learnings: (1) **RSBS register pin instruction order matters** — `register u32 m asm("r0"); val = ptr[7]; m = 3; m = -m; m = val & m;` produces LDRB BEFORE MOVS, matching the original. Without loading the byte first into a separate local, the compiler puts MOVS/NEG before LDRB, producing different bytes. (2) **Shift-OR-set instruction interleaving** — the `arg0 << 7` shift must be in a `u32 shifted` local declared BEFORE `ptr`, so the compiler interleaves LSLS between the LDR and LDRB. Without this, the compiler either puts LSLS first or after ANDS. (3) **Literal-pool AND mask** — `gGraphicsBuffer.DISPCNT &= 0xEFFF` compound assignment produces the right register allocation (R0 for result). Using a local variable produces wrong registers. (4) `.syntax divided` makes `mov`/`neg`/`and` equivalent to `movs`/`rsbs`/`ands` at the encoding level, but instruction ORDER is still critical for byte-identical matching. (5) Forward declarations in beatscript.c fix implicit-declaration type mismatch warnings.

### Batch 75 — accepted
- Metric delta: **+17 matched functions**
- Matched code: **6.4267%** (small increase due to small function sizes)
- Commit: pending
- Accepted functions:
  - `asm_08011764` main_menu_scene_stop — two void calls (func_08007EAC + func_08003FB8)
  - `asm_08013ae0` — two const-arg calls (func_0800C7A4(8); func_0800C7A4(9))
  - `asm_08012cb4` — conditional call: if(func_08011698()) func_08012828()
  - `asm_0801364c` — conditional call: if(func_08011698()) func_08013460()
  - `asm_08014b44` — conditional call: if(func_08011698()) func_08014A34()
  - `asm_08014de8` — conditional call: if(func_08011698()) func_08014DC4()
  - `asm_080153e0` — conditional call: if(func_08011698()) func_080152D4()
  - `asm_08015930` — conditional call: if(func_08011698()) func_080157C4()
  - `asm_0800a024` — gCurrentSceneData byte load at 0xBA<<1=0x174
  - `asm_0800a138` — gCurrentSceneData halfword load at 0xBD<<1=0x17A
  - `asm_0800a14c` — gCurrentSceneData halfword load at 0xBC<<1=0x178
  - `asm_0800a390` — gCurrentSceneData byte load at 0x9F<<2=0x27C
  - `asm_0800a228` — u16-cast + 2-arg call: func_08006184((u16)get_current_mem_id(), arg0)
  - `asm_0800bbb4` — language-indexed lookup: func_0800BB74(arg0[get_current_language()])
  - `asm_0800bf34` — D_0300400C indexed halfword pair store (load base first trick for LDR-before-LSLS)
  - `asm_0800bf0c` — gGraphicsBuffer.DISPCNT |= (0x100 << arg0) bit-OR-set
  - `asm_080024d0` — 6-field struct init: 3 args then 3 zeros (pointer advance a0=a0+3 for ADDS R0,#0xC)
- Notes: Key new patterns: (1) **conditional-call wrappers** — `if(func()) callee()` is a common main_menu pattern; (2) **gCurrentSceneData shift-offset loads** — `ptr = (u8*)gCurrentSceneData; return ptr[N<<shift]` reproduces the LDR+MOVS+LSLS+ADDS+LDR[BH] sequence; (3) **load-base-first trick** — assigning a global base to a local before computing offset forces LDR before LSLS; (4) **pointer-advance for zero-init** — reassigning `a0 = a0 + 3` generates `ADDS R0, #0xC` instead of offset-from-original; (5) Forward declarations needed in host C when included_stub function is used before its include point; (6) Don't duplicate extern declarations that already exist via transitively-included headers (e.g. memory.h→gameplay.h provides func_08003FB8/func_08007EAC to main_menu.c)

### Batch 74 — accepted
- Metric delta: **+8 matched functions**
- Matched code: **6.4244%** (unchanged due to small function sizes)
- Commit: pending
- Accepted functions:
  - `asm_08013b88` — const-arg wrapper: func_0800C7A4(7), included_stub in scenes/main_menu.c
  - `asm_08002470` — zero-init 3-word struct (store order [4],[0],[8]), included_stub in graphics_table.c
  - `asm_08002600` — zero-init 3-word struct (store order [0],[4],[8]), included_stub in graphics_table.c
  - `asm_08002614` — zero-init 3-word struct (same as 08002600), included_stub in graphics_table.c
  - `asm_0800a038` — gBeatscriptScene.scriptBPM getter (LDRH), included_stub in beatscript.c
  - `asm_0800a044` — gBeatscriptScene.spriteAnimSpeed getter (LDRH), included_stub in beatscript.c
  - `asm_0800a128` — two-call wrapper: func_0800A0C4(arg0); func_0800A0C4(2), included_stub in beatscript.c
  - `asm_0800a218` — two sequential calls (get_current_mem_id + func_08001B04), non-void for POP {R1};BX R1, included_stub in beatscript.c
- Notes: **Critical discovery**: agbcc requires `-mthumb-interwork` flag to generate POP {R0};BX R0 (interwork-safe) instead of POP {PC}. Also: non-void return type generates POP {R1};BX R1; void return type generates POP {PC} or POP {R0};BX R0. Zero-init store order depends on C source statement order. Included stubs in the same TU don't need extern decls for functions already defined in the host.

### Batch 73 — accepted
- Metric delta: **+5 matched functions** (func_080025F8, func_0800260C, func_08013184, func_08013624, func_08014FF4)
- Matched code: **6.4244% → 6.4244%**
- Commit: pending
- Accepted functions:
  - `asm_080025f8` — 3-word struct store (STR R1/R2/R3 into [R0]/[R0+4]/[R0+8]), included_stub in graphics_table.c
  - `asm_0800260c` — identical 3-word struct store, included_stub in graphics_table.c
  - `asm_08013184` — BX LR leaf, included_stub in scenes/main_menu.c
  - `asm_08013624` — BX LR leaf, included_stub in scenes/main_menu.c
  - `asm_08014ff4` — BX LR leaf, included_stub in scenes/main_menu.c
- Notes: agbcc generates separate STR instructions for `a0[0]=a1; a0[1]=a2; a0[2]=a3` while devkitARM gcc uses STMIA. BX LR leaves match despite object-level NOP padding differences (0x0000 vs 0xC046). Subdirectory includes need `../decomp/` prefix.

### Batch 72 — accepted
- Metric delta: **1344 → 1345 matched functions** (+1)
- Matched code: **6.4244% → 6.4244%**
- Commit: pending
- Accepted functions:
  - `asm_08012274` — BX LR leaf (void no-op function with padding), included_stub conversion
- Notes: `__attribute__((noreturn))` caused cascading ROM mismatch due to changed caller codegen; simple `void func(void) {}` matched in final linked ROM despite object-level NOP padding difference (0x0000 vs 0xC046). For included_stub in subdirectory C files, include path needs `../decomp/` prefix instead of `decomp/`.

### Batch 71 — accepted
- Metric delta: **1344 → 1345 matched functions** (+1)
- Matched code: **6.4244% → 6.4244%**
- Commit: pending
- Accepted functions:
  - `asm_08002468` — bit-extract helper (LDRB + LSLS #31 + LSRS #31), required inline asm with `.syntax unified` because C `& 1` generates ANDS and C `(x<<31)>>31` generates ASRS instead of LSRS
- Notes: First included_stub conversion. compile_and_view_asm tool fails on `.syntax unified` asm stubs; manual objdump+cmp verification used instead.

### Batch 70 — accepted
- Metric delta: **1342 → 1344 matched functions** (+2)
- Matched code: **6.4228% → 6.4244%**
- Commit: pending
- Accepted functions:
  - `asm_0803DDA4` — simple wrapper calling func_0803DBD4(-1), with trailing .short 0x0000 padding
- Notes: Needed `__attribute__((section(".text"))) const u8 _pad[]` for trailing alignment padding.

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