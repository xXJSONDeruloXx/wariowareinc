# WarioWare Inc. decomp docs

This docs set is now the canonical replacement for the old Ralph task file flow.
If an agent resumes cold, read these first:

1. `docs/wariowareinc-decomp-scaleup.md` — current baseline, priorities, next queue
2. `docs/decomp-agent-workflow.md` — exact autonomous workflow and verification loop
3. `docs/decomp-pattern-library.md` — proven families, code-shaping rules, known traps
4. `docs/decomp-batch-history.md` — accepted batch history + cleanup notes

## Current baseline
- Branch: `docs/macabeus-tooling-assessment`
- Last ROM-verified milestone: `batch 50` candidate (`sprite_id_delete` byte-offset siblings + helper wrappers)
- Current report snapshot after repo-state cleanup:
  - `matched_functions`: **1320 / 5949** (**22.188602%**)
  - `matched_code_percent`: **6.3567476%**
  - `tools/gen_objdiff.py`: **868 C / 5820 asm-only units**
- Clean Docker ROM verification was **not rerun in this cleanup pass** because the current `devkitpro/devkitarm:latest` image lacks `ffmpeg`.
- 80% target at the current function total: **4760 / 5949**
- Remaining gap to 80%: **3440 matched functions**

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

