# WarioWare Inc. decomp docs

This docs set is now the canonical replacement for the old Ralph task file flow.
If an agent resumes cold, read these first:

1. `docs/wariowareinc-decomp-scaleup.md` — current baseline, priorities, next queue
2. `docs/decomp-agent-workflow.md` — exact autonomous workflow and verification loop
3. `docs/decomp-pattern-library.md` — proven families, code-shaping rules, known traps
4. `docs/decomp-batch-history.md` — accepted batch history

## Current verified baseline
- Verified working tree: `batch 80` — func_080CD564: field copy via LDR/STR pairs at 0x28/0x2C offsets (inline asm for exact ADDS R3,R0,#0 pattern)
- `build/report.json`: **1345 / 5955 matched functions** (**22.586%**) · **6.4327%** matched code
- `tools/gen_objdiff.py`: **892 linked C TUs / 5795 non-C units**
- `src/decomp/*.c`: **939 decompiled function files** = **873 standalone_tu** + **66 included_stub**
- ROM: **`wariowareinc.gba: OK`**
- 80% target at the current function total: **4764 / 5955**
- Remaining gap to 80%: **3420 matched functions**

## How autonomous continuation should work
- Prefer the repo-local fresh-context commands over pifinity/Ralph:
  - `/decomp-next` for one fresh-context chunk
  - `/decomp-loop start` for repeated fresh-context chunks across new sessions
- On a new machine, first run:
  - `/reload`
  - `/decomp-setup` (or `/decomp-health`)
  - `/decomp-verify`
- The decomp tools now autodetect a sibling Mizuchi checkout at `../mizuchi` or use `$MIZUCHI_ROOT` / `$PI_MIZUCHI_ROOT`; if you need an env var, persist it in `~/.zshrc`.
- The docs in this directory are the durable memory that should survive context compaction and session changes.
- Any new durable learning should be written back here before the agent yields.

## Doc map
### Active operational docs
- `docs/wariowareinc-decomp-scaleup.md`
- `docs/decomp-agent-workflow.md`
- `docs/decomp-pattern-library.md`
- `docs/decomp-batch-history.md`

### Reference / historical docs
- `docs/wariowareinc-decomp-progress-audit.md` — early audit + smoke-test archive; not the live source of truth
- `docs/macabeus-tools-assessment.md` — tooling assessment notes
- `docs/mizuchi-workflow.md` — Mizuchi bootstrap notes

## Legacy note
continue maintaining the live workflow in `/docs` + `AGENTS.md`.