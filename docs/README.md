# WarioWare Inc. decomp docs

This docs set is now the canonical replacement for the old Ralph task file flow.
If an agent resumes cold, read these first:

1. `docs/wariowareinc-decomp-scaleup.md` — current baseline, priorities, next queue
2. `docs/decomp-agent-workflow.md` — exact autonomous workflow and verification loop
3. `docs/decomp-pattern-library.md` — proven families, code-shaping rules, known traps
4. `docs/decomp-batch-history.md` — accepted batch history
5. `docs/decomp-tooling-feedback.md` — tooling gaps, workarounds, and improvement notes
6. `docs/windows-tooling-notes.md` — Windows/MSYS2/Docker path issues and fixes

## Current verified baseline
- Verified working tree: `batch 300` — `func_08007EAC` now emits the linked-list heap teardown through ordinary C
- `build/report.json`: **1721 / 5924 matched functions** (**29.051315%**) · **7.743778%** matched code (**76962 / 993856**)
- `tools/gen_objdiff.py`: **1261 linked C TUs / 5426 non-C units** (**6687 total**)
- `src/decomp/*.c`: **1462 decompiled function files** = **1242 standalone_tu** + **220 included_stub**
- ROM: **`wariowareinc.gba: OK`**
- Latest accepted maintenance pass: **38 legacy included-stub files** use real C and ABI/register shaping instead of non-empty inline-asm call/load shims; batches 256–286 additionally removed two hundred seventy compiler register pins across ninety-one already-linked functions. Report function/unit metrics are unchanged because these files were already C-linked.
- Remaining naked/original asm wrapper files in `src/decomp`: **0**; remaining compiler-register-pin files: **106 files / 502 pins**
- Remaining non-volatile empty compiler barriers: **7 files / 7 barriers**; the full strict audit also reports **27 files / 31 empty barrier findings** when volatile barriers coexisting with legacy pins are included. Remaining instruction-bearing inline-asm decomp files: **0**
- `func_080EE61C` is now an ordinary C TU using the target-specific `__builtin_swi_div`; `tools/agbcc-swi.patch` makes the lowering reproducible in local/CI compiler builds
- 25% milestone at the current function total: **1481 / 5924**; now exceeded by **240** matches
- 26% milestone at the current function total: **1541 / 5924**; now exceeded by **180** matches
- 27% active working goal at the current function total: **1600 / 5924**; exceeded by **121** matches
- 30% milestone at the current function total: **1778 / 5924**; **57** more matches needed
- 80% target at the current function total: **4740 / 5924**
- Remaining gap to 80%: **3019 matched functions**

### Batch 300 — one exact ordinary-C linked-list heap teardown (2026-08-25)
- Converted `func_08007EAC` from `asm/asm_08007eac.s` to a strict ordinary-C standalone TU. A named node exposes the owned payload at `+4` and the forward link at `+0x14`; the source saves the successor before freeing the payload and node, then clears `D_0300485C`. It contains no instruction asm, barriers, register pins, non-mapped volatile, or opaque offset-heavy layout.
- The isolated linked-ELF screen reported only the legacy target's truncated 10-byte function-symbol boundary versus the complete 48-byte candidate section. The guarded force path waived that metadata only; the integrated clean Docker ROM/report gate matched the baseline ROM exactly at SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Fresh Docker report is **1721 / 5924** functions and **76962 / 993856** matched code. Unit coverage is **1261 C / 5426 asm-only**; decomp files are **1462** (**1242 standalone_tu / 220 included_stub**). The denominator drops by one because the converted legacy local-label boundary is no longer counted as a separate inferred function while total units remain unchanged.
- Evidence: `.decomp-runs/round-171-isolation.json`, `.decomp-runs/round-171-08007EAC-v1-source-audit.json`, `.decomp-runs/round-171-08007EAC-apply.json`, `.decomp-runs/round-171-full-source-audit.json`, and `.nearmiss/func_08007EAC.json`.

### Batch 299 — one exact ordinary-C standalone aligned stack-task wrapper (2026-08-25)
- Converted `func_08004C94` from `asm/asm_08004c94.s` to a strict ordinary-C standalone TU. A natural four-word local record supplies the target's aligned `0x14` frame while only its first three words are forwarded, and a separate `u16 id` local keeps the `LSLS`/`LSRS` normalization before the stack stores; the source has no instruction asm, barriers, register pins, non-mapped volatile, or opaque offset-heavy layout.
- Added the canonical `D_083A49EC = 0x083A49EC` linker-map definition already declared by `include/undefined_syms.inc`. The isolated linked-ELF screen reported only a legacy symbol-boundary metadata gap. After resolving that ROM symbol in Docker, the complete 40-byte linked `.text` sections matched byte-for-byte with SHA-256 `8eb8d7ed27b4832c77715c2a11e49b34df148f72e87e641b0548a2c5d094fa72`; the clean Docker ROM gate accepted the conversion.
- Fresh Docker report is **1720 / 5925** functions and **76914 / 993856** matched code. Unit coverage is **1260 C / 5427 asm-only**; decomp files are **1461** (**1241 standalone_tu / 220 included_stub**). The report's total-function denominator is one lower than Batch 298 while total units remain unchanged. ROM/base ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Evidence: `.decomp-runs/round-169-isolation-v2.json`, `.decomp-runs/round-169-08004C94-full-text-proof.json`, `.decomp-runs/round-169-08004C94-v4-source-audit.json`, `.decomp-runs/round-169-08004C94-apply.json`, and `.decomp-runs/round-169-08004C94-full-source-audit.json`.

### Batch 298 — one exact ordinary-C standalone callback/mask wrapper (2026-08-25)
- Converted `func_08006700` from `asm/asm_08006700.s` to a strict ordinary-C standalone TU with a named state overlay. The unsigned shift expression preserves the low-12-bit predicate, while separate `value` and `mask` locals preserve the target's `0xFFFFF000` literal-pool AND after the optional callback; the source has no instruction asm, barriers, register pins, non-mapped volatile, or opaque offset-heavy layout.
- The isolated linked-ELF screen reported a metadata-only near miss because the legacy target's local labels inferred a shorter function boundary. A separate Docker full-section proof matched the complete 52-byte `.text` sections byte-for-byte with SHA-256 `070326b04284a6b44f6cf809ab2ac3226ac1e8b9c78f047cf6bf331f16a4d44c`; the guarded force path waived only that symbol-boundary metadata, and the clean Docker ROM gate accepted the source replacement.
- Fresh Docker report is **1719 / 5926** functions and **76874 / 993854** matched code. Unit coverage is **1259 C / 5428 asm-only**; decomp files are **1460** (**1240 standalone_tu / 220 included_stub**). The report's total-function denominator is one lower than the previous report while total code and total units remain unchanged. ROM/base ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Evidence: `.decomp-runs/round-168-isolation-v4.json`, `.decomp-runs/round-168-08006700-full-text-proof.json`, `.decomp-runs/round-168-08006700-v4-source-audit.json`, `.decomp-runs/round-168-08006700-apply.json`, and `.decomp-runs/round-168-08006700-full-source-audit.json`.

### Batch 297 — two exact ordinary-C standalone clamp and scene wrappers (2026-08-25)
- Converted `func_080B3690` from `asm/asm_080b3690.s` to a strict ordinary-C standalone TU with a named state record. The two ordered comparisons preserve the target's upper-bound reset and negative-value clamp, with no instruction asm, barriers, register pins, non-mapped volatile, or opaque offset-heavy layout.
- Converted `func_0800E764` from `asm/asm_0800e764.s` to a strict ordinary-C standalone TU with a named scene-data overlay. It loads the sprite handler before the scene base, reads the +0x2D0 halfword, and calls `sprite_set_visible` in target order; the source is strict-clean ordinary C.
- Docker-compiled complete `.text` sections matched byte-for-byte: `func_080B3690` at 32 bytes with SHA-256 `6e0c50c5e6d8479d4d915bbb8f9544acb7c9c701583d09210c12ff23bc33d26a`, and `func_0800E764` at 40 bytes with SHA-256 `aeaaf3b98b8d86c44ea4ff75d8b7bf4d2dd518dd65f2508ac351016fe7683a81`. Both isolated exact and passed the transactional clean Docker ROM gate.
- Fresh Docker report is **1718 / 5927** functions and **76822 / 993854** matched code. Unit coverage is **1258 C / 5429 asm-only**; decomp files are **1459** (**1239 standalone_tu / 220 included_stub**). ROM/base ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Evidence: `.decomp-runs/round-167-isolation-v2.json`, `.decomp-runs/round-167-080B3690-full-text-proof.json`, `.decomp-runs/round-167-0800E764-full-text-proof.json`, `.decomp-runs/round-167-080B3690-apply.json`, `.decomp-runs/round-167-0800E764-apply.json`, both targeted source audits, and `.decomp-runs/round-167-full-source-audit.json`.

### Batch 296 — two exact ordinary-C standalone packed-record wrappers (2026-08-25)
- Converted `func_08078F28` from `asm/asm_08078f28.s` to a strict ordinary-C standalone TU with a named packed-record overlay. The explicit gap at +0x8 exposes the target's +0xA halfword, and a separate denominator local preserves the target's divisor load before `0x80000 / denominator`; the source has no instruction asm, barriers, register pins, non-mapped volatile, or opaque offset-heavy layout.
- Converted `func_08004E28` from `asm/asm_08004e28.s` to a strict ordinary-C standalone TU with a named pointer record. It narrows the third argument before the helper call, reloads the record's old pointer for deallocation instead of caching it across calls, and stores the helper result in target order; the source is strict-clean ordinary C.
- Docker-compiled complete `.text` sections matched byte-for-byte: `func_08078F28` at 28 bytes with SHA-256 `650f2b671d620221198d0db86813d1f8ea0a7ea6b565c6e2462de54d1eb6de29`, and `func_08004E28` at 32 bytes with SHA-256 `b9a389c3b720996b9ab159839f23dca947d164df98543e8ff6c0c4f1a70bab5d`. Both isolated exact and passed the transactional clean Docker ROM gate.
- Fresh Docker report is **1716 / 5927** functions and **76752 / 993844** matched code. Unit coverage is **1256 C / 5431 asm-only**; decomp files are **1457** (**1237 standalone_tu / 220 included_stub**). ROM/base ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Evidence: `.decomp-runs/round-166-isolation-v2.json`, `.decomp-runs/round-166-08078F28-full-text-proof.json`, `.decomp-runs/round-166-08004E28-full-text-proof.json`, `.decomp-runs/round-166-08078F28-apply.json`, `.decomp-runs/round-166-08004E28-apply.json`, both targeted source audits, and `.decomp-runs/round-166-full-source-audit.json`.

### Batch 295 — one exact ordinary-C standalone mask-order initializer (2026-08-25)
- Converted `func_0804F464` from `asm/asm_0804f464.s` to a strict ordinary-C standalone TU with a named record overlay. The source keeps the loaded byte, `-0x10` mask value, and final combine as separate ordinary locals, reproducing the target's `MOVS #0; SUBS #0x10; ANDS` sequence without instruction asm, barriers, register pins, non-mapped volatile, or opaque offset-heavy layout.
- Docker-compiled target and candidate `.text` sections matched byte-for-byte at 28 bytes with SHA-256 `132cd2024c6abc70cd640ff693bd22cb991ccf3e0d36f447dcf8205f09026486`; normalized isolation was exact and the transactional clean Docker ROM gate passed.
- Fresh Docker report is **1714 / 5927** functions and **76694 / 993844** matched code. Unit coverage is **1254 C / 5433 asm-only**; decomp files are **1455** (**1235 standalone_tu / 220 included_stub**). ROM/base ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Evidence: `.decomp-runs/round-165-isolation.json`, `.decomp-runs/round-165-0804F464-full-text-proof.json`, `.decomp-runs/round-165-0804F464-apply.json`, `.decomp-runs/round-165-0804F464-source-audit.json`, and `.decomp-runs/round-165-full-source-audit.json`.

### Batch 294 — two exact ordinary-C standalone return-shaped wrappers (2026-08-25)
- Converted `init_scheduled_function_task` from `asm/asm_08007db0.s` to a strict ordinary-C standalone TU with a named three-word allocation record. Returning the allocated record preserves the target's live R0 result and its `POP {R1}; BX R1` interworking epilogue; the source has no instruction asm, barriers, register pins, non-mapped volatile, or opaque offset-heavy layout.
- Converted `func_080D3A60` from `asm/asm_080d3a60.s` to a strict ordinary-C standalone TU with a named scene overlay. The call remains before the scene-variable reload, and returning the address of the written halfword preserves the target's live field pointer and `POP {R1}; BX R1` epilogue; the source is strict-clean ordinary C.
- Docker-compiled complete `.text` sections matched byte-for-byte: `init_scheduled_function_task` at 28 bytes with SHA-256 `c2fa87380d2a8d57ae6b1ce372816ed3e46f7c509011814b233e106b6e8afd79`, and `func_080D3A60` at 28 linked bytes with SHA-256 `1a2f62a3748d5b94e619e661e633e1f389e4ddb145dfd98b730b54df00116866`. Both isolated as exact ordinary-C candidates and passed the transactional clean Docker ROM gate.
- Fresh Docker report is **1713 / 5927** functions and **76666 / 993844** matched code. Unit coverage is **1253 C / 5434 asm-only**; decomp files are **1454** (**1234 standalone_tu / 220 included_stub**). ROM/base ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Evidence: `.decomp-runs/round-164-isolation-v5.json`, `.decomp-runs/round-164-init-full-text-proof.json`, `.decomp-runs/round-164-080D3A60-full-text-proof.json`, `.decomp-runs/round-164-init-apply.json`, `.decomp-runs/round-164-d3-apply.json`, `.decomp-runs/round-164-init-source-audit.json`, `.decomp-runs/round-164-d3-source-audit.json`, and `.decomp-runs/round-164-full-source-audit.json`.

### Batch 293 — one exact ordinary-C standalone nullable-record lookup (2026-08-25)
- Converted `func_080020FC` from `asm/asm_080020fc.s` to a strict ordinary-C standalone TU with a named record containing the target's +0xC field. It returns zero for a null record and otherwise returns that field; the source has no instruction asm, barriers, register pins, non-mapped volatile, or opaque offset-heavy layout.
- Normalized linked-ELF isolation reported a target symbol of 10 bytes versus a candidate symbol of 16 bytes because the legacy target's internal null branch label ends the inferred function early. Docker-compiled target and candidate `.text` sections were independently extracted at the complete 16-byte size and matched byte-for-byte with SHA-256 `53786ccbc1988abd29d7171d20c8a030ab21d41fbca7ba9bb9069da97d6ccef4`; the guarded metadata-only force path waived only that boundary.
- Fresh Docker report is **1711 / 5927** functions and **76610 / 993844** matched code. Unit coverage is **1251 C / 5436 asm-only**; decomp files are **1452** (**1232 standalone_tu / 220 included_stub**). ROM/base ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Evidence: `.decomp-runs/round-163-080020FC-full-text-proof.json`, `.decomp-runs/round-163-080020FC-apply.json`, `.decomp-runs/round-163-080020FC-accepted-source-audit.json`, `.decomp-runs/round-163-080020FC-full-source-audit.json`, and `.nearmiss/func_080020FC.json`.

### Batch 292 — one exact ordinary-C standalone table-address helper (2026-08-25)
- Converted `func_080047D4` from `asm/asm_080047d4.s` to a strict ordinary-C standalone TU. It computes the two-byte index from the input character and returns the corresponding entry from the table pointer stored at `D_083A49E8`; the source uses ordinary typed pointer arithmetic with no instruction asm, barriers, register pins, non-mapped volatile, or opaque offset-heavy layout.
- Added the canonical `D_083A49E8 = 0x083A49E8` standalone linker-map definition, which was already present in `include/undefined_syms.inc`. Normalized isolation reported a target symbol of 14 bytes versus a candidate symbol of 20 bytes because the legacy target's local pool label ends the inferred function early. Docker-assembled target and linked candidate `.text` sections were independently extracted at the complete 20-byte size and matched byte-for-byte with SHA-256 `6db389b7381b9ff389dfe0368cb9226a19ca82d074650ce19cb86597f11551be`; the guarded metadata-only force path waived only that boundary.
- Fresh Docker report is **1710 / 5928** functions and **76594 / 993844** matched code. Unit coverage is **1250 C / 5437 asm-only**; decomp files are **1451** (**1231 standalone_tu / 220 included_stub**). ROM/base ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Evidence: `.decomp-runs/round-162-isolation-v4.json`, `.decomp-runs/round-162-080047D4-full-text-proof.json`, `.decomp-runs/round-162-080047D4-apply.json`, `.nearmiss/func_080047D4.json`, and the strict source audit generated for `src/decomp/asm_080047d4.c`.

### Batch 291 — one exact ordinary-C standalone graphics-entry wrapper (2026-08-25)
- Converted `func_08007FA4` from `asm/asm_08007fa4.s` to a strict ordinary-C standalone TU. It queries the existing graphics-entry list and forwards the input record's ID plus the matched entry field to `func_08007E8C`; the source uses named input/entry records with no instruction asm, barriers, register pins, non-mapped volatile, or opaque offset-heavy layout.
- Normalized linked-ELF isolation reported a target symbol of 22 bytes versus a candidate symbol of 28 bytes because the legacy target's internal return label ends the inferred function early. Docker-compiled target and candidate `.text` sections were independently extracted at the complete 28-byte size and matched byte-for-byte with SHA-256 `6ceb9b46801a30727bbe8404dd3929367d9dedc5c17289b1504b4f8590e8f866`; the guarded metadata-only force path waived only that boundary.
- Fresh Docker report is **1709 / 5929** functions and **76574 / 993842** matched code. Unit coverage is **1249 C / 5438 asm-only**; decomp files are **1450** (**1230 standalone_tu / 220 included_stub**). ROM/base ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Evidence: `.decomp-runs/round-161-08007FA4-apply.json`, `.decomp-runs/round-161-08007FA4-full-text-proof.json`, `.nearmiss/func_08007FA4.json`, and the strict source audit generated for `src/decomp/asm_08007fa4.c`.

### Batch 290 — one exact ordinary-C standalone heap cleanup (2026-08-25)
- Converted `func_08004378` from `asm/asm_08004378.s` to a strict ordinary-C standalone TU with a named 0x1C-byte cleanup record. It frees the three owned pointers in target order and then the record itself; the source has no instruction asm, barriers, register pins, non-mapped volatile, or opaque offset-heavy layout.
- Normalized linked-ELF isolation reported a target symbol of 26 bytes versus a candidate symbol of 38 bytes because the legacy target's internal return label ends the inferred function early. Docker-compiled target and candidate `.text` sections were independently extracted at the complete 40-byte size and matched byte-for-byte with SHA-256 `2df6ce90e997e4751ec7b273acbecec807b9ed0294e0f6c97cebbfd76fb68c62`; the guarded metadata-only force path waived only that boundary.
- Fresh Docker report is **1708 / 5930** functions and **76546 / 993842** matched code. Unit coverage is **1248 C / 5439 asm-only**; decomp files are **1449** (**1229 standalone_tu / 220 included_stub**). ROM/base ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Evidence: `.decomp-runs/round-161-isolation.json`, `.decomp-runs/round-161-08004378-accepted-source-audit.json`, `.decomp-runs/round-161-08004378-full-source-audit.json`, `.decomp-runs/round-161-08004378-full-text-proof.json`, `.decomp-runs/round-161-08004378-apply.json`, and `.nearmiss/func_08004378.json`.

### Batch 289 — one exact ordinary-C standalone graphics-flag reset (2026-08-25)
- Converted `func_08006CC8` from `asm/asm_08006cc8.s` to a strict ordinary-C standalone TU using the existing typed `gGraphicsBuffer` bitfields. It clears the two target flags with ordinary C; the source has no instruction asm, barriers, register pins, non-mapped volatile, or opaque offset-heavy layout.
- Normalized linked-ELF isolation reported a target symbol of 24 bytes versus a candidate symbol of 30 bytes because the legacy target's local literal-pool label ends the inferred symbol early. A separate link of target and candidate with the same absolute `gGraphicsBuffer` map produced identical complete 32-byte `.text` sections and SHA-256 `416cc334dbb73b34471b3b471474b35bd2428d0ed55b1abc970dfd199272eb4f`; the guarded metadata-only force path waived only that boundary, and the clean Docker ROM gate accepted the conversion.
- Fresh Docker report is **1707 / 5931** functions and **76508 / 993842** matched code. Unit coverage is **1247 C / 5440 asm-only**; decomp files are **1448** (**1228 standalone_tu / 220 included_stub**). ROM/base ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Evidence: `.decomp-runs/20260825T153337Z-isolation.json`, `.decomp-runs/round-160-accepted-source-audit.json`, `.decomp-runs/round-160-full-source-audit.json`, `.decomp-runs/round-160-apply.json`, and `.nearmiss/func_08006CC8.json`.

### Batch 288 — one exact ordinary-C standalone linked-list lookup (2026-08-25)
- Converted `func_08007F20` from `asm/asm_08007f20.s` to a strict ordinary-C standalone TU. The function scans the `D_0300485C` linked list, compares each node's ID, returns the matching node's data address, and falls back to the input pointer. A named node model exposes the ID, data, padding, and `next` fields; there is no instruction asm, barrier, register pin, non-mapped volatile, or opaque offset-heavy layout.
- Added the canonical `D_0300485C = 0x0300485C` standalone linker-map assignment. Normalized linked-ELF isolation reported a metadata-only near miss because the legacy target symbol was inferred as 12 bytes while the complete body is 40 bytes. Linking target and candidate with the same absolute symbol map produced identical 40-byte `.text` sections and SHA-256; the narrow metadata-only force path waived only that boundary, and the clean Docker ROM gate accepted the conversion.
- Fresh Docker report is **1706 / 5932** functions and **76476 / 993840** matched code. Unit coverage is **1246 C / 5441 asm-only**; decomp files are **1447** (**1227 standalone_tu / 220 included_stub**). ROM/base ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Evidence: `.decomp-runs/20260825T152105Z-isolation.json`, `.decomp-runs/round-159-accepted-source-audit.json`, `.decomp-runs/round-159-full-source-audit.json`, `.decomp-runs/round-159-apply.json`, and `.nearmiss/func_08007F20.json`.

### Batch 287 — one exact ordinary-C standalone string-record counter (2026-08-25)
- Converted `func_08004400` from `asm/asm_08004400.s` to a strict ordinary-C standalone TU. The function walks a two-byte record stream, skips `.` and `:`, calls `func_08004770` for the remaining records, and counts non-special records. The source has no instruction asm, barriers, register pins, non-mapped volatile, or opaque offset-heavy layout; the only raw evidence is the bounded cursor stride.
- Normalized linked-ELF isolation reported a false partial match because the legacy target symbol ended at internal label `_08004408` (8 bytes), while the candidate symbol covered the complete 44-byte function. A direct full-section SHA-256 proof matched target and candidate `.text` at 44 bytes; the narrow metadata-only force path waived only symbol coverage, and the clean Docker ROM gate accepted the conversion.
- Fresh Docker report is **1705 / 5933** functions and **76436 / 993840** matched code. The total-function denominator drops by one because the legacy local-label boundary is no longer counted as a separate inferred function after conversion. Unit coverage is **1245 C / 5442 asm-only**; decomp files are **1446** (**1226 standalone_tu / 220 included_stub**). ROM/base ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Evidence: `.decomp-runs/20260825T145642Z-isolation.json`, `.decomp-runs/round-154-accepted-source-audit.json`, `.decomp-runs/round-154-full-source-audit.json`, `.decomp-runs/round-154-apply.json`, and `.nearmiss/func_08004400.json`.

### Batch 286 — three exact ordinary-C main-menu scene cleanups (2026-08-23)
- Rewrote `func_080115DC`, `func_0801522C`, and `func_08015590` as pin-free included-stub C. The spellings use named scene overlays for DMA/heap/callback fields, explicit typed handle fields, and only bounded `+0xDE` byte-pointer evidence where the target register lifetime requires it.
- The isolated screen retained only included-TU pool boundaries and external-call relocation metadata for the selected bodies; earlier direct-field variants either changed register homes or failed the strict layout gate. The integrated clean Docker ROM gate accepted all three source-only replacements.
- Clean Docker build, report regeneration, `gen_objdiff.py`, targeted/full source audits, and `decomp_cycle.py verify --no-report` passed with `wariowareinc.gba: OK`; ROM/base ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Matching report metrics remain **1704 / 5934** functions and **76392 / 993840** code because all three were already C-linked. Pin residue drops **109 → 106 files / 521 → 502 pins**; full strict barrier residue remains **27 files / 31 findings**. Evidence: `.decomp-runs/round-152-scene-family-isolation.json`, `.decomp-runs/round-152-accepted-source-audit.json`, `.decomp-runs/round-152-full-source-audit.json`, `.decomp-runs/round-152-verify.json`, and `.decomp-runs/round-152-accepted-manifest.json`.

### Batch 285 — exact ordinary-C scene-data reload cleanup (2026-08-23)
- Rewrote `func_080152A0` with an ordinary `void **base`, explicit shifted `+0xC2` halfword address calculation, and a named post-call `+0xDD` scene byte field. Seven compiler register pins were removed without instruction asm, barriers, volatile codegen shims, or opaque offset-heavy layouts.
- The mixed layout spelling preserves the target `MOV #0xC2; LSL; ADD; LDRSH [R0,R1]` sequence while using the named field for the second reload. Its isolated candidate differed only by the known included-TU pool/padding boundary; the integrated clean ROM gate accepted the source-only replacement.
- Clean Docker verification, report regeneration, `gen_objdiff.py`, and strict source/policy audits passed with `wariowareinc.gba: OK`; ROM/base ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Matching report metrics remain **1704 / 5934** functions and **76392 / 993840** code because the function was already C-linked. Pin residue drops **110 → 109 files / 528 → 521 pins**; full strict barrier residue remains **27 files / 31 findings**. Evidence: `.decomp-runs/round-151-152a0-isolation-v3.json`, `.decomp-runs/round-151-152a0-accepted-source-audit.json`, `.decomp-runs/round-151-full-source-audit.json`, `.decomp-runs/round-151-152a0-verify.json`, and `.decomp-runs/round-151-accepted-manifest.json`.

### Batch 284 — exact ordinary-C scene-data pointer-lifetime cleanup (2026-08-23)
- Rewrote `func_080116D4` with ordinary `base`, `bytePtr`, `data`, mask, and reload locals. Eight compiler register pins were removed without instruction asm, barriers, volatile codegen shims, or opaque offset-heavy layouts.
- The selected v6 spelling preserved the target `&gCurrentSceneData` base lifetime, the +0xDF byte update, the shifted +0x9E word reload, and the call predicate. Its isolated candidate differed only by the known included-TU pool/padding boundary; the integrated clean ROM gate accepted the source-only replacement.
- Clean Docker build, report regeneration, `gen_objdiff.py`, and `decomp_cycle.py verify --no-report` passed with `wariowareinc.gba: OK`; ROM/base ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Matching report metrics remain **1704 / 5934** functions and **76392 / 993840** code because the function was already C-linked. Pin residue drops **111 → 110 files / 536 → 528 pins**; full strict barrier residue remains **27 files / 31 findings**. Evidence: `.decomp-runs/round-150-116d4-v6-v7-isolation.json`, `.decomp-runs/round-150-116d4-accepted-source-audit.json`, `.decomp-runs/round-150-full-source-audit.json`, `.decomp-runs/round-150-116d4-verify.json`, and `.decomp-runs/round-150-accepted-manifest.json`.

### Batch 283 — six exact ordinary-C packed-record setters (2026-08-23)
- Rewrote `func_080F2374`, `func_080F24A0`, `func_080F24C0`, `func_080F2558`, `func_080F2578`, and `func_080F26BC` with named 0x20-byte indexed records, integer base/offset locals, and ordinary mask/result lifetimes. Twenty-four compiler register pins and five empty compiler barriers were removed without instruction asm, volatile codegen shims, or opaque scalar-pointer layouts.
- The exact-only screen classified all six strict-clean spellings as exact. The first five preserve the target field/mask register roles through separate locals; `func_080F26BC` needs a distinct `base2` local for the second reload/store.
- Clean Docker build, report regeneration, `gen_objdiff.py`, and `decomp_cycle.py verify --no-report` passed with `wariowareinc.gba: OK`; ROM/base ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Matching report metrics remain **1704 / 5934** functions and **76392 / 993840** code because all six were already C-linked. Pin residue drops **117 → 111 files / 560 → 536 pins**; full strict barrier residue drops **32 → 27 files / 36 → 31 findings**, including non-volatile barriers **12 → 7 files / 12 → 7 findings**. Evidence: `.decomp-runs/round-149-exact-isolation.json`, `.decomp-runs/round-149-accepted-source-audit.json`, `.decomp-runs/round-149-full-source-audit.json`, `.decomp-runs/round-149-verify.json`, and `.decomp-runs/round-149-accepted-manifest.json`.

### Batch 282 — three exact ordinary-C task/arithmetic helpers (2026-08-23)
- Rewrote `func_08005870` with a named 0x1C-byte task record and ordinary scan locals, `func_080058AC` with a named active-task record and separate flag/value/task-ID locals, and `func_080F1B5C` with typed nested records for the +0xC pointer/+0x1F scale arithmetic. Eleven compiler register pins were removed without instruction asm, barriers, or volatile codegen shims.
- The final screen selected exact ordinary-C spellings for `func_08005870` and `func_080F1B5C`; `func_080058AC` was instruction-identical with only target function-symbol trailing pool-boundary metadata differing in isolation. The raw ordinary-C `func_080059E4` spelling was rejected by strict layout, and named variants retained real register-home differences.
- Clean Docker build, report regeneration, `gen_objdiff.py`, and `decomp_cycle.py verify --no-report` passed with `wariowareinc.gba: OK`; ROM/base ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Matching report metrics remain **1704 / 5934** functions and **76392 / 993840** code because all three were already C-linked. Pin residue drops **120 → 117 files / 571 → 560 pins**; full strict barrier residue remains **32 files / 36 findings**. Evidence: `.decomp-runs/round-148-isolation-v12.json`, `.decomp-runs/round-148-accepted-source-audit.json`, `.decomp-runs/round-148-full-source-audit.json`, `.decomp-runs/round-148-verify.json`, and `.decomp-runs/round-148-accepted-manifest.json`.

### Batch 281 — three exact ordinary-C threshold/sprite/task helpers (2026-08-23)
- Rewrote `func_08089648` with named argument/scene overlays and separate result/difference locals, `func_0800E800` with a named scene halfword overlay and typed sprite-coordinate normalization, and `func_08005834` with a named 0x1C-byte task record and ordinary scan locals. Thirteen compiler register pins were removed without instruction asm, barriers, or volatile codegen shims.
- The task-table sibling screen found an exact spelling for `func_08005834`; the selected `func_08089648` and `func_0800E800` bodies were instruction-identical, with only target function-symbol/literal-pool tails absent from the candidate-only isolated symbols. The strict accepted-source audit and integrated clean Docker ROM gate accepted all three.
- Clean Docker build, report regeneration, `gen_objdiff.py`, and `decomp_cycle.py verify --no-report` passed with `wariowareinc.gba: OK`; ROM/base ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Matching report metrics remain **1704 / 5934** functions and **76392 / 993840** code because all three were already C-linked. Pin residue drops **123 → 120 files / 584 → 571 pins**; full strict barrier residue remains **32 files / 36 findings**. Evidence: `.decomp-runs/round-147-isolation-v11.json`, `.decomp-runs/round-147-accepted-source-audit.json`, `.decomp-runs/round-147-full-source-audit.json`, `.decomp-runs/round-147-verify.json`, and `.decomp-runs/round-147-accepted-manifest.json`.

### Batch 280 — three exact ordinary-C graphics/table/scene helpers (2026-08-23)
- Rewrote `func_0801F1A0` with a named graphics overlay, `func_080F0E14` with ordinary table/base locals, and `func_0801C2D4` with a named current-scene overlay. Eleven compiler register pins and one empty compiler barrier were removed without instruction asm or volatile codegen shims.
- All three selected bodies were instruction-identical in isolation; the only differences were literal-pool width or target trailing metadata. The strict-clean `func_0800CA5C` overlay remained evidence-only because agbcc folded the required `MOV #0x21; NEG` into a real `SUB #0x31` mismatch.
- Clean Docker build, report regeneration, `gen_objdiff.py`, and `decomp_cycle.py verify --no-report` passed with `wariowareinc.gba: OK`; ROM/base ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Matching report metrics remain **1704 / 5934** functions and **76392 / 993840** code because all three were already C-linked. Pin residue drops **126 → 123 files / 595 → 584 pins**; full strict barrier residue drops **33 → 32 files / 37 → 36 findings**. Evidence: `.decomp-runs/round-146-isolation-v2.json`, `.decomp-runs/round-146-candidate-source-audit-all-v2.json`, `.decomp-runs/round-146-accepted-source-audit.json`, `.decomp-runs/round-146-full-source-audit.json`, `.decomp-runs/round-146-verify.json`, and `.decomp-runs/round-146-accepted-manifest.json`.

### Batch 279 — three exact ordinary-C scene/global helpers (2026-08-23)
- Rewrote `func_0806F0A0` and `func_0809E804` with named current-scene/data overlays, and `func_080F2894` with typed byte/halfword global arrays. Nine compiler register pins were removed without instruction asm, barriers, or volatile codegen shims.
- The three selected bodies were instruction-identical in isolation; the only standalone differences were target pool tails or literal-pool width. The `func_0800C9C0` candidate had a real register/order mismatch and remained evidence-only. The integrated clean Docker ROM gate and verifier accepted all three replacements.
- Clean Docker build, report regeneration, `gen_objdiff.py`, and `decomp_cycle.py verify --no-report` passed with `wariowareinc.gba: OK`; ROM/base ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Matching report metrics remain **1704 / 5934** functions and **76392 / 993840** code because all three were already C-linked. Pin residue drops **129 → 126 files / 604 → 595 pins**; full strict barrier residue remains **33 files / 37 findings**. Evidence: `.decomp-runs/round-145-isolation.json`, `.decomp-runs/round-145-accepted-source-audit.json`, `.decomp-runs/round-145-full-source-audit.json`, `.decomp-runs/round-145-verify.json`, and `.decomp-runs/round-145-accepted-manifest.json`.

## Automated matching loop

The runtime-neutral lifecycle is `tools/decomp_cycle.py`:

- `isolate` evaluates a manifest of C permutations/siblings in one Docker compiler invocation, links normalized comparison ELFs using the target's absolute symbol map, and writes a structured `.decomp-runs/` receipt.
- `tools/decomp_permute.py screen` fans a directory of m2c/asmlift/manual C spellings into one isolation pass, snapshots every input under `.decomp-runs/`, and records exact/near-miss hashes; `accept` can select only a recorded exact winner for the normal transactional apply.
- `apply` requires an isolated exact match, applies the mechanical conversion, runs the strict ROM/report gate, and restores the candidate transaction plus a clean baseline on failure. A fresh screen receipt is reused when its commit and input hashes still match, avoiding a redundant isolation container.
- `apply-batch` performs the same guarded transaction for a small exact manifest, with one isolation pass and one full-ROM gate for the batch.
- `verify` runs the current-worktree Docker gate for hooks or a final check.
- New candidates also pass `python3 tools/audit_decomp_source.py ... --strict`.
  The Git hooks and transactional cycle reject original-asm wrappers, inline
  instruction asm, empty asm barriers, compiler register pins, non-mapped
  `volatile`, and opaque offset-heavy byte-pointer stand-ins. Bounded raw
  layout evidence remains visible in the receipt, scalar-pointer aliases are
  counted across later lines, and a named overlay is recorded when the layout
  is modeled.
- Batch 229 confirms the strict audit on four newly accepted standalone files:
  zero instruction asm, barriers, and register pins in every candidate. The
  linker preflight also caught and then fixed the missing canonical
  `D_083FBB44` assignment; known symbols are mapped in `undefined_syms.ld`
  rather than replaced with magic numeric literals.
- Batch 230 applies the same audit to three callback/sound wrappers: every
  accepted file is ordinary C with zero instruction asm, barriers, or register
  pins. The callbacks use the existing three-argument ABI and named callback
  data symbols; no function-body asm or numeric ROM-address substitute was
  needed.
- Batch 231 accepted one ordinary-C scene initializer; its two packed scene
  field writes are reported as pointer evidence, with no asm/pins/barriers.
  Two structurally plausible candidates were retained as near-miss evidence
  rather than being forced into the ROM.
- Batch 232 accepted four ordinary-C scene/main-menu helpers. The exact
  screen was **4/4** after one stack-shape repair; the strict source audit
  found zero instruction asm, barriers, or register pins. Two candidates use
  explicit current-scene layout offsets, which remain visible audit evidence,
  not compiler-only shaping.
- Round 84 added the companion layout-quality gate: the typed `SceneState`
  overlay for `func_080165D4` and named graphics-register overlay for
  `func_08016BF0` pass with no opaque pointer-offset blob. New offset-heavy
  candidates are policy-rejected before byte comparison; the old raw-layout
  matches remain documented evidence until their structs are recovered.
- Batch 233 accepted those two exact overlay candidates. The full gate advanced
  **1674 → 1676** matched functions and **1214 → 1216** linked C units without
  changing the ROM SHA-1.
- Batch 234 accepted six exact standalone ordinary-C scene/graphics/wrapper
  candidates. The final Round 85 screen was **6 exact / 9 near miss**; the
  rejected DMA flag wrappers and MOVS+RSBS mask spelling remain evidence-only.
  The full gate advanced **1676 → 1682** matched functions and **1216 → 1222**
  linked C units, with zero instruction asm, barriers, register pins, or opaque
  layouts in the accepted sources and unchanged ROM SHA-1.
- Batch 235 accepted the exact `func_080178C4` title-scene wrapper under the
  hardened source-quality gate. The full transaction advanced **1682 → 1683**
  matched functions and **1222 → 1223** linked C units; the `08017930` frame
  near miss and `0801776C` table/register near miss remain evidence-only.
- Batch 236 accepted eight exact standalone ordinary-C leaves/wrappers after a
  **12-candidate screen (8 exact / 4 near miss)**. m2c supplied skeletons, and
  the accepted spellings were reshaped into named overlays and typed project
  interfaces; no wrapper asm, volatile shaping, register pin, barrier, or
  opaque offset blob was admitted. The full transaction advanced **1683 →
  1691** matched functions, **1223 → 1231** linked C units, and **1407 →
  1415** decomp files. The four near misses remain in `.nearmiss/` with their
  full candidate sources and scores.
- Batch 237 accepted `func_0800C4E0` as an included-stub ordinary-C wrapper.
  Its exact candidate was selected from a two-spelling screen, then refined to
  a semantically honest `void *` return of the task helper; the clean host-TU
  Docker gate remained byte-identical. Report metrics stayed at **1691 / 5934**
  and **75984 / 993820** because included-stub conversions do not add linked
  C units; decomp files advanced **1415 → 1416** (**203 → 204 included_stub**).
  `func_08011864` remains evidence-only after five real-C spellings reached a
  best **0.74074** isolated gap and hit the documented `CMP #1; BLO` trap.
- Batch 238 accepted `func_0800C218` as an included-stub ordinary-C bitmap
  helper. Two readable spellings were exact in host-TU isolation; the selected
  form keeps the helper's `void *` return, uses an all-`s16` typed function
  pointer plus named `u32` truncation locals for the original ABI shape, and
  contains no asm, register pin, barrier, volatile codegen trick, or opaque
  offset blob. The full gate remained byte-identical. Report metrics stay at
  **1691 / 5934** and **75984 / 993820** because this is an included stub;
  decomp files advance **1416 → 1417** (**204 → 205 included_stub**).
  `func_0800C3AC` and `func_0800DE84` remain evidence-only near misses; the
  latter's instruction-identical isolated result differed only in literal-pool
  placement and was not accepted on metadata alone.
- Batch 239 accepted `func_080147B0` as a strict real-C included-stub main-menu
  wrapper. The direct existing `gMainMenu` fields produced a **99.85%**
  isolated result whose only difference was candidate-origin relocation metadata;
  the first full-context trial correctly rolled back on a conflicting
  `func_08011698` prototype, then the corrected existing `u32` declaration
  passed the complete Docker gate with `wariowareinc.gba: OK` and exact ROM
  SHA-1. The source audit reports zero instruction asm, register pins,
  barriers, non-mapped volatile accesses, and raw offset tricks. Report metrics
  remain **1691 / 5934** and **75984 / 993820** because the function was already
  byte-matching inside its host TU; decomp files advance **1417 → 1418**
  (**205 → 206 included_stub**). The v1–v6 readable near misses and both
  rollback/accept receipts remain recorded under `.decomp-runs/` and
  `.nearmiss/`.
- Batch 240 accepted `func_0800C704`, `func_0800C720`, `func_080D6D28`, and
  `func_08064D10` as standalone ordinary C. Round 91's 13-entry screen found
  **4 exact / 9 near miss** results; m2c supplied the useful skeletons, while
  asmlift declined the `LDM` walkers or failed at project-context compilation.
  The walkers use a typed sentinel-array cursor (m2c's `s32 * += 4` was
  corrected to a C `cursor++` after checking the target `LDM R4!`), the D6D28
  leaf uses the target fall-through branch shape, and D64D10 uses a named
  record overlay. The accepted sources have zero instruction asm, barriers,
  register pins, non-mapped volatile accesses, and numeric pointer offsets;
  the two walkers expose only one typed pointer cast each. The full gate
  advanced **1691 → 1695** matched functions, **1231 → 1235** linked C TUs,
  and **1418 → 1422** decomp files (**1212 → 1216 standalone_tu**), with
  ROM SHA-1 unchanged. The `08016A60`/`08016A7C` bitfield spellings and
  `08003FB8` mask spelling remain near-miss evidence.
- Batch 241 accepted `func_080194D8`, `func_08022070`, and `func_080DF224` as
  standalone ordinary C. Round 92 screened **9** spellings and classified
  **3 exact / 6 near miss**. The scene-table helper uses a named 0x20-byte
  entry overlay and `entry++`, preserving the target stride without a raw
  byte-pointer increment; the other two are ordinary call/ABI wrappers. m2c
  supplied useful skeletons. asmlift remained diagnostic only: it failed
  project-context scoring for two wrappers, declined the stack-forwarder, and
  emitted non-admitted candidates for the table/fixed-point cases. The strict
  audit found no asm, pins, barriers, non-mapped volatile, or opaque offset
  blob in the accepted sources. The full gate advanced **1695 → 1698** matched
  functions, **1235 → 1238** linked C TUs, and **1422 → 1425** decomp files
  (**1216 → 1219 standalone_tu**), with unchanged ROM SHA-1.
- Batch 242 accepted `func_08089148`, `func_080B39F0`, `func_080CF440`,
  `func_080CF6C0`, and `func_0801CB24` as standalone ordinary C. Round 93's
  13-entry screen classified **5 exact / 8 near miss**; m2c supplied the
  skeletons and asmlift remained diagnostic-only because project-context
  scoring failed for this header-heavy group. The accepted sources use named
  0x40-byte entries, named packed records, explicit load-order locals, and a
  target-ABI helper declaration; they contain no asm, pins, barriers,
  non-mapped volatile, raw pointer accesses, or numeric offsets. The full gate
  advanced **1698 → 1703** matched functions, **1238 → 1243** linked C TUs,
  and **1425 → 1430** decomp files (**1219 → 1224 standalone_tu**), with
  unchanged ROM SHA-1. The epilogue-only, mask, allocation, scene-store, and
  narrow-ABI alternatives remain near-miss evidence.
- Batch 243 accepted `func_0800C73C` as standalone ordinary C. Round 94's
  13-entry screen classified **1 exact / 12 near miss**. The winner models
  the allocated five-halfword record with a named type, preserves the helper
  call/store order, and returns the typed record pointer. The flag-table
  siblings and sentinel-list spelling remain evidence-only because their
  ordinary-C codegen differs in mask/register or literal-pool shape; no asm,
  pins, barriers, volatile, or opaque offsets were used. The full gate advanced
  **1703 → 1704** matched functions, **1243 → 1244** linked C TUs, and
  **1430 → 1431** decomp files (**1224 → 1225 standalone_tu**), with unchanged
  ROM SHA-1.
- The Conker-style provenance follow-up tightened source admission without
  changing ROM output: the audit now records/rejects codegen-forcing volatile
  accesses except direct GBA I/O registers, and catches offset-heavy aliases
  such as `u8 *p = ...; p[0x10] = ...;`. The focused source-quality suite is
  **31 focused tests** (**33** across the tools suite); Round 86's exact candidate still passes the stricter audit.

Manifests for exported symbols such as `set_soundplayer_pitch` may provide an
explicit eight-digit `address`; this lets the cycle derive canonical paths even
when the symbol name is not `func_XXXXXXXX`.

Batch 221 also confirms the narrow boundary-audit path: a normalized linked
objdiff near miss may be admitted only when a complete linked `.text` section
comparison proves equal size and SHA-256, and the same transaction passes the
clean Docker ROM gate. This exception covers target symbol metadata only; it
never waives an instruction difference.

Install the local commit/push protections with `tools/install-hooks.sh`. Near-miss
records remain in `.nearmiss/` and `tools/attempts.tsv`; each best seed now keeps
a bounded attempt history and the cycle receipts carry candidate/target hashes.
They are evidence, not permission to retain a nonmatching source change.

## How autonomous continuation should work
- Prefer the repo-local fresh-context commands over pifinity/Ralph:
  - `/decomp-next` for one fresh-context chunk
  - `/decomp-loop start` for repeated fresh-context chunks across new sessions
- Use `decomp_siblings` tool to find similar functions after matching one:
  - `strategy: same_file` - Functions in same asm file (best for batch conversion)
  - `strategy: same_module` - Functions in same source module (e.g., graphics_table)
  - `strategy: callers` - Functions that call the matched one
  - `strategy: callees` - Functions called by the matched one
  - `strategy: pattern` - Functions with similar instruction patterns
- On a new machine, first run:
  - `/reload`
  - `/decomp-setup` (or `/decomp-health`)
  - `/decomp-verify`
- The decomp tools now autodetect a sibling Mizuchi checkout at `../mizuchi` or use `$MIZUCHI_ROOT` / `$PI_MIZUCHI_ROOT`; if you need an env var, persist it in `~/.zshrc`.
- The docs in this directory are the durable memory that should survive context compaction and session changes.
- Any new durable learning should be written back here before the agent yields.

## Post-commit handoff contract

After every accepted commit, the chat handoff must include a compact current-state
table with the same metrics used by the scale-up loop: matched functions and
percentage, matched code and percentage, linked C versus asm-only units, total
and standalone/included decomp files, ROM SHA-1, and the remaining gaps to the
active working goal (currently 27%) and the 30% milestone.
This keeps progress readable across sessions even when the detailed receipts are
collapsed. The active working goal is currently 27%; update the table and this
baseline together whenever the denominator or goal changes.

## Doc map
### Active operational docs
- `docs/wariowareinc-decomp-scaleup.md`
- `docs/decomp-agent-workflow.md`
- `docs/decomp-pattern-library.md`
- `docs/decomp-batch-history.md`
- `docs/decomp-tooling-feedback.md`
- `docs/windows-tooling-notes.md`

### Reference / historical docs
- `docs/wariowareinc-decomp-progress-audit.md` — early audit + smoke-test archive; not the live source of truth
- `docs/macabeus-tools-assessment.md` — tooling assessment notes
- `docs/mizuchi-workflow.md` — Mizuchi bootstrap notes

## Legacy note
continue maintaining the live workflow in `/docs` + `AGENTS.md`.
