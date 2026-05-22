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
- `/decomp-setup` or `/decomp-health` checks whether a machine has the required Docker/Mizuchi/decomp prerequisites.

These commands are preferred over Ralph or pifinity because each chunk starts from the canonical docs with clean context.

## New-machine / portable setup
1. Clone this repo.
2. Install Docker, python3, git, and pi.
3. Clone Mizuchi next to the repo (`../mizuchi`) or, if you keep it elsewhere, add `export MIZUCHI_ROOT=/absolute/path/to/mizuchi` to `~/.zshrc` and restart/source your shell before starting pi.
4. Run `/reload`.
5. Run `/decomp-setup`.
6. Run `/decomp-verify`.

Notes:
- The decomp tools autodetect a sibling Mizuchi checkout at `../mizuchi`.
- If an environment variable is needed, persist it in `~/.zshrc` rather than setting it for one shell only.
- On macOS, do **not** rely on local `make` / `make report`; use Docker verification.

## Hard repo rules
1. One function per file in `src/decomp/`
2. Move accepted asm sources into `asm/converted/`
3. Update `wariowareinc.ld` for each standalone TU conversion
4. Use supported mechanical workflows when `preflight_candidate` reports `safeForAutonomous=true`
5. Supported workflows: `standalone_tu` (src/decomp + linker swap) and `included_stub` (guarded include-shim inside the host C TU)
6. Treat `unknown_skip` and `already_converted` as research/manual candidates; do not force generic linker-swap conversion onto them
7. Require a matching ROM before treating anything as accepted
8. Rerun report + objdiff after accepted progress
9. Commit code + docs together
10. Push immediately after verified progress

## Required verification commands
### Clean Docker build
```bash
docker run --rm -v "$PWD:/workspace" -w /workspace devkitpro/devkitarm:latest \
  bash -lc 'set -euo pipefail; rm -rf build; make -j4 2>&1 | tail -n 3'
```
Required success signal:
- `wariowareinc.gba: OK`

On macOS, do **not** rely on local `make`/`make report` for verification; use Docker. Shortcut command: `/decomp-verify`.

### Refresh report
```bash
docker run --rm -v "$PWD:/workspace" -w /workspace devkitpro/devkitarm:latest \
  bash -lc 'set -euo pipefail; make report 2>&1 | tail -n 3'
```
Shortcut command: `/decomp-report`

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
- linked C TUs
- non-C units

### Refresh decomp-file coverage
Count `src/decomp/*.c` separately.
This is a different metric from objdiff unit coverage because `included_stub` conversions live in `src/decomp/` but do **not** create new linked C translation units.
Track:
- total decompiled function files
- standalone_tu files
- included_stub files

## Batch workflow
1. Select a narrow, sibling-rich candidate set with `query_candidates` (default `conversionMode=recommended`)
2. `recommended` includes `standalone_tu` and `included_stub` candidates that the tools can apply mechanically
3. Call `preflight_candidate` before iteration; proceed when `safeForAutonomous=true`
4. If only `unknown_skip`/manual candidates remain, use `conversionMode=all` diagnostically and pick a small promising target only when you can explain the integration path
5. Explain to yourself why that family is worth testing
6. Convert the smallest safe subset first if the family is risky
7. After a 100% isolated match, prefer `apply_conversion` for mechanical edits and clean Docker verification
8. Build in Docker
9. If mismatch:
   - binary-search immediately
   - isolate the exact failing function or spelling
   - revert/fix it in the same pass
   - document the trap
10. If match:
   - rerun report
   - rerun objdiff snapshot
   - compare against the last verified baseline
11. If any tracked metric improved:
   - update docs
   - commit + push immediately
12. If no metric improved but a durable lesson was learned:
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