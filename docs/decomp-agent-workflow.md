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
1. One function per file in `src/decomp/`.
2. A chunk may convert a small linked batch when that is safer than a single function, especially in the remaining `included_stub` era. Keep each converted function in its own `src/decomp/asm_xxxxxxxx.c` file.
3. Move accepted asm sources into `asm/converted/`.
4. Update `wariowareinc.ld` for each standalone TU conversion.
5. Use supported mechanical workflows when `preflight_candidate` reports `safeForAutonomous=true`.
6. Supported workflows: `standalone_tu` (src/decomp + linker swap) and `included_stub` (guarded include-shim inside the host C TU).
7. Treat `unknown_skip` and `already_converted` as research/manual candidates; do not force generic linker-swap conversion onto them.
8. Require a matching ROM before treating anything as accepted.
9. **Avoid decompiling callers of already-converted C functions blindly**: A 100% isolated compile match does NOT guarantee ROM match when the callee has been converted to C. The isolated test compares against original asm, but the linked ROM uses the converted C callee with potentially different register allocation. Prefer callee-before-caller mini-batches, and verify the ROM after each applied function or after the smallest reversible subgroup.
10. **Clean-worktree check before diagnosing callee-risk**: If a 100% isolated match fails final ROM verification, run `git status --short` before assuming the candidate is bad. Revert unrelated manual edits and retry once; dirty edits can create false ROM mismatches.
11. **Preflight "already_converted" false positive**: If `apply_conversion` fails and auto-restores files, `preflight_candidate` may report `already_converted` on the next attempt because it checks for files before checking mizuchi-db. The files were restored but mizuchi-db still lists the function as unmatched. **Fix**: Run a manual `git commit` to actually land the changes, then refresh mizuchi-db with index-codebase, or ignore the preflight warning and proceed if you know the conversion is valid.
12. Rerun report + objdiff after accepted progress.
13. Commit code + docs together.
14. Push immediately after verified progress.

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
1. Select a narrow, sibling-rich candidate set with `query_candidates` (default `conversionMode=recommended`).

### Near-miss provenance

When a candidate compiles but does not match, preserve the evidence for a future
run instead of relying on chat history:

```bash
python3 tools/record_nearmiss.py FUNC TARGET.s SCORE candidate.c \
  --reason rom_mismatch \
  --diff-json /tmp/diff.json \
  --command 'docker run ... make -j4'
```

Scores are lower-is-better. Each attempt is appended to
`tools/attempts.tsv`; `.nearmiss/FUNC.json` and `.nearmiss/FUNC.full.c` are
updated only when the candidate beats the previous stored score. The JSON
captures the candidate hash, Git branch/commit, UTC timestamp, ROM and
baserom hashes, build command, and structured diff evidence. This is a
provenance ledger and seed store, not an acceptance mechanism: only the clean
Docker build emitting `wariowareinc.gba: OK` accepts a function.

### Automated candidate cycle

Use `tools/decomp_cycle.py` when a function has multiple C spellings or when a
small sibling batch should share one compiler startup. A manifest is a JSON
object with a `candidates` array; each entry names `function`, `candidate`, and
either `target` assembly or an existing `target_object`:

```bash
python3 tools/decomp_cycle.py isolate --manifest tools/decomp-cycle-demo.json
```

`isolate` compiles every entry in one Docker container, runs objdiff on the
host, writes `.decomp-runs/*-isolation.json`, and records non-exact candidates.
An isolated mismatch is useful evidence, never acceptance. The recorder tags
score units (`rom_bytes` versus `isolated_objdiff_gap`) so unlike measurements
are not compared by the keep-best logic.

For multiple spellings of one function, put each complete C candidate in a
directory and use the permutation fan-in. The template manifest supplies the
target and host-TU metadata once:

```bash
python3 tools/decomp_permute.py screen \
  --manifest .mizuchi-tmp/manifests/FUNC.json \
  --candidate-dir .mizuchi-tmp/permutations \
  --require-exact
```

This produces one durable `permutation_screen` receipt containing every
candidate hash and exact/near-miss result. It never changes `src/`, `asm/`, or
the linker. After reviewing the exact list, select one winner explicitly:

```bash
python3 tools/decomp_permute.py accept \
  --receipt .decomp-runs/…-permutation-FUNC.json \
  --variant .mizuchi-tmp/permutations/FUNC-best.c
```

`accept` delegates to `decomp_cycle.py apply`, so the winner is isolated again,
mechanically applied, and admitted only by the clean Docker ROM/report gate.
The Pi `apply_conversion` frontend is routed through the same cycle; its
`verify:false` mode is intentionally refused for real applies (dry-run still
works).
Generated evidence (`.decomp-runs/`, `.nearmiss/`, `.mizuchi-tmp/`, and the
attempt ledger) may remain dirty between these two commands; unrelated source,
linker, or tool edits still block the transaction.

After a selected entry reports `exact`, use the transactional path with a
manifest entry that also supplies `source`, `converted`, `linker_old`, and
`linker_new` (the defaults cover a normal standalone TU):

```bash
python3 tools/decomp_cycle.py apply --manifest candidate.json --function FUNC
```

`apply` refuses non-exact isolation, performs the source/linker/ASM move, runs
the clean Docker ROM gate plus `make report`, and leaves matching changes in
the worktree. Any full-build failure restores the exact pre-apply files and
rebuilds the baseline; the receipt records both the failed attempt and the
rollback verification. `--force` exists only for tool testing and research; it
does not weaken the ROM gate and is not part of normal decomp work.

For a current-worktree check without a candidate transaction:

```bash
python3 tools/decomp_cycle.py verify
```

Install the repository hooks once with `tools/install-hooks.sh`. `pre-commit`
checks changed C policy and runs the Docker SHA gate for staged ROM-affecting
paths; `pre-push` checks the pushed range and runs the same gate before allowing
a push. Documentation/tooling-only commits do not pay the full build cost.
2. `recommended` includes `standalone_tu` and `included_stub` candidates that the tools can apply mechanically.
3. In the current post-standalone phase, assume **one function per chunk is no longer always optimal**. Prefer a tiny linked batch when a caller/callee pair is obvious, when callee-first conversion reduces risk, or when a dirty-worktree false mismatch would otherwise burn repeated chunks.
4. Call `preflight_candidate` before iteration; proceed when `safeForAutonomous=true` for each function in the proposed batch.
5. **After matching a function**, use `decomp_siblings` to find similar functions for batch conversion:
   - `strategy: same_file` – Functions in same asm file (best for family conversion)
   - `strategy: same_module` – Functions in same source module (e.g., graphics_table)
   - `strategy: callers` – Functions that call the matched one
   - `strategy: callees` – Functions called by the matched one
   - `strategy: pattern` – Functions with similar instruction patterns (loops, conditionals, etc.)
6. If only `unknown_skip`/manual candidates remain, use `conversionMode=all` diagnostically and pick a small promising target only when you can explain the integration path.
7. Explain why that family is worth testing and what the dependency order is.
8. Convert the smallest safe subset first if the family is risky. For linked included stubs, apply the callee first, then the immediate caller.
9. After a 100% isolated match, prefer the manifest-driven `decomp_cycle.py apply` path so the attempt and rollback receipt is automatic. `apply_conversion` remains a valid Pi frontend fallback, but its result must still be represented in a run receipt before commit. For a batch, keep the subgroup small enough to revert immediately.
10. Build in Docker.
11. If mismatch:
   - binary-search immediately
   - isolate the exact failing function or spelling
   - revert/fix it in the same pass
   - document the trap
12. If match:
   - rerun report
   - rerun objdiff snapshot
   - compare against the last verified baseline
13. If any tracked metric improved (matched code, linked C TUs, or `src/decomp/*.c` included_stub coverage):
   - update docs
   - commit + push immediately
14. If no metric improved but a durable lesson was learned:
   - keep only safe/useful changes
   - document the lesson clearly
   - do not pretend it was progress

## End-of-chunk tooling reflection
Before ending any chunk, briefly assess whether the current tools helped or failed:
- Did `query_candidates`/`preflight_candidate` surface the real integration risk?
- Did `compile_and_view_asm` give a useful isolated signal, or did final ROM verification expose a gap?
- Did `apply_conversion` restore cleanly after failure?
- Was a manual workaround needed?

Record durable tool/workflow observations in `docs/decomp-tooling-feedback.md`. Do not reduce existing tool scope or bypass clean ROM verification; use the notes to improve future automation and candidate selection.

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

## Programmatic anti-ASM guardrail

The project extension `.pi/extensions/warioware-decomp-guard.js` blocks common asm cop-outs at the tool layer:
- `compile_and_view_asm` / `apply_conversion` calls whose `cCode` contains `__attribute__((naked))`, non-empty inline asm statements, `thumb_func_start`, or direct `asm/*.s` includes
- `write` / `edit` attempts that add those patterns to `src/decomp/*.c`
- `bash` commands that appear to write naked/original/non-empty inline asm into `src/decomp/*.c`
- `git add` / `git commit` when changed `src/decomp/*.c` files still contain banned asm patterns

Use the `decomp_guard_check` tool before committing if a chunk touched `src/decomp`. Existing legacy asm files are allowed only while untouched; converting them to real C is allowed and encouraged. Empty asm barriers/clobbers are still allowed for C shaping, but instruction/call shims are not accepted for new progress.

## Naked asm → real C conversion (maintenance pass only)

Each chunk MAY spend spare effort converting an existing `__attribute__((naked))` or non-empty-inline-asm decomp file to real C with register pinning + empty asm barriers/clobbers only. This is a dedicated maintenance pass, not a fallback when the primary candidate is stubborn.

**Process:**
1. List legacy asm files: use `decomp_guard_check(includeLegacy=true)` for a guarded scan, or `grep -rl '__attribute__((naked))' src/decomp/` for the one remaining naked wrapper.
2. Pick only a file with a clear C-shaping plan and a documented reason it is probably convertible.
3. Write a real C version using register pinning + empty asm barriers/clobbers. Do not use `asm volatile("bl callee")`, `ldrsh`, `svc`, `add`, or other non-empty instruction shims for new progress.
4. Keep iterating until perfect, or pick another real-C candidate if this file is not yielding.
5. If it still won't match, stop, document the blocker, and leave the existing naked asm untouched. Do not create a new naked asm wrapper and do not use naked asm as a fallback for a near-miss primary conversion.

**Known hard cases that should stay naked:**
- Functions using `_call_via_r1` (no C equivalent)
- Functions with `CMP #1; BGE` (agbcc always optimizes to `CMP #0; BGT`)
- Functions with s16 return type where the epilogue sign-extends but the original doesn't (e.g., `sprite_get_anim_duration`, `sprite_anim_get_cel_total`)
- Functions where R3 must survive across a LDR instruction (scratch register can't be callee-saved without extra push/pop)

## Commit style
Use short, one-line semantic commits, e.g.:
- `feat: add batch 49 (sprite_id_delete siblings, gCSV shifts)`
- `docs: update scale-up baseline to 1304 matched functions`
