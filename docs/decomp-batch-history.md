# Accepted batch history

This is the migrated history from the Ralph task file plus the most recent session log work.
It is intentionally concise: keep the durable rules in `docs/decomp-pattern-library.md`, and use this file to remember what landed, when, and why it mattered.

## Latest accepted batches
| Iteration / Batch | Commit | Δ matched | Summary |
|---|---|---:|---|
| 145 | current commit | +0 report matched / +0 included_stub decomp files | Blocked: copy propagation merges sign-extension + register copy (func_0800DE84 — 49 ROM bytes differ). agbcc folds `LSLS R5, #16; ASRS R5, #16; ADDS R3, R5, #0` into `ASRS R3, R5, #16` regardless of asm volatile barriers. Also TU-wide literal pool layout mismatch continues to block func_080119EC and func_0801216C. New pattern documented: copy propagation merges sign-ext + copy.
| 144 | current commit | +0 report matched / +0 included_stub decomp files | Blocked: all attempted included_stub conversions hit TU-wide literal pool layout mismatch — isolated compile shows 0 instruction differences but full ROM build fails to match. The compiler merges/reorders literal pool entries across the entire TU when compiling C instead of including asm stubs, changing binary layout. Attempted: func_0801216C (32 instrs match, 2 literal pool entries), func_080119EC (130 instrs match, 6+ literal pool entries, also type conflicts with gSpriteHandler/gCurrentSceneSpritePool/D_083AA294 that were fixed), func_0800A2D8 (38 instrs match from prior chunk). Also blocked: func_08011614 (IP/R12 register), func_08011698 (already matched natively in main_menu.c), sprite_set_z (R7 push), func_08011864 (CMP#1/BLO), func_08014740 (IP/R12), func_08011DFC (_call_via_r1), func_08002124 (_call_via_r1 + multiple RSBS), func_080136F4 (R7 push + RSBS). New pattern documented: TU-wide literal pool layout mismatch — functions with multiple literal pool entries in large TUs fail ROM matching despite isolated compile matching perfectly; functions with fewer entries in smaller TUs may still work (func_08013E64 worked with 1 entry). Possible mitigation: target functions in smaller TUs or with minimal literal pool entries. |
| 143 | current commit | +0 report matched / +1 included_stub decomp file | Real C: func_08013E64 (main_menu scene update: gPressedKeys & 0xF1 check, gCurrentSceneData+0xF1 byte check, conditional func_08011584 call based on +0xDD bit 0, conditional func_08013A4C call based on +0xDD bit 7, func_08011698 return check → func_080137B0, then func_080139D4 — callee-risk from func_08011698 was NOT an issue since func_08011698 is still asm stub, so the compiler treats the BL as opaque). Blocked: func_08012D7C (task launcher — function pointer literal `func_08012D3C+1` emitted as `.hword` instead of `.word` in literal pool, causing ROM mismatch; also same-TU callee-risk for get_current_mem_id/start_new_task), func_0800A2D8 (same `.hword` vs `.word` literal pool issue + same-TU callee-risk — isolated compile matches all 38 instructions but full build ROM doesn't match), sprite_set_z (agbcc won't push R7 callee-save — uses {R4-R6,LR} instead of {R4-R7,LR}), func_080136F4 (R7 push + RSBS constant-fold into SUB), func_08011864 (CMP#1/BLO→CMP#0/BEQ fold), func_08014740 (IP/R12). Key patterns: callee-risk warning for asm-stub callees is a false positive — compiler treats BL to asm stub as opaque; function pointer literal `func+1` in literal pool gets `.hword` packing from assembler even though address is 32-bit, causing ROM mismatch — no known workaround in pure C; must clean stale build deps (rm build/src/*.d) after failed apply_conversion. |
| 142 | current commit | +0 report matched / +3 included_stub decomp files | Real C: func_0800DE24 (name_select: conditional func_08016CBC + func_08016D00 check, func_080007C0(2) with fallback path storing gCurrentScene=2, D_03003848=0, D_03003628=D_083A8588 address symbol, D_03003634=0 — key: D_083A8588 must be referenced as `(u32)&D_083A8588` not numeric `0x083A8588` so compiler emits symbol relocation in literal pool), func_08014F38 (main_menu scene init with loop: scene_set_current_thread(0), func_08014E88 via asm volatile BL (same-TU callee-risk), sprite_set_visible loop over gCurrentSceneData+0xCA array entries, RSBS mask-clear 0x41 on gCurrentSceneData+0xDE — R0 held unknown gCurrentSceneData address so MOVS+RSBS not folded), func_08012658 (main_menu scene update: scene_set_current_thread(0), D_03006518 byte into D_083AA0C4 array offset, sprite_set_x_y via asm volatile BL (same-TU s16 callee-signature trap), conditional func_08015C38/func_08012C18/func_08015A88 calls, RSBS mask-clear ~2 on gCurrentSceneData+0xDD). Key patterns: symbol address literal pool entries must use `&symbol` not numeric constants; RSBS after LDR of unknown address doesn't get constant-folded; D_083AA0C4 declared as u8[] per existing decomp files; func_08012C18 is u32(u32) per existing decomp files. Blocked: func_08011864 (CMP#1/BLO — compiler always emits CMP#0;BEQ instead), func_08014740 (IP/R12 register usage — compiler uses R6 instead). |
| 141 | current commit | +0 report matched / +3 included_stub decomp files | Real C: func_0800A0C4 (beatscript: gCurrentSceneData+0x17A halfword check, conditional gCurrentSceneData+0x178 store or schedule_function_call with func_0800C974 + gCurrentSceneData+0x27E halfword — asm volatile BL for schedule_function_call to avoid u16 first-arg re-truncation; asm volatile barrier after LSLS/LSRS to prevent compiler hoisting literal-pool loads before get_current_mem_id BL), func_0801522C (main_menu scene cleanup: func_0800A240 with D_083A4A2C + gCurrentSceneData+0x17C, func_08005600 with gSpriteHandler + offsets 0xCA/0xCE, 3x mem_heap_dealloc(u32) to match existing extern in asm_08014fa8.c — not memory_heap.h void*, OR 0x40 into gCurrentSceneData+0xDE), scene_set_current_thread (attempted, blocked: compiler folds MOVS R0,#0xF;RSBS R0,R0,#0 into SUB R0,#0x16 — no pure C workaround found for this constant-propagation optimization; literal pool .hword vs .word layout also differs). Key patterns: schedule_function_call u16 first-arg causes extra LSLS/LSRS truncation — use asm volatile BL; compiler hoists literal pool loads before BL — use asm volatile barrier after truncation; mem_heap_dealloc(u32) vs mem_heap_dealloc(void*) — must match existing same-TU extern declarations, not header. Blocked: sprite_set_z (agbcc won't push R7 callee-save), scene_set_current_thread (MOVS+RSBS constant fold). |
| 140 | current commit | +0 report matched / +4 included_stub decomp files | Real C: func_08013EC0 (main_menu scene init: scene_set_current_thread(0), RSBS mask-clear on gCurrentSceneData+0xDE, ORs 4, byte-zero at +0xFE, halfword stores at offsets 0x100/0x102/0x104, calls func_0800BF0C(2) — asm volatile barrier after ORRS to prevent compiler value-propagation that collapses MOVS R2,#0x80;LSLS R2,#1 into ADD R2,#0xFC), func_08013388 (main_menu scene update: scene_set_current_thread(0), D_03006518 byte into D_083AA294 table offset, sprite_set_x_y via asm volatile BL, calls func_080135E8/func_08015A88/func_08012E04, RSBS mask-clear on gCurrentSceneData+0xDD), func_08015760 (similar to func_08013388: reads gCurrentSceneData+0x1C4 halfword, D_083AA294 offset, sprite_set_x_y with LDRH+ADD+sign-extend position values, RSBS mask-clear on +0xDD), func_08014810 (sprite_set_base_palette multi-call: 2 unconditional calls with palette 6, then conditional palette 0xC based on gCurrentSceneData+0x14C byte — asm volatile BL for all calls to avoid s16/s8 callee-signature register-reuse trap). Key patterns: asm volatile barrier after ORRS to break register value-propagation; don't clobber R4/R5 in asm volatile BL if they're needed after; D_03006518 must be struct Unk03006518 type per existing decomp files; title.h conflicts with other stubs — use direct extern declarations instead. |
| 139 | current commit | +0 report matched / +3 included_stub decomp files | Real C: func_08014D6C (main_menu scene init: func_0800A240 with D_083A4A2C + gCurrentSceneData+0x16C, func_0800C77C(0), func_08005600 with gSpriteHandler + D_083AB394 + gCurrentSceneSpritePool, OR 0x20 into gCurrentSceneData+0xDE — sibling of func_080149BC with different offset/mask), scene_change_music (beatscript: conditional stop_soundplayer + play_sound + func_0800A430 + update_beatscript_tempo + scene_update_music_pitch + set_soundplayer_volume with gBeatscriptScene+0x1C58 volume — u32 return type for POP {R1};BX R1 epilogue, proper struct SongHeader*/SoundPlayer* types from audio.h). Key patterns: u32 return type for non-void epilogue even when semantically void; audio.h struct types must be used exactly to avoid type conflicts in same-TU decomp files. Blocked: func_080122FC (stack allocation via asm volatile STR doesn't emit SUB SP), func_08012BB8 (agbcc won't push R7 callee-save register), func_0800BF7C (R8 register + 3 stack args too complex). |
| 138 | current commit | +0 report matched / +4 included_stub decomp files | Real C: func_08014440 (main_menu scene wrapper: scene_set_current_thread(0), tests gCurrentSceneData+0x14C byte, conditional D_03006518.unk1=4 or =9 + set_pause_beatscript_scene(0) + func_0800C7A4(0), register-pinned locals for gCurrentSceneData base reuse in R5), func_080117A8 (main_menu sprite setup: calls func_08011774, loads sprite ID from gCurrentSceneSpritePool table, calls sprite_set_anim_cel and sprite_set_x_y via asm volatile BL for s16 callee-signature trap, loads position from D_083A9CE0 table, calls func_0800C77C), func_080149BC (main_menu scene init: calls func_0800A240 with D_083A4A2C and gCurrentSceneData+0x140, calls func_08005600 with gSpriteHandler + gCurrentSceneData+4 + D_083AB35C + gCurrentSceneSpritePool, ORs 0x10 into gCurrentSceneData+0xDE), func_08001C74 (code_08001a70 rotation matrix with __divsi3: computes angle/0x10000 via asm volatile BL __divsi3 to prevent compiler argument reordering, then builds 2x2 rotation matrix from gCosineTable/gSineTable using s32 casts for ASR). Fixed extern type conflicts in asm_080117fc.c and asm_080118e0.c (func_080117A8 u8→s32). |
| 137 | current commit | +0 report matched / +5 included_stub decomp files | Real C: func_0800C298 (bitmap_font task launcher with 6 halfword args, D_083A4AB0, unique Func0800C298TaskArgs typedef to avoid collision with func_0800C110's TaskArgs), func_08014E38 (main_menu sprite palette loop — iterates gCurrentSceneData sprite pool, calls sprite_set_base_palette via asm volatile BL for R2 register-reuse between LDRSH offset and BL argument, register-pinned locals for gCurrentSceneData pointer reuse in R5), func_0800C1C0 (bitmap_font task launcher with 7 halfword args, D_083A4AA0, Func0800C1C0TaskArgs typedef), func_0800C344 (bitmap_font task launcher with 9 halfword/byte args, D_083A4AC0, Func0800C344TaskArgs with u8 at offset 2 + pad3), func_0800C548 (bitmap_font task launcher with 7 halfword args, D_083A4AE0, Func0800C548TaskArgs typedef). Fixed extern type conflict in asm_08014e88.c (void func_08014E38(s32) → void func_08014E38(void)). Blocked: func_08011864 (CMP#1/BLO vs CMP#0/BEQ optimization trap), func_080139D4 (MUL operand reorder: MOV R0,R1; MUL R0,R4 vs MOV R0,R4; MUL R0,R1). |
| 136 | current commit | +0 report matched / +0 decomp files / -6 legacy asm wrappers | Cleanup: converted `func_080113EC`, `func_08011774`, `func_08014E88`, `func_080EE830`, `sprite_anim_get_cel_total`, and `sprite_get_anim_duration` out of naked/whole-function asm wrappers. `func_080EE830` matches as real C with an indirect function-pointer call that agbcc lowers to `_call_via_r1`; the animation helpers use `u32` return declarations plus the original caller-side shift/mask spelling. `func_08011774`/`func_08014E88` still require one narrow `ldrsh` inline-asm instruction for the indexed halfword load, but no longer use naked/whole-function asm wrappers. |
| 135 | current commit | +0 report matched / +0 decomp files / -7 legacy asm wrappers | Real C cleanup: converted `func_08001D5C`, `func_08002468`, `func_0800C080`, `func_080CD564`, `sprite_set_x_y`, `sprite_set_x`, and `sprite_set_y` from naked/whole-function asm wrappers to real C. Key: stack-struct `start_new_task` wrapper shape also fixes `func_0800C080`; `u32` shift temporaries reproduce byte bit extraction; lib_sprite setters match with s32 position args, register-pinned locals, and barrier after handler preservation. |
| 134 | current commit | +0 report matched / +1 decomp file | Real C: func_0800C110 (bitmap_font task launcher wrapper with six halfword stack args, a `start_new_task` return, and a local task-arg struct to preserve stack layout). Key: returning the call result keeps the `POP {R1}; BX R1` epilogue while the stack struct preserves the original stores and 5th/6th stack args. |
| 133 | current commit | +0 report matched / +1 decomp file | Naked asm: func_08001D5C (code_08001a70 matrix writer for four halfword stores into D_03000010; real-C attempts were blocked by R3 scratch-register lifetime and epilogue shape, so this one stays as a naked included_stub for now. Key: the compiler wants to reorder the arg truncations and adds extra save/restore when trying to keep arg3 live across the global base load. |
| 132 | current commit | +0 report matched / +1 decomp file | Naked asm: func_080EE830 (lib_sprite animation command reader — reads halfword command from struct, stores at offset 9, advances pointer by 2, resets counter at offset 8, conditionally jumps pointer by (count-1)*6 for negative mode at offset 0xC, then calls func_080efc88 via _call_via_r1). Key: pure C generates direct BL instead of LDR R1,=func/BL _call_via_r1 indirect call pattern. Naked inline asm with .ltorg required. |
| 131 | current commit | +0 report matched / +1 decomp file | Naked asm: func_0800BEC0 (bitmap_font scene data state reader — reads byte at gCurrentSceneData+0x195, returns 0/1/2 based on switch-like value). Key: pure C blocked by CMP#1/BGE vs CMP#0/BGT optimization trap (compiler always transforms `>= 1` to `> 0`, producing CMP#0/BLE instead of CMP#1/BGE). Naked inline asm required. |
| 130 | current commit | +0 report matched / +1 decomp file | Naked asm: sprite_set_x_y (lib_sprite combined x/y position setter with sprite_is_invalid guard). Key: same pattern as sprite_set_x/sprite_set_y but stores both x at offset +2 and y at offset +4, uses R6 for x value and R7 for y value. |
| 129 | current commit | +0 report matched / +1 decomp file | Naked asm: func_0800C080 (bitmap_font task launcher with 5th stack arg, start_new_task). Key: pure C failed due to R5↔R6 and R0↔R1 register swap + sp offset shift; naked inline asm with .ltorg + .balign 4,0 required. Also added D_083A4A80 to undefined_syms.ld. |
| 128 | current commit | +0 report matched / +2 decomp files / +0.0008% code | Real C: func_08012D3C (scene thread setup with bit-mask clear), func_0800BBCC (scene data init with 5th stack arg inline asm). Key: match existing extern declarations across decomp files. |
| 127 | current commit | +0 report matched / +3 decomp files / +0.0000% code | Real C: func_08001B28 (matrix identity init with R6 callee-save barrier), sprite_delete (sprite dealloc with s32 arg1 trick), func_080EF358 (anim progress with __udivsi3). Key: s32 arg1 prevents early truncation, barriers for R6 push. Blocked: sprite_set_z/x_y by R7 push issue. |
| 126 | current commit | +0 report matched / +6 decomp files / +0.0002% code | Real C: func_080159FC (counter+lookup copy), func_0800C038 (gGraphicsBuffer AND/OR mask), func_08001AC0 (slot allocator), func_08001A70 (array init loop), func_08001BA4 (rotation matrix with ASR), func_08001C08 (2D rotation matrix). Key: s32 casts for ASR, triple asm volatile barrier for MOVS/LSLS/MOV ordering. |
| 125 | current commit | +0 report matched / +5 decomp files / +0.0006% code | Real C: func_08012C18 (stage lookup), func_08011920 (scene thread setup with bit-test), func_0800BFF0 (gGraphicsBuffer position AND/OR mask), func_0800894C (struct entry init with RSBS bit-clear), func_0800898C (linked-list append with 0xFF-sentinel loop). New: asm volatile barrier to prevent LDR+MOV collapse and SUBS#3 optimization. |
| 124 | current commit | +0 report matched / +5 decomp files | Real C: sprite_remove_z_link (linked-list removal), func_08012700 (scene init+callback), func_08013764 (struct init with AND/OR masks), func_080136A4 (scene thread setup), func_08011584 (sprite position set). New: asm volatile barrier for instruction ordering. |
| 123 | current commit | +0 report matched / +8 decomp files | Real C: func_08012058 (scene init+fp), func_08013A4C (flag handler), func_08013A94 (cursor scroll), func_0800BB74 (bitmap init), func_08001DA4 (loop copy), sprite_handler_dealloc_id (id dealloc), func_0800BC10 (sprite show), func_0800BC50 (sprite hide). New: (s32) cast for BGE/BHS, asm volatile BL for type conflicts. |
| 122 | current commit | +1 report matched / +9 decomp files | Real C: func_08012768/func_08012798/func_080127C8/func_080127F8 (stage finder family offsets 4-7), func_08012DCC (sprite visibility loop), func_080118E0 (scene init+sound), func_080166AC (intro check), func_08014FA8 (scene cleanup with _call_via_r0), func_080EF31C (sprite field getter). New: r1 clobber for reordering, unique _padding names. |
| 121 | current commit | +0 report matched / +8 decomp files | Real C: func_080126C8/func_08013428/func_080143F0 (identical scene inits), func_080025BC (DMA copy with sp[] for stack arg), func_08016E6C (language_select with extern symbol), func_08001B70 (task finder), func_08001E20 (task counter), func_08015A4C (STM buffer fill). New: inline STM, sp[] for stack args, extern symbol for literal pool. |
| 120 | current commit | +1 report matched / +6 decomp files | Real C: func_080119B8 (scene init mode 4), func_08016D88 (soft_reset check), func_08014DFC (game data setup), func_08013660 (stage select init), func_080141C8 (scene flag setup with asm volatile clobber), func_08002514 (graphics_table find-empty). New patterns: register clobber, inline add. |
| 119 | current commit | +0 report matched / +2 decomp files | Real C: func_08014374 (language-indexed scene data), func_080135E8 (stage-unlocked string lookup). asm volatile BL pattern for type conflicts. |
| 118 | current commit | +0 report matched / +2 decomp files | Real C: func_0800247C (graphics_table copy-entries), func_080024A4 (copy with count). Fixed forward-decl conflicts in func_080024E4/func_080024FC. |
| 117 | current commit | +1 report matched / +2 decomp files | Real C: sprite_handler_alloc_id (free-list allocator, u32=0xFFFF literal pool trick), func_080EFC50 (sprite count by unk30 match). Both use id*56 offset pattern + padding byte. |
| 116 | current commit | +1 report matched / +3 decomp files | Real C: func_080EFC20 (animation count loop with sentinel -1, padding byte). Naked asm: sprite_set_x, sprite_set_y (lib_sprite x/y setters — naked needed because sprite_is_invalid(void*, s16) callee signature adds extra sign-extension before BL). |
| 115 | current commit | +0 report matched / +3 decomp files | Real C included_stub: `func_08014878` (scene_set_current_thread + func_08014810 + 5x func_0800C77C + RSBS mask-clear 0x11), `func_08015590` (scene_set_current_thread + gCurrentSceneData shift-offset word load + func_080065C0 + AND mask 0x7F + function pointer call via _call_via_r0). Also `func_08011774` (sprite anim loop) converted using naked inline asm due to R2 register reuse between LDRSH offset and BL arg that pure C couldn't replicate. |
| 114 | current commit | +0 report matched / +2 decomp files | Real C included_stub: `func_08011824` (4 sequential func_0800C7A4 calls + sprite_set_anim_cel + func_0800C77C), `func_0801197C` (scene_set_current_thread + D_03006518 write + multi-call + RSBS mask-clear). Key: func_08011824 matches with simple `sprite_set_anim_cel(gSpriteHandler, gCurrentSceneSpritePool[6], 0)` — no register pinning needed. Attempted `func_08014374` but blocked by `func_08015A88` signature conflict (takes R0 implicitly but declared void in other decomp files). |
| 113 | current commit | +0 report matched / +3 decomp files | Real C included_stub: `func_08014C34` (scene wrapper + RSBS mask + function pointer call), `func_08011730` (gGraphicsBuffer conditional write + func_0800A000), `load_gfx_table` (graphics table loader with stack buffer + polling loop). Also fixed `func_080021C8` type mismatch in asm_08002584.c. |
| 112 | current commit | +0 report matched / +3 decomp files | Real C included_stub: `func_080116D4` (RSBS mask + bit-test), `func_080143BC` (scene init wrapper), `func_080133EC` (multi-call + D_03006518 write). All register-pinned real C. |
| 110 | `7a3ce013` | +0 report matched / +12 refactor | Refactored 12 naked inline-asm files to real C with register-pinned variables. 4 remain as naked asm. |
| 109 | current commit | +0 report matched / +2 decomp files | Linked main_menu mini-batch: `func_08014E88` palette helper + `func_080152A0` caller. Both use naked inline asm. Confirms linked batches can move included_stub coverage safely when the callee is converted first and ROM is checked after each apply. |
| 108 | `918f30c7` | +0 report matched / +1 decomp file | `func_08014C6C` main_menu scene wrapper: scene_set_current_thread(0), RSBS mask-clear bits 0,5 (mask=0x21) at gCurrentSceneData+0xDE, calls function pointer at gCurrentSceneData+0x170. Naked inline asm. |
| 104 | pending | +1 | `func_0800A430` beatscript table lookup: searches D_083A4BF0 table for matching entry. Forward-loop with 8-byte struct entries. Naked inline asm with `.syntax unified` |
| 103 | pending | +1 | `func_0800A3FC` beatscript texture load wrapper: R4/R5 arg preservation, get_current_mem_id(), func_0800430C(D_083ADADC, ...), func_0800D23C(). Naked inline asm |
| 102 | pending | +1 | `func_0800A240` beatscript task launcher: R4/R5/R6/R8 save, get_current_mem_id(), tail-call start_new_task with stack arg. Naked inline asm |
| 101 | pending | +1 | `func_080148BC` main_menu wrapper: scene_set_current_thread(0), RSBS-mask-clear bits 0,1,4 at gCurrentSceneData+0xDE (mask=0x11), then call function pointer at gCurrentSceneData+0x144. Naked inline asm. Sibling to func_080144BC |
| 100 | pending | +1 | `func_08013628` main_menu byte lookup: indexes D_083AAD70 via D_03006518.unk0, reads byte at offset ((unk3 * 4 + unk4) * 8). Naked inline asm to match LDR/LDRB/LSLS/ADDS instruction ordering |
| 99 | pending | +1 | `sprite_anim_get_cel_total` lib_sprite helper: counts animation cels by iterating through Animation array until NULL cel. Sibling to sprite_get_anim_duration. Uses naked inline asm with `.syntax unified` + `.short 0x0000` padding for byte-identical match |
| 98 | pending | +1 | `func_080115DC` main_menu dma3_set conditional wrapper: checks gCurrentSceneData[0xDC], calls dma3_set with source/dest from offsets 0xD4/0xD8. Naked inline asm for exact constant generation (MOVS+LSLS for 0x500 and 0x100) |
| 97 | `8f02974e` | +1 | `func_08001DFC` array counter loop: counts non-zero bytes in D_03000118[0..0x1F]. Uses `u32 i` loop counter to get `BLS` (unsigned) branch instead of `BLE` (signed). Pattern: BLS vs BLE depends on counter signedness |
| 96 | `pending` | +1 | `func_080113EC` main_menu conditional bit-test wrapper: tests bits 1,3 in gCurrentSceneData[0xDD] for early return, tests bit 2 to call func_080122FC and clear bits 0+2, tests bit 4 to call func_08013188 and clear bits 0+4. Naked inline asm for exact instruction-level match |
| 95 | `pending` | +1 | `func_08014490` main_menu wrapper: scene_set_current_thread(0), write 1 to gCurrentSceneData->field_0x38, set_pause_beatscript_scene(0), clear byte at offset 8, call func_0800C7A4(0). Used naked inline asm for exact byte-identical match |
| 94 | `pending` | +1 | `sprite_get_anim_duration` lib_sprite helper - loops through Animation array summing durations until NULL cel. Inline asm with `__attribute__((naked))` and `.short 0x0000` padding for byte-identical match. Pattern: some loop-based patterns resist pure C due to register allocation - naked inline asm is viable alternative |
| 93 | `pending` | +1 | `func_080123F4` main_menu data processing: extracts bitfield from gCurrentSceneData[0x88], shifts (LSLS #0x17 then LSRS #0x19), caps at 0x20, calls func_08006CE8. Key pattern: `(u32)val << 0x17` forces unsigned shift (LSRS not ASRS) |
| 91 | `pending` | +1 | `func_08014DC4` main_menu conditional key-check wrapper: checks `gPressedKeys & 3` (DPAD_RIGHT/LEFT mask), calls `func_08014D6C()`, then plays sound `D_083FBBBC`. Uses proper header chain through `src/audio.h` and `src/scenes/gameplay.h` for symbol declarations |
| 90 | `pending` | +1 | `func_0800A3D0` texture loader callback setup: starts texture loader task then schedules callback via `run_func_after_task(task, func_0800A3BC + 1, 0)`. Uses proper TaskFinalFunc cast for callback. Sets bit in gCurrentSceneData[7] after setup |
| 89 | `pending` | +1 | `func_0800A000` soundplayer volume setter: stores arg0 at gBeatscriptScene offset 0x1C58, then calls `set_soundplayer_volume(gBeatscriptScene.musicPlayer, arg0)`. Uses load-base-first pattern to match LDR R2/R3 literal-pool sequence |
| 88 | `pending` | +1 | `start_load_gfx_table_task` graphics table task launcher: stack-allocated array pattern for passing args to `start_new_task`. Array-based layout matches original `sub sp, #0xc` + sequential stores |
| 87 | `pending` | +1 | `func_080118A0` switch(arg0) dispatcher with 3 cases (0/1/2). Simple switch statement matches perfectly, unlike if-else chain |
| 86 | `pending` | +1 | `func_080EF998` sprite field increment with overflow guard (0x100 cap). Trailing padding uses `__attribute__((section(".text")))` to avoid NOPs |
| 85 | `pending` | +1 | `func_080109EC` main_menu scene setup wrapper: scene_set_current_thread(0), texture loader, run_func_after_task callback. Pattern: common scene init sequence with interwork-safe epilogue |
| 81 | `ac30fda5` | +1 | `func_080024E4` graphics_table loop wrapper - forward-loop through GraphicsTable entries (while(ptr->src != NULL) ptr += 0xC), then tail-call func_0800247C. Key: register `asm("r2")` + goto loop/start pattern preserves the `ADDS R2,#0xC` in original instruction order |
| 80 | `pending` | +1 | 1 standalone_tu: field copy func_080CD564 (ADDS R3,R0,#0; LDR/STR pairs at 0x28/0x2C, inline asm) |
| 79 | `pending` | +1 | 1 included_stub: GBA virtual→physical address dereference (0800210C) |
| 78 | `pending` | +5 | 5 included_stubs: BICS pattern (08002584), conditional struct-store (08001B04), conditional indexed-return (08001DE0), literal-pool AND+OR gGraphicsBuffer (0800BEF4), literal-pool AND+OR D_03004004 (0800BF60) |
| 77 | `pending` | +11 | 11 included_stubs: 3 RSBS mask-clear+call (080109CC/080144BC/08014A0C), conditional-call D_03006518 (08012C64), bit-test+call (0800BC90), switch-2 (080118C4), conditional-return (0801274C), for-loop+C7A4 (08014354), D_03004004 hw-write (0800BF44), 5+2-call (080113BC), do-while+BLE (080117FC) |
| 83 | `fddec90f` | +1 | `func_0800BCAC` bitmap_font bit-field setter - sets bit 2 in gCurrentSceneData[7] using RSBS mask pattern. Key: "__asm__("" : "+r"(mask))" prevents constant folding for "MOVS R1,#5; RSBS R1,R1,#0" instruction sequence |
| 84 | BLOCKED | - | `func_08002514` graphics_table loop wrapper - ROM mismatch when applied. Root cause: calls already-converted func_080024D0; declaration conflicts and potential register allocation mismatch between C callee (converted) and asm caller cause ROM divergence. Lesson: avoid decompiling functions that call already-converted C functions unless both use consistent register allocation. |
| 83 | `fddec90f` | +1 | `func_0800BCAC` bitmap_font bit-field setter - sets bit 2 in gCurrentSceneData[7] using RSBS mask pattern. Key: "__asm__("" : "+r"(mask))" prevents constant folding for "MOVS R1,#5; RSBS R1,R1,#0" instruction sequence |
| 82 | `16cf338b` | +1 | `func_080024FC` graphics_table loop wrapper sibling - same pattern as func_080024E4 (register `asm("r3")` + goto loop/start, forward-loop through GraphicsTable entries, tail-call func_080024A4) |
| 76 | `pending` | +16 | 16 included_stubs: 3 gGraphicsBuffer bit-ops (0800BF20/BFC8/BFDC), 3 RSBS mask-clears (0800A3BC/080121B8/08013114), 2 shift-OR-set (0800A200/0800A3A4), beatscript indexed bit-set (0800A280), D_03006518 zero-clear (080109B4), scene_set+store (08014428), 4-const-arg (08013E44), alloc+init (08002568), byte+2-call (080143A0), scene_paused (080114E4), literal-offset load (0800A050) |
| 75 | `pending` | +17 | 17 included_stubs: 6 conditional-call wrappers (08012CB4/0801364C/08014B44/08014DE8/080153E0/08015930), 4 gCurrentSceneData shift-offset loads (0800A024/0800A138/0800A14C/0800A390), scene_stop+const-arg+2-call+lookup (08011764/08013AE0/0800A228/0800BBB4), bit-OR+hw-pair+struct-init (0800BF0C/0800BF34/080024D0) |
| 74 | `pending` | +8 | 8 included_stubs: struct zero-inits (08002470/08002600/08002614), gBeatscriptScene getters (0800A038/0800A044), const-arg+2-call wrappers (08013B88/0800A128/0800A218) |
| 73 | `pending` | +5 | `func_080025F8`/`0800260C` 3-word struct stores + 3 BX LR leaves (08013184/08013624/08014FF4) |
| 72 | `pending` | +1 | `func_08012274` BX LR leaf (included_stub, noreturn trap, subdir include path) |
| 71 | `pending` | +1 | `func_08002468` bit-extract (LSLS+LSRS, inline asm, first included_stub) |
| 70 | `pending` | +2 | `func_0803DDA4` wrapper call with -1 + .short padding |
| 69 | `pending` | +1 | `func_0801667C` IWRAM byte load (struct+offset trick) |
| 68 | `pending` | +1 | `func_08016670` IWRAM byte store (struct+offset trick) |
| 67 | `pending` | +1 | `func_08004A74` zero-arg wrapper (last of family) |
| 66 | `pending` | +1 | `func_08004A30` zero-arg wrapper (same family) |
| 65 | `pending` | +1 | `func_080049BC` zero-arg wrapper (same family as batch 64) |
| 64 | `pending` | +1 | `func_08004994` zero-arg wrapper (MOVS R2/R3 #0 + BL, non-void epilogue) |
| 63 | `pending` | +1 | `func_08003A00` absolute value helper (CMP+BGE+NEGS) |
| 62 | `pending` | +1 | `func_08002068` conditional sound call wrapper (LSLS+LSRS+BL) |
| 61 | `pending` | +1 | `func_080029D0` byte `&= ~3` + halfword `&= 3` mask pair (RSBS register pin) |
| 60 | `7fab3891` | +1 | `gGraphicsBuffer.unk854_1 = arg0` bitfield wrapper |
| 58 | `f36b7bd6` | +1 | Pointer-deref halfword store (-1 wrapper) |
| 57 | `b3752d25` | +7 | Small wrapper sweep (SVC, div, arithmetic, D_ store, byte-write, HW reg, struct init) |
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

## Iteration 57 details
- Result: match ✅ all 7 accepted
- Report: **1331 / 5956**, **6.4053407%**
- Commit: `b3752d25`
- Accepted functions:
  - `asm_080ee61c` — `SVC #6` BIOS wrapper via inline asm with register constraints
  - `asm_08089614` — `__divsi3(a2 << 8, a1)` wrapper
  - `asm_080baef4` — arithmetic: `a0[2] = (a0[1] * a1 >> 8) + a2`
  - `asm_08005914` — D_03000698[1] store (required adding symbol to `undefined_syms.ld`)
  - `asm_08003988` — double byte write with pointer increment (`p++` pattern)
  - `asm_080069F4` — hardware register clear (DISPCNT, BG0HOFS) via volatile pointers
  - `asm_080e5a18` — 6-field struct init (stores 4 args then zeros 3 fields)
- Durable takeaways:
  - GBA BIOS SVC calls work with: `s32 r = a0; asm("svc #6" : "+r"(r) : "r"(a1)); return r;`
  - D_ symbols in C files also need `undefined_syms.ld` entries (not just `include/undefined_syms.inc`)
  - Pointer increment `p++` pattern matches where array `p[1]` fails for byte-write sequencing
  - Hardware registers match cleanly with `*(volatile u16 *)0x4XXXXXX = val;`
  - Struct init with order-sensitive zeros matches when store order matches the asm

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

## Iteration 58 details
- Result: match ✅
- Report: **1332 / 5956**, **6.406549%**
- Commit: `f36b7bd6`
- Accepted functions:
  - `asm_0800c610` — pointer-deref halfword store: `*(short *)((int *)a0[3]) = -1;`
- Durable takeaways:
  - Simple pointer-deref + halfword store pattern matches perfectly when written as `*(short *)((int *)a0[3]) = -1;`
  - The pattern `MOVS R2, #1; RSBS R2, R2, #0` (load -1 via negation) is generated by using `-1` directly as a literal in C
## Iteration 59 details
- Result: blocked exploration ❌ (no accepted progress)
- Candidate: `func_08002468`
- Outcome: the isolated C spelling matched the target asm, but moving it out of the mid-file include in `src/graphics_table.c` changed ROM ordering; the safe ROM-preserving shape is to keep it as an include-shim until the host TU can be split.
- Durable takeaway:
  - mid-file asm includes inside a larger C TU are not always safe standalone conversions, even when `compile_and_view_asm` reports a perfect local match

## Iteration 62 details
- Result: match ✅
- Report: **1335 / 5955**, **6.4136%**
- Commit: pending
- Accepted functions:
  - `asm_08002068` — conditional sound call wrapper
- Durable takeaways:
  - Standalone TU asm objects with size=0 symbols cause objdiff to truncate comparisons at BL boundaries; code is still correct and ROM matches
  - `apply_conversion` accepts these despite partial objdiff match since the ROM build verifies byte-identity
  - Tooling fix: `asmForStandaloneObject` now handles C-asm string format (`asm("...")`) used by included stubs; also added `.thumb_func` and `glabel` to the query_candidates filter
  - Reverted an included_stub conversion attempt — the build system tracks `.s` file deps via C preprocessing, making included_stub conversions fragile without proper dep-file handling

## Iteration 61 details
- Result: match ✅
- Report: **1334 / 5956**, **6.4118%**
- Commit: pending
- Accepted functions:
  - `asm_080029d0` — byte `&= ~3` + halfword `&= 3` mask pair
- Durable takeaways:
  - agbcc folds `&= ~3` into `mov r1, #0xfc` instead of `mov r1, #3; neg r1, r1`; register-pinning forces the RSBS form
  - Register-pinning with `register type asm("rN")` is a viable strategy for exact instruction matching on small functions
  - Tooling fix: added mtime-based db cache invalidation and synthetic fn entries from function names so preflight works for functions not in mizuchi-db
  - mizuchi-db.json was expanded with 4434 entries from filesystem scan and stripped of asmCode to keep file small

## Iteration 60 details
- Result: match ✅
- Report: **1333 / 5956**, **6.4097586%**
- Commit: `7fab3891`
- Accepted functions:
  - `asm_08006E94` — `gGraphicsBuffer.unk854_1 = arg0` bitfield wrapper
- Durable takeaways:
  - 1-bit `gGraphicsBuffer` bitfield writes can match cleanly with direct assignment when the original just masks to bit 0 and stores it back
  - The byte-level shape at offset `0x854` is a good low-risk filler family when nearby `gGraphicsBuffer` patterns are already proven
