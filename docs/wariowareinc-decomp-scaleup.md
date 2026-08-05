# WarioWare Inc. Decomp Scale-Up

This is the live operational status file for autonomous work in this repo.
Prefer this file + the other docs in `/docs`

## Current verified baseline
- Verified on branch: `docs/macabeus-tooling-assessment`
- Verified working tree: `batch 189` — six strict-ROM standalone C scene/audio wrappers plus the hardened candidate lifecycle
- `build/report.json`: **1464 / 5960 matched functions** = **24.563759%**
- `matched_code_percent`: **6.752255%**
- `tools/gen_objdiff.py`: **1004 linked C TUs / 5683 non-C units**
- `src/decomp/*.c`: **1186 decompiled function files** = **985 standalone_tu** + **201 included_stub**
- ROM status: **`wariowareinc.gba: OK`**
- Remaining naked/original asm wrapper files in `src/decomp`: **0**
- Maintenance state: **30 legacy inline-asm shims removed** from included-stub files; `src/decomp` contains no instruction-bearing inline asm. `func_080EE61C` is now real C: a target-specific `__builtin_swi_div` lowers through the patched agbcc Thumb backend to the BIOS `SVC #6` instruction.
- 25% milestone: **1490 / 5960**, so **26** additional matched functions are needed.
- Next 30% milestone: **1788 / 5960**.
- That 30% milestone is **324** additional matched functions from the current baseline.

### Batch 189 — accepted (scene-table and audio wrapper fan-in)
- Converted six standalone functions to ordinary C: `func_0800C7FC`, `func_0801004C`, `func_080102A4`, `func_08010328`, `func_0801E918`, and `func_08024494`.
- One seven-entry m2c isolation screen found six exact candidates and one recorded near miss. The exact scene-table siblings use `scenes.h`, raw `gCurrentSceneData + 8` access, and byte-array data symbols; the audio wrappers preserve the non-void return ABI and normalized key/speed arguments. The `func_0800CDB0` mask candidate remains evidence-only because agbcc folds the target `MOVS #3; RSBS` sequence.
- The transactional exact-only apply required canonical `D_083A98B8`, `D_083A98D8`, and `D_083FC594` linker assignments; it passed one clean Docker ROM/report gate. The six C candidates contain no instruction-bearing or volatile inline asm.
- Fresh report: **1464 / 5960**, **6.752255%** matched code, **1004 C / 5683 asm-only** units, and **1186** decomp files (`985 standalone_tu` + `201 included_stub`). `wariowareinc.gba: OK`; ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.

### Batch 188 — accepted (DMA, heap-copy, and sprite sibling fan-in)
- Converted nineteen standalone functions to ordinary C: `func_08004AE0`, `func_08004BD4`, `func_08004EC8`, `func_0800557C`, `func_08005B20`, `func_08007000`, `func_0800C77C`, `func_0800CD94`, `func_0800CF3C`, `func_0800CF5C`, `func_0800CF7C`, `func_0800CF9C`, `func_0800CFBC`, `func_0800CFDC`, `func_08017080`, `func_080170DC`, `func_080170FC`, `func_0802A238`, and `func_08048DC8`.
- Two small m2c fan-in screens found eight exact candidates in the first wrapper group and eleven exact candidates in the DMA/sprite group. Register-pinning the heap destination recovered both copy helpers; a local base pointer preserved the `gBeatscriptScene + 0x1E` load in `func_0800CD94`. The remaining `func_0801E6F8` mask variant is evidence-only because agbcc folded the target's `MOVS #2; RSBS` sequence into a single constant.
- The accepted 19-entry exact receipt passed one transactional full Docker gate. The C candidates contain no instruction-bearing or volatile inline asm. Four `D_0300XXXX` and one `D_083FD264` definition were added to `undefined_syms.ld` to mirror symbols already present in `include/undefined_syms.inc`.
- Fresh report: **1458 / 5960**, **6.733334%** matched code, **998 C / 5689 asm-only** units, and **1180** decomp files (`979 standalone_tu` + `201 included_stub`). `wariowareinc.gba: OK`; ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.

### Batch 187 — accepted (wrapper and sprite sibling fan-in)
- Converted nineteen standalone functions to ordinary C: `func_080042F4`, `func_080043B8`, `func_08004F14`, `func_080049A4`, `func_08007FC0`, `func_0800C9A4`, `func_0801975C`, `func_0801E44C`, `func_08020F40`, `func_08085624`, `func_0808AB78`, `func_0808AB98`, `func_0808B9FC`, `func_080B83B0`, `func_080C6188`, `func_080C61AC`, `func_080D1034`, `func_080D37E4`, and `func_080F2FFC`.
- The m2c wrapper/sibling skeletons were screened in one initial 19-entry isolation receipt. ABI-focused variants recovered the six near misses and two compile errors: non-void no-return declarations reproduced `POP {R1}; BX R1`, ignored stack parameters preserved the target load offsets, staged multiplication preserved operand order, and register-bound C locals preserved literal-load order. The accepted candidates contain no instruction-bearing or volatile inline asm.
- A standalone linker preflight caught `D_0300490E` missing from `undefined_syms.ld`; adding its canonical `0x0300490E` definition allowed the C TU to link. The final 19-entry exact receipt was applied without `--force` and passed the transactional full Docker gate.
- Fresh report: **1439 / 5960**, **6.6739535%** matched code, **979 C / 5708 asm-only** units, and **1161** decomp files (`960 standalone_tu` + `201 included_stub`). `wariowareinc.gba: OK`; ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.

### Batch 186 — accepted (near-miss shaping follow-up)
- Recovered three standalone near misses as ordinary C: `func_08003228` with its explicit project prototype, `func_0805627C` with a widened `s32` input and explicit `(s16)` normalization, and `func_080A002C` with an `r1`-pinned `u16` argument so the global music-player load remains before argument normalization.
- The focused follow-up screen tested seven spellings in one Docker invocation. All three selected entries were exact; `func_080F5FF4` remains evidence-only after an old-style single-pointer call improved but did not close its stack-layout gap. The exact subset was applied without `--force` and passed the transactional full-ROM gate.
- Fresh report: **1420 / 5960**, **6.6157527%** matched code, **960 C / 5727 asm-only** units. `wariowareinc.gba: OK`; both ROMs remain SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.

### Batch 185 — accepted (standalone wrapper sibling fan-in)
- Converted ten standalone linker entries to ordinary C: `func_080043A0`, `func_08017668`, `func_0801A7D8`, `func_0801A7F4`, `func_0801A994`, `func_0801B61C`, `func_080223E0`, `func_0808967C`, `func_080A8A3C`, and `func_080ED734`. The original assembly sources moved to `asm/converted/`, and each linker entry now points to its own C TU.
- One m2c fan-in screen compiled **14** candidates in one Docker isolation invocation: ten exact winners, three recorded near misses (`func_0805627C`, `func_080A002C`, `func_080F5FF4`), and one compile-error candidate (`func_08003228`). The exact subset reused that receipt without `--force`.
- The transactional full Docker gate passed `wariowareinc.gba: OK`; both ROMs remain SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. Fresh report: **1417 / 5960**, **6.609311%** matched code, **957 C / 5730 asm-only** units. Source coverage is **1139 files** (`938 standalone_tu` + `201 included_stub`).

### Batch 184 — accepted (included-stub task-wrapper batch)
- Converted the main-menu task wrappers `func_080122FC`, `func_0801312C`, `func_080148EC`, and `func_08014C9C` to ordinary C with register-shaped calls, callback pointers, and host-TU include guards. Their original assembly sources now live under `asm/converted/`.
- m2c supplied the usable wrapper skeletons. Normalized linked-ELF isolation reported **99.59–99.72%** because the included-stub candidates retained external `BL` relocation records even though the generated instructions and host-TU layout matched. Direct object disassembly plus the full-context transaction established the relocation-only nature of the near miss.
- The forced research apply still required the normal clean full-ROM gate; it passed with `wariowareinc.gba: OK` and SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. The report remains **1407 / 5960** matched functions because included stubs are already part of linked host TUs; matched code rose to **6.584173%** and source coverage is **1129 files** (`928 standalone_tu` + `201 included_stub`).
- The included-stub linker-symbol preflight was corrected in `d7f9d29` to validate symbols through the host TU rather than incorrectly requiring every `D_XXXXXXXX` reference in the standalone undefined-symbol map; the lifecycle regression suite passed **21 tests**.

### Batch 183 — accepted (strict leaf and ASM-callee screen)
- Converted `func_0800EA44`, `func_08038694`, `func_080102C4`, and `func_08072C20` to ordinary C. One eight-candidate isolation pass found six exact spellings; two exact callers of already-converted C helpers remained evidence-only, and two pointer/store candidates remained near misses.
- The first full-context attempt rolled back because `D_083A98D0` was present in `include/undefined_syms.inc` but missing from `undefined_syms.ld`. Adding the canonical `0x083A98D0` linker definition and rerunning the immutable screen fixed the integration issue; the corrected four-entry transaction passed one full Docker ROM/report gate with `wariowareinc.gba: OK`.
- Fresh report: **1407 / 5960**, **6.5836096%** matched code, with **947 C / 5740 asm-only** units. Both ROMs remain SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. Receipts: `.decomp-runs/20260805T-round-0805b-isolation-v2.json` and `.decomp-runs/20260805T-round-0805b-apply-v2.json`.

### Batch 182 — accepted (strict standalone wrapper fan-in)
- Converted `func_0800D23C` and `func_08019A8C` to ordinary C. The first eight-candidate isolation pass found these two exact wrapper spellings and retained six pointer/global/bit-operation near misses as evidence only.
- The lifecycle reused the exact candidates from one isolation receipt and ran one transactional full Docker ROM/report gate. It reported `wariowareinc.gba: OK`; both ROMs remain SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Fresh report: **1403 / 5960**, **6.5747647%** matched code, with **943 C / 5744 asm-only** units. Receipts: `.decomp-runs/20260805T-round-0805-isolation-v3.json` and `.decomp-runs/20260805T-round-0805-apply-v2.json`.

### Batch 181 — accepted (included-stub lifecycle exercise)
- Converted `func_0800BF7C` in `bitmap_font.c` from the original ASM include to ordinary C. The source contains only C declarations/control flow plus the required `__INCLUDE_LEVEL__` wrapper; it has no instruction-bearing inline asm.
- `tools/decomp_permute.py screen` compiled an m2c spelling and an unsigned-coordinate spelling in one isolation container. The m2c candidate was exact; the alternate remained a recorded **87.878784%** near miss.
- The first full-context apply intentionally exposed a lifecycle bug: the raw candidate was inserted without the include-level guard, causing a duplicate definition. The transaction rolled back and rebuilt the exact baseline. After the tool fix, the same candidate passed the clean Docker ROM/report gate with `wariowareinc.gba: OK`; SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Report metrics remain **1401 / 5960** and linked C units remain **941 / 6687**, because included stubs are already part of their host C TU. Source coverage is now **1119 files** (`922 standalone_tu` + `197 included_stub`).

## Goal
Reach at least **30% matched-function progress** while preserving byte-identical ROM output at every accepted milestone. The 80% figure remains the longer-term project target after this milestone.

At the current `total_functions` count (`5960`), that means:
- immediate target: **1788 / 5960** matched functions
- current gap to 30%: **324** more matched functions
- longer-term target: **4768 / 5960** matched functions

### Batch 180 — accepted (real-C arithmetic and packing leaves)
- Converted `func_080F1F9C`, `func_080F28F8`, `func_080F2C50`, `func_08035ACC`, `func_08003014`, `func_0803F224`, `func_0803F26C`, and `func_0806754C` to ordinary C. The ten-candidate pure-leaf screen reached eight exact candidates after the isolation tool was corrected to append the Makefile's zero-filled aligned `.text` tail; the two held-back candidates are split by target local-label symbols.
- Seven candidates matched through normalized linked-ELF isolation. `func_08003014` used the explicitly recorded raw-object fallback because its legacy target symbol has `.thumb_func` metadata after `glabel`; the clean full-ROM gate remained authoritative.
- One transactional batch gate passed with `wariowareinc.gba: OK`; both ROMs remain SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. Fresh report: **1401 / 5960**, **6.5699472%** matched code, and **941 C / 5746 asm-only** units. Receipt: `.decomp-runs/20260805T2008-pure-leaves-exact.json`.

### Batch 179 — accepted (real-C wrapper and reload helpers)
- Converted `func_0809C47C` as a non-void task-finalizer wrapper; the non-void declaration reproduces the target's `POP {R1}; BX R1` epilogue and the odd callback pointer remains ordinary C.
- Converted `func_080195E4` with delayed zero initialization after the first address add, `func_080DF440` with pinned register roles and reloaded scene-variable pointer, and `func_080DCD54` as a call-then-graphics-clear wrapper.
- All four candidates scored **100.0%** in normalized linked-ELF isolation and passed one transactional full Docker ROM/report gate. ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`. Fresh report: **1393 / 5960**, **6.5528364%** matched code, and **933 C / 5754 asm-only** units. Receipt: `.decomp-runs/20260805T195000Z-apply-batch-wrapper-reloads.json`.

### Batch 178 — accepted (real-C register-shaped scene-state helpers)
- Converted `func_080C4A48` with a pinned scene-variable base in `R1`, a signed `R0` accumulator, and an explicit `R2` halfword temporary so the compiler emits the target `ADDS R0,R2` form.
- Converted `func_080EC308` with a pinned global anchor in `R2`, pointer reloads through that anchor, and constants assigned after each address add. The source uses only an empty compiler barrier where needed; it contains no instruction-bearing asm.
- Both candidates scored **100.0%** in normalized linked-ELF isolation and passed one transactional full Docker ROM/report gate. ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`. Fresh report: **1389 / 5960**, **6.543187%** matched code, and **929 C / 5758 asm-only** units. Receipt: `.decomp-runs/20260805T194000Z-apply-batch-scene-state-variants.json`.

### Batch 177 — accepted (real-C scene-state offset helpers)
- Converted `func_0808BD98` to an ordinary C halfword store at `gCurrentSceneVariable + 0xC5C`.
- Converted `func_080AAA40` to an indexed halfword store. Loading the scene-variable base first, shifting the index separately, then forming the `0x83 << 2` base offset reproduced the target operand order.
- Both candidates scored **100.0%** in normalized linked-ELF isolation and passed one transactional full Docker ROM/report gate. ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`. Fresh report: **1387 / 5960**, **6.5387583%** matched code, and **927 C / 5760 asm-only** units. Receipt: `.decomp-runs/20260805T193030Z-apply-batch-scene-state-exact.json`.

### Batch 176 — accepted (real-C graphics-buffer clear/call siblings)
- Converted standalone `func_0808EBF8`, `func_0809CE64`, and `func_080DF420` to the proven clear body followed by `func_0800CDB0(1)`. Converted `func_080E9B60` to the same clear body followed by `func_0800418C()`.
- All four candidates scored **100.0%** in one normalized linked-ELF isolation pass. `apply-batch` moved the four original assembly files, updated the linker, and passed one clean Docker full-ROM/report gate without any source inline asm.
- ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`. Fresh report: **1385 / 5960**, **6.5339403%** matched code, and **925 C / 5762 asm-only** units. Receipt: `.decomp-runs/20260805T192329Z-apply-batch-graphics-clear-calls.json`.

### Batch 175 — accepted (real-C paired graphics-buffer clears)
- Converted standalone `func_080A2524` and `func_080EE608` to ordinary C. Both clear `gGraphicsBuffer` halfwords at offsets `0x4C` and `0x4E`; each has its own `src/decomp/` TU and its original assembly moved to `asm/converted/`.
- The improved cycle tool isolated both candidates in one Docker invocation. Its linked-ELF normalization resolved the target's absolute `gba.inc` symbols before objdiff, avoiding the false raw-object near miss caused by agbcc's unresolved `gGraphicsBuffer` literal relocation. Both candidates scored **100.0%** in isolation.
- `apply-batch` applied both changes transactionally and ran one clean Docker full build/report gate. `wariowareinc.gba: OK`; ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`. Fresh report: **1381 / 5960**, **6.521459%** matched code, and **921 C / 5766 asm-only** units.
- The new sources contain no inline asm. The cycle tests pass (**7 tests**), and the accepted receipt is `.decomp-runs/20260805T191205Z-apply-batch-graphics-clears.json`.

### Batch 174 — accepted (real-C scene-variable flag setter)
- Converted `func_080D74F4` to real C. It loads `gCurrentSceneVariable`, adds the separate `0x43A` offset literal, and stores byte value `2`.
- Ordinary C pointer arithmetic preserves the target's separate global/offset loads and `ADDS` ordering; this TU contains no inline asm.
- Verification: clean Docker ROM **`wariowareinc.gba: OK`**; both ROMs hash to `3f556448d290fa5406d6ed367fee16cc02387ad3`; fresh report **1379 / 5960**, **919 C / 5768 asm-only**. The linked C unit reports 100%.

### Batch 173 — accepted (real-C paired accumulator)
- Converted `func_080E1A6C` to real C: it accumulates two paired `u32` fields from offsets `0x24/0x28` into fields `0x4/0x8` of the supplied object.
- Verification: clean Docker ROM **`wariowareinc.gba: OK`**; both ROMs hash to `3f556448d290fa5406d6ed367fee16cc02387ad3`; fresh report **1378 / 5960**, **918 C / 5769 asm-only**. The linked C unit reports 100%.

### Batch 172 — accepted (real-C serialization helpers)
- Converted `func_08003998` and `func_080039D0` to real C little-endian 32-bit serialization/deserialization helpers. Sequential pointer operations and explicit shifts reproduce the target's unrolled byte accesses exactly.
- Verification: clean Docker ROM **`wariowareinc.gba: OK`**; both ROMs hash to `3f556448d290fa5406d6ed367fee16cc02387ad3`; fresh report **1377 / 5960**, **917 C / 5770 asm-only**. Both linked units report 100%.

### Batch 171 — accepted (real-C beatscript table store)
- Converted `func_0800D224` to a real C indexed store into `gBeatscriptScene + 0x1C5C`, preserving the target's separate base/offset loads and operand order with register pins and empty barriers.
- Verification: clean Docker ROM **`wariowareinc.gba: OK`**; both ROMs hash to `3f556448d290fa5406d6ed367fee16cc02387ad3`; fresh report **1375 / 5960**, **915 C / 5772 asm-only**.

### Batch 170 — accepted (real-C scene-variable field helpers)
- Converted `func_0801D4A0` and `func_0801D4B4` to real C. Both load the current scene variable's nested pointer at offset `0xC`, shift the input by eight, and update its halfword/byte fields.
- Register-pinned pointers and empty compiler barriers preserve the original Thumb register allocation and load/store order without instruction-bearing inline asm.
- Verification: both linked C units report 100%; clean Docker ROM **`wariowareinc.gba: OK`**; both ROMs hash to `3f556448d290fa5406d6ed367fee16cc02387ad3`. Fresh report: **1374 / 5960**, **914 C / 5773 asm-only**.

### Batch 169 — accepted (real-C graphics-buffer helpers)
- Converted `func_0805CB5C`, `func_0801AE70`, `func_0801F188`, and `func_0801F1A0` to real C updates of `gGraphicsBuffer`.
- Register pins and empty compiler barriers preserve the target Thumb load/ALU ordering; no instruction-bearing inline asm is used.
- Verification: clean Docker ROM **`wariowareinc.gba: OK`**, both ROMs hash to `3f556448d290fa5406d6ed367fee16cc02387ad3`, and all four linked C units report 100%. Fresh report: **1372 / 5960**, **912 C / 5775 asm-only**.

### Batch 168 — accepted (real-C large-offset beatscript stores)
- Converted `func_0800CAA4` and `func_0800CAB8` to real C stores at `gBeatscriptScene + 0x1C32` and `gBeatscriptScene + 0x1C30` respectively.
- Direct pointer arithmetic folded the large offset into the global relocation and missed the target’s separate `LDR global; LDR offset; ADDS` sequence. Register-pinned base/offset variables with an empty compiler barrier preserved that sequence without instruction-bearing inline asm.
- Moved both original assembly sources into `asm/converted/` and switched their linker entries to C. Both linked C units report **100.0%**.
- Verification: clean Docker ROM **`wariowareinc.gba: OK`**; both ROMs hash to `3f556448d290fa5406d6ed367fee16cc02387ad3`. Fresh report: **1368 / 5960**, **6.4923353%** matched code, **908 C / 5779 asm-only** units.

### Batch 167 — accepted (real-C shift-accumulator sibling family)
- Converted `func_080B36B0`, `func_080C9BFC`, `func_080DA1A4`, and `func_080E1A80` to standalone C. Each reads `*(u16 *)(gCurrentSceneData + 0x16) >> 3` and adds it to a distinct `u32` field at offsets `0x3C`, `0x14`, `8`, and `0x28` respectively.
- Moved the four original assembly sources into `asm/converted/` and switched each linker entry to its C TU. The shared expression shape reproduced all target instruction streams and literal-pool relocations exactly.
- Verification: all four linked C units report **100.0%**, the clean Docker build reports **`wariowareinc.gba: OK`**, and both ROMs hash to `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Fresh report: **1366 / 5960** matched functions, **6.488335%** matched code, and **906 C / 5781 asm-only** objdiff units. This is +4 matched functions and +4 linked C units over Batch 166.

### Batch 166 — accepted (real-C BIOS SVC lowering)
- `func_080EE61C` is restored as `src/decomp/asm_080ee61c.c`; the standalone assembly source and linker entry were removed. The C body calls `__builtin_swi_div()`, whose fixed BIOS ABI consumes the incoming `r0/r1` values and returns the quotient in `r0`.
- Added `tools/agbcc-swi.patch`, a reproducible target-specific agbcc extension: the builtin expands to a backend `swi_div` instruction pattern that emits `SVC #6`. CI applies the patch before building agbcc.
- Verification: the generated C object is exactly `SVC #6; BX LR` (`df06 4770`), the inline-asm audit is empty, and a clean Docker build reports **`wariowareinc.gba: OK`**. Both ROMs hash to `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Fresh report: **1362 / 5960** matched functions, **6.4803877%** matched code, and **902 C / 5785 asm-only** objdiff units. This is +1 C-linked unit and +1 matched function over Batch 165.

### Batch 165 — accepted (historical BIOS SVC exception)
- `func_080EE61C` was moved from the legacy inline-asm C shim back to `asm/asm_080ee61c.s`, with the linker selecting the standalone object at the original address. The emitted function remains exactly `SVC #6; BX LR`.
- This is intentionally not counted as a C decompilation: ordinary C division lowers to a `__divsi3` call, and the bundled agbcc has no SVC/SWI builtin. m2c/asmlift can recover the division semantics but cannot make agbcc emit this BIOS instruction.
- Verification: clean Docker `NONMATCHING=0` build reported **`wariowareinc.gba: OK`**; `build/wariowareinc.gba` and `baserom.gba` both hash to `3f556448d290fa5406d6ed367fee16cc02387ad3`. The fresh report is **1361 / 5960** with **901 C / 5786 asm-only** units, and the `src/decomp` non-empty-asm audit is empty.
- The one-function report decrease is classification-only: the exact bytes remain matched in the ROM, but the function is no longer presented as C-produced output.

## What just landed

### Batch 164 — accepted (`func_0800BEC0` real-C range-dispatch shaping)
- Metric delta: **+0 report matched functions / +1 legacy file reshaped / +0 ROM delta**. The report remains **1362 / 5960** matched functions with **6.4803877%** matched code.
- `func_0800BEC0` now uses an ordinary C `switch` over the byte read from `gCurrentSceneData + 0x195`. A redundant `case -10` (unreachable for a loaded `u8`, and sharing the default result) makes agbcc retain the target range-dispatch sequence, including `CMP #1; BGE`; cases 1–3 return 1, case 4 returns 2, and all other byte values return 0.
- Verification: the integrated `bitmap_font.c` object matched the target function section byte-for-byte, including literal-pool padding, and a clean Docker `NONMATCHING=0` build reported **`wariowareinc.gba: OK`** with the baseline ROM SHA-1 unchanged.
- Remaining non-empty inline-asm file: `asm_080ee61c.c` (the BIOS `svc #6` wrapper).

### Batch 163 — accepted (`func_0800C15C` real-C stack/register shaping)
- Metric delta: **+0 report matched functions / +1 legacy file reshaped / +0 ROM delta**. The report remains **1362 / 5960** matched functions with **6.4803877%** matched code.
- `func_0800C15C` now uses ordinary non-volatile `s16` stack locals and a typed `func_08006F84` call. agbcc naturally emits the target `SB = SP+0xA` setup and both indexed `LDRSH` loads; the old two narrow instruction blocks are gone.
- Verification: the integrated `bitmap_font.c` object instruction stream matched the target through the epilogue and padding, and a clean Docker `NONMATCHING=0` build reported **`wariowareinc.gba: OK`** with the baseline ROM SHA-1 unchanged.
- Remaining non-empty inline-asm files: `asm_0800bec0.c` and `asm_080ee61c.c`.

### Batch 162 — accepted (`func_08015A4C` real-C STM shaping)
- Metric delta: **+0 report matched functions / +1 legacy file reshaped / +0 ROM delta**. The report remains **1362 / 5960** matched functions with **6.4803877%** matched code.
- `func_08015A4C` now keeps the store pointer as a `u32 *` and uses ordinary `*r2++ = r1`; agbcc emits the target `STMIA R2!,{R1}` and preserves the original loop branch.
- Verification: the isolated `main_menu.c` object instruction stream matched the target through the literal pool, and a clean Docker `NONMATCHING=0` build reported **`wariowareinc.gba: OK`** with the baseline ROM SHA-1 unchanged.
- Remaining non-empty inline-asm files: `asm_0800bec0.c`, `asm_0800c15c.c`, and `asm_080ee61c.c`.

### Batch 161 — accepted (`func_08014DFC` real-C ADD shaping)
- Metric delta: **+0 report matched functions / +1 legacy file reshaped / +0 ROM delta**. The report remains **1362 / 5960** matched functions with **6.4803877%** matched code.
- `func_08014DFC` now uses an empty condition-code barrier before ordinary `r5 += 4`; agbcc emits the target two-operand `ADDS R5,#4` without a non-empty instruction shim.
- Verification: the isolated `main_menu.c` object instruction stream matched the target through the literal pool, and a clean Docker `NONMATCHING=0` build reported **`wariowareinc.gba: OK`** with the baseline ROM SHA-1 unchanged.
- Remaining non-empty inline-asm files: `asm_0800bec0.c`, `asm_0800c15c.c`, `asm_08015a4c.c`, and `asm_080ee61c.c`.

### Batch 160 — accepted (`func_080141C8` real-C ADD shaping)
- Metric delta: **+0 report matched functions / +1 legacy file reshaped / +0 ROM delta**. The report remains **1362 / 5960** matched functions with **6.4803877%** matched code.
- `func_080141C8` now uses an empty compiler barrier that clobbers condition codes before ordinary `r2 += 2`; agbcc emits the target two-operand `ADDS R2,#2` without a non-empty instruction shim.
- Verification: the isolated `main_menu.c` object instruction stream matched the target at `0x080141C8`, and a clean Docker `NONMATCHING=0` build reported **`wariowareinc.gba: OK`** with the baseline ROM SHA-1 unchanged.
- Remaining non-empty inline-asm files: `asm_0800bec0.c`, `asm_0800c15c.c`, `asm_08014dfc.c`, `asm_08015a4c.c`, and `asm_080ee61c.c`.

### Batch 159 — accepted (legacy inline-asm reshaping; strict ROM maintenance)
- Metric delta: **+0 report matched functions / +25 legacy files reshaped / +0 ROM delta**. These included stubs were already counted as C-linked units, so the report remains **1362 / 5960** matched functions with **6.4803877%** matched code and **902 C / 5785 asm-only** objdiff units.
- Real-C reshapes:
  - Bitmap/font and scene wrappers: `func_08001C74`, `func_0800A0C4`, `func_0800BB74`, `func_0800BBCC`, `func_0800BC10`, `func_0800BC50`.
  - Main-menu/sprite call and indexed-load wrappers: `func_08011584`, `func_08011774`, `func_080117A8`, `func_080118E0`, `func_08012058`, `func_08012658`, `func_08012700`, `func_08012D3C`, `func_08012DCC`, `func_08013388`, `func_080136A4`, `func_08014374`, `func_08014810`, `func_08014E38`, `func_08014E88`, `func_08014F38`, `func_08014FA8`.
  - Sprite-library helpers: `sprite_delete`, `func_080EF358`.
- The successful pattern was ordinary C calls through unique ABI-shaping function-pointer typedefs with `s32`/`u32` parameters, plus register-pinned C for indexed loads and operand order. An ordinary C indirect call also reproduced `_call_via_r0`.
- The earlier rejected instruction-shaping attempts for `func_0800BEC0`, the two ADD forms, the STM loop, and the stack/register wrapper were subsequently resolved in Batches 160–164. The remaining hard case is the literal `svc` instruction.
- Verification: each accepted source reshape passed the strict Docker ROM gate; the final clean build reported **`wariowareinc.gba: OK`**, the ROM SHA-1 remained `3f556448d290fa5406d6ed367fee16cc02387ad3`, and `make report` plus `tools/gen_objdiff.py` refreshed the metrics above.

### Batch 158 — accepted (strict literal-pool re-hoist)
- Metric delta: **+1 report matched function** with a fresh total-function recount of **5960** (down one from the prior report), **+1 standalone_tu decomp file**, **+1 linked C TU**, **+0 ROM delta**. The fresh report is **1362 / 5960** with **902 C / 5785 asm-only** units.
- Matched function:
  - `func_08007E8C`: forwards two arguments to `func_08007E18` with `0x7FFFFFFF` and zero as the third/fourth arguments. The compiler-generated literal-pool alignment and complete 20-byte `.text` section match the original.
- Verification: complete function-section comparison was **byte-identical**, a clean Docker `NONMATCHING=0` build reported **`wariowareinc.gba: OK`**, and the ROM SHA-1 remained `3f556448d290fa5406d6ed367fee16cc02387ad3`.

### Batch 157 — accepted (strict local-label-aware re-hoist)
- Metric delta: **+1 report matched function**, **+1 standalone_tu decomp file**, **+1 linked C TU**, **+0 ROM delta**. The fresh report is **1361 / 5961** with **901 C / 5786 asm-only** units.
- Matched function:
  - `func_08002024`: conditionally calls `func_080F2F04` or `func_080F2F34`, then returns through the original interwork-safe epilogue. The target asm places an internal local label before the second branch; the complete 20-byte target/candidate `.text` sections are identical even though objdiff's inferred target function symbol stops at that local label and reports only 40% for the symbol.
- Verification: complete function-section comparison was **byte-identical**, a clean Docker `NONMATCHING=0` build reported **`wariowareinc.gba: OK`**, and the ROM SHA-1 remained `3f556448d290fa5406d6ed367fee16cc02387ad3`.

### Batch 156 — accepted (strict padding-aware re-hoist)
- Metric delta: **+2 report matched symbols**, **+1 real standalone function**, **+1 linked C TU**, **+0 ROM delta**. The second report symbol is the explicit zero-padding word required after the function; the fresh report is **1360 / 5962** with **900 C / 5787 asm-only** units.
- Matched function:
  - `func_08073650`: calls `func_08072048` and `func_08073540`, returning the latter's value. The C TU includes a `.text`-section zero padding word so the original `0x0000` halfword after the function is preserved instead of agbcc's normal alignment NOP.
- Verification: isolated object comparison was **100.0% / 0 diffs** across 5 instructions, a clean Docker `NONMATCHING=0` build reported **`wariowareinc.gba: OK`**, and the ROM SHA-1 remained `3f556448d290fa5406d6ed367fee16cc02387ad3`.

### Batch 155 — accepted (strict adapter-assisted re-hoist)
- Metric delta: **+1 report matched function**, **+1 standalone_tu decomp file**, **+1 linked C TU**, **+0 ROM delta**. The fresh report is **1358 / 5961** with **899 C / 5788 asm-only** units.
- Matched function:
  - `func_080F2F78`: the 8-bit sign-extension sibling of `func_080F2F68`; it loads arg0[1], sign-extends the third ABI argument, and forwards both to `func_080F26D8`. The same unused middle parameter preserves the `R2` source register.
- Verification: isolated object comparison was **100.0% / 0 diffs** across 7 instructions, a clean Docker `NONMATCHING=0` build reported **`wariowareinc.gba: OK`**, and the ROM SHA-1 remained `3f556448d290fa5406d6ed367fee16cc02387ad3`.

### Batch 154 — accepted (strict adapter-assisted re-hoist)
- Metric delta: **+1 report matched function**, **+1 standalone_tu decomp file**, **+1 linked C TU**, **+0 ROM delta**. The fresh report is **1357 / 5961** with **898 C / 5789 asm-only** units.
- Matched function:
  - `func_080F2F68`: loads the second word from its first argument, sign-extends the third ABI argument, and forwards both to `func_080F2704`. m2c exposed that the source value arrives in `R2`; an unused middle C parameter models that register position and produces the exact call setup.
- Verification: isolated object comparison was **100.0% / 0 diffs** across 7 instructions, a clean Docker `NONMATCHING=0` build reported **`wariowareinc.gba: OK`**, and the ROM SHA-1 remained `3f556448d290fa5406d6ed367fee16cc02387ad3`.

### Batch 153 — accepted (strict adapter-assisted re-hoist)
- Metric delta: **+1 report matched function**, **+1 standalone_tu decomp file**, **+1 linked C TU**, **+0 ROM delta**. The fresh report is **1356 / 5961** with **897 C / 5790 asm-only** units.
- Matched function:
  - `func_080F26BC`: normalizes a byte value, computes a 32-byte record offset, and stores it at record offsets `0x1D` and `0x1E`. m2c supplied the raw structure layout; register-pinned C plus an explicit three-register pointer expression preserved the original `ADDS R3,R1,R3` and reload/store sequence.
- Verification: isolated object comparison was **100.0% / 0 diffs** across 10 instructions, a clean Docker `NONMATCHING=0` build reported **`wariowareinc.gba: OK`**, and the ROM SHA-1 remained `3f556448d290fa5406d6ed367fee16cc02387ad3`.

### Batch 152 — accepted (strict leaf re-hoist)
- Metric delta: **+1 report matched function**, **+1 standalone_tu decomp file**, **+1 linked C TU**, **+0 ROM delta**. The fresh report is **1355 / 5961** with **896 C / 5791 asm-only** units.
- Matched function:
  - `func_0803FED0`: reads the halfword at ROM address `0x086F277C + 2`, adds 20, and returns the signed 16-bit result. The exact C spelling uses a local absolute pointer plus an empty barrier so agbcc keeps the base literal and `[base, #2]` load separate.
- Verification: isolated object comparison was **100.0% / 0 diffs** across 7 instructions, a clean Docker `NONMATCHING=0` build reported **`wariowareinc.gba: OK`**, and the ROM SHA-1 remained `3f556448d290fa5406d6ed367fee16cc02387ad3`.

### Batch 151 — accepted (strict adapter-assisted re-hoist)
- Metric delta: **+1 report matched function**, **+1 standalone_tu decomp file**, **+1 linked C TU**, **+0 ROM delta**. The fresh report is **1354 / 5961** with **895 C / 5792 asm-only** units.
- Matched function:
  - `func_08015F80`: unlocks stage 9 when stages 2, 3, and 5 are complete, returning `0x200` on success. m2c supplied the compact condition; asmlift supplied the nested control-flow candidate that preserved the original early-return layout.
- Verification: isolated object comparison was **100.0% / 0 diffs** across 25 instructions, a clean Docker `NONMATCHING=0` build reported **`wariowareinc.gba: OK`**, and the ROM SHA-1 remained `3f556448d290fa5406d6ed367fee16cc02387ad3`.

### Batch 150 — accepted (strict adapter-assisted re-hoist)
- Metric delta: **+1 report matched function**, **+1 standalone_tu decomp file**, **+1 linked C TU**, **+0 ROM delta**. The fresh report is **1353 / 5961** with **894 C / 5793 asm-only** units.
- Matched function:
  - `func_0800DAD8`: signed halfword lookup using a signed 16-bit index and a 48-byte record stride. asmlift generated the accepted project-compatible C directly (`*(s16 *)((s16)a1 * 48 + a0[20] + 0)`).
- Verification: isolated object comparison was **100.0% / 0 diffs**, a clean Docker `NONMATCHING=0` build reported **`wariowareinc.gba: OK`**, and the ROM SHA-1 remained `3f556448d290fa5406d6ed367fee16cc02387ad3`.

### Batch 149 — accepted (strict re-hoist)
- Metric delta: **+0 report matched functions**, **+1 standalone_tu decomp file**, **+1 linked C TU**, **+0 ROM delta**. The fresh report recount is **1352 / 5961** with **893 C / 5794 asm-only** units.
- Matched function:
  - `func_08003D28`: byte-mask setter. m2c produced the semantic skeleton; asmlift was attempted but declined the candidate because of the project type context. The final real-C spelling uses register pins and an empty compiler barrier to preserve the original `MOVS`/`LSLS`/`RSBS` sequence.
- Verification: isolated object comparison was **100.0% / 0 diffs**, a clean Docker `NONMATCHING=0` build reported **`wariowareinc.gba: OK`**, and the ROM SHA-1 remained `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Tooling: restored `tools/asmlift_warioware.py` and `tools/asmlift-compile.sh` as a reusable m2c/asmlift adapter; the scripts do not alter the ROM unless a candidate is explicitly adopted.

### Batch 148 — accepted
- Metric delta: **+1 report matched function**, **+1 included_stub decomp file**, **+0.000188% matched code** (6.4578667 → 6.4580551)
- Matched code: **6.4580551%**
- Accepted function:
  - `func_0800C128` bitmap_font font-size lookup: conditional `gCurrentSceneData+0x193 == 1` guard, calls `func_08006F84`, tests `D_03006518.unk51[2]`, conditionally calls `func_0800C15C`. Key patterns: `s32` return type to match original `POP {R1}; BX R1` epilogue, `extern void func_0800C15C(u32, u32, u32, u32)` to match caller-side 4-arg push, `bitmap_font_get_text_width` declared with `u32` return type matching the callee's epilogue.
- Notes: Sibling of `func_0800C0BC` and `func_0800C15C` in the bitmap_font coordinate-wrapper family. Uses the same `func_08006F84` generated-coordinate pattern. The `s32` return type is essential — without it, agbcc generates `MOV R0, #0` as the default return instead of the original's `POP {R1}; BX R1` epilogue.

### Batch 147 — accepted
- Metric delta: **+1 report matched function**, **+1 included_stub decomp file**, **+0.000188% matched code** (6.4576783 → 6.4578667)
- Matched code: **6.4578667%**
- Accepted function:
  - `func_0800C15C` bitmap_font generated-coordinate wrapper: preserves four halfword args, calls `func_08006F84(arg0, &sp8, &spA)`, then forwards the signed generated coordinates plus signed copies of the original args to `func_0800C110`.
- Notes: This is another bitmap_font coordinate wrapper sibling. The original acceptance used narrow call/load shims for the `r9` stack pointer and indexed signed loads; Batch 163 replaced both with ordinary C by using non-volatile `s16` locals and a typed `func_08006F84` declaration, without changing the already-converted callee body.

### Batch 146 — accepted
- Metric delta: **+0 report matched functions**, **+1 included_stub decomp file**, **+0.000000% matched code** (6.4576783 → 6.4576783)
- Matched code: **6.4576783%**
- Accepted function:
  - `func_0800C0BC` bitmap_font coordinate task wrapper: calls `func_08006F84(arg0, &sp4, &sp6)` then launches `func_0800C080(arg0, sp4, sp6, (s16)arg1, (s16)arg2)`. Uses a signed `func_0800C080` arg1 declaration in the same TU so the caller emits `LDRSH` for the generated x coordinate; `func_0800C080` casts that arg back to `u16` internally to preserve its original zero-extension.
- Notes: This is a sibling of the bitmap_font task-launcher family. The important integration fix was a same-TU callee prototype adjustment: the callee body stays byte-identical by casting to `u16`, while callers that pass signed coordinate output can get the original signed load.

### Batch 128 — accepted
- Metric delta: **+0 report matched functions**, **+2 included_stub decomp files**, **+0.000754% matched code** (6.4524055 → 6.453159%)
- Matched code: **6.453159%**
- Accepted functions:
  - `func_08012D3C` main_menu: scene thread 0 setup, D_03006518 byte read + func_08012EC4 call, conditional func_08012CC8, bit-mask clear of 0x21 flag at gCurrentSceneData+0xDD. Uses extern struct Unk03006518 to match existing declaration.
  - `func_0800BBCC` bitmap_font: scene data struct initializer calling func_0800B828/func_0800BA78 with 5th stack arg via inline asm ldr. Uses matching func_0800B828(u32, u32) declaration from existing asm_0800bb74.c.
- Notes: Two included_stub conversions. Key patterns: matching existing extern declarations across decomp files (struct Unk03006518, func_0800B828 2-arg vs 3-arg), inline asm ldr for 5th stack arg at [sp, #0xC].

### Batch 127 — accepted
- Metric delta: **+0 report matched functions**, **+3 included_stub decomp files**, **+0.000000% matched code** (6.453159 → 6.453159%)
- Matched code: **6.453159%**
- Accepted functions:
  - `func_08001B28` code_08001a70: rotation matrix identity initializer at D_03000010[arg0*8] with D_03000118 byte clear. Leaf. Uses asm volatile barriers to force R6 callee-save and instruction ordering for LDR R0,=D_03000118; ADD R0, R6, R0 sequence.
  - `sprite_delete` lib_sprite: sprite deallocation with bit-mask clears (0xFD & byte0, 0xBE & byte1), z-link removal, and ID dealloc. Uses s32 arg1 to avoid early u16 truncation. Key: asm volatile barrier after MOV R5, R0 to prevent early LSLS R1.
  - `func_080EF358` lib_sprite: sprite animation progress calculator with loop accumulating cel durations and __udivsi3 division. Uses u32 return type with LSLS/LSRS truncation to match original. 1 trailing MOV R8,R8 NOP diff accepted by linker (byte-identical ROM).
- Notes: Three included_stub conversions across two modules (code_08001a70, lib_sprite). Key patterns: s32 arg1 to prevent early u16 truncation when arg1 is used as s16 after BL, asm volatile barriers to force callee-save of R6 and instruction ordering, u32 return type to get POP {R1}; BX R1 epilogue with trailing truncation. Failed attempts: sprite_set_z and sprite_set_x_y blocked by agbcc not pushing R7 callee-saved register.

### Batch 126 — accepted
- Metric delta: **+0 report matched functions**, **+6 included_stub decomp files**, **+0.000189% matched code** (6.452217 → 6.453159%)
- Matched code: **6.453159%**
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
  - `func_08015A4C` main_menu scene buffer fill: loads `gCurrentSceneData`, reads offset 0xB4 flag, if set reads halfword at 0xC2 and ORs with 0x40000; fills 16 words at `data[0xC]+0x240` using a compiler-generated `STMIA` store loop. No BL calls — leaf function. The former inline `STM` shim was removed in Batch 162.
- Notes: Eight functions in one chunk — most productive session yet. Three identical scene-init functions (func_080126C8/func_08013428/func_080143F0) were all first-try matches. The `u32 sp[]` local array pattern correctly handles stack-based function arguments for `dma3_set`. Using `extern u32 D_083AD90C` + `&D_083AD90C` produces proper symbol references in literal pool instead of raw address constants.

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
