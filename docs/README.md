# WarioWare Inc. decomp tooling assessment

This docs set records a real tooling pass over `wariowareinc`, focused on:

- current decomp progress
- how `macabeus/mizuchi` fits this repo
- how `macabeus/kappa` fits this repo
- what is and is not practical inside the **pi coding agent harness**
- real smoke-test results, not just theory

## Latest verified scale-up checkpoint (2026-05-20)

- Clean Docker build: `wariowareinc.gba: OK`
- `make report`: **1056 / 5961 matched functions = 17.715149%**
- `matched_code_percent`: **5.906414%**
- `tools/gen_objdiff.py`: **609 C / 6078 asm-only units**
- Accepted batch result: **+5 matched functions** and **+5 C units** versus the previous verified baseline (`1051 -> 1056`, `604 -> 609`)
- Latest successful pattern: a 5-function pair-add helper family that updates two adjacent words from two sibling source words
- Proven workflow detail: preflighting candidate C spellings by compiling just the object files and diffing their disassembly/section sizes against the original asm objects continues to de-risk standalone TU batches before linker edits
- New successful spelling from this batch: `u32 *arg0; arg0[1] += arg0[a]; arg0[2] += arg0[b];` matched cleanly across all five siblings after object preflight
- Important trap from the prior batch still applies: for large byte offsets, writing the total offset directly can change Thumb addressing shape. `*(u8 *)((u8 *)gCurrentSceneVariable + 0x26) = N;` compiled as `adds #0x26; strb #0`, but the target required `adds #8; strb #0x1e`. Using an intermediate pointer (`u8 *p = (u8 *)gCurrentSceneVariable + 8; p[0x1E] = N;`) restored the original codegen.
- Important metric note still applies: in this repo, some standalone conversions improve explicit C coverage without changing `matched_functions`, so both objdiff match metrics and linker/unit coverage must be tracked together.

## Main takeaways

1. **The repo builds cleanly and matches the USA ROM today** using a Dockerized `devkitpro/devkitarm` flow plus `pret/agbcc`.
2. **`make report` now says ~5.40868% matched code**, which is close to the user estimate, but that is **not the same thing as decompiled C coverage**.
3. A rough, repo-local heuristic based on current C definitions puts **explicit C function coverage closer to ~2.768% by function count** (`165 / 5961`).
4. **Mizuchi is the better fit for pi** because it has a real CLI/server workflow.
5. **Kappa is still useful**, but mostly as:
   - a source of prompt-builder/indexing ideas
   - a manual VS Code workflow for a human
   - not as the primary automation path inside pi
6. Three real asm→C smoke tests were completed and kept the ROM matching:
   - `src/memory_heap.c` → `mem_heap_alloc`
   - `src/beatscript.c` → `func_0800A270`
7. One standalone asm-only leaf function outside the existing C-scaffold was also converted successfully:
   - `asm/asm_080f26d0.s` -> `src/asm_080f26d0.c`
   - this moved the official report from `470` to `471` matched functions and from `18` to `19` C units
8. A repo-local Mizuchi bootstrap was added:
   - `mizuchi.yaml`
   - `tools/mizuchi/export-asm.py`
   - `tools/mizuchi/get-context.sh`
   - `tools/mizuchi/compile-in-docker.sh`

## Files in this docs set

- `docs/wariowareinc-decomp-progress-audit.md`
  - build baseline
  - progress numbers
  - smoke-test evidence
  - Mizuchi smoke-test notes
- `docs/macabeus-tools-assessment.md`
  - Kappa vs Mizuchi
  - pi harness accessibility matrix
  - recommended happy path
- `docs/mizuchi-workflow.md`
  - how to use the added Mizuchi helper files in this repo

## Added project files

- `mizuchi.yaml`
- `tools/mizuchi/export-asm.py`
- `tools/mizuchi/get-context.sh`
- `tools/mizuchi/compile-in-docker.sh`

## Branch used

- `docs/macabeus-tooling-assessment`
