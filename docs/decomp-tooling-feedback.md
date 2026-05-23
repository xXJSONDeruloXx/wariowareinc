# Decomp tooling feedback log

Use this file to record where the current decomp tools helped, where they missed integration risk, and what manual workaround was needed. The goal is to improve future automation without reducing current verification rigor or tool scope.

## Batch 109 — linked included-stub mini-batch
- Tools that helped:
  - `compile_and_view_asm` correctly proved isolated instruction matches for `func_08014E88` and `func_080152A0`.
  - `apply_conversion` safely performed the include-shim edits, asm moves, clean Docker builds, and ROM identity checks for each function.
- Tooling/workflow gap:
  - Chunk 29 initially misdiagnosed several candidates as callee-risk because the worktree had an unrelated manual edit in `src/decomp/asm_08012c64.c` from a failed `func_08014374` experiment. `apply_conversion` restored its own attempted edits, but it cannot know whether unrelated pre-existing manual edits are accidental.
  - `query_candidates` and `preflight_candidate` are useful but do not express linked-batch opportunities or dirty-worktree risk.
- Manual workaround:
  - Run `git status --short` before and after failed `apply_conversion` attempts.
  - Revert unrelated edits before retrying a 100% isolated match.
  - For linked included stubs, apply the callee first and verify ROM, then apply the caller.
- Desired tooling improvement:
  - Add a pre-apply clean-worktree warning or at least list unrelated dirty files before running the Docker build.
  - Add a candidate-family mode that suggests tiny callee-before-caller batches instead of only one-function candidates.

## Batch 133 — real-C-first / naked-asm follow-up
- Tools that helped:
  - `compile_and_view_asm` made it obvious when a candidate was only failing on register allocation or epilogue shape, which is exactly the point where a real-C pass is worth trying.
  - `make report` + `tools/gen_objdiff.py` confirmed the post-chunk baseline after the accepted `func_08001D5C` included-stub conversion.
- Tooling/workflow gap:
  - The loop prompt previously did not explicitly reserve time for revisiting existing naked asm files with a real-C attempt after the primary chunk work was done.
  - Some candidates that look like simple C rewrites still collapse into return-shape or scratch-register problems, so the prompt needs to steer agents toward "real C first, naked asm last" instead of treating naked asm as a default fallback.
- Manual workaround:
  - I updated `docs/decomp-agent-workflow.md`, `docs/decomp-pattern-library.md`, and the decomp-loop prompt to make the real-C-first rule explicit and to reserve spare capacity for revisting existing naked asm files.
- Desired tooling improvement:
  - Surface a lightweight reminder in chunk prompts when the candidate queue is thin: try one real-C conversion pass on an existing naked file before reaching for a new naked file or another manual asm wrapper.
