# WarioWare Inc. decomp tooling assessment

This docs set records a real tooling pass over `wariowareinc`, focused on:

- current decomp progress
- how `macabeus/mizuchi` fits this repo
- how `macabeus/kappa` fits this repo
- what is and is not practical inside the **pi coding agent harness**
- real smoke-test results, not just theory

## Latest verified scale-up checkpoint (2026-05-20)

- Clean Docker build: `wariowareinc.gba: OK`
- `make report`: **1081 / 5961 matched functions = 18.134542%**
- `matched_code_percent`: **5.960588%**
- `tools/gen_objdiff.py`: **629 C / 6058 asm-only units**
- Accepted batch result: **+5 matched functions** and **+5 C units** versus the previous verified baseline (`1076 -> 1081`, `624 -> 629`)
- Latest successful pattern: continuing the `D_03006520` compare-and-call wrapper family, including the two-call variant `if (D_03006520 == IMM) { func1(); func2(); }` and the large-immediate variant `if (D_03006520 == 500) func();` which uses `MOVS R0, #0xFA; LSLS R0, #1` to construct 0x1F4
- The `if (D_03006520 == IMM) func_target();` pattern matched cleanly for three more single-BL siblings
- The two-call variant `if (D_03006520 == IMM) { func1(); func2(); }` also matched cleanly
- The large-immediate `== 500` variant matched with `if (D_03006520 == 500) func();` — agbcc correctly emits the `MOVS R0, #0xFA; LSLS R0, R0, #1; CMP R1, R0` sequence since 500 doesn't fit in Thumb's CMP #imm8
- Important trap from prior batches still applies: for large byte offsets, writing the total offset directly can change Thumb addressing shape
- Important metric note still applies: in this repo, some standalone conversions improve explicit C coverage without changing `matched_functions`, so both objdiff match metrics and linker/unit coverage must be tracked together

## Main takeaways

1. **The repo builds cleanly and matches the USA ROM today** using a Dockerized `devkitpro/devkitarm` flow plus `pret/agbcc`.
2. **The latest verified checkpoint says 5.960588% matched code**, which is close to the user estimate, but that is **not the same thing as decompiled C coverage**.
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
