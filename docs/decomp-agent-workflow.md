# Autonomous decomp workflow

## Resume protocol
When resuming cold or after compaction:
1. Read `docs/wariowareinc-decomp-scaleup.md`
2. Read `docs/decomp-pattern-library.md`
3. Read `docs/decomp-batch-history.md` if you need recent examples
4. Only fall back `~/.pi/agent/sessions/--Users-kurt-Developer-wariowareinc--/` if the docs are missing needed context

## Preferred fresh-context commands
- `/decomp-next` starts one fresh-context chunk in a new session.
- `/decomp-loop start` runs repeated fresh-context chunks across new sessions.
- `/decomp-loop stop` halts the loop.
- `/decomp-loop status` shows the current loop state.

These commands are preferred over Ralph or pifinity because each chunk starts from the canonical docs with clean context.

## Hard repo rules
1. One function per file in `src/decomp/`
2. Move accepted asm sources into `asm/converted/`
3. Update `wariowareinc.ld` for each standalone TU conversion
4. Require a matching ROM before treating anything as accepted
5. Rerun report + objdiff after accepted progress
6. Commit code + docs together
7. Push immediately after verified progress

## Required verification commands
### Clean Docker build
```bash
docker run --rm -v "$PWD:/workspace" -w /workspace devkitpro/devkitarm:latest \
  bash -lc 'set -euo pipefail; make clean >/dev/null 2>&1; make -j4 2>&1 | tail -n 3'
```
Required success signal:
- `wariowareinc.gba: OK`

### Refresh report
```bash
docker run --rm -v "$PWD:/workspace" -w /workspace devkitpro/devkitarm:latest \
  bash -lc 'set -euo pipefail; make report 2>&1 | tail -n 3'
```
Then read `build/report.json` for:
- `matched_functions`
- `matched_functions_percent`
- `matched_code_percent`
- `total_functions`

### Refresh unit coverage
```bash
python3 tools/gen_objdiff.py
```
Track:
- C units
- asm-only units

## Batch workflow
1. Select a narrow, sibling-rich candidate set
2. Explain to yourself why that family is worth testing
3. Convert the smallest safe subset first if the family is risky
4. Build in Docker
5. If mismatch:
   - binary-search immediately
   - isolate the exact failing function or spelling
   - revert/fix it in the same pass
   - document the trap
6. If match:
   - rerun report
   - rerun objdiff snapshot
   - compare against the last verified baseline
7. If any tracked metric improved:
   - update docs
   - commit + push immediately
8. If no metric improved but a durable lesson was learned:
   - keep only safe/useful changes
   - document the lesson clearly
   - do not pretend it was progress

## Documentation contract
On every accepted batch, update at least:
- `docs/README.md`
- `docs/wariowareinc-decomp-scaleup.md`
- `docs/decomp-batch-history.md`
- `docs/decomp-pattern-library.md` when the batch taught a reusable pattern or trap

## Strategy-adjustment triggers
Stop repeating the same approach if any of these happen:
- 2 consecutive non-improving passes
- 2 mismatch batches from the same family
- a previously safe family stops matching reliably
- documentation gaps caused the same mistake again

When triggered, document:
1. what was attempted
2. what failed
3. whether the issue was candidate choice, code-shaping, repo mechanics, or missing docs
4. the new rule / changed strategy

## Practical source-shaping reminders
- Prefer `types.h` over bare `extern` declarations for g-symbols
- Use `scenes.h` for `gCurrentSceneData`
- Use local pointer vars when literal-pool shape or offset form matters
- Keep declarations before statements (C89)
- Reuse known-good spellings from prior accepted siblings whenever possible

## Commit style
Use short, one-line semantic commits, e.g.:
- `feat: add batch 49 (sprite_id_delete siblings, gCSV shifts)`
- `docs: update scale-up baseline to 1304 matched functions`