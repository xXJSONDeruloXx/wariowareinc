# WarioWare Inc. decomp docs

This docs set is now the canonical replacement for the old Ralph task file flow.
If an agent resumes cold, read these first:

1. `docs/wariowareinc-decomp-scaleup.md` — current baseline, priorities, next queue
2. `docs/decomp-agent-workflow.md` — exact autonomous workflow and verification loop
3. `docs/decomp-pattern-library.md` — proven families, code-shaping rules, known traps
4. `docs/decomp-batch-history.md` — accepted batch history
5. `docs/decomp-tooling-feedback.md` — tooling gaps, workarounds, and improvement notes

## Current verified baseline
- Verified working tree: `batch 130` — sprite_set_x_y naked asm included_stub conversion (lib_sprite combined x/y position setter)
- `build/report.json`: **1350 / 5960 matched functions** (**22.6510%**) · **6.453159%** matched code
- `tools/gen_objdiff.py`: **892 linked C TUs / 5795 non-C units**
- `src/decomp/*.c`: **1043 decompiled function files** = **873 standalone_tu** + **170 included_stub**
- ROM: **`wariowareinc.gba: OK`**
- Remaining naked asm files: **10** (func_0800BEC0, func_0800C080, func_080113EC, func_08011774, func_08014E88, sprite_anim_get_cel_total, sprite_get_anim_duration, sprite_set_x, sprite_set_y, sprite_set_x_y)
- 80% target at the current function total: **4765 / 5956**
- Remaining gap to 80%: **3409 matched functions**

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

## Doc map
### Active operational docs
- `docs/wariowareinc-decomp-scaleup.md`
- `docs/decomp-agent-workflow.md`
- `docs/decomp-pattern-library.md`
- `docs/decomp-batch-history.md`
- `docs/decomp-tooling-feedback.md`

### Reference / historical docs
- `docs/wariowareinc-decomp-progress-audit.md` — early audit + smoke-test archive; not the live source of truth
- `docs/macabeus-tools-assessment.md` — tooling assessment notes
- `docs/mizuchi-workflow.md` — Mizuchi bootstrap notes

## Legacy note
continue maintaining the live workflow in `/docs` + `AGENTS.md`.