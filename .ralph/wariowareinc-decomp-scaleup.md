# WarioWare Inc. Decompilation Scale-Up

## Goal
Reach **80% matched/decompiled-function progress** on the `docs/macabeus-tooling-assessment` branch in `/Users/kurt/Developer/wariowareinc`, while preserving a **byte-identical ROM match** at every accepted milestone.

Target function count:
- **4768 / 5961 matched functions**

## Latest Verified Baseline
Use this as the starting point until a newer verified `make report` run replaces it.

- **Matched functions:** `1046 / 5961` = **17.547392%**
- **Matched code percent:** **5.890302%**
- **C units in linker graph:** `599 / 6687`
- **ASM-only units in linker graph:** `6088`
- **ROM status:** `wariowareinc.gba: OK`
- **Accepted delta vs prior verified baseline:** `+5 matched functions`, `+5 C units`, `-5 asm-only units`

## Current Working State
- HEAD is matching after a clean Docker build and includes a verified mixed wrapper batch combining object-field stores and `gCurrentSceneVariable` word clears after `scene_set_current_thread(1)`.
- The latest accepted files are `asm_0808bae0`, `asm_080afb98`, `asm_080d330c`, `asm_080b9a98`, and `asm_080c0718`.
- `asm_0800cba4` stays in C using a local pointer form to force `LDR base; LDRB/STRB #1` codegen.
- `asm_0800ccb4` remains in asm because the C forms either changed the mask/codegen or shrank the TU by 4 bytes.

## Success Criteria For Any Accepted Progress
A batch only counts as real progress if all of the following are true:
1. The ROM still matches exactly.
2. `make report` has been rerun.
3. At least one measurable metric improved versus the previously documented verified baseline.
4. The improvement, learnings, traps, and next actions were documented in `/docs`.
5. The code changes **and** the documentation updates were committed and pushed together.

## Primary Measurable Metrics
Track these explicitly after each successful batch:
- `build/report.json`:
  - `matched_functions`
  - `matched_functions_percent`
  - `matched_code_percent`
- `wariowareinc.ld` / objdiff-style unit coverage:
  - number of `build/src/...` units
  - number of asm-only units

## Hard Rules
1. **One function per C file** in `src/decomp/` — grouping breaks ROM match.
2. **Move converted `.s` files to `asm/converted/`** — prevents wildcard collision and linker errors.
3. **Update `wariowareinc.ld`** for every standalone TU conversion:
   - swap `build/asm/X.s.o(.text*);`
   - to `build/src/decomp/X.c.o(.text*);`
4. **Every accepted batch must pass a clean Docker build** and report `wariowareinc.gba: OK`.
5. **Every accepted batch must rerun `make report`** and refresh the documented baseline.
6. **Any measurable improvement must be committed and pushed immediately** — do not leave verified progress sitting locally.
7. **Documentation is mandatory for every measurable improvement**:
   - update `docs/README.md` if the headline metrics changed
   - update `docs/wariowareinc-decomp-progress-audit.md` with new metrics and what changed
   - update any other relevant `/docs` file with tooling/workflow learnings, traps, or pattern notes
8. `#include "global.h"` at the top of every C file; add `#include "types.h"` when referencing g-symbols from `types.h`.
9. **g-symbol references**: prefer `#include "types.h"` over bare `extern` declarations — bare `extern u32 gSym;` can cause type conflicts.
10. **D_ symbols**: use `*(volatile u8/u16/u32 *)0xNNNNNNNN` syntax with `u32` arg type; only offset `0` is known-good here.
11. **Function pointer + 1 syntax**: use `(void *)(funcptr + 1)` cast or a symbol form proven to preserve literal-pool behavior.
12. **If a build mismatches**: binary-search the new conversions, isolate the bad function(s), revert or fix them, and document the trap.
13. **POP {R0}/BX R0 vs POP {R1}/BX R1**: agbcc emits `POP {R0}; BX R0`. Functions whose original ROM uses `POP {R1}; BX R1` are not safe matches for the simple wrapper strategy.

## Required Commands
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
Then parse:
- `build/report.json`

### Refresh linker / unit coverage snapshot
```bash
python3 tools/gen_objdiff.py
```
Use its totals or recompute directly from `wariowareinc.ld`.

## Iteration Workflow
For every iteration, follow this exact loop:
1. Select a narrow candidate set.
2. State why those candidates were chosen.
3. Convert the smallest safe batch first.
4. Build in Docker.
5. If mismatch:
   - binary-search the batch
   - identify the failing pattern or function
   - revert/fix
   - record the trap
6. If the ROM matches:
   - run `make report`
   - refresh unit coverage
   - compare against the previous verified baseline
7. If there is **measurable improvement**:
   - update `/docs`
   - commit code + docs together
   - push immediately
   - update the verified baseline in this task file
8. If there is **no measurable improvement** but useful knowledge was gained:
   - keep only changes that are still strategically useful and safe
   - document the learning/trap if it affects future work
   - do **not** claim progress that did not move a tracked metric
9. Queue the next candidate batch based on what was learned.

## Self-Correction Rules
This loop must adapt, not just repeat.

Trigger a strategy adjustment when any of these happen:
- **2 consecutive iterations** with no measurable metric improvement
- **2 consecutive mismatch batches** from the same pattern family
- a supposedly safe pattern stops matching reliably
- a documentation gap caused repeated confusion

When triggered, do all of the following before continuing:
1. Write down what was attempted.
2. State exactly what failed or stagnated.
3. Identify whether the issue was:
   - candidate selection
   - codegen pattern choice
   - repo integration mechanics
   - linker/script handling
   - documentation gap
4. Change the approach explicitly.
5. Record the new rule or trap in `/docs` and in this task file.

## Required Per-Iteration Record
Keep this task file updated with a concise running record. After each completed iteration, update these sections.

### Iteration Log
- iteration number
- candidate set attempted
- result: match / mismatch / partial
- metric deltas
- commit hash if progress was accepted
- next action

### Known Good Patterns
List patterns that repeatedly match safely.

### Known Traps / Non-Matching Patterns
List patterns or mechanics that caused mismatches, wasted time, or integration issues.

### Next Candidate Queue
Short prioritized queue for the next iteration or two.

## Documentation Requirements For Successful Progress
On every accepted measurable improvement, update `/docs` with:
- **what changed**
- **why it worked**
- **what nearly failed / what to avoid**
- **new metric values**
- **what pattern should be tried next**

At minimum, review/update:
- `docs/README.md`
- `docs/wariowareinc-decomp-progress-audit.md`

Also update any relevant workflow/tooling doc when the iteration teaches something durable about:
- candidate selection
- agbcc matching behavior
- linker handling
- inline asm stub replacement
- Mizuchi / Atlas / helper-script workflow

## High-Value Pattern Ideas
- empty BX LR functions
- simple void call wrappers that preserve `POP {R0}; BX R0`
- MOVS `R0, #const`; BX LR return-constant functions
- simple getter/setter pairs with g-symbol offsets that work with `types.h`
- bitfield extract / bit-clear / zero-init helpers already proven by prior batches
- inline asm stubs in C files when the surrounding TU already exists in C
- functions with 5-8 instructions, 0-1 branches, 0-2 BL calls
- `LDR R0, =gSym; LDR R0, [R0]; LDR R0, [R0, #offset]; BX LR` getter chains
- `PUSH {R4, LR}; BL f; BL f2; POP {R4}; POP {R0}; BX R0` save/restore call pairs

## Current Known Good Patterns
- empty BX LR
- simple tail-call wrappers
- void call-wrappers
- D_ setter/getters
- MOVS `R0, #N`; BX LR
- bitfield extract
- abs
- decrement
- bit-clear
- zero-init
- repeated standalone `BX LR` batches can improve linker/unit C coverage even when `matched_functions` stays flat
- short direct `gCurrentSceneVariable` setters can move both matched-function totals and linker/unit coverage
- short `gCurrentSceneVariable` pointer-deref helpers like `u8 *p = *(u8 **)((u8 *)gCurrentSceneVariable + off); *p = 1;` can also match cleanly
- shift-based `gCurrentSceneVariable` byte setters like `u8 *p = (u8 *)gCurrentSceneVariable; p[(IMM << SHIFT)] = VALUE;` can fan out across many siblings
- one-call wrappers of the form `scene_set_current_thread(1); *(u8 *)((u8 *)gCurrentSceneVariable + off) = val;` can also match cleanly across sibling groups
- `PUSH {R4, LR}` wrappers that preserve the incoming object pointer and then store a byte field after `scene_set_current_thread(1)` can match cleanly
- one-call wrappers that clear a `gCurrentSceneVariable` word slot after `scene_set_current_thread(1)` can also match cleanly
- preflighting candidate C spellings as object files before linker edits is effective for short standalone TU batches

## Current Known Traps / Non-Matching Patterns
- grouping multiple functions in one C file breaks ROM matching
- leaving converted `.s` files in place causes wildcard / linker collisions
- bare `extern` declarations for g-symbols can conflict with existing types
- `POP {R1}; BX R1` wrappers are not safe under agbcc’s normal output
- `((u8 *)&gBeatscriptScene)[N]` may compile as a **symbol+offset literal relocation** instead of loading the base symbol and using `[base, #N]`
- even when function bytes look close, TU-level padding/alignment can still break the final ROM; compare `.text` section sizes when a full-ROM mismatch survives seemingly matched code
- for large byte offsets, writing the full offset directly can change Thumb address splitting (`+0x26` / `[+0]` instead of `+8` / `[+0x1E]`); shape the pointer expression to preserve the original immediate split

## Next Candidate Queue
1. continue mining sibling-rich one-call wrapper families, especially `scene_set_current_thread` groups that differ only by store offset/value or object-field offset
2. continue mining short `gCurrentSceneVariable` sibling families where one validated spelling can fan out to multiple siblings
3. mine short `LDR global; STR/STRB/STRH` setters that do not use risky symbol+offset forms
4. mine short `LDR global; LDR/LDRB/LDRH` getters and bitfield extracts that already have matched siblings
5. selectively harvest more `BX LR` leaves only when we want cheap C-unit coverage gains

## Reflection Checkpoint (Iteration 6)
1. **What has been accomplished so far?**
   - The verified baseline moved from `1004 / 5961` to `1041 / 5961` matched functions.
   - Linker/unit coverage improved to `594 C / 6093 asm-only`.
   - Several durable short-function families are now proven and reusable.
2. **What's working well?**
   - Sibling-rich families with one validated spelling reused across multiple asm files.
   - Object-level preflight before linker edits.
   - Tight 5-item batches with immediate Docker/report verification.
3. **What's not working or blocking progress?**
   - One-off exploratory functions are still lower-yield.
   - Minor source spelling changes can still drift codegen.
   - Some accepted batches improve unit coverage without moving matched-function totals, so candidate selection needs care.
4. **Should the approach be adjusted?**
   - Yes: continue prioritizing sibling families, but expand beyond plain setters into one-BL wrappers when they cluster tightly and preflight cleanly.
   - Keep object preflight as a standard gate before touching `wariowareinc.ld`.
5. **What are the next priorities?**
   - Continue mining sibling-rich `gCurrentSceneVariable` families.
   - Prefer families differing only by immediates or one fixed call plus one store.
   - Keep using easy `BX LR` leaves only as opportunistic filler, not the main driver.

## Iteration Log
- **Iteration 1**
  - Candidate set: previously converted small wrapper/getter/setter family now present at HEAD `878d178b`
  - Result: **partial** — project builds cleanly, objdiff metrics improve locally to `1017 / 5961 (17.06%)`, but full ROM no longer matches
  - Metric delta vs verified baseline: `+13 matched functions`, but **not accepted yet** because ROM identity regressed
  - Verification: clean Docker build reaches link step and reports `Build succeeded, but did not match the official ROM.`; local ARM64 `objdiff-cli` generated `build/report.json`
  - Next action: binary-search or selectively revert this batch until `wariowareinc.gba: OK` returns, then rerun report and update docs before claiming progress
- **Iteration 2**
  - Candidate set: binary-search / repair pass over the mismatching batch, focusing first on `asm_0800ccb4` and neighboring `asm_0800cba4` because the ROM diff landed around `0x0800CBA4`
  - Result: **match**
  - Metric delta vs previous verified baseline: `1004 -> 1016 matched functions` (**+12**), `matched_code_percent 5.821934% -> 5.848815%`
  - Verification: clean Docker build returned `wariowareinc.gba: OK`; `make report` refreshed `build/report.json`; `python3 tools/gen_objdiff.py` reported `564 C / 6123 asm-only units`
  - Learnings: `asm_0800cba4` required a local pointer variable to force `[base, #1]` codegen; `asm_0800ccb4` was reverted to asm after revealing both a wrong initial mask and a 4-byte TU-size mismatch risk
  - Next action: commit code + docs together, push immediately, then queue another narrow proven-pattern batch
- **Iteration 3**
  - Candidate set: five standalone `BX LR` leaf stubs adjacent to already-accepted decomp units — `asm_080202dc`, `asm_080202f8`, `asm_08023240`, `asm_08023ca0`, `asm_08026458`
  - Result: **match**
  - Metric delta vs previous verified baseline: `1016 -> 1021 matched functions` (**+5**), `matched_code_percent 5.848815% -> 5.849822%`
  - Verification: clean Docker build returned `wariowareinc.gba: OK`; `make report` refreshed `build/report.json`; `python3 tools/gen_objdiff.py` reported `569 C / 6118 asm-only units`
  - Learnings: the repo’s established `void func(void) {}` spelling still matches pure standalone `BX LR` leaves reliably when each function gets its own TU and the original asm is moved to `asm/converted/`
  - Next action: commit code + docs together, push immediately, then keep mining nearby empty leaves and trivial setters/getters
- **Iteration 4**
  - Candidate set: another five standalone `BX LR` leaf stubs in the same successful neighborhood — `asm_080203b8`, `asm_080203bc`, `asm_080203f4`, `asm_08020b60`, `asm_08020fac`
  - Result: **match**
  - Metric delta vs previous verified baseline: `matched_functions` stayed flat at `1021 / 5961`, `matched_code_percent` stayed flat at `5.849822%`, but linker/unit coverage improved from `569 C / 6118 asm-only` to `574 C / 6113 asm-only`
  - Verification: clean Docker build returned `wariowareinc.gba: OK`; `make report` refreshed `build/report.json`; `python3 tools/gen_objdiff.py` reported `574 C / 6113 asm-only units`
  - Learnings: accepted standalone asm→C conversions do not always move objdiff match totals in this repo; linker/unit coverage must be tracked as a first-class metric alongside matched-function counts
  - Next action: commit code + docs together, push immediately, then pivot toward short setters/getters or return-constant helpers that are more likely to move both metric families
- **Iteration 5**
  - Candidate set: five short direct `gCurrentSceneVariable` setters — `asm_0801ae64`, `asm_080a7c5c`, `asm_080c4a5c`, `asm_080d6f4c`, `asm_080d6fe4`
  - Result: **match** after one repair pass on the two `0x26` byte-store helpers
  - Metric delta vs previous verified baseline: `1021 -> 1026 matched functions` (**+5**), `matched_code_percent 5.849822% -> 5.850829%`, linker/unit coverage `574 C / 6113 asm-only -> 579 C / 6108 asm-only`
  - Verification: clean Docker build returned `wariowareinc.gba: OK`; `make report` refreshed `build/report.json`; `python3 tools/gen_objdiff.py` reported `579 C / 6108 asm-only units`
  - Learnings: direct `gCurrentSceneVariable` setter families are good targets, but large byte offsets may need pointer shaping to preserve the original Thumb immediate split (`+8` then `[+0x1E]` instead of `+0x26` then `[+0]`)
  - Next action: commit code + docs together, push immediately, then continue mining short direct `gCurrentSceneVariable` setters/getters with matched siblings
- **Iteration 6**
  - Candidate set: a mixed 5-function `gCurrentSceneVariable` helper family — `asm_0801ec38`, `asm_080208cc`, `asm_080258dc`, `asm_080258ec`, `asm_080258fc`
  - Result: **match**
  - Metric delta vs previous verified baseline: `1026 -> 1031 matched functions` (**+5**), `matched_code_percent 5.850829% -> 5.858079%`, linker/unit coverage `579 C / 6108 asm-only -> 584 C / 6103 asm-only`
  - Verification: clean Docker build returned `wariowareinc.gba: OK`; `make report` refreshed `build/report.json`; `python3 tools/gen_objdiff.py` reported `584 C / 6103 asm-only units`
  - Learnings: once a candidate spelling is preflighted successfully at the object-file level, near-identical siblings become cheap verified wins; both pointer-deref stores (`*(u8 **)(base + off)`) and repeated direct byte-flag initializers matched cleanly in this batch
  - Next action: commit code + docs together, push immediately, then continue mining sibling-rich `gCurrentSceneVariable` helper families
- **Iteration 7**
  - Candidate set: a 5-function shift-based `gCurrentSceneVariable` byte-setter family — `asm_08053264`, `asm_080c61d0`, `asm_080d24a8`, `asm_080d2890`, `asm_080d7738`
  - Result: **match**
  - Metric delta vs previous verified baseline: `1031 -> 1036 matched functions` (**+5**), `matched_code_percent 5.858079% -> 5.866135%`, linker/unit coverage `584 C / 6103 asm-only -> 589 C / 6098 asm-only`
  - Verification: clean Docker build returned `wariowareinc.gba: OK`; `make report` refreshed `build/report.json`; `python3 tools/gen_objdiff.py` reported `589 C / 6098 asm-only units`
  - Learnings: sibling families whose asm differs only by immediates are excellent batch targets; the spelling `u8 *p = (u8 *)gCurrentSceneVariable; p[(IMM << SHIFT)] = VALUE;` matched cleanly across all five functions once preflighted at the object-file level
  - Next action: commit code + docs together, push immediately, then continue prioritizing sibling-rich `gCurrentSceneVariable` families that can reuse one validated spelling across multiple asm files
- **Iteration 8**
  - Candidate set: a 5-function `scene_set_current_thread(1)` + `gCurrentSceneVariable` byte-setter wrapper family — `asm_08043cc4`, `asm_080b1254`, `asm_080b12c0`, `asm_080b134c`, `asm_080b13d8`
  - Result: **match**
  - Metric delta vs previous verified baseline: `1036 -> 1041 matched functions` (**+5**), `matched_code_percent 5.866135% -> 5.876205%`, linker/unit coverage `589 C / 6098 asm-only -> 594 C / 6093 asm-only`
  - Verification: clean Docker build returned `wariowareinc.gba: OK`; `make report` refreshed `build/report.json`; `python3 tools/gen_objdiff.py` reported `594 C / 6093 asm-only units`
  - Learnings: the one-call wrapper form `scene_set_current_thread(1); *(u8 *)((u8 *)gCurrentSceneVariable + off) = val;` matched cleanly across a sibling group once object-preflight confirmed the exact push/pop/literal-pool shape
  - Next action: commit code + docs together, push immediately, then continue mining sibling-rich families that combine one fixed call with one simple store
- **Iteration 9**
  - Candidate set: a mixed 5-function one-call wrapper batch combining three `PUSH {R4, LR}` object-field byte stores (`asm_0808bae0`, `asm_080afb98`, `asm_080d330c`) with two `gCurrentSceneVariable` word clears (`asm_080b9a98`, `asm_080c0718`) after `scene_set_current_thread(1)`
  - Result: **match**
  - Metric delta vs previous verified baseline: `1041 -> 1046 matched functions` (**+5**), `matched_code_percent 5.876205% -> 5.890302%`, linker/unit coverage `594 C / 6093 asm-only -> 599 C / 6088 asm-only`
  - Verification: clean Docker build returned `wariowareinc.gba: OK`; `make report` refreshed `build/report.json`; `python3 tools/gen_objdiff.py` reported `599 C / 6088 asm-only units`
  - Learnings: one-call wrapper families can safely mix `R4`-preserving object-field stores and `gCurrentSceneVariable` word clears when object-preflight confirms exact wrapper shape, field offsets, and section sizes first
  - Next action: commit code + docs together, push immediately, then keep mining sibling-rich one-call wrapper families before returning to lower-yield one-offs
