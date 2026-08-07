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
- Verified working tree: `batch 236` — eight standalone ordinary-C leaf/wrapper conversions admitted after a 12-candidate strict screen and one complete full-context Docker gate
- `build/report.json`: **1691 / 5934 matched functions** (**28.4968%**) · **7.6456504%** matched code (**75984 / 993820**)
- `tools/gen_objdiff.py`: **1231 linked C TUs / 5456 non-C units** (**6687 total**)
- `src/decomp/*.c`: **1415 decompiled function files** = **1212 standalone_tu** + **203 included_stub**
- ROM: **`wariowareinc.gba: OK`**
- Latest accepted maintenance pass: **32 legacy included-stub files** now use real C and ABI/register shaping instead of non-empty inline-asm call/load shims; report metrics are unchanged because these files were already C-linked.
- Remaining naked/original asm wrapper files in `src/decomp`: **0**
- Remaining instruction-bearing inline-asm decomp files: **0**
- `func_080EE61C` is now an ordinary C TU using the target-specific `__builtin_swi_div`; `tools/agbcc-swi.patch` makes the lowering reproducible in local/CI compiler builds
- 25% milestone at the current function total: **1484 / 5934**; now exceeded by **207** matches
- 26% milestone at the current function total: **1543 / 5934**; now exceeded by **148** matches
- 27% active working goal at the current function total: **1603 / 5934**; exceeded by **88** matches
- 30% milestone at the current function total: **1781 / 5934**; **90** more matches needed
- 80% target at the current function total: **4748 / 5934**
- Remaining gap to 80%: **3065 matched functions**

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
