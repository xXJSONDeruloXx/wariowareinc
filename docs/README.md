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
- Verified working tree: `batch 272` — two additional ordinary-C included-stub main-menu/sprite wrappers (`func_080133EC` and `func_080136A4`) now emit the same bytes without compiler register pins
- `build/report.json`: **1704 / 5934 matched functions** (**28.715876%**) · **7.686549%** matched code (**76392 / 993840**)
- `tools/gen_objdiff.py`: **1244 linked C TUs / 5443 non-C units** (**6687 total**)
- `src/decomp/*.c`: **1445 decompiled function files** = **1225 standalone_tu** + **220 included_stub**
- ROM: **`wariowareinc.gba: OK`**
- Latest accepted maintenance pass: **35 legacy included-stub files** use real C and ABI/register shaping instead of non-empty inline-asm call/load shims; batches 256–272 additionally removed one hundred ten compiler register pins across forty-nine wrappers. Report function/unit metrics are unchanged because these files were already C-linked.
- Remaining naked/original asm wrapper files in `src/decomp`: **0**; remaining compiler-register-pin files: **148 files / 662 pins**
- Remaining non-volatile empty compiler barriers: **11 files / 11 barriers**; the full strict audit also reports **35 files / 39 empty barrier findings** when volatile barriers coexisting with legacy pins are included. Remaining instruction-bearing inline-asm decomp files: **0**
- `func_080EE61C` is now an ordinary C TU using the target-specific `__builtin_swi_div`; `tools/agbcc-swi.patch` makes the lowering reproducible in local/CI compiler builds
- 25% milestone at the current function total: **1484 / 5934**; now exceeded by **220** matches
- 26% milestone at the current function total: **1543 / 5934**; now exceeded by **161** matches
- 27% active working goal at the current function total: **1603 / 5934**; exceeded by **101** matches
- 30% milestone at the current function total: **1781 / 5934**; **77** more matches needed
- 80% target at the current function total: **4748 / 5934**
- Remaining gap to 80%: **3044 matched functions**

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
