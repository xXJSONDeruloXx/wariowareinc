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
- Verified working tree: `batch 205` — one new strict-ROM standalone scene-table setter sibling; known blocked families remain preserved as provenance
- `build/report.json`: **1530 / 5951 matched functions** (**25.709967%**) · **6.9603233%** matched code (**69160 / 993632**)
- `tools/gen_objdiff.py`: **1070 linked C TUs / 5617 non-C units** (**6687 total**)
- `src/decomp/*.c`: **1252 decompiled function files** = **1051 standalone_tu** + **201 included_stub**
- ROM: **`wariowareinc.gba: OK`**
- Latest accepted maintenance pass: **30 legacy included-stub files** now use real C and ABI/register shaping instead of non-empty inline-asm call/load shims; report metrics are unchanged because these files were already C-linked.
- Remaining naked/original asm wrapper files in `src/decomp`: **0**
- Remaining instruction-bearing inline-asm decomp files: **0**
- `func_080EE61C` is now an ordinary C TU using the target-specific `__builtin_swi_div`; `tools/agbcc-swi.patch` makes the lowering reproducible in local/CI compiler builds
- 25% milestone at the current function total: **1488 / 5951**; now exceeded by **42** matches
- 30% milestone at the current function total: **1786 / 5951**; **256** more matches needed
- 80% target at the current function total: **4761 / 5951**
- Remaining gap to 80%: **3231 matched functions**

## Automated matching loop

The runtime-neutral lifecycle is `tools/decomp_cycle.py`:

- `isolate` evaluates a manifest of C permutations/siblings in one Docker compiler invocation, links normalized comparison ELFs using the target's absolute symbol map, and writes a structured `.decomp-runs/` receipt.
- `tools/decomp_permute.py screen` fans a directory of m2c/asmlift/manual C spellings into one isolation pass, snapshots every input under `.decomp-runs/`, and records exact/near-miss hashes; `accept` can select only a recorded exact winner for the normal transactional apply.
- `apply` requires an isolated exact match, applies the mechanical conversion, runs the strict ROM/report gate, and restores the candidate transaction plus a clean baseline on failure. A fresh screen receipt is reused when its commit and input hashes still match, avoiding a redundant isolation container.
- `apply-batch` performs the same guarded transaction for a small exact manifest, with one isolation pass and one full-ROM gate for the batch.
- `verify` runs the current-worktree Docker gate for hooks or a final check.

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
and standalone/included decomp files, ROM SHA-1, and the remaining gap to 30%.
This keeps progress readable across sessions even when the detailed receipts are
collapsed.

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
