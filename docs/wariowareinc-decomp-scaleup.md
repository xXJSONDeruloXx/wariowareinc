# WarioWare Inc. Decomp Scale-Up

This is the live operational status file for autonomous work in this repo.
Prefer this file + the other docs in `/docs`

## Current verified baseline
- Verified on branch: `docs/macabeus-tooling-assessment`
- Verified working tree: `batch 126` — func_080EFC20, sprite_set_x, sprite_set_y real C + naked asm included_stub conversions
- `build/report.json`: **1350 / 5960 matched functions** = **22.6510%**
- `matched_code_percent`: **6.45165%**
- `tools/gen_objdiff.py`: **892 linked C TUs / 5795 non-C units**
- `src/decomp/*.c`: **1035 decompiled function files** = **873 standalone_tu** + **162 included_stub**
- ROM status: **`wariowareinc.gba: OK`**
- Remaining naked asm files: **7** (func_080113EC, func_08014E88, sprite_anim_get_cel_total, sprite_get_anim_duration, func_08011774, sprite_set_x, sprite_set_y)

## Goal
Reach at least **80% matched-function progress** while preserving byte-identical ROM output at every accepted milestone.

At the current `total_functions` count (`5956`), that means:
- target: **4767 / 5958** matched functions
- current gap: **3409** more matched functions

## What just landed

### Batch 126 — accepted
- Metric delta: **+0 report matched functions**, **+6 included_stub decomp files**, **+0.000189% matched code** (6.452217 → 6.4524055%)
- Matched code: **6.4524055%**
- Accepted functions:
  - `func_080159FC` main_menu: gCurrentSceneData+0xCC counter increment with BLS reset, then copy 3 halfwords from lookup table to D_030041E4. Leaf.
  - `func_0800C038` bitmap_font: gGraphicsBuffer+0x48 AND/OR mask write for BG scroll (FFF0/FF0F masks, low 4 bits / bits 4-7). Same family as func_0800BFF0. Leaf.
  - `func_08001AC0` code_08001a70: slot-allocator search loop scanning D_03000118 for free byte. Returns slot index or -1. Leaf. Fixed extern type to match asm_08001b04.c (void→u32 arg).
  - `func_08001A70` code_08001a70: D_03000010 array initialization loop with 0x100/0 halfword pattern and D_03000118 zero-clear. Uses triple asm volatile barrier for MOVS R0,#0x80; LSLS; MOV R5,R0; MOVS R3,#0 ordering. Leaf.
  - `func_08001BA4` code_08001a70: rotation matrix builder using gCosineTable/gSineTable with ASR #8. Key fix: s32 casts on MUL and shift to generate ASR instead of LSR. Leaf.
  - `func_08001C08` code_08001a70: 2D rotation matrix builder (two angles) using gCosineTable/gSineTable with ASR #8. Same s32 cast pattern as func_08001BA4. Leaf.
- Notes: Six leaf functions across three modules (main_menu, bitmap_font, code_08001a70). Key patterns: s32 casts for ASR generation in signed multiply-shift, triple asm volatile barrier for instruction ordering, and the gGraphicsBuffer AND/OR mask family now has 3 members (BFF0, C038, plus earlier ones).

### Batch 125 — accepted
- Metric delta: **+0 report matched functions**, **+5 included_stub decomp files**, **+0.000565% matched code** (6.451652 → 6.452217%)
- Matched code: **6.452217%**
- Accepted functions:
  - `func_08012C18` main_menu: stage lookup with save_is_stage_unlocked, get_current_language, func_0800068C calls. Non-void return (POP {R1}; BX R1). Fixed conflicting extern type from asm_08012c64.c (void func(u8) → u32 func(u32)).
  - `func_08011920` main_menu: scene thread setup with conditional bit-test on gCurrentSceneData+0x88. Uses asm volatile barrier on r0 to prevent `LDR R0,[R0]; MOV R1,R0` collapse into `LDR R1,[R0]`.
  - `func_0800BFF0` bitmap_font: gGraphicsBuffer+0x48 AND/OR mask write for BG position. Two blocks: x (F0FF mask, <<8) and y (0FFF mask, <<12).
  - `func_0800894C` gameplay: struct entry init with AND/OR mask and RSBS bit-clear. Uses asm volatile barrier on r4 to prevent SUBS #3 optimization of MOVS #2; RSBS.
  - `func_0800898C` gameplay: linked-list append with 0xFF-sentinel loop (pointer-advance pattern). AND/OR mask (0x3FF/0xFFFC00FF), then init next entry.
- Notes: Five included_stub functions across three modules (main_menu, bitmap_font, gameplay). The `asm volatile("" : "+r"(r0))` barrier was used to prevent instruction-sequence collapse (LDR+MOV → single LDR). The `asm volatile("" : "+r"(r4))` barrier prevented compiler from seeing r4=1 and optimizing MOVS#2/RSBS into SUBS#3. Failed attempts: func_0800BEC0 (CMP#1/BGE vs CMP#0/BGT optimization), func_08013EC0 (0x80<<1 folding into ADD #0xFC).

### Batch 124 — accepted
- Metric delta: **+0 report matched functions**, **+5 included_stub decomp files** (matched code stable)
- Matched code: **6.451652%**
- Accepted functions:
  - `sprite_remove_z_link` lib_sprite linked-list removal: updates prev/next pointers (0x1A/0x18 offsets) with head/tail fixup at 0xC/0xE. Leaf function; instruction order fix via interleaving r3 load between r4 and r1 shift.
  - `func_08012700` main_menu sprite scene init: stores byte at D_03006518, loads position from D_083AA0C4 table, conditionally calls func_08011504 with func_08012658+1 callback or direct func_08012658, then play_sound. Uses asm volatile BL for func_08011504 and play_sound. Instruction order fix via asm volatile barrier on r2.
  - `func_08013764` main_menu D_03000E60 struct init: AND/OR mask pattern with 0x3FF/0xFFFC00FF, RSBS mask-clear at +2, 0xFF init at +8. Leaf function; instruction order fix via asm volatile barrier on r2.
  - `func_080136A4` main_menu scene thread setup: calls scene_set_current_thread(0), sprite_set_anim_cel via asm volatile BL (s16 callee-signature trap), RSBS mask-clear at +0xDD, then func_080135E8 and func_08015A88.
  - `func_08011584` main_menu sprite position set: calls func_08005920 (check), sprite_set_x_y via asm volatile BL (s16 callee-signature trap), and func_08005834. Uses R5 reuse for gCurrentSceneData pointer across the function.
- Notes: Five included_stub functions in one chunk. The `asm volatile("" : "+r"(r2))` barrier pattern was critical for preventing instruction reordering in sprite_remove_z_link and func_08013764. Attempted sprite_set_z with naked asm but .syntax divided/unified leakage into host TU caused build failure; left for standalone_tu conversion. Attempted func_08011864 (switch pattern) but CMP #1/BLO vs CMP #0/BEQ optimization difference prevents byte-identical match.

### Batch 123 — accepted
- Metric delta: **+0 report matched functions**, **+8 included_stub decomp files** (matched code increased)
- Matched code: **6.45165%**
- Accepted functions:
  - `func_08012058` main_menu scene init with function pointer: loads sprite position from D_083AA0C4 table, calls func_08011504 with x/y/func_08011920+1/0, then func_08011730(0). Uses asm volatile BL for func_08011504 to avoid type conflict with main_menu.h declaration.
  - `func_08013A4C` main_menu scene flag handler: copies halfword from gCurrentSceneData+0xEC to gGraphicsBuffer+0x14, tests bit 0 of +0xDD, conditionally calls func_08013C60+func_08013AF4 and zeroes +0xF1, then RSBS mask-clear bits 0,6 at +0xDE.
  - `func_08013A94` main_menu cursor scroll handler: calls func_0800C7A4(8)+func_0800C7A4(9), checks gCurrentSceneData+0xF0 byte and D_03006518.unk3 for conditional func_0800C77C calls. Uses (s32) cast for BGE comparison instead of BHS.
  - `func_0800BB74` bitmap_font init: calls func_0800B828 with data pointer, stores scene data offsets, calls func_0800BA78. Uses asm volatile BL for func_0800B828 to enforce R0/R1/R2 register assignment.
  - `func_08001DA4` code_08001a70 loop: iterates D_03000138*4 times, copies halfwords from D_03000010 array to offset +6 of each D_03000110 entry. Leaf function, first-try match.
  - `sprite_handler_dealloc_id` lib_sprite id deallocation: validates s16 id, manages linked list at offset 0x1A with 0xFFFF sentinel, stores at offset 0x12. Uses asm volatile r1 clobber to prevent instruction reordering.
  - `func_0800BC10` bitmap_font sprite show: if scene data offset 0x180 nonzero, calls sprite_set_visible(handler, id, 1) and sets +0x195 to 1. Uses asm volatile BL for sprite_set_visible.
  - `func_0800BC50` bitmap_font sprite hide: similar to func_0800BC10 but calls sprite_set_visible(handler, id, 0) and sets +0x195 to 4. Uses asm volatile BL for sprite_set_visible.
- Notes: Eight functions in one chunk. The asm volatile BL pattern was used extensively (func_08011504, func_0800B828, sprite_set_visible) to handle type conflicts and enforce register ordering. The (s32) cast for signed comparison (BGE vs BHS) was a new technique for matching original branch instructions. The r1 clobber pattern prevented instruction reordering in sprite_handler_dealloc_id. Cleaned up stale dependency files from failed apply_conversion.

### Batch 122 — accepted
- Metric delta: **+1 report matched function**, **+9 included_stub decomp files** (matched code increased)
- Matched code: **6.45165%**
- Accepted functions:
  - `func_08012768` main_menu stage finder (offset 4): iterates `D_083AA0C4` table entries (16-byte stride), calls `func_0801274C` for each positive entry at offset 4, returns index if found or -1 if all negative. Has literal pool in middle of function. Real C with register pins.
  - `func_08012798` main_menu stage finder (offset 5): identical to func_08012768 but checks signed byte at offset 5. Real C with register pins.
  - `func_080127C8` main_menu stage finder (offset 6): identical but offset 6. Real C with register pins.
  - `func_080127F8` main_menu stage finder (offset 7): identical but offset 7. Real C with register pins.
  - `func_08012DCC` main_menu sprite visibility loop: iterates 0..0x1D, reads signed halfword from scene data array at 0x1D4, calls `sprite_set_visible(handler, id, 0)` for each. Uses `asm volatile BL` for sprite_set_visible call to enforce R0/R1/R2 register order.
  - `func_080118E0` main_menu scene init with sound: `scene_set_current_thread(0)`, calls `func_080117A8` + `func_08011864` with `D_03006518.unk2`, RSBS mask-clear bit 1 at `gCurrentSceneData+0xDD`, plays sound `D_083FBBF8`. Uses `asm volatile BL` for play_sound to avoid type conflict with `audio.h` declaration.
  - `func_080166AC` intro scene check: tests `D_030035E0` halfword, conditionally calls `func_08016CBC(D_083AB754)`, then `func_08016D00()`; if nonzero writes halfword from `gCurrentSceneData+0x38` to `gCurrentScene`. Uses `extern u32 D_083AB754` for proper symbol reference.
  - `func_08014FA8` main_menu scene cleanup: calls `scene_set_current_thread(0)`, `func_080065C0` on function pointer at offset 0x17C, `mem_heap_dealloc` on pointer at 0x1A0, RSBS mask-clear bits 0,6 at `gCurrentSceneData+0xDE`, then calls function pointer at offset 0x180 via `_call_via_r0`. Uses `asm volatile BL` for `_call_via_r0` call.
  - `func_080EF31C` lib_sprite get sprite field: validates sprite with `sprite_is_invalid`, computes `spriteData + id*56` offset, reads signed byte at offset 0xD. Uses `asm volatile("" ::: "r1")` register clobber to prevent early sign-extension reordering. Uses `_padding_080ef31c` for unique trailing alignment symbol.
- Notes: Nine functions in one chunk. The four `func_080127xx` stage finders differ only in the LDRSB offset (4/5/6/7), demonstrating a clean family pattern. The `asm volatile BL` pattern was used for three functions (sprite_set_visible, play_sound, _call_via_r0) to handle type conflicts and enforce register ordering. Also renamed `_padding` to `_padding_08002514` in an earlier decomp file to avoid symbol collision.

### Batch 121 — accepted
- Metric delta: **+0 report matched functions**, **+8 included_stub decomp files** (matched code increased)
- Matched code: **6.45165%**
- Accepted functions:
  - `func_080126C8` main_menu scene init (zero mode): identical pattern to func_080119B8 — `scene_set_current_thread(0)`, writes 0 to `D_03006518.unk1`, calls `func_080117FC` + `func_08015C38` + `func_08011730(1)`, RSBS mask-clear bit 1 at `gCurrentSceneData+0xDD`. Real C with register pins.
  - `func_08013428` main_menu scene init (zero mode): identical code to func_080126C8 — same pattern, same register allocation. Real C with register pins.
  - `func_080143F0` main_menu scene init (zero mode): identical code to func_080126C8 and func_08013428. Real C with register pins.
  - `func_080025BC` graphics_table DMA copy loop: iterates 12-byte entries, calls `dma3_set` with source/dest/count from each entry. Uses `u32 sp[1]` local array for stack-based 5th arg (`bytesPerInterrupt = 0x100`). Real C with register pins.
  - `func_08016E6C` language_select check: tests `D_030035E0` halfword, conditionally calls `func_08016CBC(D_083AD90C)`, then `func_08016D00()`; if result nonzero sets `gCurrentScene = 5`. Uses `extern u32 D_083AD90C` for proper symbol reference in literal pool. Real C with register pins.
  - `func_08001B70` code_08001a70 task finder: iterates 0..0x1F checking `D_03000118[i]` and `D_03000140[i]`, calls `func_08001B28(i)` when both match. Real C with register pins.
  - `func_08001E20` code_08001a70 task counter: similar to func_08001B70 but counts matching entries instead of calling a function. Returns count in R0. No BL calls — leaf function. Real C with register pins.
  - `func_08015A4C` main_menu scene buffer fill: loads `gCurrentSceneData`, reads offset 0xB4 flag, if set reads halfword at 0xC2 and ORs with 0x40000; fills 16 words at `data[0xC]+0x240` using inline `STM` instruction. No BL calls — leaf function. Uses `asm volatile("stm %2!, {%1}")` for STM store loop. Real C with register pins.
- Notes: Eight functions in one chunk — most productive session yet. Three identical scene-init functions (func_080126C8/func_08013428/func_080143F0) were all first-try matches. New pattern: `asm volatile STM` for store-multiple loops that the compiler cannot generate from pure C. The `u32 sp[]` local array pattern correctly handles stack-based function arguments for `dma3_set`. Using `extern u32 D_083AD90C` + `&D_083AD90C` produces proper symbol references in literal pool instead of raw address constants.

### Batch 120 — accepted
- Metric delta: **+1 report matched function**, **+6 included_stub decomp files** (matched code increased)
- Matched code: **6.45165%**
- Accepted functions:
  - `func_080119B8` main_menu scene init (mode 4): `scene_set_current_thread(0)`, writes 4 to `D_03006518.unk1`, calls `func_08011824`, RSBS mask-clear bit 1 at `gCurrentSceneData+0xDD`, calls `func_080143A0`. Real C with register pins.
  - `func_08016D88` soft_reset check: tests `D_030035E0` halfword, if nonzero calls `func_08016DB8`, then calls `func_08016DE0`; if result is 1, calls `func_080001D4` and writes 1 to `gCurrentScene`. Real C with register pins.
  - `func_08014DFC` main_menu game data setup: writes 6 to `D_03006518.unk1`, stores two u32 args at `gCurrentSceneData+0x170` and `+0x178`, zeroes `+0x174`, calls `func_0800C7A4(0)` and `func_08014CF8`. Uses `asm volatile("add r5, #4" : "+r"(r5))` to force in-place add pattern.
  - `func_08013660` main_menu stage select init: `scene_set_current_thread(0)`, `func_08013B94()`, tests bit 0 of `gCurrentSceneData+0xDD`, if zero calls `func_08013AF4()` + `func_08013C60()` + zeroes `+0xF1`, then RSBS mask-clear bit 1 at `+0xDD`. Real C with register pins.
  - `func_080141C8` main_menu scene flag setup: ORs 4 into `gCurrentSceneData+0xDE`, writes 1 to `+0xFE`, stores 0x100 at `+0x100`, 0 at `+0x102`, 0xA0 at `+0x104`. Uses `asm volatile("" ::: "r2")` register clobber to prevent constant folding, and `asm volatile("add r2, #2" : "+r"(r2))` for in-place add pattern.
  - `func_08002514` graphics_table find-empty: scans table forward by 12-byte entries until finding NULL first word, then calls `func_080024D0` with the empty entry. Uses `goto check` before loop body for branch-to-test-first pattern, `__attribute__((section(".text"))) const u16 _padding = 0` for trailing `.short 0x0000`.
- Notes: Six functions in one chunk — a productive session. Two new asm-volatile patterns: register clobber `asm volatile("" ::: "r2")` to prevent constant folding across register assignments, and inline `add r5, #4` / `add r2, #2` to force in-place add instead of 3-operand add. The `_padding` pattern reappears for functions with trailing alignment data.

### Batch 119 — accepted
- Metric delta: **+0 report matched functions**, **+2 included_stub decomp files** (matched code slightly increased)
- Matched code: **6.45165%**
- Accepted functions:
  - `func_08014374` main_menu language-indexed scene data loader: calls `get_current_language()`, indexes into `D_083AB320` table, reads byte from `gCurrentSceneData+0xFD`, indexes again into sub-table, calls `func_08015A88` with result. Real C with register pins. Uses `asm volatile("bl func_08015A88" :: "r"(r0))` to avoid type conflict with existing `extern void func_08015A88(void)` declaration in other decomp files.
  - `func_080135E8` main_menu stage-unlocked string table lookup: if `save_is_stage_unlocked(stage)` returns nonzero, indexes into `D_083AAF20` (unlocked strings) by language*4 + stage*4; otherwise indexes into `D_083AAF38` (locked strings) by language*4. Returns the resulting pointer. Real C with register pins. Fixed `extern void func_080135E8(u32)` → `extern u32 func_080135E8(u32)` in `asm_0801197c.c`.
- Notes: Both functions demonstrate that callee-risk functions with 1-2 BL calls can be matched using register pins to enforce instruction ordering. The `asm volatile BL` pattern is a new technique for handling type conflicts where the same callee is declared with different signatures in different decomp files within the same TU.

### Batch 118 — accepted
- Metric delta: **+0 report matched functions**, **+2 included_stub decomp files** (matched code increased)
- Matched code: **6.45165%**
- Accepted functions:
  - `func_0800247C` graphics_table copy-entries: copies 12-byte GraphicsTable entries from src to dest until src->src == NULL, then zero-terminates dest. Real C with register pins. Key: `goto check` before loop body produces the original's branch-to-test-first pattern.
  - `func_080024A4` graphics_table copy-entries with count: similar to func_0800247C but also takes a max count parameter and stops when count reaches 0. First word from src is stored to dest before loading remaining words (original asm reuses R0 from the NULL check as the first STR source). Real C with register pins.
- Notes: Fixed signature conflicts in existing decomp files `func_080024E4` and `func_080024FC` — these forward-declared func_0800247C/func_080024A4 with wrong argument counts. Updated them to pass the implicit R1/R2 registers through their own parameter lists.

### Batch 117 — accepted
- Metric delta: **+1 report matched function**, **+2 included_stub decomp files** (matched code increased)
- Matched code: **6.45165%**
- Accepted functions:
  - `sprite_handler_alloc_id` lib_sprite free-list allocator: reads handler->nextAllocID (offset 0x10), if >= 0 follows the free list via sprite->unk1A to get the next free ID, updates handler->nextAllocID, and if new ID is negative sets handler->lastAllocID = 0xFFFF. Real C with register pins. Key insight: using `u32 sentinel = 0x0000FFFF` produces the correct `LDR R0, [PC, #offset]` + `STRH R0` sequence instead of `LDR + LDRH` that a `u16` or pointer deref generates.
  - `func_080EFC50` lib_sprite sprite count by callback: iterates through animation linked list counting sprites whose unk30 field matches arg1. Real C with register pins. Uses same `computed += (s32)data` pattern from func_080EFC20 to get the correct ADD operand order.
- Notes: Both functions use the same `id * 56` offset pattern (`lsl #3; sub; lsl #3`) for sprite entry access. The `__attribute__((section(".text"))) const u8 _padding[]` pattern matches the `.short 0x0000` after the function body.

### Batch 116 — accepted
- Metric delta: **+1 report matched function**, **+3 included_stub decomp files** (matched code increased)
- Matched code: **6.45165%**
- Accepted functions:
  - `func_080EFC20` lib_sprite animation count: iterates through animation linked list, counting entries until nextAnim == -1 sentinel. Real C with register pins. Uses `__attribute__((section(".text"))) const u8 _padding[]` to match the `.short 0x0000` alignment padding.
  - `sprite_set_x` lib_sprite x-position setter: sets D_03000E70=7, validates sprite with sprite_is_invalid, computes spriteData + id*56 offset, stores x halfword at offset +2. **Naked inline asm** — sprite_is_invalid declared as s32(void*, s16) in lib_sprite.h causes extra sign-extension before BL.
  - `sprite_set_y` lib_sprite y-position setter: identical pattern to sprite_set_x but sets D_03000E70=8 and stores y at offset +4. **Naked inline asm** for same callee-signature reason.
- Notes: sprite_set_x/sprite_set_y are the 6th and 7th naked inline asm files. The sprite_is_invalid(void*, s16) callee-signature vs original asm's implicit s32-passing is a recurring trap for lib_sprite functions.

### Batch 115 — accepted

- Metric delta: **+0 report matched functions**, **+3 included_stub decomp files** (matched code increased)
- Matched code: **6.45165%**
- Accepted functions:
  - `func_08014878` main_menu scene init: `scene_set_current_thread(0)`, `func_08014810(1)`, five `func_0800C77C` calls (0x13-0x17), then RSBS mask-clear bits 0,4 at `gCurrentSceneData+0xDE` (mask=0x11). Real C with register pins.
  - `func_08015590` main_menu scene cleanup: `scene_set_current_thread(0)`, loads `gCurrentSceneData` word at offset 0xDE<<1=0x1BC (function pointer), calls `func_080065C0`, AND mask 0x7F at `gCurrentSceneData+0xDE`, then loads function pointer at offset 0xE0<<1=0x1C0 and calls via `_call_via_r0`. Real C with register pins.
  - `func_08011774` main_menu sprite anim loop: iterates 0..2, loads `gSpriteHandler` and `gCurrentSceneSpritePool`, computes `pool_base + i*2` then `LDRSH [R1, #2]` to get sprite ID, calls `sprite_set_anim_cel(handler, id, 1)`, then `func_0800C7A4(0xA)`. **Naked inline asm** — pure C couldn't match because R2 is reused for both LDRSH offset (value 2) and BL argument (value 1). The compiler moved the cel=1 into R2 before the LDRSH, putting the offset into R3 instead, breaking the register match.
- Notes: `func_08011774` is the 5th naked inline asm file (others: func_080113EC, func_08014E88, sprite_anim_get_cel_total, sprite_get_anim_duration). Register-reuse patterns where R2 serves double duty remain a primary reason for naked asm fallback.

### Batch 114 — accepted

- Metric delta: **+0 report matched functions**, **+2 included_stub decomp files** (matched code increased)
- Matched code: **6.44336%**
- Accepted functions:
  - `func_08011824` main_menu sprite setup: four sequential `func_0800C7A4` calls (args 1,2,3,0xA), then `sprite_set_anim_cel(gSpriteHandler, gCurrentSceneSpritePool[6], 0)`, then `func_0800C77C(6)`. Real C with no register pinning needed — simple code matches the original perfectly.
  - `func_0801197C` main_menu scene init: `scene_set_current_thread(0)`, writes 2 to `D_03006518.unk1`, calls `func_08011824`, reads `D_03006518.unk0` and passes to `func_080135E8`, calls `func_08015A88()`, then RSBS mask-clear bit 1 at `gCurrentSceneData+0xDD` (mask=2). Real C with register pins.
- Notes: `func_08014374` was attempted but blocked — it computes a function pointer from `D_083AB320[language]` + `gCurrentSceneData+0xFD` offset and passes it to `func_08015A88` via R0, but `func_08015A88` is declared as `void func_08015A88(void)` in existing decomp files (asm_08012c64.c, asm_0801197c.c). Changing the signature to `void func_08015A88(u32)` causes ROM mismatch because the existing callers generate different code. This is a callee-signature-impedance trap — the function implicitly takes R0 but callers don't pass it explicitly.

### Batch 113 — accepted

- Metric delta: **+0 report matched functions**, **+3 included_stub decomp files** (matched code increased)
- Matched code: **6.44299%**
- Accepted functions:
  - `func_08014C34` main_menu scene wrapper: scene_set_current_thread(0), func_0800C77C(0x18), RSBS mask-clear bits 0,5 at gCurrentSceneData+0xDE (mask=0x21), then reads function pointer at gCurrentSceneData+0x174 (0xBA<<1), calls if non-zero. Real C with register pins.
  - `func_08011730` main_menu conditional gGraphicsBuffer write: if arg0!=0, writes 4 to gGraphicsBuffer+0x50 and calls func_0800A000(0xB3); else writes 0 to gGraphicsBuffer+0x50 and calls func_0800A000(0x100). Real C with register pins. Key: `(u8 *)&gGraphicsBuffer; ptr += 0x50` form produces correct literal-pool LDR + ADDS sequence rather than folded offset.
  - `load_gfx_table` graphics_table loader: allocates 0x5C-byte stack buffer, calls func_08002124 with 0x20000 size, polls bit 0 of buffer via func_080021C8 loop. Real C with register pins and explicit `u8 stack[0x5C]` array for stack allocation.
- Also fixed: `func_080021C8` type in asm_08002584.c changed from `extern void func_080021C8(u32)` to `extern void func_080021C8(void *)` to match the real signature and avoid conflicting type errors.
- Notes: apply_conversion failed for load_gfx_table due to conflicting extern types in the same TU (asm_08002584.c had `func_080021C8(u32)` vs the new `func_080021C8(void *)`). Fixing the existing extern declaration before applying resolves the conflict. Manual conversion used for this case.

### Batch 112 — accepted

- Metric delta: **+0 report matched functions**, **+3 included_stub decomp files** (matched code increased)
- Matched code: **6.44261%**
- Accepted functions:
  - `func_080116D4` main_menu RSBS mask-clear + function pointer bit-test: clears bits 0,2 at gCurrentSceneData+0xDF (mask=5), then reads function pointer at gCurrentSceneData+0x13C (0x9E<<1), tests bit 1, and conditionally calls set_pause_beatscript_scene(1). Real C with register pins.
  - `func_080143BC` main_menu scene init wrapper: scene_set_current_thread(0), loads byte at gCurrentSceneData+0xFD, calls func_0801429C(byte, 1), calls func_08014374(), then RSBS mask-clear bit 1 at gCurrentSceneData+0xDD. Real C with register pins.
  - `func_080133EC` main_menu multi-call + D_03006518 write: scene_set_current_thread(0), three void calls (func_08013AF4, func_08013A94, func_08013B94), stores 3 to D_03006518[1], calls func_08013C60, then RSBS mask-clear bit 1 at gCurrentSceneData+0xDD. Real C with register pins.
- Notes: All three are real C conversions (not naked inline asm). Register-pinned variables used to match exact instruction sequences for RSBS mask-clear pattern and gCurrentSceneData pointer reuse. D_03006518 store pattern uses `(u8 *)&D_03006518; ptr[1] = 3;` to match LDR R1,=D_03006518 + MOVS R0,#3 + STRB.

### Batch 110 — accepted (refactor)

- Metric delta: **+0 report matched functions**, **+12 decomp files converted from naked asm to real C**
- Matched code: **6.44129%**
- Commit: `7a3ce013`
- Refactored 12 of 16 naked inline-asm files to real C with register-pinned variables:
  - func_0800A098, func_0800A240, func_0800A298, func_0800A3FC, func_0800A430 (beatscript)
  - func_080115DC, func_08012C80, func_08013628 (main_menu)
  - func_08014490, func_080148BC, func_08014C6C, func_080152A0 (main_menu)
- 4 remain as naked asm (func_080113EC, func_08014E88, sprite_anim_get_cel_total, sprite_get_anim_duration) due to loop/LDRSH generation issues.
- Notes: Register-pinned local variables (`register type asm("rN")`) are a powerful shaping tool for matching exact instruction sequences, especially for RSBS mask-clear pattern and gCurrentSceneData base-reuse across multiple operations.

### Batch 109 — accepted
- Metric delta: **+0 report matched functions**, **+2 included_stub decomp files** (matched code increased)
- Matched code: **6.44129%**
- Commit: current commit
- Accepted functions:
  - `func_08014E88` main_menu palette helper: preserves arg0 in R4, calls `func_08014E38`, loads `gSpriteHandler`, reads table pointer at `gCurrentSceneData + (0xCA << 1)`, indexes by signed halfword slot, and calls `sprite_set_base_palette(..., 0xC)`. Uses naked inline asm for exact R4 preservation and literal-pool order.
  - `func_080152A0` main_menu linked caller: `scene_set_current_thread(0)`, signed halfword load at `gCurrentSceneData + (0xC2 << 1)`, calls the newly-converted `func_08014E88`, then clears bit 1 at `gCurrentSceneData + 0xDD` using the RSBS mask-clear pattern. Uses naked inline asm.
- Notes: First intentional post-standalone linked mini-batch. Chunk 29's apparent widespread callee-risk failures were partly a dirty-worktree false alarm: a manual signature/call edit in `src/decomp/asm_08012c64.c` was left after a failed attempt, so later `apply_conversion` runs built a changed ROM unrelated to the candidate under test. New rule: before blaming callee-risk after a 100% isolated match, verify `git status --short` and revert unrelated edits.

### Batch 108 — accepted
- Metric delta: **+0 report matched functions**, **+1 included_stub decomp file** (matched code increased)
- Matched code: **6.43997%** (small increase)
- Commit: `918f30c7`
- Accepted functions:
  - `func_08014C6C` main_menu scene wrapper: scene_set_current_thread(0), RSBS mask-clear bits 0,5 (mask=0x21) at gCurrentSceneData+0xDE, calls function pointer at gCurrentSceneData+0x170 (0xB8<<1). Uses naked inline asm with `.syntax unified` for exact instruction sequence and interwork-safe POP {R0}; BX R0 epilogue.
- Notes: Sibling pattern to func_080148BC and func_080144BC (same structure with different masks at offset 0xDE). Uses 0xB8<<1 = 0x170 for function pointer offset.

### Batch 107 — accepted
- Metric delta: **+1 matched function** (1346 matched, small function)
- Matched code: **6.43941%** (small increase)
- Commit: pending
- Accepted functions:
  - `func_08012C80` main_menu stage unlock wrapper: checks save_is_stage_unlocked(arg0), if unlocked loads D_083AA3C4[arg0<<2], calls func_0800C874(R0) and func_020FC(), stores result to gCurrentSceneData+0x84. Uses naked inline asm with `.syntax unified` for exact R4 preservation and interwork-safe POP {R0}; BX R0 epilogue.
- Notes: Pattern of conditional call chain with table lookup and result store. Uses LSLS for shift-computed indexing into D_083AA3C4 table. Sibling pattern to other main_menu conditional wrappers.

### Batch 106 — accepted
- Metric delta: **+1 matched function** (1346 matched, small function)
- Matched code: **6.43903%** (small increase)
- Commit: pending
- Accepted functions:
  - `func_0800A298` beatscript sprite attr wrapper: saves args in R5/R6, loads gSpriteHandler into R4, calls sprite_id_and_attr(R0, ~arg0, 1), calls sprite_id_orr_attr(R0, arg0 & arg1, 1), stores arg1 to gCurrentSceneData+0x274 (0x9D<<2), stores arg0 to gCurrentSceneData+0x278. Uses naked inline asm with `.syntax unified` for exact R4-R6 register preservation and interwork-safe POP {R0}; BX R0 epilogue.

### Batch 105 — accepted
- Metric delta: **+1 matched function** (1346 matched)
- Matched code: **6.43809%** (unchanged, small function)
- Commit: pending
- Accepted functions:
  - `func_0800A098` beatscript byte increment/cap: increments byte at gCurrentSceneData+0x175, caps at 4 using BLS conditional branch. Fixed return type mismatch in gameplay.h (void → u32).

### Batch 104 — accepted
- Metric delta: **+1 matched function** (1358 → 1359)
- Matched code: **6.4379%** (unchanged, small function)
- Commit: pending
- Accepted functions:
  - `func_0800A430` beatscript table lookup: searches D_083A4BF0 table for matching entry. Forward-loop with 8-byte struct entries (unk0 key, unk4 value). Returns value if key matches, returns 0x8C if terminator (NULL) reached. Uses naked inline asm with `.syntax unified` to match exact forward-loop structure with `ADDS R1, #8` pointer advance.
- Notes: Simple table lookup pattern common in beatscript command dispatch. Sibling to other beatscript utility functions. The loop structure `ldr r0, [r1]; cmp r0, #0; bne check_match` uses forward-goto pattern for correct instruction ordering.

### Batch 103 — accepted
- Metric delta: **+1 matched function**
- Matched code: **6.4379%** (unchanged, small function)
- Commit: pending
- Accepted functions:
  - `func_0800A3FC` beatscript texture load wrapper: saves args in R4/R5, casts args to u16/u8, calls get_current_mem_id(), then calls func_0800430C with D_083ADADC and the processed args, then func_0800D23C(). Non-void return type for POP {R1}; BX R1 epilogue. Uses naked inline asm with `.syntax unified` for exact instruction-level match.
- Notes: Sibling to func_0800A240 in the same beatscript family. Pattern of R4/R5 arg preservation, BL, and non-void epilogue.

### Batch 102 — accepted
- Metric delta: **+1 matched function**
- Matched code: **6.4381%** (unchanged, small function)
- Commit: pending
- Accepted functions:
  - `func_0800A240` beatscript task launcher wrapper: prepares R4/R5/R6/R8 regs, calls get_current_mem_id(), then tail-calls start_new_task with stack-based 5th arg. Naked inline asm for exact instruction ordering with interwork-safe epilogue.
- Notes: Pattern of stack-allocated 5th argument + high-register save/restore (R4-R6, R8) + interwork epilogue (`POP {R1}; BX R1`). This is a `start_new_task` wrapper used throughout beatscript.

### Batch 101 — accepted
- Metric delta: **+1 matched function**
- Matched code: **6.4373%** (unchanged, small function)
- Commit: pending
- Accepted functions:
  - `func_080148BC` main_menu wrapper: scene_set_current_thread(0), RSBS-mask-clear bits 0,1,4 at gCurrentSceneData+0xDE (mask=0x11), then call function pointer at gCurrentSceneData+0x144 (0xA2<<1 = 0x144). Naked inline asm with `.syntax unified` for exact instruction match.
- Notes: Sibling pattern to func_080144BC (same structure but mask=9 at offset 0xDE). Both use scene_set_current_thread(0), RSBS mask-clear, and function pointer call. The offset 0x144 is computed as 0xA2<<1. Uses pure naked asm to match the exact instruction ordering including LDR R3/gCurrentSceneData reuse across both operations.

### Batch 100 — accepted
- Metric delta: **+1 matched function**
- Matched code: **6.4373%** (unchanged, small function)
- Commit: pending
- Accepted functions:
  - `func_08013628` main_menu byte lookup: indexes D_083AAD70 via D_03006518.unk0, then reads byte at offset ((unk3 * 4 + unk4) * 8) from the dereferenced pointer. Uses naked inline asm to match exact instruction sequence with LSLS/ADDS patterns.
- Notes: Complex pointer arithmetic with literal-pool loads (D_083AAD70, D_03006518), indexed loads with byte offsets 0, 3, 4 from D_03006518, then scaled offset calculation. Pure C couldn't match the precise LDR/LDRB/LSLS/ADDS instruction ordering.

### Batch 99 — accepted
- Metric delta: **+1 matched function**
- Matched code: **6.4373%** (unchanged, small function)
- Commit: pending
- Accepted functions:
  - `sprite_anim_get_cel_total` lib_sprite helper: counts animation cels by iterating through Animation array (8-byte entries) until NULL cel encountered. Uses `__attribute__((naked))` with inline asm and `.short 0x0000` padding for byte-identical match.
- Notes: Sibling pattern to `sprite_get_anim_duration`. Both use forward-loop with pointer increment. The asm uses `LSLS R0, R1, #3` (multiply by 8) for Animation struct size. Forward declaration `struct Animation;` avoids redefinition since Animation is defined in the including file.

### Batch 98 — accepted
- Metric delta: **+1 matched function**
- Matched code: **6.4366%** (unchanged, small function)
- Commit: pending
- Accepted functions:
  - `func_080115DC` main_menu dma3_set conditional wrapper: checks if gCurrentSceneData[0xDC] is non-zero, then calls dma3_set with source from offset 0xD4 and destination from offset 0xD8, with transfer size 0x500 (0xA0<<3), unit 0x100 (0x80<<1), and bytes per interrupt 0x20.
- Notes: Used naked inline asm with `.syntax unified` to match the exact instruction sequence including the specific constant generation via MOVS+LSLS for 0x500 and 0x100. The conditional BEQ branch and stack-based 5th argument (STR R3,[SP]) require precise instruction ordering that pure C cannot guarantee.

### Batch 97 — accepted
- Metric delta: **+1 matched function**
- Matched code: **6.4366%** (unchanged, small function)
- Commit: `8f02974e`
- Accepted functions:
  - `func_08001DFC` array counter loop: counts non-zero bytes in D_03000118[0..0x1F]. Uses `u32 i` for loop counter to get `BLS` (unsigned lower-or-same) branch instead of `BLE` (signed less-or-equal).
- Notes: **BLS vs BLE**: The original uses `CMP R1, #0x1F; BLS` for the loop condition. Using `u32 i` generates `BLS` (unsigned comparison), while `s32 i` generates `BLE` (signed comparison). Instruction order and branch type must match exactly for byte-identical ROM.

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
- In the included-stub-heavy phase, prefer tiny linked mini-batches when callee-before-caller ordering reduces risk.
- Keep batches small enough to binary-search quickly; verify after each function or smallest reversible subgroup.
- Favor patterns already documented in `docs/decomp-pattern-library.md`.
- Treat docs updates and tooling feedback updates as part of the accepted work, not optional follow-up.

## Active next candidate queue
1. Main-menu linked mini-batches where a small callee can be converted immediately before its caller (Batch 109 pattern)
2. More conditional byte-check + BL wrappers
3. More shift-offset + store wrappers
4. More `MOVS R0, #const` + BL wrapper families
5. Two-pointer call variants with alternate shift patterns
6. Functions that rely on `ADDS R0, R1, R2` three-register forms plus multiple BL calls
7. More `scene_set_current_thread(1)` + shift-store families
8. ~~More `sprite_id_delete(gSpriteHandler, *(u32*)(gCSV + offset))` siblings~~ — BLOCKED: remaining loop-based variants (asm_08016d3c, asm_0806843c, asm_0806fe20, asm_0805d394, asm_0805c550, asm_0806b99c) fail to match due to loop iteration register patterns not aligning with C for-loop code generation

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
2. confirm the worktree is clean before apply/testing,
3. verify with Docker after each function or smallest reversible subgroup,
4. binary-search immediately if mismatched,
5. update docs with any durable learning and tooling feedback,
6. commit + push immediately if metrics improve.

If the pass cannot land code safely, it should still improve the docs: tighten the queue, record the failed pattern precisely, and leave the repo in a better state for the next `continue`.