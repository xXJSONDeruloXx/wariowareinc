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
- Verified working tree: `batch 287` — `func_08004400` now emits the same bytes as its legacy asm through ordinary C, with a complete linked-text proof for the target's truncated internal-label symbol
- `build/report.json`: **1705 / 5933 matched functions** (**28.737568%**) · **7.6909766%** matched code (**76436 / 993840**)
- `tools/gen_objdiff.py`: **1245 linked C TUs / 5442 non-C units** (**6687 total**)
- `src/decomp/*.c`: **1446 decompiled function files** = **1226 standalone_tu** + **220 included_stub**
- ROM: **`wariowareinc.gba: OK`**
- Latest accepted maintenance pass: **38 legacy included-stub files** use real C and ABI/register shaping instead of non-empty inline-asm call/load shims; batches 256–286 additionally removed two hundred seventy compiler register pins across ninety-one already-linked functions. Report function/unit metrics are unchanged because these files were already C-linked.
- Remaining naked/original asm wrapper files in `src/decomp`: **0**; remaining compiler-register-pin files: **106 files / 502 pins**
- Remaining non-volatile empty compiler barriers: **7 files / 7 barriers**; the full strict audit also reports **27 files / 31 empty barrier findings** when volatile barriers coexisting with legacy pins are included. Remaining instruction-bearing inline-asm decomp files: **0**
- `func_080EE61C` is now an ordinary C TU using the target-specific `__builtin_swi_div`; `tools/agbcc-swi.patch` makes the lowering reproducible in local/CI compiler builds
- 25% milestone at the current function total: **1484 / 5933**; now exceeded by **221** matches
- 26% milestone at the current function total: **1543 / 5933**; now exceeded by **162** matches
- 27% active working goal at the current function total: **1602 / 5933**; exceeded by **103** matches
- 30% milestone at the current function total: **1780 / 5933**; **75** more matches needed
- 80% target at the current function total: **4747 / 5933**
- Remaining gap to 80%: **3042 matched functions**

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
