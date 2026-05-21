# WarioWare Inc. decomp docs

This docs set is now the canonical replacement for the old Ralph task file flow.
If an agent resumes cold, read these first:

1. `docs/wariowareinc-decomp-scaleup.md` — current baseline, priorities, next queue
2. `docs/decomp-agent-workflow.md` — exact autonomous workflow and verification loop
3. `docs/decomp-pattern-library.md` — proven families, code-shaping rules, known traps
4. `docs/decomp-batch-history.md` — accepted batch history migrated from `.ralph` + session logs

## Current verified baseline
- Branch: `docs/macabeus-tooling-assessment`
- HEAD: `0f121592` (`feat: add batch 48 ...`)
- `matched_functions`: **1297 / 5957** (**21.772705%**)
- `matched_code_percent`: **6.285469%**
- `tools/gen_objdiff.py`: **845 C / 5842 asm-only units**
- ROM: **`wariowareinc.gba: OK`**
- 80% target at the current function total: **4766 / 5957**
- Remaining gap to 80%: **3469 matched functions**

## How autonomous continuation should work
- Pifinity / `continue` should keep moving forward without asking for a focus area unless truly blocked.
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
The old `.ralph/wariowareinc-decomp-scaleup.md` file should be treated as legacy source material. Keep it only as an archive; continue maintaining the live workflow in `/docs` + `AGENTS.md`.