# WarioWare Inc. Autonomous Decomp Agent Guide

## Canonical memory
When resuming work in this repo, use these files as the primary source of truth:
1. `docs/README.md`
2. `docs/wariowareinc-decomp-scaleup.md`
3. `docs/decomp-agent-workflow.md`
4. `docs/decomp-pattern-library.md`
5. `docs/decomp-batch-history.md`

If those docs are not enough, mine prior session history from:
- `~/.pi/agent/sessions/--Users-kurt-Developer-wariowareinc--/`

## Autonomous behavior
- Treat a bare `continue` or pifinity auto-continue as permission to keep pushing the decomp forward without asking what aspect to focus on.
- Only stop to ask the user something if you are truly blocked by missing assets, a broken toolchain, contradictory repo state, or a choice that cannot be made safely from the docs/history.
- If context gets compacted or thin, re-read the docs above and continue.
- Persist every durable learning back into `/docs` so the next autonomous pass can pick up cold.

## Current mission
- Preserve a byte-identical ROM at every accepted milestone.
- Push matched-function progress upward from the latest verified baseline documented in `docs/wariowareinc-decomp-scaleup.md`.
- Prefer small, sibling-rich batches that can be verified and committed quickly.

## Non-negotiable workflow rules
1. One function per file in `src/decomp/`.
2. Move converted asm files into `asm/converted/`.
3. Update `wariowareinc.ld` for every standalone TU conversion.
4. After any candidate batch, run the clean Docker build and require `wariowareinc.gba: OK`.
5. After any accepted batch, rerun `make report` and `python3 tools/gen_objdiff.py`.
6. Update `/docs` before ending the successful pass.
7. Commit code + docs together and push immediately after verified progress.
8. If a batch mismatches, binary-search it immediately, revert/fix the bad function(s), and document the trap.

## Candidate selection bias
Prioritize:
- proven sibling families
- tiny wrappers with already-validated C spellings
- gCurrentSceneVariable / gCurrentSceneData / gGraphicsBuffer patterns already documented in `docs/decomp-pattern-library.md`
- easy filler only when it still moves tracked metrics or unit coverage

Avoid re-learning known traps unless you are deliberately testing a new spelling that is documented as unresolved.

## Documentation contract
Any accepted progress should update, at minimum:
- `docs/README.md`
- `docs/wariowareinc-decomp-scaleup.md`
- `docs/decomp-pattern-library.md` if a new durable pattern/trap was learned
- `docs/decomp-batch-history.md`

## Commit style
Use short, one-line, semantic commits.