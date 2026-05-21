# WarioWare Inc. decomp docs

This docs set is now the canonical replacement for the old Ralph task file flow.
If an agent resumes cold, read these first:

1. `docs/wariowareinc-decomp-scaleup.md` — current baseline, priorities, next queue
2. `docs/decomp-agent-workflow.md` — exact autonomous workflow and verification loop
3. `docs/decomp-pattern-library.md` — proven families, code-shaping rules, known traps
4. `docs/decomp-batch-history.md` — accepted batch history

## Current verified baseline
- Verified working tree: `batch 63` — `func_08003A00` abs-value helper
- `matched_functions`: **1336 / 5954** (**22.447%**)
- `matched_code_percent`: **6.4148%**
- `tools/gen_objdiff.py`: **884 C / 5803 asm-only units**
- ROM: **`wariowareinc.gba: OK`**
- 80% target at the current function total: **4763 / 5954**
- Remaining gap to 80%: **3427 matched functions**

## How autonomous continuation should work
- Prefer the repo-local fresh-context commands over pifinity/Ralph:
  - `/decomp-next` for one fresh-context chunk
  - `/decomp-loop start` for repeated fresh-context chunks across new sessions
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