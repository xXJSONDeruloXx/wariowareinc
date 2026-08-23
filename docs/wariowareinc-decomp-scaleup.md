# WarioWare Inc. Decomp Scale-Up

This is the live operational status file for autonomous work in this repo.
Prefer this file + the other docs in `/docs`

## Current verified baseline
- Verified on branch: `docs/macabeus-tooling-assessment`
- Verified working tree: `batch 285` — `func_080152A0` now emits the same bytes with ordinary scene-data pointer C and no compiler register pins
- `build/report.json`: **1704 / 5934 matched functions** = **28.715876%**
- `matched_code`: **76392 / 993840** = **7.686549%**
- `tools/gen_objdiff.py`: **1244 linked C TUs / 5443 asm-only units** (**6687 total**)
- `src/decomp/*.c`: **1445 decompiled function files** = **1225 standalone_tu** + **220 included_stub**
- ROM status: **`wariowareinc.gba: OK`**
- Remaining naked/original asm wrapper files in `src/decomp`: **0**
- Remaining compiler-register-pin files: **109 / 521 pins**; remaining non-volatile empty compiler-barrier files: **7 / 7 barriers**. The full strict audit reports **27 files / 31 empty barrier findings** when volatile barriers coexisting with legacy pins are included.
- Maintenance state: **32 legacy inline-asm shims removed** from included-stub files, plus two hundred fifty-one compiler register pins removed across batches 256–285; `src/decomp` contains no instruction-bearing inline asm. `func_080EE61C` is now real C: a target-specific `__builtin_swi_div` lowers through the patched agbcc Thumb backend to the BIOS `SVC #6` instruction.
- New-candidate admission is now strict real C, following Conker's `no-asm-pin` rule: wrappers, instruction asm, empty barriers, compiler register pins, non-mapped `volatile`, and opaque offset-heavy byte-pointer stand-ins are rejected by the source audit, cycle, and Git hooks. Bounded raw pointer casts/offsets are reported as evidence, scalar-pointer aliases are counted across later lines, and named overlays are preferred for multi-field records.
- Batch 229's four accepted standalone files each passed that strict audit with zero instruction asm, barriers, or register pins. The only raw-memory evidence is the known scene-data byte/halfword layout in `func_08016798`, `func_08016850`, and `func_08016DB8`; none contains an asm wrapper or compiler-only register trick.
- 25% milestone: **1484 / 5934**, now exceeded by **220** matched functions.
- 26% milestone: **1543 / 5934**; current progress is **1704**, exceeding it by **161** matches.
- Active 27% working goal: **1603 / 5934**; current progress exceeds it by **101** matches.
- Next 30% milestone: **1781 / 5934**; **77** additional matched functions are needed.

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
### Batch 278 — four exact ordinary-C table/scene helpers (2026-08-23)
- Rewrote `func_08003028` with a named fixed-size table cursor, and `func_080B27B8`, `func_080C6898`, and `func_080D28A4` with named current-scene overlays. Fourteen compiler register pins were removed without instruction asm, barriers, or volatile codegen shims.
- The table cursor was an exact isolated match. The three scene candidates were instruction-identical and differed only by the legacy target's trailing zero/pool symbol boundary; the integrated clean Docker ROM gate and verifier confirmed those metadata-only differences do not alter the linked ROM. The strict accepted-source audit passed. The `08002FC0` and `0800247C` typed table-copy candidates remained real near misses because agbcc reused the check value instead of reloading it in the body.
- Clean Docker build, report regeneration, `gen_objdiff.py`, and `decomp_cycle.py verify --no-report` passed with `wariowareinc.gba: OK`; ROM/base ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Matching report metrics remain **1704 / 5934** functions and **76392 / 993840** code because all four were already C-linked. Pin residue drops **133 → 129 files / 618 → 604 pins**; full strict barrier residue remains **33 files / 37 findings**. Evidence: `.decomp-runs/round-144-isolation.json`, `.decomp-runs/round-144-scene-isolation.json`, `.decomp-runs/round-144-accepted-source-audit.json`, `.decomp-runs/round-144-full-source-audit.json`, `.decomp-runs/round-144-verify.json`, and `.decomp-runs/round-144-accepted-manifest.json`.

### Batch 277 — three exact ordinary-C scene/table helpers (2026-08-23)
- Rewrote `func_080F0DFC` with ordinary mapped-table base/offset/mask locals, `func_08062488` with a named current-scene field overlay, and `func_080526D0` with named current-scene halfword/word fields. Nine compiler register pins were removed without instruction asm, barriers, or volatile codegen shims.
- The selected candidates were body-identical to their targets; standalone isolation reported only literal-pool width or function-symbol pool-boundary metadata. The integrated clean Docker ROM gate and verifier both passed, confirming those differences do not alter the linked ROM. Real register-home near misses in `080F1B5C`, `080EC308`, and `080DF440` remain evidence-only.
- Clean Docker build, report regeneration, `gen_objdiff.py`, and `decomp_cycle.py verify --no-report` passed with `wariowareinc.gba: OK`; ROM/base ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Matching report metrics remain **1704 / 5934** functions and **76392 / 993840** code because all three were already C-linked. Pin residue drops **136 → 133 files / 627 → 618 pins**; full strict barrier residue remains **33 files / 37 findings**. Evidence: `.decomp-runs/round-143-isolation.json`, `.decomp-runs/round-143-v2-isolation.json`, `.decomp-runs/round-143-accepted-source-audit.json`, `.decomp-runs/round-143-full-source-audit.json`, `.decomp-runs/round-143-verify.json`, and `.decomp-runs/round-143-accepted-manifest.json`.

### Batch 276 — three exact ordinary-C low-level helpers (2026-08-23)
- Rewrote `func_080029D0` with a typed byte/halfword union, `func_080039D0` with a named four-byte little-endian record, and `func_080F2C68` with ordinary scan-loop locals. Eight compiler register pins were removed without instruction asm, barriers, or volatile codegen shims.
- The strict screen classified the three accepted candidates as exact. The rejected scene/graphics probes were kept as evidence: `0801A688` needed a real callee-saved base, `080C69CC` had real register-home differences, and raw scalar-pointer layouts for simple helpers were correctly policy-rejected. The accepted-source audit passed and the clean Docker ROM gate preserved the baseline ROM.
- The clean `make clean && make -j4` Docker build reported `wariowareinc.gba: OK`; report and `gen_objdiff.py` were regenerated. `decomp_cycle.py verify --no-report` was also attempted, but its `rm -rf build; make -j4` path hit the known missing-output-directory race; `.decomp-runs/round-142-verify.json` records that tooling failure, while the explicit clean Docker build passed. ROM/base ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Matching report metrics remain **1704 / 5934** functions and **76392 / 993840** code because all three were already C-linked. Pin residue drops **139 → 136 files / 635 → 627 pins**; full strict barrier residue remains **33 files / 37 findings**. Evidence: `.decomp-runs/round-142-typed-isolation.json`, `.decomp-runs/round-142-screen-isolation.json`, `.decomp-runs/round-142-accepted-source-audit.json`, `.decomp-runs/round-142-full-source-audit.json`, `.decomp-runs/round-142-verify.json`, and `.decomp-runs/round-142-accepted-manifest.json`.

### Batch 275 — three exact ordinary-C graphics/scene helpers (2026-08-23)
- Rewrote `func_0801AE70`, `func_08039A44`, and `func_080C4A48` with named graphics/scene overlays and ordinary mask, zero, delta, and current-value locals. Nine compiler register pins and two empty compiler barriers were removed without instruction asm or volatile codegen shims.
- The four-entry screen selected three body-identical candidates whose only differences were literal-pool/object-boundary representation. `func_08003D28` remained evidence-only because its ordinary bit-mask locals swapped the target R1/R2 homes. The strict accepted-source audit passed and the standalone Docker gate preserved the ROM.
- The clean Docker ROM/report gate, local `gen_objdiff.py`, and `decomp_cycle.py verify --no-report` passed with `wariowareinc.gba: OK`; ROM/base ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Matching report metrics remain **1704 / 5934** functions and **76392 / 993840** code because all three were already C-linked. Pin residue drops **142 → 139 files / 644 → 635 pins**; full strict barrier residue drops **35 → 33 files / 39 → 37 findings**. Evidence: `.decomp-runs/round-141-isolation.json`, `.decomp-runs/round-141-candidate-source-audit.json`, `.decomp-runs/round-141-accepted-source-audit.json`, `.decomp-runs/round-141-full-source-audit.json`, `.decomp-runs/round-141-verify.json`, and `.decomp-runs/round-141-accepted-manifest.json`.

### Batch 274 — three exact ordinary-C table/scene/graphics helpers (2026-08-23)
- Rewrote `func_080127F8` with the labeled table-helper loop used by its exact siblings, `func_080126C8` with a named scene overlay and typed mode store, and `func_080186AC` with a named graphics-register overlay. Nine compiler register pins were removed without barriers, volatile accesses, or instruction asm.
- The five-entry screen selected one exact table helper plus two body-identical host/pool-boundary candidates. The raw offset-heavy `0804E290` and `08082934` spellings were policy-rejected; `080186AC` retained only a literal-pool representation difference, while `080126C8` had only included-host BL/pool metadata. The integrated host-TU/standalone Docker gate accepted the three source replacements.
- The clean Docker ROM/report gate, local `gen_objdiff.py`, and `decomp_cycle.py verify --no-report` passed with `wariowareinc.gba: OK`; ROM/base ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Matching report metrics remain **1704 / 5934** functions and **76392 / 993840** code because all three were already C-linked. Pin residue drops **145 → 142 files / 653 → 644 pins**; full strict barrier residue remains **35 files / 39 findings**. Evidence: `.decomp-runs/round-140-isolation.json`, `.decomp-runs/round-140-candidate-source-audit.json`, `.decomp-runs/round-140-accepted-source-audit.json`, `.decomp-runs/round-140-full-source-audit.json`, `.decomp-runs/round-140-verify.json`, and `.decomp-runs/round-140-accepted-manifest.json`.

### Batch 273 — three exact included-stub table helpers (2026-08-23)
- Rewrote `func_08012768`, `func_08012798`, and `func_080127C8` with ordinary `index`/`table`/`value` locals and bounded signed-byte table access. Nine compiler register pins were removed without barriers, volatile accesses, or instruction asm.
- The final sibling screen was **3 exact / 0 near miss**. A labeled ordinary-C loop with the body before the check preserved the target backward `BGE` and natural callee-saved value lifetime; a structured `if` or generic `while` produced real branch-layout mismatches and was retained as evidence only. The integrated host-TU gate accepted all three source-only included-stub replacements.
- The clean Docker ROM/report gate, local `gen_objdiff.py`, and `decomp_cycle.py verify --no-report` passed with `wariowareinc.gba: OK`; ROM/base ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Matching report metrics remain **1704 / 5934** functions and **76392 / 993840** code because all three were already C-linked. Pin residue drops **148 → 145 files / 662 → 653 pins**; full strict barrier residue remains **35 files / 39 findings**. Evidence: `.decomp-runs/round-138-final-isolation.json`, `.decomp-runs/round-138-final-candidate-source-audit.json`, `.decomp-runs/round-138-accepted-source-audit.json`, `.decomp-runs/round-138-full-source-audit.json`, `.decomp-runs/round-138-verify.json`, and `.decomp-runs/round-138-accepted-manifest.json`.

### Batch 272 — two exact included-stub main-menu/sprite wrappers (2026-08-23)
- Rewrote `func_080133EC` and `func_080136A4` with named scene overlays, typed `D_03006518` fields, ordinary mask locals, and the existing typed sprite interface. Eight compiler register pins were removed without barriers, volatile accesses, or instruction asm.
- The three-entry host-object screen selected both bodies: `080133EC` and `080136A4` matched through their epilogues, with only candidate BL relocations and pool/symbol-boundary metadata in the isolated near-miss scores. `func_0801197C` remained evidence-only because its ordinary-local candidate changed the prologue and mask register homes.
- The clean Docker ROM/report gate, local `gen_objdiff.py`, and `decomp_cycle.py verify --no-report` passed with `wariowareinc.gba: OK`; ROM/base ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Matching report metrics remain **1704 / 5934** functions and **76392 / 993840** code because both were already C-linked. Pin residue drops **150 → 148 files / 670 → 662 pins**; full strict barrier residue remains **35 files / 39 findings**. Evidence: `.decomp-runs/round-137-isolation.json`, `.decomp-runs/round-137-candidate-source-audit.json`, `.decomp-runs/round-137-accepted-source-audit.json`, `.decomp-runs/round-137-full-source-audit.json`, `.decomp-runs/round-137-verify.json`, and `.decomp-runs/round-137-accepted-manifest.json`.

### Batch 271 — three exact included-stub main-menu wrappers (2026-08-23)
- Rewrote `func_080119B8`, `func_08013428`, and `func_080143BC` with named scene overlays, typed `D_03006518.unk1` stores, ordinary mask locals, and an explicit scene-base lifetime. Eleven compiler register pins were removed without barriers, volatile accesses, or instruction asm.
- The five-entry host-object screen selected the two direct sibling spellings and `func_080143BC` v2. All three instruction bodies matched through their epilogues; candidate-only BL relocations and target pool/symbol-boundary tails explain the isolated near misses. The v1 `080143BC` spelling had a real byte-load home mismatch; passing the byte expression directly to `func_0801429C` fixed it. The strict accepted-source audit passed, and the integrated host-TU Docker gate was the acceptance authority.
- The clean Docker ROM/report gate, local `gen_objdiff.py`, and `decomp_cycle.py verify --no-report` passed with `wariowareinc.gba: OK`; ROM/base ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Matching report metrics remain **1704 / 5934** functions and **76392 / 993840** code because all three were already C-linked. Pin residue drops from **153 to 150 files / 681 to 670 pins**; full strict barrier residue remains **35 files / 39 findings**. Evidence: `.decomp-runs/round-136-isolation.json`, `.decomp-runs/round-136-candidate-source-audit.json`, `.decomp-runs/round-136-accepted-source-audit.json`, `.decomp-runs/round-136-full-source-audit.json`, `.decomp-runs/round-136-verify.json`, and `.decomp-runs/round-136-accepted-manifest.json`.

### Batch 270 — three exact included-stub main-menu wrappers (2026-08-23)
- Rewrote `func_080143F0`, `func_08014490`, and `func_080148BC` with named scene overlays, ordinary locals, and explicit callback address shaping. Twelve compiler register pins were removed without barriers, volatile accesses, or instruction asm.
- The five-entry host-object screen selected `080143F0` and `080148BC` v1 spellings plus `08014490` v3. All three instruction bodies matched through their epilogues; candidate-only BL relocations and target pool/symbol-boundary tails explain the isolated near misses. For `08014490`, v1/v2 reloaded the post-call byte pointer into R1; the v3 separate `data` local reproduced the target R0 reload. The strict accepted-source audit passed, and the integrated host-TU Docker gate was the acceptance authority.
- The clean Docker ROM/report gate, local `gen_objdiff.py`, and `decomp_cycle.py verify --no-report` passed with `wariowareinc.gba: OK`; ROM/base ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Matching report metrics remain **1704 / 5934** functions and **76392 / 993840** code because all three were already C-linked. Pin residue drops from **156 to 153 files / 693 to 681 pins**; full strict barrier residue remains **35 files / 39 findings**. Evidence: `.decomp-runs/round-135-isolation.json`, `.decomp-runs/round-135-candidate-source-audit.json`, `.decomp-runs/round-135-accepted-source-audit.json`, `.decomp-runs/round-135-full-source-audit.json`, `.decomp-runs/round-135-verify.json`, and `.decomp-runs/round-135-accepted-manifest.json`.

### Batch 269 — three exact included-stub main-menu wrappers (2026-08-23)
- Rewrote `func_08014878`, `func_08014C34`, and `func_08014C6C` with named scene-byte overlays, ordinary `value`/`mask` locals, and explicit callback address locals. Seventeen compiler register pins were removed without barriers, volatile accesses, or instruction asm.
- The five-entry host-object screen selected v1 spellings whose instruction bodies matched through their epilogues; the isolated near-miss scores were only external-call relocations plus target pool/symbol-boundary tails. The v2 named-large-callback-field spellings were retained as evidence because they changed real register operands. The strict accepted-source audit passed, and the integrated host-TU Docker gate was the acceptance authority.
- The clean Docker ROM/report gate, local `gen_objdiff.py`, and `decomp_cycle.py verify --no-report` passed with `wariowareinc.gba: OK`; ROM/base ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Matching report metrics remain **1704 / 5934** functions and **76392 / 993840** code because all three were already C-linked. Pin residue drops from **159 to 156 files / 710 to 693 pins**; full strict barrier residue remains **35 files / 39 findings**. Evidence: `.decomp-runs/round-134-isolation.json`, `.decomp-runs/round-134-candidate-source-audit.json`, `.decomp-runs/round-134-accepted-source-audit.json`, `.decomp-runs/round-134-full-source-audit.json`, `.decomp-runs/round-134-verify.json`, and `.decomp-runs/round-134-accepted-manifest.json`.

### Batch 268 — three exact included-stub mask wrappers (2026-08-23)
- Rewrote `func_080109CC`, `func_080144BC`, and `func_08014A0C` with small named scene overlays and ordinary `value`/`mask` locals around the existing scene/thread calls. Three compiler register pins were removed without barriers, volatile accesses, or instruction asm.
- The three-entry host-object screen produced instruction-identical ordinary-C bodies; isolated near-miss scores were only the known external-call and symbol/pool-boundary artifact. `func_080109CC` used an explicit function extern in its standalone candidate to avoid an unrelated broad-header warning. The strict accepted-source audit passed, and the integrated host-TU Docker gate was the acceptance authority.
- The clean Docker ROM/report gate, local `gen_objdiff.py`, and `decomp_cycle.py verify --no-report` passed with `wariowareinc.gba: OK`; ROM/base ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Matching report metrics remain **1704 / 5934** functions and **76392 / 993840** code because all three were already C-linked. Pin residue drops from **162 to 159 files / 713 to 710 pins**; full strict barrier residue remains **35 files / 39 findings**. Evidence: `.decomp-runs/round-133-isolation.json`, `.decomp-runs/round-133-candidate-source-audit.json`, `.decomp-runs/round-133-accepted-source-audit.json`, `.decomp-runs/round-133-full-source-audit.json`, `.decomp-runs/round-133-verify.json`, and `.decomp-runs/round-133-accepted-manifest.json`.

### Batch 267 — three exact included-stub mask setters (2026-08-23)
- Rewrote `func_0800A3BC`, `func_080121B8`, and `func_08013114` with small named scene-data overlays and ordinary `value`/`mask` locals. Three compiler register pins were removed without barriers, volatile accesses, or instruction asm.
- The six-entry host-object screen produced instruction-identical ordinary-C bodies for the selected three; isolated near-miss scores were only the known host symbol/pool-boundary artifact. The strict candidate audit passed, and the integrated host-TU Docker gate was the acceptance authority.
- The clean Docker ROM/report gate and `decomp_cycle.py verify --no-report` passed with `wariowareinc.gba: OK`; ROM/base ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Matching report metrics remain **1704 / 5934** functions and **76392 / 993840** code because all three were already C-linked. Pin residue drops from **165 to 162 files / 716 to 713 pins**; full strict barrier residue remains **35 files / 39 findings**. Evidence: `.decomp-runs/round-132-isolation.json`, `.decomp-runs/round-132-candidate-source-audit.json`, `.decomp-runs/round-132-accepted-source-audit.json`, `.decomp-runs/round-132-full-source-audit.json`, `.decomp-runs/round-132-verify.json`, and `.decomp-runs/round-132-accepted-manifest.json`.

### Batch 266 — three exact ordinary-C scene/audio/graphics wrappers (2026-08-23)
- Rewrote `func_0801D4B4` with a named scene/data overlay and ordinary shifted value/flag stores, `func_080A002C` with a widened value local normalized after the music-player load, and `func_080195E4` with named scene-variable/graphics overlays. Three compiler register pins and two empty barriers were removed without instruction asm or volatile codegen shims.
- The fifteen-entry screen found **7 exact / 4 near miss / 4 policy-rejected** candidates. The raw-pointer 195E4 forms were rejected by the layout gate; named overlays supplied the exact source shape. The selected three-entry screen passed the strict source audit with zero instruction asm, barriers, register pins, or non-mapped volatile accesses.
- The clean Docker ROM/report gate and `decomp_cycle.py verify --no-report` passed with `wariowareinc.gba: OK`; ROM/base ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Matching report metrics remain **1704 / 5934** functions and **76392 / 993840** code because all three were already C-linked. Pin residue drops from **168 to 165 files / 719 to 716 pins**; full strict barrier residue drops from **36 to 35 files / 41 to 39 findings**. Evidence: `.decomp-runs/round-131-isolation.json`, `.decomp-runs/round-131-candidate-source-audit.json`, `.decomp-runs/round-131-accepted-source-audit.json`, `.decomp-runs/round-131-full-source-audit.json`, `.decomp-runs/round-131-verify.json`, and `.decomp-runs/round-131-accepted-manifest.json`.

### Batch 265 — three exact included-stub scene/table wrappers (2026-08-23)
- Rewrote `func_08012DCC` with a named scene-ID overlay and ordinary indexed locals, `func_08014374` with named language/scene table overlays plus an ordinary offset local, and `func_080166AC` with a named scene-data overlay and ordinary predicate/result flow. Six compiler register pins were removed without barriers, volatile accesses, or instruction asm.
- The twelve-entry host-object screen retained the genuine `func_08002514` register-home near miss and policy-rejected raw layouts for early `func_08014374` spellings. The selected three forms were instruction-identical to their built host-object targets; candidate-only boundary metadata reported near misses, so the integrated host-TU Docker gate was the acceptance authority.
- The clean Docker build/report gate and `decomp_cycle.py verify --no-report` passed with `wariowareinc.gba: OK`; ROM/base ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Matching report metrics remain **1704 / 5934** functions and **76392 / 993840** code because all three were already C-linked. Pin residue drops from **171 to 168 files / 725 to 719 pins**; full strict barrier residue remains **36 files / 41 findings**. Evidence: `.decomp-runs/round-130-isolation.json`, `.decomp-runs/round-130-accepted-source-audit.json`, `.decomp-runs/round-130-full-source-audit.json`, `.decomp-runs/round-130-verify.json`, and `.decomp-runs/round-130-accepted-manifest.json`.

### Batch 264 — three exact included-stub scene/table wrappers (2026-08-23)
- Rewrote `func_080024E4` with a named 12-byte table-entry overlay and an ordinary cursor loop, and rewrote `func_08016E6C`/`func_08016D88` with ordinary scene predicates, saved results, and typed globals. The three existing included stubs lose six compiler register pins without introducing barriers, volatile accesses, or instruction asm.
- The eight-entry screen found **3 exact / 5 near miss** ordinary-C spellings; the selected three-entry subset passed the strict source audit with zero instruction asm, barriers, register pins, non-mapped volatile accesses, or opaque layouts. The original assembly for these already-converted host stubs was retained in `asm/converted/`, so this was a source-only host-TU cleanup.
- The clean Docker ROM/report gate and `decomp_cycle.py verify --no-report` passed with `wariowareinc.gba: OK`; ROM/base ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Matching report metrics remain **1704 / 5934** functions and **76392 / 993840** code because all three were already C-linked. Source residue drops from **174 to 171 pin-bearing files / 731 to 725 pins**; full strict barrier residue remains **36 files / 41 findings**. Evidence: `.decomp-runs/round-129-isolation.json`, `.decomp-runs/round-129-source-audit.json`, `.decomp-runs/round-129-accepted-source-audit.json`, `.decomp-runs/round-129-full-source-audit.json`, and `.decomp-runs/round-129-verify.json`.

### Batch 255 — three strict main-menu included stubs (2026-08-23)
- Converted `func_08011D0C`, `func_08012AE8`, and `func_08014CF8` from the main-menu host TU's asm includes to guarded ordinary C. The batch covers a position/sprite launcher using named table overlays, a digit-sprite division loop using a named scene overlay, and a scene cleanup/task-launch wrapper using named `+4`, `+0x16C`, and `+0xDE` fields.
- All three candidates passed the strict real-C and layout audits with zero instruction asm, barriers, register pins, or non-mapped volatile. The first apply rolled back on a shared-host declaration conflict (`D_083A4A1C` scalar versus the existing array declaration); matching the host declaration fixed it without changing generated bytes.
- The linked isolation screen reported only the known candidate-only BL displacement artifact: `0.28571`, `0.454544`, and `0.408165` gaps. Raw function text and literal-pool bytes matched the target host object, and the full Docker gate accepted the batch with `wariowareinc.gba: OK`, `rom_exact: true`, and ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Fresh report/objdiff metrics are **1704 / 5934** matched functions (**28.715876%**), **76392 / 993840** matched code (**7.686549%**), **1244 C / 5443 asm-only** units, and **1445** decomp files (**1225 standalone_tu + 220 included_stub**). Evidence: `.decomp-runs/20260823T151301Z-isolation.json`, `.decomp-runs/20260823T151711Z-apply_batch.json` (rollback), `.decomp-runs/20260823T151743Z-isolation.json`, and `.decomp-runs/20260823T151904Z-apply_batch.json`.

### Batch 256 — three no-pin random-song wrappers (2026-08-23)
- Rewrote `func_0801911C`, `func_0801913C`, and `func_0801915C` as ordinary C with normal local declarations. Each wrapper keeps its table base live across `get_random_range`/`play_sound` and still emits the target's callee-saved `R4` sequence; no compiler register pin, barrier, volatile access, or instruction asm remains in the three files.
- The seven-entry screen found five policy-clean exact candidates, one genuine constant-folding near miss, and one target-link error. The accepted three-sibling screen was **3 exact / 0 rejected**; the full Docker verification rebuilt the image with `wariowareinc.gba: OK` and equal ROM/base ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Linked matching metrics remain **1704 / 5934** functions and **76392 / 993840** code because these were already C-linked. The source residue drops from **197 to 194** pin-bearing files and from **772 to 769** compiler pins. Evidence: `.decomp-runs/round-108-screen.json`, `.decomp-runs/round-108-accepted-isolation.json`, and `.decomp-runs/round-108-verify.json`.

### Batch 257 — three additional no-pin wrappers (2026-08-23)
- Rewrote `func_08001B04`, `func_08019644`, and `func_080F1574` with ordinary local declarations. The batch removes the `R4` saved-argument pin from the included `code_08001a70` host TU, the `R2` value pin from the sprite-visibility wrapper, and both `R1`/`R0` pins from the indexed flag helper.
- The three-entry screen was **3 exact / 0 rejected** under the strict source audit. The clean Docker build/report gate and repo verify both emitted `wariowareinc.gba: OK` with equal ROM/base ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Linked matching metrics remain **1704 / 5934** functions and **76392 / 993840** code because these functions were already C-linked. The source residue drops from **194 to 191** pin-bearing files and from **769 to 765** compiler pins. Evidence: `.decomp-runs/round-109-isolation.json` and `.decomp-runs/round-109-verify.json`.

### Batch 258 — three no-pin/barrier-free wrappers (2026-08-23)
- Rewrote `func_0801B174` with a named scene-variable overlay and explicit flag local, removing three register pins plus the empty memory barrier; `func_080F282C` now uses an ordinary fixed-IWRAM halfword pointer and typed reload local, removing four pins plus its ordering barrier; `func_08004BD4` now uses a named four-word heap-record overlay and returns the allocated record pointer without its R0 pin.
- The three-entry isolation screen was **3 exact / 0 rejected** under the strict source audit. The clean Docker build, report refresh, and repo verify all emitted `wariowareinc.gba: OK`; ROM/base ROM SHA-1 stayed `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Linked matching metrics remain **1704 / 5934** functions and **76392 / 993840** code because these functions were already C-linked. Source residue drops from **191 to 188** pin-bearing files and from **765 to 757** pins; empty barriers drop from **13 to 11**. Evidence: `.decomp-runs/round-117-isolation.json` and `.decomp-runs/round-117-verify.json`.

### Batch 259 — two exact standalone no-pin wrappers (2026-08-23)
- Rewrote `func_080F28F8` with ordinary `result`/`value` locals that preserve its target stack epilogue and arithmetic register homes, removing its R0 pin. Rewrote the sibling `func_0800557C` with a named four-word heap-record overlay and an explicit allocated-pointer return, removing its R0 pin.
- The two-entry isolation screen was **2 exact / 0 rejected** under the strict source audit. The clean Docker build, report refresh, and repo verify all emitted `wariowareinc.gba: OK`; ROM/base ROM SHA-1 stayed `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Linked matching metrics remain **1704 / 5934** functions and **76392 / 993840** code because these functions were already C-linked. Source residue drops from **188 to 186** pin-bearing files and from **757 to 755** pins. Evidence: `.decomp-runs/round-121-isolation.json` and `.decomp-runs/round-121-verify.json`.

### Batch 260 — three exact mask/RMW no-pin wrappers (2026-08-23)
- Rewrote `func_0801AF18`, `func_0801B3E4`, and `func_0801BEA8` with named scene overlays plus ordinary `value`/`mask` locals. The forms preserve the target's byte read, `MOVS/RSBS/ANDS`, optional OR mask, and byte store register roles while removing six compiler register pins.
- The three-entry isolation screen was **3 exact / 0 rejected** under the strict source audit. The clean Docker build, report refresh, and repo verify all emitted `wariowareinc.gba: OK`; ROM/base ROM SHA-1 stayed `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Linked matching metrics remain **1704 / 5934** functions and **76392 / 993840** code because these functions were already C-linked. Source residue drops from **186 to 183** pin-bearing files and from **755 to 749** pins. Evidence: `.decomp-runs/round-123-isolation.json` and `.decomp-runs/round-123-verify.json`.

### Batch 261 — three exact ordinary-C no-pin wrappers (2026-08-23)
- Rewrote `func_08003998` with an ordinary cursor local and sequential post-increment byte stores, `func_0801E44C` with ordinary data-base and sprite-handler locals, and `func_080F1FB4` with ordinary arithmetic temporaries. The target instruction streams remain exact without six compiler register pins.
- A thirteen-entry cleanup screen found six exact candidates, one policy-clean near miss, and six strict-policy rejections. The selected three-entry exact-only screen passed the strict source audit with zero instruction asm, barriers, register pins, or non-mapped volatile accesses.
- The clean Docker ROM/report gate and `decomp_cycle.py verify` passed with `wariowareinc.gba: OK`; ROM/base ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Matching report metrics remain **1704 / 5934** functions and **76392 / 993840** code because all three were already C-linked. Source residue is now **180 pin-bearing files / 743 pins** and **11 empty-barrier files / 11 barriers**. Evidence: `.decomp-runs/round-124-isolation.json`, `.decomp-runs/round-124-accepted-isolation.json`, and `.decomp-runs/round-124-verify.json`.

### Batch 262 — three exact beatscript field wrappers (2026-08-23)
- Rewrote `func_0800CAA4`, `func_0800CAB8`, and `func_0800D224` with ordinary mapped-base, offset, and scaled-index locals. The target literal loads, additions, and halfword/word stores remain exact without six compiler register pins or five empty barriers.
- The eight-entry standalone screen was **8 exact / 0 rejected** under the strict source audit. The final three-entry exact-only screen was **3 exact / 0 rejected**; each accepted source contains only bounded layout evidence and no instruction asm, barrier, register pin, or non-mapped volatile access.
- The clean Docker ROM/report gate and `decomp_cycle.py verify` passed with `wariowareinc.gba: OK`; ROM/base ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Matching report metrics remain **1704 / 5934** functions and **76392 / 993840** code because all three were already C-linked. Source residue is now **177 pin-bearing files / 737 pins** and **11 non-volatile empty-barrier files / 11 barriers**; the full strict audit reports **38 files / 45 empty barrier findings**. Evidence: `.decomp-runs/round-126-isolation.json`, `.decomp-runs/round-126-accepted-isolation.json`, and `.decomp-runs/round-126-verify.json`.

### Batch 263 — three exact nested-scene/graphics/runtime wrappers (2026-08-23)
- Rewrote `func_0801D4A0` with named nested scene/data records, `func_0805CB5C` with a named graphics field and typed field pointer, and `func_080F0DE0` with ordinary runtime-base/offset locals. Six compiler register pins and four volatile empty barriers were removed while the target bytes stayed exact.
- The eight-entry candidate screen found exact ordinary-C spellings for the selected three functions; the final three-entry exact-only screen was **3 exact / 0 rejected** under the strict source audit. The accepted sources contain named layout evidence or bounded raw evidence only.
- The clean Docker ROM/report gate and `decomp_cycle.py verify` passed with `wariowareinc.gba: OK`; ROM/base ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Matching report metrics remain **1704 / 5934** functions and **76392 / 993840** code because all three were already C-linked. Source residue is now **174 pin-bearing files / 731 pins** and **11 non-volatile empty-barrier files / 11 barriers**; the full strict audit reports **36 files / 41 empty barrier findings**. Evidence: `.decomp-runs/round-127-isolation.json`, `.decomp-runs/round-128-isolation.json`, `.decomp-runs/round-128-accepted-isolation.json`, and `.decomp-runs/round-128-verify.json`.

### Tooling hardening follow-up — Conker provenance and source quality (2026-08-07)
- The Conker comparison confirmed that its durable strengths are hash-identified candidate receipts, retained near misses, and a full-ROM pre-commit gate; its legacy MIPS source model does not itself prohibit inline ASM or volatile codegen shaping. WarioWare keeps the provenance model and enforces the stricter source rule before isolation/apply.
- `tools/check_decomp_policy.py` now follows scalar-pointer aliases across later lines, so a cast on `u8 *p = ...` cannot hide a multi-offset blob. It also classifies ordinary `volatile` and allows only direct fixed GBA mapped-memory addresses.
- `tools/decomp_cycle.py` and `tools/audit_decomp_source.py` use that same semantic-quality result. A deliberately cheap candidate is rejected before compilation; the Round 86 `func_080178C4` candidate remains exact under the stricter check, while the `08017930` and `0801776C` spellings remain evidence-only near misses.
- This is ROM-neutral tooling work. The verified baseline remains **1682 / 5934**, **75634 / 993802** matched code, **1222 / 5465** linked units, and ROM SHA-1 **`3f556448d290fa5406d6ed367fee16cc02387ad3`**.

### Batch 252 — accepted (strict included-stub beatscript scene-thread setter)
- Converted `scene_set_current_thread` (`asm_0800a330`) from the beatscript host TU's asm include to guarded ordinary C: it stores the thread id into the `gBeatscriptScene.currentThread:3` bitfield (byte RMW at offset 1 with `-0xF` clear mask), calls `sprite_handler_set_mem_id(gSpriteHandler, get_current_mem_id())`, then repoints `gCurrentSceneVariable` at `&localVariables[threadId]` (stride `0xD68`) and `gCurrentSceneSpritePool` at `threads[threadId].sprites` (`&scene + 0x7E + id*0x9C`). Existing named struct/bitfield members in `include/types.h` reproduced the exact codegen on the first spelling.
- Exactness evidence: all 76 instruction bytes identical in isolation; pool layout identical with the four pointer literals at matching offsets — target bytes are baked absolute addresses while candidate words carry relocations that resolve to the same addresses via `undefined_syms.ld`. Full Docker gate: `wariowareinc.gba: OK`, `rom_exact: true`, ROM SHA-1 unchanged `3f556448d290fa5406d6ed367fee16cc02387ad3`. Linked report metrics stay **1704 / 5934** and **76374 / 993822**; decomp files advance **1439 → 1440** (**1225 standalone_tu / 215 included_stub**).
- Evidence: `.decomp-runs/round-103-manifest.json`, `.decomp-runs/20260821T230150Z-apply-scene_set_current_thread.json`.

### Batch 251 — accepted (strict included-stub beatscript task-spawn helper)
- Converted `func_0800A2D8` (`asm_0800a2d8`) from the beatscript host TU's asm include to guarded ordinary C: it builds a four-word stack task-argument block whose first word is written through two agbcc bitfield stores (`lo : 2` byte RMW at bits 0..1, `mid : 15` word RMW at bits 2..16 with `0x7FFF`/`0xFFFE0003` pool masks), stores three plain words, then returns `start_new_task((u16)get_current_mem_id(), D_083A4AF0, &taskArgs, NULL, 0)`.
- Key learnings: (1) the overlapping byte+word masked writes were bitfield-insert codegen — declaring `u32 lo : 2; u32 mid : 15; u32 hi : 15` reproduced the exact mask/shift sequence including operand register choices; (2) the target's `pop {r1}; bx r1` epilogue (vs the common `pop {r0}`) revealed the function returns the task handle, not void — returning `start_new_task(...)` directly fixed the last instruction pair.
- Exactness evidence: all 80 pre-pool bytes identical in isolation; the trailing `D_083A4AF0` pool word is relocation-vs-absolute and resolves identically at link. Full Docker gate: `wariowareinc.gba: OK`, `rom_exact: true`, ROM SHA-1 unchanged `3f556448d290fa5406d6ed367fee16cc02387ad3`. Linked report metrics stay **1704 / 5934** and **76372 / 993820** code; decomp files advance **1438 → 1439** (**1225 standalone_tu / 214 included_stub**).
- Evidence: `.decomp-runs/round-102-manifest.json`, `.decomp-runs/20260821T224915Z-apply-func_0800A2D8.json`.

### Batch 250 — accepted (strict included-stub main-menu task-spawn helper)
- Converted `func_08012D7C` (`asm_08012d7c`) from the main_menu host TU's asm include to guarded ordinary C: `scene_set_current_thread(0)`, a five-word stack block (`VRAMBase+0x8000` twice, `0x4000`, `0x1000`, `4`) passed by address to the unconverted `start_new_task((u16)get_current_mem_id(), (void *)D_083A4B28, &stack_args[0], NULL, 0)`, then `run_func_after_task(task, (TaskFinalFunc)(func_08012D3C + 1), 0)`. Plain hex constants reproduce agbcc's `movs/lsls` synthesis; reverse-order assignment of the first two words matches the target's `[SP+8]`-then-`[SP+4]` store order.
- First apply rolled back: the host TU already carries `extern u16 get_current_mem_id(void)` declarations (from included decomp files before and after this one), so a fresh `u32` declaration was a conflicting-types error. Re-declaring as `u16` (call site casts to u16 anyway, codegen identical) fixed the build.
- Exactness evidence: all 48 instruction bytes match byte-for-byte; the trailing `D_083A4B28` pool word differs only as relocation-vs-absolute in isolation and resolves identically at link. Full Docker gate: `wariowareinc.gba: OK`, `rom_exact: true`, ROM SHA-1 unchanged `3f556448d290fa5406d6ed367fee16cc02387ad3`. Linked report metrics stay **1704 / 5934** and **76372 / 993820** code; decomp files advance **1437 → 1438** (**1225 standalone_tu / 213 included_stub**).
- Evidence: `.decomp-runs/round-101-manifest.json`, `.decomp-runs/20260821T222831Z-apply-func_08012D7C.json` (rolled back), `.decomp-runs/20260821T223236Z-apply-func_08012D7C.json`.

### Batch 249 — accepted (strict included-stub main-menu scene-state helper)
- Converted `func_0801216C` (`asm_0801216c`) from the main_menu host TU's asm include to guarded ordinary C: a `~3` byte-mask clear at `(u8 *)gCurrentSceneData + 0xDD`, an eight-argument call to the unconverted engine helper `func_08005E48` (four register args plus four stack words: `gcs+0x7C`, `0xF`, `0`, `0xE`, `2`, `gMainMenu.unkD0`, `0xF`, `0`), and a tail call to `func_08012420(D_03006518.unk0)`. Typed `gMainMenu.unkD0` covers the 0xD0 load with no numeric evidence; only two bounded raw anchors (`0xDD`, `0x7C`) remain.
- The isolate receipt reported the known included-stub spurious near miss (empty candidate side from `.size` stripping), so acceptance used the batch 220/221 documented symbol-boundary path: the complete 76-byte candidate `.text` (code plus literal pool) matched the converted target byte-for-byte with SHA-256 `1ecc34c4d0ac88ab286ce589da47d4c03264dc4c79df4565a9db1eed21db60cc`.
- Full host-TU Docker gate: `wariowareinc.gba: OK`, `rom_exact: true`, ROM SHA-1 unchanged `3f556448d290fa5406d6ed367fee16cc02387ad3`. Linked report metrics stay **1704 / 5934** and **76372 / 993820** code; decomp files advance **1436 → 1437** (**1225 standalone_tu / 212 included_stub**).
- Evidence: `.decomp-runs/round-100-manifest.json`, `.decomp-runs/20260821T221735Z-apply-func_0801216C.json`.

### Batch 248 — accepted (strict included-stub main-menu scene-flag switch)
- Converted `func_08011864` (`asm_08011864`) from the main_menu host TU's asm include to guarded ordinary C: a three-case switch whose case bodies are single `*(u8 *)((u8 *)gCurrentSceneData + 0xDD) |= mask` expression statements plus a `func_080140C0()` delegate. The pure-expression spelling (no named pointer local) is what reproduces the target's register homes (pool address r0, pointer r1, byte r0, mask r2); named-local spellings consistently land the pointer in r2. Bounded layout evidence (2 raw lines / 2 numeric offsets), no asm, pins, barriers, or volatile.
- The two sibling decomp files (`asm_080113bc.c`, `asm_080118e0.c`) carried stale `extern void func_08011864(u8)` approximations that conflicted with the true 32-bit parameter (the target has no widening prologue and uses unsigned `blo` dispatch). Both prototypes were corrected to `(u32)`; call sites pass zero-extended bytes, so their codegen is unchanged (verified by the byte-identical ROM).
- Symbol-level comparison was exact (100%). Full host-TU Docker gate: `wariowareinc.gba: OK`, `rom_exact: true`, ROM SHA-1 unchanged `3f556448d290fa5406d6ed367fee16cc02387ad3`. Linked report metrics stay **1704 / 5934** and **1244 C / 5443 asm-only**; decomp files advance **1435 → 1436** (**1225 standalone_tu / 211 included_stub**).
- Evidence: `.decomp-runs/round-99-manifest.json`, `.decomp-runs/20260821T215721Z-apply-func_08011864.json`.
- Same chunk, maintenance pass: legacy `src/decomp/asm_080118e0.c` was rewritten pin-free (the batch 248 prototype correction touched this file, and the guard blocks editing files that still carry compiler register pins). The strict spelling — typed `D_03006518.unk2` reads, one bounded `(u8 *)gCurrentSceneData + 0xDD` anchor, `val`/`mask` locals for the `movs/neg/ands` sequence, and a direct `play_sound((struct SongHeader *)&D_083FBBF8)` call — is symbol-level exact (100%) against the converted asm target. Full Docker gate re-run: `wariowareinc.gba: OK`, ROM SHA-1 unchanged.

### Batch 247 — accepted (strict included-stub sprite handler constructor)
- Converted `sprite_handler_create` (`asm_080ee7b4`) from the lib_sprite host TU's asm include to guarded ordinary C. The OAM clear loop is modeled with a named 8-word `OamClearChunk` overlay (chained member assignment reproduces the target's eight descending word stores plus the 0x20-stride advance), the remainder-word tail is a plain u32 cursor walk, and the error-byte clear at handler+0x24 uses a two-member named overlay anchored at `handler->unk20` because that storage is a bitfield unit in the project record. No asm, pins, barriers, non-mapped volatile, or opaque offset blob.
- Two strict-audit rejections were resolved without weakening semantics: a `u8 pad[0x24]` gap array was classified as an opaque byte-pointer layout (re-anchoring at `unk20` removed it), and the chained `oam[0]..oam[7]` subscripts through a raw `u32 *` alias counted as nine numeric offsets (the typed chunk model replaced them). Lesson recorded in the pattern library.
- `sprite_clone` (`asm_080ef038`) was screened to **93.3%** and left as near-miss evidence: everything matches except agbcc LICM hoists the `mov/lsl #0x10000` materialization out of the copy loop while the original rematerializes it inside. Receipts in `.nearmiss/sprite_clone.json` + attempt ledger; do not force-apply.
- Symbol-level comparison was exact (100%). Full host-TU Docker gate: `wariowareinc.gba: OK`, `rom_exact: true`, ROM SHA-1 unchanged `3f556448d290fa5406d6ed367fee16cc02387ad3`. Linked report metrics stay **1704 / 5934** and **1244 C / 5443 asm-only**; decomp files advance **1434 → 1435** (**1225 standalone_tu / 210 included_stub**).
- Evidence: `.decomp-runs/round-98-manifest.json`, `.decomp-runs/20260821T213503Z-apply-sprite_handler_create.json`.

### Batch 246 — accepted (strict included-stub sprite XYZ setter)
- Converted `sprite_set_x_y_z` (`asm_080ef1ac`) from the lib_sprite host TU's asm include to guarded ordinary C. Widened `s32` parameters with `(u16)` narrowings declared/initialized in x, y, z order put x in r7, y in SB, z in R8 exactly as the target; the body reloads `handler->sprites[id]` per access and calls `sprite_remove_z_link`/`sprite_update_z_link` around the conditional `zDepth` store. No asm, pins, barriers, non-mapped volatile, or raw offsets.
- The first full-context apply rolled back cleanly: the candidate's `sprite_remove_z_link`/`sprite_update_z_link` externs used `(struct SpriteHandler *, s16)` while the already-accepted host-TU decomp files (`asm_080eecdc.c`, `asm_080ef2cc.c`) declare them as `(void *, s16)`, producing conflicting-types errors. Re-declaring with the existing host-TU prototypes passed. Lesson recorded in the pattern library.
- Symbol-level comparison was exact (100%). Full host-TU Docker gate: `wariowareinc.gba: OK`, `rom_exact: true`, ROM SHA-1 unchanged `3f556448d290fa5406d6ed367fee16cc02387ad3`. Linked report metrics stay **1704 / 5934** and **1244 C / 5443 asm-only**; decomp files advance **1433 → 1434** (**1225 standalone_tu / 209 included_stub**).
- Evidence: `.decomp-runs/round-97-manifest.json`, `.decomp-runs/20260821T205347Z-apply-sprite_set_x_y_z.json` (rollback), `.decomp-runs/20260821T205653Z-apply-sprite_set_x_y_z.json` (accepted).

### Batch 245 — accepted (strict included-stub sprite anim-progress selector)
- Converted `sprite_set_anim_progress` (`asm_080eeb50`) from the lib_sprite host TU's asm include to guarded ordinary C. The source uses the widened-parameter narrowing-order pattern from batch 244 — `(u8)progress` before the `D_03000E70` operation store, `(s16)id` after — then walks the typed `struct Animation` table accumulating `duration` bytes until the `(progress * sprite->totalDuration) >> 8` target and calls `sprite_set_anim_cel(handler, id, (s8)index)`. No asm, register pin, barrier, non-mapped volatile, or raw offset access.
- The cel-index increment required the documented agbcc u8-increment idiom: `index = ((index << 24) + 0x1000000) >> 24` on a `u32` reproduces the target's high-byte add exactly. Compound spellings (`index++`, `index += 1`, `(u8)(index + 1)`) all fuse into the cheaper add-first truncate form, and compound in-place shifts swap the temp-register roles.
- The first spelling was exact (100%) at the symbol level in isolated comparison; the cycle's raw-object fallback again reported only placement residuals (symbol out of section bounds, diff_count 0), the documented included-stub false near miss. The full host-TU Docker gate was the final authority: `wariowareinc.gba: OK`, `rom_exact: true`, ROM SHA-1 unchanged `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Linked report metrics remain **1704 / 5934** matched functions, **76372 / 993820** matched code, **1244 C / 5443 asm-only** units. Decompiled-file coverage advances **1432 → 1433** (**1225 standalone_tu / 208 included_stub**).
- Evidence: `.decomp-runs/round-96-manifest.json`, `.decomp-runs/20260821T204507Z-apply-sprite_set_anim_progress.json`.

### Batch 244 — accepted (strict included-stub sprite Z setter)
- Converted `sprite_set_z` (`asm_080ef2cc`) from the lib_sprite host TU's asm include to guarded ordinary C. The source declares widened `s32` parameters and performs the target's explicit narrowings in target order — `(u16)z` before the `D_03000E70` operation store, `(s16)id` after it — then uses typed `handler->sprites[id].zDepth` accesses around `sprite_remove_z_link`/`sprite_update_z_link`. No asm, register pin, barrier, non-mapped volatile, or raw offset access.
- The first spelling was exact at the symbol level in `compile_and_view_asm` (100%). The cycle's whole-host-object fallback comparison reported only placement-address residuals (branch/literal-pool targets at different link offsets plus the target's local literal-pool symbol boundary), the documented included-stub false near miss. Per the established rule, the full host-TU Docker gate was the final authority: `wariowareinc.gba: OK`, `rom_exact: true`, ROM SHA-1 unchanged `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Because this is an included-stub conversion, linked report metrics remain **1704 / 5934** matched functions, **76372 / 993820** matched code, and **1244 C / 5443 asm-only** units. Decompiled-file coverage advances **1431 → 1432**, from **1225 standalone_tu / 206 included_stub** to **1225 / 207**.
- Evidence: `.decomp-runs/round-95-manifest.json`, `.decomp-runs/20260821T201654Z-isolation.json`, `.decomp-runs/20260821T202217Z-apply-sprite_set_z.json`, and `.nearmiss/sprite_set_z.json` (the relocation-only fallback receipt).

### Batch 235 — accepted (strict title-scene wrapper)
- Converted `func_080178C4` to standalone ordinary C. It performs the title-scene setup calls and stores the loader result through a small named `SceneVariableRoot` overlay at offset zero; there is no asm, register pin, barrier, non-mapped volatile, or opaque offset blob.
- Round 86's hardened isolation screen remained **1 exact / 2 near miss** after the tooling commit. The exact candidate passed the strict source audit and the rollback-capable full Docker transaction; the report advanced **1682 → 1683**, linked C units **1222 → 1223**, and decomp files **1406 → 1407**.
- `func_08017930` remains evidence-only because ordinary C reserves `SP,#8` while the target reserves `SP,#0x54`; no dummy stack array was added to fake that frame. `func_0801776C` remains a real table-base/register-order near miss. Evidence: `.decomp-runs/round-86-manifest.json`, `round-86-isolation-v1.json` through `round-86-isolation-v7.json`, `round-86-source-audit.json`, `round-86-accepted-source-audit.json`, `round-86-apply.json`, `.nearmiss/func_08017930.*`, `.nearmiss/func_0801776C.*`, and `tools/attempts.tsv`.
- ROM SHA-1 remains **`3f556448d290fa5406d6ed367fee16cc02387ad3`** and the final accepted source passes the 31 focused / 33 total tooling tests.

### Batch 236 — accepted (strict named-overlay leaf/wrapper batch)
- Converted `func_0807DC6C`, `func_0808EF04`, `func_080D6C30`, `func_080526EC`, `func_08086970`, `func_08025174`, `func_080D70EC`, and `func_0808828C` to standalone ordinary C. The candidates use named input/output, scene, scene-data, table, and sprite-record overlays with explicit preserved gaps; they do not use a wrapped asm body, inline instruction asm, compiler register pin, empty barrier, non-mapped volatile, or opaque scalar-pointer offset blob.
- Round 87 screened **12** fresh candidates in one isolation pass: **8 exact / 4 near miss**. m2c supplied the initial semantic skeletons; manual source shaping selected the readable named-field forms, and no asmlift-generated spelling was admitted. The exact-only transaction passed the clean Docker gate with `wariowareinc.gba: OK`, advancing **1683 → 1691** matched functions, **1223 → 1231** linked C units, and **1407 → 1415** decomp files.
- The four rejected candidates remain evidence-only: `func_080D906C` (**39.9375**), `func_08040AAC` (**8.066666**), `func_080B2450` (**7.117645**), and `func_08082BB0` (**0.125**). Their full C seeds, normalized results, and attempt-ledger rows are retained under `.nearmiss/`, `.decomp-runs/round-87-isolation-v1.json`, and `tools/attempts.tsv`.
- `round-87-source-audit.json` reports zero instruction asm, barriers, register pins, volatile accesses, raw pointer accesses, and numeric pointer-offset lines in all eight accepted files; `func_080D70EC`'s single handler alias is an API parameter, not a layout blob. The ROM SHA-1 remains **`3f556448d290fa5406d6ed367fee16cc02387ad3`**.

### Batch 237 — accepted (strict included-stub bitmap wrapper)
- Converted `func_0800C4E0` from the bitmap-font host TU's asm include to guarded ordinary C. The source uses typed helper prototypes, an explicit `u16` temporary for the fifth argument's ABI truncation, and a real `void *` return of `func_0800C430`; it contains no wrapped asm, instruction asm, register pin, barrier, non-mapped volatile, or opaque pointer-offset layout.
- Round 88 exercised the included-stub host-object normalization path. The first spelling was a **91.7%** near miss; the second spelling was exact in the original host object, and the final return-type refinement was rechecked against that exact current object before the clean full-ROM gate. `wariowareinc.gba: OK` and the ROM SHA-1 remained **`3f556448d290fa5406d6ed367fee16cc02387ad3`**.
- Because this is an included-stub conversion, report metrics remain **1691 / 5934** matched functions, **75984 / 993820** matched code, and **1231 C / 5456 asm-only** linked units. Decompiled-file coverage advances **1415 → 1416**, from **1212 standalone_tu / 203 included_stub** to **1212 / 204**.
- `func_08011864` was deliberately not forced: five readable real-C variants were screened, with the best **0.74074** isolated gap. The remaining difference is the documented `CMP R0,#1; BLO` versus `CMP R0,#0; BEQ` compiler lowering, retained in `.nearmiss/func_08011864.*` and the Round 88 receipts.

### Batch 238 — accepted (strict included-stub bitmap helper)
- Converted `func_0800C218` from the bitmap-font host TU's asm include to guarded ordinary C. The selected source preserves the helper's real `void *` result and uses a unique all-`s16` function-pointer typedef plus named `u32` truncation locals to preserve the target argument shaping. It contains no wrapped or instruction asm, register pin, barrier, non-mapped volatile, or opaque pointer-offset stand-in.
- Round 89 screened eight c218 spellings and seven c3ac spellings under the strict source audit. c218 v7 and v8 were both exact in the host-TU comparison; v7 was selected because its return type is semantically honest. The clean full-context Docker gate reported `wariowareinc.gba: OK` with unchanged ROM SHA-1 **`3f556448d290fa5406d6ed367fee16cc02387ad3`**.
- This included-stub conversion changes source coverage but not linked report metrics: **1691 / 5934** matched functions, **75984 / 993820** matched code, and **1231 C / 5456 asm-only** linked units remain unchanged. Decompiled-file coverage advances **1416 → 1417**, from **1212 / 204** to **1212 / 205** standalone/included files.
- `func_0800C3AC` remains evidence-only after its best strict real-C spelling reached a **13.046875** isolated gap. `func_0800DE84` reached a **0.34884** gap with instruction-identical text but literal-pool metadata differences; it was not treated as exact or forced into the ROM.

### Batch 239 — accepted (strict included-stub main-menu wrapper)
- Converted `func_080147B0` from the main-menu host TU's asm include to guarded ordinary C. The accepted source uses the existing typed `gMainMenu` fields and typed helper calls; it contains no wrapped or instruction asm, compiler register pin, barrier, non-mapped volatile, or opaque offset-heavy pointer stand-in.
- Round 90 screened seven readable real-C spellings. The selected direct-field form reached a **99.85%** isolated result; its instruction sequence was identical and the residual was candidate-origin relocation metadata. Because that result was not an ordinary exact receipt, the full-context transaction was run under the guarded research path. Its first attempt rolled back cleanly when a locally declared `s32 func_08011698()` conflicted with the host TU's existing `u32` declaration. After correcting the declaration to the host ABI, the complete Docker gate accepted the conversion with `wariowareinc.gba: OK`, `rom_exact: true`, and ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- The strict source audit reports zero instruction asm, barriers, compiler register pins, non-mapped volatile accesses, raw pointer accesses, and numeric pointer-offset lines. Report metrics remain **1691 / 5934** matched functions and **75984 / 993820** matched code because this included stub was already byte-matching within its host object; decomp files advance **1417 → 1418**, from **1212 standalone_tu / 205 included_stub** to **1212 / 206**. The six rejected readable spellings remain near-miss evidence.

### Batch 240 — accepted (strict standalone wrapper/leaf batch)
- Converted `func_0800C704`, `func_0800C720`, `func_080D6D28`, and `func_08064D10` to standalone ordinary C. The two bitmap helpers walk typed sentinel-terminated `u32` tables, the D6D28 leaf preserves the signed threshold/fall-through layout, and D64D10 uses a small named record overlay for fields at `0x30` and `0x34`.
- Round 91 screened **13** spellings in two isolation passes: **4 exact / 9 near miss**. m2c supplied the semantic skeletons. asmlift declined the `LDM R4!` walkers as an unsupported effect and failed to score the project-context candidates for the remaining leaves; no asmlift-generated source was admitted. The m2c walker spelling was manually corrected from its erroneous `s32 * += 4` output to the semantically correct `cursor++` stride.
- The strict audit reports zero instruction asm, barriers, compiler register pins, non-mapped volatile accesses, and numeric pointer-offset lines in all four accepted files. The only raw-pointer evidence is the typed `const u32 *` cast used to traverse each sentinel table; D64D10 uses a named overlay rather than an offset blob. The full Docker gate passed `wariowareinc.gba: OK`, `rom_exact: true`, and ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Report metrics advance **1691 → 1695** matched functions, **75984 → 76092** matched code, and **1231 → 1235** linked C TUs. Decompiled-file coverage advances **1418 → 1422**, from **1212 standalone_tu / 206 included_stub** to **1216 / 206**. The `func_08016A60`/`08016A7C` bitfield variants and `func_08003FB8` mask variant remain near-miss evidence.

### Batch 241 — accepted (strict ordinary-C wrapper/table batch)
- Converted `func_080194D8`, `func_08022070`, and `func_080DF224` to standalone ordinary C. The first preserves the four-call/key-bit sequence, the table helper models its 0x20-byte records with a named padded entry and `entry++`, and the sound helper preserves the `u16` callback ABI and store order.
- Round 92 screened **9** spellings in one isolation pass: **3 exact / 6 near miss**. m2c supplied the useful skeletons. asmlift was diagnostic only: it failed project-context scoring for two wrappers, declined the stack-forwarder because it takes the address of a local frame, and produced non-admitted table/fixed-point spellings. No asmlift output entered the source tree.
- The accepted sources pass the strict audit with zero instruction asm, barriers, compiler register pins, non-mapped volatile accesses, and opaque offset-heavy pointer manipulation. The table's layout is named rather than represented by a raw byte-pointer increment. The clean full Docker gate passed `wariowareinc.gba: OK`, `rom_exact: true`, and ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Report metrics advance **1695 → 1698** matched functions, **76092 → 76192** matched code, and **1235 → 1238** linked C TUs. Decompiled-file coverage advances **1422 → 1425** (**1216 → 1219 standalone_tu**, included stubs remain **206**). The stack-forwarder and fixed-point candidates remain evidence-only near misses.

### Batch 242 — accepted (strict named-record leaf batch)
- Converted `func_08089148`, `func_080B39F0`, `func_080CF440`, `func_080CF6C0`, and `func_0801CB24` to standalone ordinary C. The sources use a named 0x40-byte entry stride, named records for packed fields, explicit load-order locals for the fixed-point siblings, and the target's widened `get_random_range` ABI declaration; none uses wrapped asm, instruction asm, barriers, register pins, non-mapped volatile, or raw/numeric pointer layout.
- Round 93 screened **13** spellings in two isolation passes: **5 exact / 8 near miss**. m2c supplied the useful skeletons. asmlift was diagnostic only: all project-context candidates failed its scoring/compile boundary, so no generated lift was admitted. The second isolation pass selected the explicit mask, load-order, and widened-return spellings where they were legitimate ordinary-C ABI/source-shape hypotheses.
- The strict accepted-source audit reports zero instruction asm, barriers, compiler register pins, non-mapped volatile accesses, raw pointer accesses, and numeric pointer offsets in all five files. The full Docker gate passed `wariowareinc.gba: OK`, `rom_exact: true`, and ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Report metrics advance **1698 → 1703** matched functions, **76192 → 76334** matched code, and **1238 → 1243** linked C TUs. Decompiled-file coverage advances **1425 → 1430** (**1219 → 1224 standalone_tu**, included stubs remain **206**). The allocation/scene-store, mask, and narrow-ABI alternatives remain evidence-only near misses.

### Batch 243 — accepted (strict allocator-record leaf)
- Converted `func_0800C73C` to standalone ordinary C. The source models the allocated five-halfword record with a named type, stores the two helper-derived/default fields in target order, and returns the typed record pointer without asm or compiler-only metadata.
- Round 94 screened **13** candidates in one isolation pass: **1 exact / 12 near miss**. The sentinel pointer-list candidate remains a literal-pool/constant-shape miss, while the 0x20-byte flag-table siblings remain register/mask-layout near misses despite named overlays. Their receipts and readable seeds are retained; no cheap source shaping was added.
- The accepted source passes the strict audit with zero instruction asm, barriers, compiler register pins, non-mapped volatile accesses, raw pointer accesses, and numeric pointer offsets. The clean full Docker gate passed `wariowareinc.gba: OK`, `rom_exact: true`, and ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Report metrics advance **1703 → 1704** matched functions, **76334 → 76372** matched code, and **1243 → 1244** linked C TUs. Decompiled-file coverage advances **1430 → 1431** (**1224 → 1225 standalone_tu**, included stubs remain **206**).

### Batch 234 — accepted (strict ordinary-C scene/graphics wrappers)
- Converted `func_08017054`, `func_0801709C`, `func_0801720C`, `func_0801743C`, `func_080179A8`, and `func_080179E4` to standalone ordinary C. The sources use the existing sprite/gameplay types plus two small named overlays for the previously unnamed graphics register at `0x48` and the scene-variable root at offset zero; no opaque offset blob was admitted.
- Round 85's final isolation screen classified **6 exact / 9 near miss**. m2c supplied the semantic skeletons. asmlift was run as a diagnostic path (declines/project-compile failures for several stack/ABI shapes), and none of its generated output was admitted. The DMA siblings `func_0801711C`, `func_08017164`, and `func_080171AC` remain evidence-only because agbcc emits a signed-branch/register-layout near miss; `func_080174A4` remains evidence-only for the known `MOVS`+`RSBS` mask-folding trap.
- The exact-only transaction passed the clean Docker ROM gate with `wariowareinc.gba: OK`; post-apply report/objdiff refreshed **1682 / 5934** matched functions, **75634 / 993802** matched code, and **1222 C / 5465 asm-only** units. ROM SHA-1 remains **`3f556448d290fa5406d6ed367fee16cc02387ad3`**.
- The strict source audit reports zero instruction asm, barriers, register pins, and opaque layouts in all six accepted files. `round-85-source-audit.json` records the named overlays and contains no ASM escape. Evidence: `.decomp-runs/round-85-manifest.json`, `round-85-isolation-v1.json` through `round-85-isolation-v14.json`, `round-85-exact-manifest.json`, `round-85-source-audit.json`, and `round-85-apply.json`; rejected spellings remain in `.nearmiss/` and `tools/attempts.tsv`.

### Batch 233 — accepted (named scene/graphics overlays)
- Converted `func_080165D4` and `func_08016BF0` to standalone ordinary C. The first is the title-scene state dispatcher with a named packed `SceneState` overlay; the second initializes the main-menu graphics register fields through a named `GraphicsMenuRegisters` overlay.
- Round 84's broad screen began with **2 exact / 3 near miss** after compile-context repairs. The exact-only manifest was re-screened after the new layout policy and reached **2 exact / 0 rejected**. `func_080165D4`'s initial exact raw-byte-pointer spelling was deliberately reshaped to the named overlay and remained exact; asmlift's BF0 generic pointer output was not admitted.
- The exact-only transaction passed the clean Docker ROM gate with `wariowareinc.gba: OK`; post-apply report/objdiff refreshed **1676 / 5934** matched functions, **75264 / 993780** matched code, and **1216 C / 5471 asm-only** units. ROM SHA-1 remains **`3f556448d290fa5406d6ed367fee16cc02387ad3`**.
- The strict source/layout audit reports zero instruction asm, barriers, register pins, raw pointer accesses, and numeric pointer offsets in both accepted files. Evidence: `.decomp-runs/round-84-manifest.json`, `round-84-isolation-v1.json` through `round-84-isolation-v5.json`, `round-84-source-audit.json`, and `round-84-apply.json`; rejected candidates remain in `.nearmiss/` and `tools/attempts.tsv`.

### Batch 232 — accepted (scene/main-menu helpers)
- Converted `func_080167D4`, `func_08016CBC`, `func_08016808`, and `func_08016C60` to standalone ordinary C. The group covers the sound-stop/scene-thread flag leaf, beatscript scene bootstrap, main-menu state dispatch, and graphics-buffer scene update.
- m2c supplied all four semantic skeletons. asmlift was run as a comparison path but remained at the project-header boundary; no generated lift was admitted. The first screen had two header-context compile errors and one stack-size near miss. Adding the missing `graphics.h` context and modeling the bootstrap's four-pointer local array produced **4 exact / 0 rejected** in Round 83 v3.
- The exact-only transaction passed the clean Docker ROM gate with `wariowareinc.gba: OK`; post-apply report/objdiff refreshed **1674 / 5934** matched functions, **75056 / 993772** matched code, and **1214 C / 5473 asm-only** units. ROM SHA-1 remains **`3f556448d290fa5406d6ed367fee16cc02387ad3`**.
- `tools/audit_decomp_source.py --strict` reports zero instruction asm, empty barriers, and register pins in all four accepted files. `func_080167D4` and `func_08016CBC` have no raw-pointer evidence; `func_08016808` records one current-scene `+0x3A` read, and `func_08016C60` records the five explicit packed scene-field reads. These are visible layout evidence, not inline asm or compiler register shaping. Evidence: `.decomp-runs/round-83-isolation-v1.json` through `round-83-isolation-v3.json`, `round-83-source-audit.json`, `round-83-apply.json`, and the retained `func_08016CBC` near miss.

### Batch 231 — accepted (scene initializer)
- Converted `func_08016F14` to standalone ordinary C. It allocates the scene sprite, stores the returned ID in the current scene record, installs the known `D_083AD81C` sprite table/callback pool, runs `func_08016EF8`, and clears the scene byte at offset `+4`.
- m2c supplied the direct initialization skeleton. asmlift was run as a comparison path but reached its project-header compile boundary; no generated lift was admitted. A three-entry screen classified **1 exact / 2 near miss**. `func_08016DE0` remains a real state-machine branch/layout near miss; `func_08016C24` remains a real scene-base register-order near miss. Neither was force-applied.
- The exact-only transaction passed the clean Docker ROM gate with `wariowareinc.gba: OK`; post-apply report/objdiff refreshed **1670 / 5934** matched functions, **74788 / 993762** matched code, and **1210 C / 5477 asm-only** units. ROM SHA-1 remains **`3f556448d290fa5406d6ed367fee16cc02387ad3`**.
- `tools/audit_decomp_source.py --strict` reports zero instruction asm, empty barriers, and register pins in the accepted file. Its only low-level evidence is the two explicit current-scene field stores. The canonical `D_083AD81C` map entry was added and verified in prerequisite commit `afd59602`. Evidence: `.decomp-runs/round-82-isolation-v1.json`, `round-82-isolation-v2.json`, `round-82-linker-verify.json`, and `round-82-apply.json`; near-miss seeds are in `.nearmiss/` and `tools/attempts.tsv`.

### Batch 230 — accepted (main-menu callback/sound wrappers)
- Converted `func_08016B4C`, `func_08016B88`, and `func_08016BC4` to standalone ordinary C. The sibling trio plays the callback sound, normalizes the sprite ID from the callback ABI, sets callback cels 7/0x11/-1, and installs the next callback/data pair for the first two wrappers.
- m2c recovered the essential three-argument callback shape. asmlift was run as a comparison path but reached its project-header compile boundary; no generated lift was admitted. The first C screen had one warning-only compile failure per callback-data wrapper, fixed by matching the project prototype's integer callback-data argument with `(u32)&D_083FF654/67C`. The final isolation classified **3 exact / 0 rejected**.
- The exact-only transaction passed the clean Docker ROM gate with `wariowareinc.gba: OK`; post-apply report/objdiff refreshed **1669 / 5934** matched functions, **74720 / 993762** matched code, and **1209 C / 5478 asm-only** units. ROM SHA-1 remains **`3f556448d290fa5406d6ed367fee16cc02387ad3`**.
- `tools/audit_decomp_source.py --strict` reports zero instruction asm, empty barriers, and register pins in all three accepted files. Callback data required two canonical linker assignments (`D_083FF654`, `D_083FF67C`) in `75c4148d`; that prerequisite commit and its Docker gate were completed before the exact batch transaction. Evidence: `.decomp-runs/round-81-isolation-v1.json` through `round-81-isolation-v3.json`, `round-81-linker-verify.json`, and `round-81-apply.json`.

### Batch 229 — accepted (main-menu update/input helpers)
- Converted `func_08016D00`, `func_08016DB8`, `func_08016798`, and `func_08016850` to standalone ordinary C. The group covers the main-menu update fan-in, soft-reset cleanup, input-triggered sound/state transition, and the paired scene/graphics readiness predicate.
- m2c supplied the semantic skeletons. asmlift was run as a comparison path but reached its project-header compile boundary for this Splat-shaped main-menu neighborhood; no asmlift output was admitted. A first isolation pass had three compile errors from missing header context and then three exact candidates; adding the canonical header order/prototypes recovered the build, and the only remaining miss was repaired with an ordinary C branch layout matching the target's `BNE` fall-through.
- The final v3/v4/v5 isolation receipts classified **4 exact / 0 rejected**. The transactional apply passed the clean Docker ROM gate with `wariowareinc.gba: OK`; the post-apply report and objdiff refresh report **1666 / 5934** matched functions, **74556 / 993756** matched code, and **1206 C / 5481 asm-only** units. ROM SHA-1 remains **`3f556448d290fa5406d6ed367fee16cc02387ad3`**.
- `tools/audit_decomp_source.py --strict` reports zero instruction asm, empty barriers, and register pins in all four accepted files. The linker preflight found `D_083FBB44` missing from `undefined_syms.ld`; commit `fc3419a6` added the canonical symbol assignment and passed its own byte-identical Docker gate before the batch transaction. Evidence: `.decomp-runs/round-80-isolation-v1.json` through `round-80-isolation-v5.json`, `round-80-linker-verify.json`, and `round-80-apply.json`; the branch is pushed at the follow-up batch commit.

### Batch 228 — accepted (scene animation helpers)
- Converted `func_0801646C` and `func_080164CC` to standalone C. The pair initializes/decrements the main-menu scene animation state, scans the scene's enabled-sprite bitmask, and updates sprite visibility/animation cels.
- m2c supplied the semantic loop and scene-field skeletons. asmlift hit project-context compile errors for both candidates. The initial byte-exact `func_0801646C` spelling used compiler-only register declarations, but the strict real-C follow-up removed all six pins and retained the exact ROM bytes; the final accepted source uses ordinary C only.
- Six isolated revisions preserved the convergence: v1 **0 exact / 2 near miss**, v2 **1 exact / 1 near miss**, v3 **1 exact / 1 compile error**, v4 **1 exact / 1 near miss**, v5 **1 exact / 1 near miss**, and v6 **2 exact / 0 rejected**. The exact-only apply passed the clean full Docker ROM/report gate with `wariowareinc.gba: OK`; ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Fresh report: **1662 / 5934 matched functions** (**28.008090%**), **74346 / 993742** matched code (**7.481419%**), **1202 C / 5485 asm-only** units, and **1386** decomp files (`1183 standalone_tu` + `203 included_stub`). Evidence: `.decomp-runs/round-79-isolation-v1.json` through `round-79-isolation-v6.json` and `.decomp-runs/round-79-apply.json`; near-miss seeds remain in `.nearmiss/`.

### Batch 227 — accepted (counted save-unlock family and aggregate)
- Converted `func_08016140`, `func_0801618C`, and `func_080163B8` to standalone ordinary C. The first two count bit 0/bit 1 microgame flags across the save buffer and unlock stages 0x12/0x13; the third ORs the complete stage-unlock result family into one aggregate flag word.
- m2c recovered the counted-loop and aggregate-call skeletons. asmlift explicitly declined the post-loop form and the aggregate's alignment halfword, so it served as a diagnostic boundary rather than a candidate generator; the final C was shaped from m2c plus the sibling patterns.
- One exact-only isolation screen classified **3 exact / 0 rejected**. The exact-only `apply-batch` transaction passed the clean full Docker ROM/report gate with `wariowareinc.gba: OK`; ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Fresh report: **1660 / 5934 matched functions** (**27.974384%**), **74166 / 993728** matched code (**7.463410%**), **1200 C / 5487 asm-only** units, and **1384** decomp files (`1181 standalone_tu` + `203 included_stub`). Evidence: `.decomp-runs/round-78-stage-isolation-v1.json` and `.decomp-runs/round-78-stage-apply.json`.

### Batch 226 — accepted (richer stage-unlock predicates)
- Converted `func_08015FBC`, `func_08015FF8`, `func_08016028`, `func_08016060`, `func_08016098`, `func_080161D8`, `func_08016220`, `func_08016268`, and `func_080162B0` to standalone ordinary C. These siblings extend the stage-unlock family with nested threshold checks and count-normalized achievement predicates.
- m2c and the globally installed asmlift supplied useful semantic skeletons and cross-checks. The final C preserved explicit nonzero-to-one count normalization, `count > 1` threshold branches, and the scalar-return epilogue without inline asm.
- One exact-only isolation screen classified **9 exact / 0 rejected**. The exact-only `apply-batch` transaction passed the clean full Docker ROM/report gate with `wariowareinc.gba: OK`; ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Fresh report: **1657 / 5934 matched functions** (**27.923828%**), **73836 / 993728** matched code (**7.430203%**), **1197 C / 5490 asm-only** units, and **1381** decomp files (`1178 standalone_tu` + `203 included_stub`). Evidence: `.decomp-runs/round-77-stage-isolation-v1.json` and `.decomp-runs/round-77-stage-apply.json`.

### Batch 225 — accepted (stage-unlock wrapper fan-in)
- Converted `func_08015E40`, `func_08015E68`, `func_08015E90`, `func_08015EB8`, `func_08015EE0`, `func_08015F08`, `func_08015F30`, `func_08015F58`, `func_080160C8`, `func_080160F0`, `func_08016118`, `func_080162F8`, `func_08016328`, `func_08016358`, and `func_08016388` to standalone ordinary C. The fifteen functions are sibling save-unlock predicates covering both achievement checks and gameplay-progress thresholds.
- m2c and the globally installed asmlift both recovered the same semantic skeletons. The exact ordinary-C spelling keeps the nested failure branches, the non-void `POP {R1}; BX R1` epilogue, and explicit `0x80 << N` return expressions where the target materializes shifted flags.
- One 15-entry isolation screen classified **15 exact / 0 rejected**. The exact-only `apply-batch` transaction passed the clean full Docker ROM/report gate with `wariowareinc.gba: OK`; ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`. The accepted sources contain no inline asm.
- Fresh report: **1648 / 5934** matched functions (**27.772161%**), **73284 / 993728** matched code (**7.374654%**), **1188 C / 5499 asm-only** units, and **1372** decomp files (`1169 standalone_tu` + `203 included_stub`). Evidence: `.decomp-runs/round-76-baseline-verify.json`, `.decomp-runs/round-76-stage-isolation-v1.json`, and `.decomp-runs/round-76-stage-apply.json`.

### Batch 224 — accepted (task/scene/sprite wrapper fan-in)
- Converted `func_08016E9C`, `func_08016EC8`, `func_080210D4`, `func_0803E9A0`, `func_0803E9C4`, `func_0803E9E8`, `func_0806A958`, `func_0806A97C`, `func_080EC55C`, `func_080EC62C`, `func_0803E244`, and `func_080EB1F4` to standalone ordinary C. The twelve functions cover task-loader callbacks, scene-table wrappers, update-call fan-in, scene-thread/sprite-visibility wrappers, and a conditional sound wrapper.
- Round 75's first isolation pass classified **9 exact / 2 near miss / 1 compile error**. Adding the ignored middle ABI parameter to preserve the third argument in R2 fixed both sprite wrappers; adding `scenes.h` supplied the missing `gCurrentSceneData` declaration. The repaired second pass classified **12 exact / 0 rejected**.
- The exact-only `apply-batch` transaction passed the clean full Docker ROM/report gate with `wariowareinc.gba: OK`; ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`. The new C contains no instruction-bearing or volatile inline asm.
- Fresh report: **1633 / 5934** matched functions (**27.519380%**), **72674 / 993728** matched code (**7.313269%**), **1173 C / 5514 asm-only** units, and **1357** decomp files (`1154 standalone_tu` + `203 included_stub`). Evidence: `.decomp-runs/20260806T233701Z-full_verify.json`, `.decomp-runs/round-75-isolation-v1.json`, `round-75-isolation-v2.json`, and `round-75-apply.json`; the first-pass near misses remain in `.nearmiss/`.

### Batch 223 — accepted (wrapper and scene-state fan-in)
- Converted `func_08022650`, `func_0803292C`, `func_08024F68`, `func_08072700`, `func_080733AC`, `func_08017238`, `func_08039A44`, `func_0801A688`, and `func_08016708` to standalone ordinary C. The batch covers a multi-call key wrapper, a four-call R4-preserving wrapper, scene-thread/data helpers, a sprite-visibility wrapper, a graphics-buffer mask/clear, a random sound-table lookup, and a graphics-task callback wrapper.
- Round 74's first screen classified **5 exact / 2 compile errors / 2 near misses**. Adding the project header declarations, pinning the graphics leaf's R1/R2/R0 roles, and keeping the random table base live across `get_random_range` produced a second **9 exact / 0 rejected** screen.
- The exact-only `apply-batch` transaction passed the full Docker rebuild/report gate with `wariowareinc.gba: OK`; ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`. The accepted C contains no instruction-bearing or volatile inline asm; the two empty/register compiler constraints are metadata-only.
- Fresh report: **1621 / 5934** matched functions (**27.317154%**), **72218 / 993722** matched code (**7.267425%**), **1161 C / 5526 asm-only** units, and **1345** decomp files (`1142 standalone_tu` + `203 included_stub`). Evidence: `.decomp-runs/round-74-isolation-v1.json`, `round-74-isolation-v2.json`, and `round-74-apply.json`; the first-pass near misses remain in `.nearmiss/`.

### Batch 222 — accepted (scene-data wrappers)
- Converted `func_08016B14` and `func_080241E8` to standalone ordinary C. Both load the current scene base, derive the target field pointer and signed halfword, and call the already-converted `func_08007000`; no instruction-bearing or volatile inline asm was added.
- Round 73 screened three short candidates in one Docker isolation invocation: **2 exact / 1 near miss**. The exact pair passed the full-context `apply-batch` transaction, including a clean Docker rebuild/report gate, with `wariowareinc.gba: OK` and unchanged ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- `func_080047D4` remains evidence-only: all seven instructions matched, but the numeric-address spelling omitted the target's absolute table-address literal-pool word. It was not force-applied; `.nearmiss/func_080047D4.json`, the attempts ledger, and Round 73 receipts preserve the repair seed.
- Fresh report: **1612 / 5934** matched functions (**27.165487%**), **71908 / 993710** matched code (**7.236317%**), **1152 C / 5535 asm-only** units, and **1336** decomp files (`1133 standalone_tu` + `203 included_stub`). Evidence: `.decomp-runs/round-73-isolation-v1.json` and `.decomp-runs/round-73-apply.json`.

### Batch 221 — accepted (gameplay wrappers / data lookup)
- Converted `func_08003DE0`, `func_080E1F48`, and `func_08023494` to standalone ordinary C. The first uses a numeric absolute IWRAM pointer to preserve the indirect `_call_via_r1` wrapper; the second preserves the target `R4` table base and widened `u32` callee result; the third uses the project audio prototype and two calls through the same ROM song address.
- Round 72 used five cheap isolated revisions. The final screen classified **2 exact / 1 linked-objdiff near miss**. `func_080E1F48` converged after a `u32` callee declaration, an `R4` pin, and an empty `"+r"` compiler dependency; `func_08023494` converged after using `src/audio.h`; `func_08003DE0` remained a symbol-boundary near miss.
- The complete linked `.text` audit for `func_08003DE0` proved **20/20** bytes and matching SHA-256 `d479c6a815c77122b2ce066889fd20bbcb86355f9174a64085fd1d1148d38f39`. The forced transaction waived only that target-symbol boundary and passed a clean Docker build/report gate with `wariowareinc.gba: OK`; ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- The accepted files contain ordinary C only plus one empty compiler metadata constraint in `func_080E1F48`; there is no volatile or instruction-bearing inline asm. Round 72 receipts are `.decomp-runs/round-72-isolation-v1.json` through `round-72-isolation-v5.json`, `round-72-boundary-audit.json`, and `round-72-apply.json`.

### Batch 220 — accepted (named sound-player wrappers)
- Converted `set_soundplayer_pitch` and `set_soundplayer_volume` to standalone ordinary C. Both preserve the widened argument normalization and call the existing helper with the target `u16`/`s16` value shape.
- Round 71's linked isolation correctly reported the known local-label symbol-boundary artifact (**33.333336** and **42.857143** gaps), so the complete candidate/target `.text` sections were separately assembled and audited. Both were **28/28** and **24/24** byte-identical with matching SHA-256 hashes; no instruction-bearing or volatile inline asm was used.
- The narrow documented metadata exception was applied with `--force` only after the raw audit. The transactional full Docker ROM/report gate passed with `wariowareinc.gba: OK`; ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Fresh report: **1607 / 5935** matched functions (**27.076664%**), **71772 / 993704** matched code (**7.222674%**), **1147 C / 5540 asm-only** units, and **1331** decomp files (`1128 standalone_tu` + `203 included_stub`). Evidence: `.decomp-runs/round-71-boundary-isolation-v4.json`, `round-71-boundary-audit.json`, and `round-71-boundary-apply.json`.

### Batch 219 — accepted (sprite positioning and heap-record cleanup)
- Converted `func_0800E800` and `func_08004EAC` to standalone ordinary C. The sprite wrapper pins the handler and scene-data globals in the target load order, uses the `0xB4 << 2` scene-data index, and performs signed halfword/argument normalization. The heap wrapper frees the two record-owned pointers at offsets `+8` and `+0xC` before freeing the record itself.
- Round 71's initial screen classified **0 exact / 4 near miss / 1 compile error**. After repairing the widened sound-player normalization, global-load order, and heap deallocator declaration, the final v2/v3 screen classified **2 exact / 3 near miss**. The three rejected candidates remain evidence-only in `.nearmiss/` and `tools/attempts.tsv`.
- The accepted sources contain ordinary C and register-bound compiler metadata only; no volatile or instruction-bearing inline asm was added. The named-symbol manifest path was enabled by the pushed ROM-neutral tooling commit `6ac13d21`.
- The exact-only transaction reused the fresh v3 isolation receipt and passed the clean Docker ROM/report gate with `wariowareinc.gba: OK`; ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Fresh report: **1605 / 5937** matched functions (**27.033857%**), **71720 / 993700** matched code (**7.2174697%**), **1145 C / 5542 asm-only** units, and **1329** decomp files (`1126 standalone_tu` + `203 included_stub`). Evidence: `.decomp-runs/round-71-isolation-v1.json`, `round-71-isolation-v2.json`, `round-71-isolation-v3.json`, and `round-71-apply.json`.

### Batch 218 — accepted (scene predicates, clamp/update helpers, and byte copy)
- Converted `func_0805C5D8`, `func_0806EC7C`, `func_08088B80`, `func_08089648`, and `func_0809C0C0` to standalone ordinary C. The five winners cover a byte-copy loop, two scene predicates, a record clamp/changed flag, and a scene-time threshold predicate.
- Round 70 used asmlift/m2c output to select five compact candidates. The first isolation pass found **2 exact / 3 near miss**; source-level branch polarity for the two boolean leaves and register-bound load sequencing for `08089648` produced **5 exact / 0 rejected** on v2.
- The accepted sources contain ordinary C and register-bound compiler metadata only; there is no volatile or instruction-bearing inline asm. The three repaired first-pass candidates remain recorded in `.nearmiss/` with both isolation receipts.
- The exact-only transaction passed the explicit clean Docker rebuild with `wariowareinc.gba: OK`; report, objdiff, policy, and SHA-1 checks passed. ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Fresh report: **1603 / 5937** matched functions (**27.000168%**), **71640 / 993690** matched code (**7.2094917%**), **1143 C / 5544 asm-only** units, and **1327** decomp files (`1124 standalone_tu` + `203 included_stub`). Evidence: `.decomp-runs/round-70-isolation-v1.json`, `round-70-isolation-v2.json`, and `round-70-apply-v1.json`.

### Batch 217 — accepted (scene-state, predicate, and update siblings)
- Converted `func_080A7A74`, `func_0809E804`, `func_080526D0`, `func_08072DD4`, `func_0805F08C`, `func_080855D8`, `func_0808EA3C`, `func_080D25C4`, and `func_0806F0A0` to standalone ordinary C. The batch covers scene predicates, a scaled scene-data store, a conditional record update, an indexed byte fallback, a conditional halfword store, and two accumulator/clamp helpers.
- Round 69 screened ten candidates through eight isolated revisions. The first pass found **3 exact / 6 near miss / 1 exact candidate later repaired**; branch polarity, operand ordering, and register-bound locals repaired nine candidates to exact. `func_080DD8A4` remains withheld because agbcc keeps the indexed halfword offset in R0 rather than the target R1 despite several metadata-only C permutations.
- The accepted sources contain ordinary C and register-bound compiler metadata only; no instruction-bearing or volatile inline asm was accepted. The exact subset used one guarded full-context transaction, while the DD8A4 attempts remain near-miss provenance.
- The transaction passed the clean Docker ROM/report gate with `wariowareinc.gba: OK`; post-apply report, objdiff, policy, and SHA-1 checks passed. ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Fresh report: **1598 / 5937** matched functions (**26.915949%**), **71486 / 993690** matched code (**7.1939936%**), **1138 C / 5549 asm-only** units, and **1322** decomp files (`1119 standalone_tu` + `203 included_stub`). Evidence: `.decomp-runs/round-69-isolation-v1.json` through `round-69-isolation-v8.json`, plus `.decomp-runs/round-69-apply-v1.json`.

### Batch 216 — accepted (scene-data wrappers, predicates, and copy loops)
- Converted `func_080B2724`, `func_080B274C`, `func_080C68F8`, `func_080D74D0`, `func_080D750C`, `func_080D6FF4`, `func_080C69CC`, `func_080721A0`, and `func_080721BC` to standalone ordinary C. The batch covers three scene-data sound wrappers, three boolean scene predicates, a two-field state clear, and byte/halfword copy loops.
- Round 68 screened all nine candidates together. The first pass found **6 exact / 1 near miss / 2 compile errors**; reversing the `080D74D0` condition to match the target's fall-through branch and adding `global.h` before `types.h` for the copy loops produced **9 exact / 0 rejected**.
- The accepted sources contain ordinary C and register-bound compiler metadata only; there is no instruction-bearing or volatile inline asm. The loop pair uses straightforward typed pointer/count C, while `080C69CC` uses register declarations only to preserve the target's reload and zero-materialization order.
- The exact-only transaction passed the clean Docker ROM/report gate with `wariowareinc.gba: OK`; post-apply report, objdiff, policy, and SHA-1 checks passed. ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Fresh report: **1589 / 5937** matched functions (**26.76436%**), **71212 / 993686** matched code (**7.166449%**), **1129 C / 5558 asm-only** units, and **1313** decomp files (`1110 standalone_tu` + `203 included_stub`). Evidence: `.decomp-runs/round-68-isolation-v1.json`, `round-68-isolation-v2.json`, and `round-68-apply-v1.json`; the `080D74D0` first-pass source is preserved in `.nearmiss/`.

### Batch 215 — accepted (beatscript, scene-data, save, and key wrappers)
- Converted `func_0800CDB0`, `func_080166E4`, `func_08075E34`, `func_080B2704`, `func_08015E24`, `func_0801E4EC`, and `func_08020FB0` to standalone ordinary C. The batch covers a beatscript bit-field setter, a texture-loader/task-finalizer wrapper, two ROM-sound scene-data wrappers, a save-unlock decision, and two multi-call key-test wrappers.
- Round 67 screened seven candidates through four isolated C-shape revisions: the initial pass found **3 exact / 3 near miss / 1 compile error**, and the final v4 pass found **7 exact / 0 rejected**. The repairs were a compiler-only mask dependency, branch fall-through reshaping, the `scenes.h` global declaration, and an explicit `+1` Thumb callback pointer.
- The accepted `func_0800CDB0` source uses one empty nonvolatile `"+r"` compiler constraint to preserve the target's separate `MOVS #3; RSBS` mask sequence; it emits no instruction text and contains no volatile or instruction-bearing asm.
- The exact-only transaction passed the clean Docker ROM/report gate with `wariowareinc.gba: OK`; `make report`, `gen_objdiff.py`, and policy checks passed. ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Fresh report: **1580 / 5937** matched functions (**26.612768%**), **70918 / 993672** matched code (**7.1369624%**), **1120 C / 5567 asm-only** units, and **1304** decomp files (`1101 standalone_tu` + `203 included_stub`). Evidence: `.decomp-runs/round-67-isolation-v1.json`, `round-67-isolation-v2.json`, `round-67-isolation-v3.json`, `round-67-isolation-v4.json`, and `.decomp-runs/round-67-apply-v1.json`.

### Batch 214 — accepted (scene-variable leaves and indexed state wrappers)
- Converted `func_080CAAEC`, `func_080B29C8`, `func_0806F0D4`, `func_080AA3DC`, `func_080D2450`, `func_080C691C`, `func_080A99D0`, and `func_08046518` to standalone ordinary C. The batch covers a hardware-register/random-number wrapper, two sound/state wrappers, byte/bit scene leaves, an indexed scene-state store, a ROM-table call wrapper, and a signed scene-byte forwarding wrapper.
- Round 66 screened nine candidates in one isolation container: **8 exact / 1 near miss**. `func_080D3A60` remains evidence-only because the natural post-call scene-halfword spelling did not reproduce the target instruction sequence; it was not admitted to the ROM transaction.
- Numeric absolute ROM addresses were used for the two song-table constants and the hardware register where that spelling was already proven to preserve the target bytes. No linker-map assignment was needed.
- The exact-only transaction passed the clean Docker ROM/report gate with `wariowareinc.gba: OK`; `make report`, `gen_objdiff.py`, and the policy scan passed. ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Fresh report: **1573 / 5937** matched functions (**26.494864%**), **70690 / 993666** matched code (**7.1140604%**), **1113 C / 5574 asm-only** units, and **1297** decomp files (`1094 standalone_tu` + `203 included_stub`). Evidence: `.decomp-runs/round-66-isolation-v1.json`, `.decomp-runs/round-66-apply-v1.json`, and `.nearmiss/func_080D3A60.json`.

### Batch 213 — accepted (gameplay and call-wrapper siblings)
- Converted `func_0800E834`, `func_080253BC`, `func_08025514`, `func_08025530`, and `func_080DF458` to standalone ordinary C. The batch covers a two-call initialization wrapper, two gameplay-state setup/teardown pairs, and a scene-variable sound-loader wrapper.
- Round 65 screened eight candidates: **5 exact / 2 near miss / 1 compile error**. Numeric ROM-address shaping from Batch 212 avoided new linker assignments for the ROM constants; the rejected `func_080E1F48` and `func_08003DE0` hypotheses remain near-miss evidence.
- The exact-only transaction passed the clean Docker ROM/report gate with `wariowareinc.gba: OK`, `make report`, `gen_objdiff.py`, and policy checks. ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Fresh report: **1565 / 5937** matched functions (**26.360115%**), **70450 / 993662** matched code (**7.089936%**), **1105 C / 5582 asm-only** units, and **1289** decomp files (`1086 standalone_tu` + `203 included_stub`). Evidence: `.decomp-runs/round-65-isolation-v1.json` and `.decomp-runs/round-65-apply-v1.json`.

### Batch 212 — accepted (numeric-address ROM leaves)
- Converted `func_080B0760` and `func_080ED380` to standalone ordinary C. The first writes the current scene's halfword then plays the ROM song at `0x083FC170`; the second copies the ROM word at `0x08124E38` into two scene fields.
- Their first ABI-shaped candidates were exact in Round 64 but referenced `D_083FC170`/`D_08124E38`, which are not yet assigned in `undefined_syms.ld`. Numeric-address spellings isolated exact and avoided a dirty linker-map transaction; the full ROM gate, report, and objdiff refresh accepted both.
- Accepted sources contain no instruction-bearing or volatile inline asm. ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Fresh report: **1560 / 5937** matched functions (**26.275898%**), **70298 / 993662** matched code (**7.074639%**), **1100 C / 5587 asm-only** units, and **1284** decomp files (`1081 standalone_tu` + `203 included_stub`). Evidence: `.decomp-runs/round-64b-isolation-v1.json` and `.decomp-runs/round-64b-apply-v1.json`.

### Batch 211 — accepted (scene-slot setters and sprite visibility wrappers)
- Converted seven standalone functions to ordinary C: `func_08022EC8`, `func_08062410`, `func_080205B8`, `func_080A8418`, `func_08022010`, `func_08022030`, and `func_08022050`. The batch covers the post-call key test, a scene-variable setter, identical `sprite_id_set_visible` wrappers, and three `0x20`-stride scene-slot setters.
- Round 64 used m2c skeletons, a raw-`glabel` inventory, and asmlift scoring. The three word-slot setters and both sprite wrappers were exact on the first C spelling; the key/scene leaves were exact as well. `func_08022070` and `func_0804F464` remain near-miss evidence, while two exact ROM-symbol candidates were deferred until their missing linker-map assignments can be handled transactionally.
- The exact-only batch reused the fresh isolation receipt and passed one rollback-capable full Docker ROM/report gate with `wariowareinc.gba: OK`. `make report`, `python3 tools/gen_objdiff.py`, and policy checks passed. ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Fresh report: **1558 / 5937** matched functions (**26.242208%**), **70246 / 993662** matched code (**7.069406%**), **1098 C / 5589 asm-only** units, and **1282** decomp files (`1079 standalone_tu` + `203 included_stub`). Evidence: `.decomp-runs/round-64-isolation-v1.json` and `.decomp-runs/round-64-apply-v1.json`.

### Batch 210 — accepted (scene predicates, call wrappers, and bitmap wrappers)
- Converted six standalone functions to ordinary C: `func_0802DA38`, `func_08033D10`, `func_08037AAC`, `func_0803CD90`, `func_080550C4`, and `func_08085E2C`. The first two are three-call wrappers; the remaining four are the repeated `gCurrentSceneData + 0x173` predicate family.
- Converted included bitmap stubs `func_0800C2E4` and `func_0800C5A0` to ordinary C. A widened `u32` return preserves the target interwork epilogue, and local ABI-shaped function-pointer typedefs avoid conflicting host-TU prototypes without instruction-bearing asm.
- Round 63 used m2c/asmlift semantic skeletons plus a repo-local `glabel` inventory. Six standalone candidates were exact; four additional standalone candidates remain near-miss evidence and one compile-error candidate was withheld. The two included conversions improve real-C coverage but do not change the matched-function denominator because they were already part of linked C host TUs.
- The clean Docker ROM/report gate passed with `wariowareinc.gba: OK`, `make report`, `gen_objdiff.py`, and the no-instruction-asm policy check. ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Fresh report: **1551 / 5937** matched functions (**26.124304%**), **70026 / 993648** matched code (**7.047365%**), **1091 C / 5596 asm-only** units, and **1275** decomp files (`1072 standalone_tu` + `203 included_stub`). Evidence: `.decomp-runs/round-63-standalone-isolation-v1.json`, `.decomp-runs/round-63-standalone-apply-v1.json`, `.decomp-runs/round-63-bitmap-font-c2e4-isolation-u32.json`, `.decomp-runs/round-63-bitmap-font-c2e4-apply-u32.json`, `.decomp-runs/round-63-bitmap-font-c5a0-isolation-u32.json`, and `.decomp-runs/round-63-bitmap-font-c5a0-apply-u32.json`.

### Batch 209 — accepted (soundplayer wrappers and bootstrap record)
- Converted `func_08002038`, `func_0800207C`, `func_080020E0`, and `func_08006148` to standalone ordinary C. m2c supplied the semantic skeletons; asmlift was useful as a diagnostic comparison but declined or failed on the project-specific wrapper contexts.
- Normalized linked-ELF isolation reported symbol-boundary near misses because the legacy target symbols stop at internal return/literal-pool labels. A separate linked `.text` byte audit proved equal **20**, **20**, **28**, and **44**-byte sections; the equal SHA-256 pairs are recorded in `.decomp-runs/20260805T-round-62-linked-bytecheck.json`. No instruction mismatch was waived.
- The v2 C spellings were selected for `0800207C` and `080020E0`; alternate spellings had equal `.text` bytes, but the E0 variant also emitted an unnecessary `.rodata` section. One fresh forced four-entry transaction passed the clean Docker ROM/report gate with `wariowareinc.gba: OK`, preserving ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Accepted sources contain no instruction-bearing or volatile inline asm. `func_08006148` uses register-bound declarations only as compiler allocation metadata. Fresh report: **1545 / 5937**, **26.023245%**, **69846 / 993640** matched code (**7.029306%**), **1085 C / 5602 asm-only** units, and **1267** decomp files (`1066 standalone_tu` + `201 included_stub`). Receipts: `.decomp-runs/20260805T-round-62-{isolation,v2-isolation,linked-bytecheck,apply}.json`.

### Batch 208 — accepted (wrapper and heap-record allocator siblings)
- Converted `func_0800200C`, `func_080041B4`, and `func_08005F64` to standalone ordinary C. The first is a nested zero/nonzero dispatcher, the second is a `D_03000684` flag wrapper, and the third allocates/initializes a small heap record through two calls to `func_08006184`.
- Round 61 used m2c and asmlift for semantic skeletons, then project-aware C shaping. `0800200C` and `080041B4` were normalized-isolation near misses only because their legacy target literal-pool/symbol metadata differs; linked `.text` audits proved exact **24-byte** sections for both. `08005F64` was exact in isolation and its linked `.text` audit proved an exact **60-byte** section. Equal SHA-256 pairs are preserved in `.decomp-runs/20260805T-round-61-linked-bytecheck.json`.
- Added the canonical `D_03000684 = 0x03000684` linker assignment. A fresh forced three-entry transaction waived only the documented symbol-boundary classification; the clean Docker ROM/report gate passed with `wariowareinc.gba: OK`, preserving ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Accepted sources contain no instruction-bearing or volatile inline asm. `func_08005F64` uses one empty nonvolatile `"+r"` compiler constraint to preserve a register dependency; it emits no instruction text. Fresh report: **1541 / 5941**, **7.0184655%** matched code, **1081 C / 5606 asm-only** units, and **1263** decomp files (`1062 standalone_tu` + `201 included_stub`). Receipts: `.decomp-runs/20260805T-round-61-isolation.json`, `.decomp-runs/20260805T-round-61-linked-bytecheck.json`, and `.decomp-runs/20260805T-round-61-apply.json`.

### Batch 207 — accepted (task-pool scan and mutation siblings)
- Converted `func_08005834`, `func_08005870`, `func_080058AC`, `func_080058DC`, `func_080059A8`, and `func_08005A54` to standalone ordinary C. The six functions reuse the `D_030006A0` task pool and its `0x1C`-byte slot stride: owner scans, active-owner cancellation, state-mask setting, field update, and ID/state mutation.
- Round 59 used m2c for the semantic skeletons. asmlift was not needed for this already-proven overlapping-field family; its earlier Round 58 diagnostics had already shown the struct-model limitation. Register-bound locals and goto-shaped loops supplied the exact compiler shape without instruction-bearing asm.
- The normalized linked-ELF screen classified all six as symbol-boundary near misses (`0%` reported match) because the legacy target symbols stop before their literal-pool coverage. A separate linked `.text` byte audit, using the same zero-filled `.align 2, 0` normalization as `decomp_cycle.py`, proved target/candidate equality for **60**, **60**, **48**, **56**, **60**, and **60** bytes. Hashes are preserved in `.decomp-runs/20260805T-round-59-linked-bytecheck.json`; no instruction mismatch was waived.
- Because `--force` cannot reuse a non-exact isolation receipt, `apply-batch` reran a fresh isolation pass and then applied all six in one rollback-capable transaction. The clean Docker ROM/report gate passed with `wariowareinc.gba: OK`; ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Accepted sources contain ordinary C and register-bound compiler metadata only—no instruction-bearing or volatile inline asm. Fresh report: **1538 / 5943**, **7.0077977%** matched code, **1078 C / 5609 asm-only** units, and **1260** decomp files (`1059 standalone_tu` + `201 included_stub`). Receipts: `.decomp-runs/20260805T-round-59-isolation.json`, `.decomp-runs/20260805T-round-59-linked-bytecheck.json`, and `.decomp-runs/20260805T-round-59-apply.json`.

### Batch 206 — accepted (task-pool state scan and cancel siblings)
- Converted `func_08005920` and `func_080059E4` to standalone ordinary C. Both walk the `D_030006A0` task slots at `0x1C`-byte stride; the first scans active slots by owner/state and the second writes the task ID and conditionally calls `task_stop`.
- Round 58 used m2c for the task-slot semantic skeleton. asmlift declined the first overlapping-field reconstruction and could not score the second normalized candidate; manual C shaping then recovered the target branch layout, callee-saved `R7`, and exact loop register roles without instruction-bearing asm.
- Normalized linked-ELF isolation reported a symbol-boundary near miss for both legacy literal-pool targets. A linked `.text` byte audit proved all **76** bytes of `func_08005920` and all **56** bytes of `func_080059E4` identical before the documented metadata-only `--force` transaction. The clean Docker ROM/report gate passed with `wariowareinc.gba: OK`; ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Added the canonical `D_030006A0 = 0x030006A0` linker assignment. Accepted sources use ordinary C plus register-bound declarations only—no instruction-bearing or volatile inline asm. Fresh report: **1532 / 5949**, **6.973608%** matched code, **1072 C / 5615 asm-only** units, and **1254** decomp files (`1053 standalone_tu` + `201 included_stub`). Receipts: `.decomp-runs/20260805T-round-58-final-isolation.json`, `.decomp-runs/20260805T-round-58-bytecheck.json`, and `.decomp-runs/20260805T-round-58-apply.json`.

### Batch 205 — accepted (scene-table byte setter sibling)
- Converted `func_08030F7C` to standalone ordinary C. It reuses the exact `gCurrentSceneVariable` base/table-pointer locals and staged `arg1 * 0xE + arg0` arithmetic proven by `func_08030F9C`, then stores the byte argument through the same indexed table.
- Round 57 screened this one standalone sibling in isolation and scored it exact. Because it has no calls or new absolute symbols, it was applied as a one-function transaction; the clean Docker ROM/report gate passed with `wariowareinc.gba: OK`, ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`, and no instruction-bearing or volatile inline asm.
- Fresh report: **1530 / 5951**, **6.9603233%** matched code, **1070 C / 5617 asm-only** units, and **1252** decomp files (`1051 standalone_tu` + `201 included_stub`). Receipts: `.decomp-runs/20260805T-round-57-isolation.json` and `.decomp-runs/20260805T-round-57-apply.json`.

### Batch 204 — accepted (global-context setter and scene-table byte lookup)
- Converted `func_08024E34` and `func_08030F9C` to standalone ordinary C. The first uses an explicitly typed absolute `D_083C8B64` pointer so agbcc retains the target's `R4` global-pointer load and four sequential word stores. The second loads `gCurrentSceneVariable` first, then its table pointer, and spells the `arg1 * 0xE + arg0` index so the target's `LSLS/SUBS/LSLS/ADDS` accumulator remains in R2 before the final byte load.
- Round 56 screened four standalone candidates in one Docker isolation pass: these two were exact, while `func_080020FC` remained a branch-layout near miss and `func_08035FEC` remained a register/zero-materialization near miss. Neither near miss entered the ROM transaction.
- Added the canonical `D_083C8B64 = 0x083C8B64` assignment to `undefined_syms.ld`; `include/undefined_syms.inc` already carried the address. The exact-only two-entry transaction passed the clean Docker ROM/report gate with `wariowareinc.gba: OK`, ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`, and no instruction-bearing or volatile inline asm in the accepted sources.
- Fresh report: **1529 / 5951**, **6.957103%** matched code, **1069 C / 5618 asm-only** units, and **1251** decomp files (`1050 standalone_tu` + `201 included_stub`). Receipts: `.decomp-runs/20260805T-round-56-isolation.json` and `.decomp-runs/20260805T-round-56-apply.json`.

### Batch 203 — accepted (table-copy and bounded-wrapper helpers)
- Converted `func_08002FC0`, `func_08002FE8`, `func_08003028`, `func_08003040`, and `func_08003058` to standalone ordinary C. The four table-copy/scanning helpers use register-pinned pointers and explicit goto-shaped loops; `func_08003058` preserves the incoming R1–R3 values through an old-style C call to the already-converted initializer.
- Round 55's final six-entry screen left all six candidates outside the normal exact-only path because the legacy target symbols terminate at internal labels. A direct `arm-none-eabi-objcopy -O binary -j .text` comparison then proved the five accepted candidates byte-identical, including their aligned text tails. `func_08007AD4` was withheld: its candidate differed in two real prologue bytes (`R4`/`R5` copy order), so it remains near-miss evidence rather than a force candidate.
- The five-entry transaction used the documented narrow `--force` metadata exception only after the direct byte receipt; no instruction mismatch was waived. The clean Docker ROM/report gate passed with `wariowareinc.gba: OK`, ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`, and no instruction-bearing or volatile inline asm in the accepted sources.
- Fresh report: **1527 / 5951**, **6.9522724%** matched code, **1067 C / 5620 asm-only** units, and **1249** decomp files (`1048 standalone_tu` + `201 included_stub`). Receipts: `.decomp-runs/20260805T-round-55-isolation-v3.json`, `.decomp-runs/20260805T-round-55-apply.json`, and `.decomp-runs/20260805T-round-55-bytecheck.json`.

### Batch 202 — accepted (graphics, scene-state, and DMA-table helpers)
- Converted `func_080186AC`, `func_080195B8`, `func_080F154C`, `func_08002620`, and `func_0800774C` to standalone ordinary C. The graphics clear preserves the target's delayed first store; the scene helper preserves the mixed scene/graphics store order; the runtime initializer is direct C; and the DMA-table siblings preserve the post-write volatile read and register order.
- Round 54's final five-entry screen produced **3 strict exact / 2 symbol-boundary near miss** results. The two DMA-table functions had matching instruction bodies, but normalized linked-ELF symbol inference included the compiler's literal pool in the candidate symbol. Because the complete code/pool bytes were accounted for, the documented narrow `--force` metadata exception was used; no instruction mismatch was waived.
- The five-entry transaction passed one clean full Docker ROM/report gate with `wariowareinc.gba: OK`; canonical `D_030001C0`, `D_03000C18`, `D_03003FE4`, `D_03003FF0`, `D_03006580`, `D_03006588`, `D_030068A4`, and `D_03007204` linker assignments were added. No instruction-bearing or volatile inline asm was introduced: the sources use ordinary C, register-bound locals, and volatile memory pointers only. ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Fresh report: **1522 / 5956**, **6.9363704%** matched code, **1062 C / 5625 asm-only** units, and **1244** decomp files (`1043 standalone_tu` + `201 included_stub`). Receipts: `.decomp-runs/20260805T-round-54-isolation.json`, `.decomp-runs/20260805T-round-54-isolation-v2.json`, and `.decomp-runs/20260805T-round-54-apply.json`.

### Batch 201 — accepted (beatscript and runtime-table helpers)
- Converted `func_0800C9C0`, `func_0800CA5C`, and `func_080F2894` to standalone ordinary C. The beatscript setters preserve the target's two distinct base-plus-literal destinations; `func_0800CA5C` uses an empty register-output constraint to prevent agbcc from folding the required `MOVS #0x21; RSBS` mask into `SUB`; and `func_080F2894` uses canonical absolute runtime symbols plus explicit `r0 + r1` operand order.
- Round 53's final five-entry isolation screen produced **3 exact / 2 near miss** results in one Docker invocation. `func_08004770` remains an ABI/prologue near miss (`PUSH {LR}` / `POP {R1}; BX R1`), and `func_08006148` remains a literal-pool symbol-boundary near miss even though its instruction body and pool bytes are accounted for. Neither entered the ROM transaction.
- The exact-only three-entry transaction passed one clean full Docker ROM/report gate and moved the original assembly to `asm/converted/`; canonical `D_03000E80`, `D_03000E88`, and `D_03000E90` linker assignments were added. Accepted sources contain no instruction-bearing or volatile inline asm; the only asm syntax is empty compiler metadata. ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Fresh report: **1517 / 5958**, **6.9134383%** matched code, **1057 C / 5630 asm-only** units, and **1239** decomp files (`1038 standalone_tu` + `201 included_stub`). Receipts: `.decomp-runs/20260805T-round-53-isolation.json`, `.decomp-runs/20260805T-round-53-isolation-v2.json`, `.decomp-runs/20260805T-round-53-isolation-v3.json`, and `.decomp-runs/20260805T-round-53-apply.json`.

### Batch 200 — accepted (scene-state zero/setter siblings)
- Converted `func_080B27B8`, `func_080C6898`, `func_080D2768`, `func_080D286C`, and `func_080D28A4` to standalone ordinary C. Each uses explicit register-bound scene-variable base/offset temporaries to preserve the target's non-sequential reloads, scaled offsets, store widths, and literal offset forms.
- One five-entry m2c/manual isolation screen found all five exact. The exact-only transaction moved the assembly sources to `asm/converted/`, updated the linker, and passed one clean full Docker ROM gate. No inline asm was needed. ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Fresh report: **1514 / 5958**, **6.9005837%** matched code, **1054 C / 5633 asm-only** units, and **1236** decomp files (`1035 standalone_tu` + `201 included_stub`). Receipts: `.decomp-runs/20260805T-round-52-isolation.json` and `.decomp-runs/20260805T-round-52-apply.json`.

### Batch 199 — accepted (scene, graphics, and mask helpers)
- Converted `func_0801B174`, `func_0801C2D4`, `func_0801F698`, and `func_08062488` to standalone ordinary C. The scene setters preserve raw byte ABI width and reload order; the graphics helper uses an empty output constraint to keep the target's `R3` constant and `R3 → R0` copy; and the scene mask uses the established pinned R0/R1/R2 form.
- Round 50's `func_080F253C`/`080F2598`/`080F25B8` screen remains blocked: all three still fold the target's `MOVS; RSBS` mask into `SUB`, even after separating the first mask expression. Round 51's `func_080047D4` remains evidence-only because its candidate changed the literal-pool symbol boundary/alignment.
- The exact four-entry transaction reused the Round 51 isolation receipt and passed the clean full Docker ROM gate. No instruction-bearing asm was added; `func_0801B174` uses an empty memory barrier and `func_0801F698` uses an empty register output constraint for compiler shaping. ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Fresh report: **1509 / 5958**, **6.8833017%** matched code, **1049 C / 5638 asm-only** units, and **1231** decomp files (`1030 standalone_tu` + `201 included_stub`). Receipts: `.decomp-runs/20260805T-round-51-v3-isolation.json` and `.decomp-runs/20260805T-round-51-apply.json`.

### Batch 198 — accepted (scene predicate, string length, and PRNG leaf)
- Converted `func_08016F60`, `func_080F2C68`, and `func_080F282C` to standalone ordinary C. The scene predicate uses an explicit `if (value != 0) return 1; return 0;` to preserve the target `BNE` direction; the string-length helper returns `u32` so the target's final `MOV R0,R1` is not widened into an extra byte-normalization pair; and the PRNG helper uses the target's unsigned input normalization and absolute `0x03000E78` address.
- The Round 49 screen used m2c/asmlift skeletons plus two focused C-shape revisions. The initial predicate and `u8` length return were recorded as near misses; the final three-entry exact receipt passed the transactional full Docker ROM gate. No instruction-bearing asm was added; `func_080F282C` uses only an empty memory barrier for ordering. ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Fresh report: **1505 / 5958**, **6.8712525%** matched code, **1045 C / 5642 asm-only** units, and **1227** decomp files (`1026 standalone_tu` + `201 included_stub`). Receipts: `.decomp-runs/20260805T-round-49-final-isolation.json` and `.decomp-runs/20260805T-round-49-apply.json`.

### Batch 197 — accepted (runtime arithmetic and clamp leaves)
- Converted `func_080F1B5C` and `func_080F1FB4` to standalone ordinary C. The arithmetic leaf preserves the target's unsigned shift-pair extraction and three-register multiply sequence; the clamp uses the target-shaped `if (temp <= 0x3F) return 0x7F;` layout so `BLS` lands on the constant-return block.
- The Round 48 screen also tested `func_08003FB8`, `func_08006CC8`, and `func_08006EE0`. Their instruction streams were close, but the candidates changed literal-pool symbol boundaries or encoded the target `.word` offsets as narrower pool entries; none entered the ROM transaction. The first `func_080F1FB4` spelling was retained as a branch-layout near miss, then the v2 spelling became exact.
- The exact two-entry transaction reused one combined isolation receipt and passed the clean full Docker ROM gate. Both sources contain no inline asm. ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Fresh report: **1502 / 5958**, **6.861806%** matched code, **1042 C / 5645 asm-only** units, and **1224** decomp files (`1023 standalone_tu` + `201 included_stub`). Receipts: `.decomp-runs/20260805T-round-48-exact-isolation.json` and `.decomp-runs/20260805T-round-48-apply.json`.

### Batch 196 — accepted (runtime-table ordering repair)
- Converted `func_080F2374`, `func_080F24A0`, `func_080F24C0`, `func_080F2558`, and `func_080F2578` to standalone ordinary C. Empty `asm("" ::: "memory")` barriers keep the required argument normalization ahead of the base load; `func_080F2558` additionally pins the field result to R0 and literal mask to R3 to reproduce the target register reuse.
- The focused six-entry repair screen produced five exact candidates and one near miss. The exact-only five-entry transaction reused that receipt, moved the original assembly sources to `asm/converted/`, updated the linker, and passed the clean full Docker ROM gate. The C bodies contain no instruction-bearing asm; the barriers emit no instructions. `func_080F253C` remains evidence-only because agbcc still folds `MOVS #2; RSBS` into `SUB #3`.
- Fresh report: **1500 / 5958**, **6.856774%** matched code, **1040 C / 5647 asm-only** units, and **1222** decomp files (`1021 standalone_tu` + `201 included_stub`). ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`. Receipts: `.decomp-runs/20260805T-round-47-isolation.json` and `.decomp-runs/20260805T-round-47-apply.json`.

### Batch 195 — accepted (runtime-table bitfield siblings)
- Converted `func_080F0DFC` and `func_080F2358` to standalone ordinary C. The first helper uses the established pinned `D_030068E8` base/offset form and RSBS mask; the second preserves the runtime-record stride, signed 14-bit argument normalization, and halfword mask update.
- One eight-entry isolation pass used one Docker candidate compile: these two entries were exact, while `func_080F2374`, `func_080F24A0`, `func_080F24C0`, `func_080F253C`, `func_080F2558`, and `func_080F2578` were retained as near-miss evidence. The six misses all hoisted the base load before the target's `LSLS/LSRS #24` normalization; `func_080F253C` also folded the target `MOVS #2; RSBS` into `SUB #3`, and `func_080F2558` swapped the target `LDR R3`/literal-mask register roles.
- The exact two-entry transactional apply reused that isolation receipt, moved both original assembly sources to `asm/converted/`, updated the linker, and passed the clean full Docker ROM gate. No instruction-bearing or volatile inline asm was added. ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Fresh report: **1495 / 5958**, **6.840671%** matched code, **1035 C / 5652 asm-only** units, and **1217** decomp files (`1016 standalone_tu` + `201 included_stub`). Receipts: `.decomp-runs/20260805T-round-46-isolation.json` and `.decomp-runs/20260805T-round-46-apply.json`; near misses are retained under `.nearmiss/` and `tools/attempts.tsv`.

### Batch 194 — accepted (key-test wrappers and runtime table helpers)
- Converted eight standalone functions to ordinary C: `func_0801B780`, `func_080203F8`, `func_080227F0`, `func_0801CCC0`, `func_0801CDDC`, `func_08016EF8`, `func_080F0DE0`, and `func_080F0E14`.
- The final Round 45 screen used one exact-only manifest after variant shaping. The five key-test siblings use explicit `gCurrentKeys`/`func_08009EE4` declarations; the scheduler preserves the Thumb callback address with an ordinary `(u8 *)&func + 1` expression; `func_080F0DE0` uses the established pinned runtime-base form; and `func_080F0E14` uses `R4` for the table address, `R3` for the first loaded base, `R0` for the shifted offset, and an `R1` reload for the second store.
- `func_080F0E9C` and `func_080F0EBC` remain evidence-only near misses. Their pointer/argument/register streams match, but agbcc folds the target `MOVS #5/#9; RSBS` mask materialization into `SUB #6/#10` after the preceding `#1` mask. No instruction-bearing asm was added.
- The exact eight-entry transaction passed the clean full Docker ROM gate. Fresh report: **1493 / 5958**, **6.835438%** matched code, **1033 C / 5654 asm-only** units, and **1215** decomp files (`1014 standalone_tu` + `201 included_stub`). `wariowareinc.gba: OK`; ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`. Receipts: `.decomp-runs/20260805T-round-45-exact-isolation.json` and `.decomp-runs/20260805T-round-45-apply.json`; exploratory near misses remain in the Round 45 receipts and `.nearmiss/`.

### Batch 193 — accepted (scene, graphics, and runtime helpers)
- Converted six standalone functions to ordinary C: `func_080DA0B0`, `func_08082934`, `func_0801BEA8`, `func_0801AF18`, `func_0801B3E4`, and `func_080F1574`.
- The scene accumulator preserves the target's global-load order; the graphics transfer pins the `R0` offset and `R1` graphics base and assigns the base between shifts; the three scene-variable masks use a register-bound mask/result so `mask &= value` retains the target `AND R0,R2`/`AND R0,R1` orientation; and the runtime table lookup pins the base to `R1` and uses the target two-operand add.
- A multi-round m2c/variant screen kept the unresolved `func_08016A60`/`func_08016A7C` bit setters as near misses: every ordinary-C mask materialization after the argument's `#1` mask was canonicalized to `SUB #3/#4` rather than the target `MOVS #2/#3; RSBS`. No instruction-bearing asm was used to force it. The accepted six-entry exact receipt passed one transactional full Docker ROM/report gate after adding canonical `D_030068E8`.
- Fresh report: **1485 / 5958**, **6.8121023%** matched code, **1025 C / 5662 asm-only** units, and **1207** decomp files (`1006 standalone_tu` + `201 included_stub`). `wariowareinc.gba: OK`; ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`. Receipts: `.decomp-runs/20260805T-round-44-isolation.json` and `.decomp-runs/20260805T-round-44-apply.json`.

### Batch 192 — accepted (scene wrappers and runtime-buffer byte setter)
- Converted `func_0801002C`, `func_08010308`, and `func_080F3C60` to ordinary C. The two scene wrappers use the direct `gCurrentSceneData + 8` load and existing scene-table declarations; the runtime helper writes four bytes through the absolute `D_030068F0` buffer symbol.
- One eleven-entry m2c isolation screen found **3 exact / 5 near miss / 3 compile error**; only the three exact candidates entered the transaction. Added canonical `D_030068F0 = 0x030068F0` to `undefined_syms.ld` before apply.
- The exact subset passed one transactional full Docker ROM/report gate. The C candidates contain no instruction-bearing or volatile inline asm. Fresh report: **1479 / 5958**, **6.797637%** matched code, **1019 C / 5668 asm-only** units, and **1201** decomp files (`1000 standalone_tu` + `201 included_stub`). `wariowareinc.gba: OK`; ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`. Receipts: `.decomp-runs/20260805T-round-35-isolation.json` and `.decomp-runs/20260805T-round-35-apply.json`.

### Batch 191 — accepted (pure-leaf clamp and signed-absolute helpers)
- Converted `func_080039EC` and `func_08008058` to ordinary C. The signed helper keeps the target's explicit `s16` normalization around the conditional absolute value; the clamp uses the direct lower/upper bound shape.
- Both legacy target objects contain internal local labels that make normalized linked-ELF objdiff infer a truncated function symbol. The candidate instructions were byte-consistent through the inferred boundary, and the rollback-capable `apply-batch --force` path admitted them only after the clean full Docker ROM gate passed.
- The two C candidates contain no instruction-bearing or volatile inline asm. The final gate reported `wariowareinc.gba: OK`; ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Fresh report: **1476 / 5958**, **6.7887807%** matched code, **1016 C / 5671 asm-only** units, and **1198** decomp files (`997 standalone_tu` + `201 included_stub`). Receipts: `.decomp-runs/20260805T-round-34-isolation.json` and `.decomp-runs/20260805T-round-34-apply.json`.

### Batch 190 — accepted (sprite, scene, music-table, and graphics helpers)
- Converted ten standalone functions to ordinary C: `func_0800C7A4`, `func_0800CE6C`, `func_08016688`, `func_08018534`, `func_0801911C`, `func_0801913C`, `func_0801915C`, `func_08019644`, `func_0804E290`, and `func_080C477C`.
- Three sprite/scene wrappers matched through existing typed headers and raw scene-data offsets. The three adjacent music-table siblings required a register-bound base plus an `R0` address accumulator so the compiler emitted the target `ADD R0,R4`, table-entry load, and call sequence. The graphics-buffer indexed store used the same accumulator pattern to preserve the separate `gGraphicsBuffer` literal and `+0x54` add; the division helper matched as direct signed C arithmetic.
- One combined ten-entry m2c isolation receipt passed exact-only selection, followed by one transactional full Docker ROM/report gate. The C candidates contain no instruction-bearing or volatile inline asm. Added canonical `D_083AE428`, `D_083AE430`, and `D_083AE438` linker assignments.
- Fresh report: **1474 / 5960**, **6.7847543%** matched code, **1014 C / 5673 asm-only** units, and **1196** decomp files (`995 standalone_tu` + `201 included_stub`). `wariowareinc.gba: OK`; ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.

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

At the current `total_functions` count (`5934`), that means:
- immediate target: **1781 / 5934** matched functions
- current gap to 30%: **133** more matched functions
- longer-term target: **4748 / 5934** matched functions

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
1. More already-C-linked sibling wrappers whose compiler register pins can be removed with exact ordinary-C spellings (Batch 256 pattern)
2. Main-menu linked mini-batches where a small callee can be converted immediately before its caller (Batch 109 pattern)
3. More conditional byte-check + BL wrappers
4. More shift-offset + store wrappers
5. More `MOVS R0, #const` + BL wrapper families
6. Two-pointer call variants with alternate shift patterns
7. Functions that rely on `ADDS R0, R1, R2` three-register forms plus multiple BL calls
8. More `scene_set_current_thread(1)` + shift-store families
9. ~~More `sprite_id_delete(gSpriteHandler, *(u32*)(gCSV + offset))` siblings~~ — BLOCKED: remaining loop-based variants (asm_08016d3c, asm_0806843c, asm_0806fe20, asm_0805d394, asm_0805c550, asm_0806b99c) fail to match due to loop iteration register patterns not aligning with C for-loop code generation

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
