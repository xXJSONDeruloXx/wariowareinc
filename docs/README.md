# WarioWare Inc. decomp tooling assessment

This docs set records a real tooling pass over `wariowareinc`, focused on:

- current decomp progress
- how `macabeus/mizuchi` fits this repo
- how `macabeus/kappa` fits this repo
- what is and is not practical inside the **pi coding agent harness**
- real smoke-test results, not just theory

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
