# Mizuchi workflow for `wariowareinc`

This repo now contains a minimal Mizuchi bootstrap tailored to its current structure.

## Added files

- `mizuchi.yaml`
- `tools/mizuchi/export-asm.py`
- `tools/mizuchi/get-context.sh`
- `tools/mizuchi/compile-in-docker.sh`

## Why these files exist

`wariowareinc` is not a plain “raw asm files + raw C files” repo.

It has many asm stubs stored as wrapper files like:

```c
asm("...\n\\");
```

That breaks prompt quality and complicates automation unless you normalize it first.

The helper files solve three specific problems:

1. **normalize inline asm wrappers** for Atlas/indexing
2. **resolve function name -> including source file** for context generation
3. **compile snippets in Docker** instead of assuming a perfect native host toolchain

---

## 1. Export normalized asm mirror

Run from the repo root:

```bash
./tools/mizuchi/export-asm.py
```

This generates a sanitized mirror under:

- `.mizuchi-asm/asm`

The current `mizuchi.yaml` points `nonMatchingAsmFolders` there instead of at the raw `asm/` tree.

### Why this matters

Without this step, Atlas prompt generation for inline stubs contains escaped wrapper garbage.

With this step, Atlas sees clean raw asm.

---

## 2. Index the repo with Mizuchi

Assuming this repo is your current directory, either clone Mizuchi next to it (`../mizuchi`) or persist `MIZUCHI_ROOT` in `~/.zshrc` first:

```bash
echo 'export MIZUCHI_ROOT=/absolute/path/to/mizuchi' >> ~/.zshrc
source ~/.zshrc
cd "$MIZUCHI_ROOT"
npm start -- index-codebase --config /absolute/path/to/wariowareinc/mizuchi.yaml --skip-embeddings
```

### Notes

- `--skip-embeddings` was used in this pass to avoid pulling large Python model dependencies.
- This is enough to validate indexing and Atlas prompt generation.

---

## 3. Build Atlas UI and start Atlas server

From the Mizuchi repo:

```bash
cd "$MIZUCHI_ROOT"
npm run build:decomp-atlas
npm start -- atlas --config /absolute/path/to/wariowareinc/mizuchi.yaml
```

Observed working result:

- `Decomp Atlas server running at http://localhost:3000`

---

## 4. How context generation works here

`tools/mizuchi/get-context.sh` does this:

1. find the asm stub file containing the target function name
2. find the `src/*.c` file that includes that stub
3. collect only the top `#include` lines from that source file
4. preprocess them inside the devkitARM container
5. strip comments and obvious unwanted macro noise
6. print the context to stdout for Mizuchi

### Important limitation

This is designed for **functions still represented by included asm stubs**.

If you have already replaced a stub with C, the automatic stub discovery path will not find it anymore.

That is fine for the normal decomp workflow, because the function would no longer be an unmatched stub.

---

## 5. How isolated compilation works here

`tools/mizuchi/compile-in-docker.sh` does this:

1. copy the candidate preprocessed C into a repo-local temp folder
2. run `tools/agbcc/bin/agbcc` inside `devkitpro/devkitarm`
3. assemble with `arm-none-eabi-as`
4. copy the resulting object file back to the host path Mizuchi expects

### Why Docker is used here

This avoids relying on:

- host devkitPro layout
- host `DEVKITARM` rules
- host-native Linux binaries for `tools/agbcc/bin/agbcc`

The repo’s CI is already container-shaped, so this is the lowest-friction path.

---

## 6. Current known limitation

The isolated compile path is **close but not perfect**.

For very small functions, current output can differ from the target by tail padding, e.g.:

- compiled candidate ends with a trailing `nop`
- target object ends with a trailing `.short 0x0000`

So today this setup is best described as:

- **good enough to explore candidates and iterate**
- **not yet fully tuned for zero-friction automatic matching on every tiny wrapper**

---

## 7. Recommended operator workflow

## Fast path

```bash
cd /absolute/path/to/wariowareinc
./tools/mizuchi/export-asm.py

cd "$MIZUCHI_ROOT"
npm start -- index-codebase --config /absolute/path/to/wariowareinc/mizuchi.yaml --skip-embeddings
npm start -- atlas --config /absolute/path/to/wariowareinc/mizuchi.yaml
```

Then:

1. browse Atlas
2. pick a small stub
3. generate a prompt
4. draft candidate C
5. validate with either:
   - isolated compile/compare for quick iteration, or
   - full repo build for final confidence

## Final confidence path

For anything that looks promising, always finish with a full repo build:

```bash
make -j$(nproc)
```

or the repo’s Dockerized equivalent used in this assessment.

---

## 8. Suggested next improvements

If you want to push Mizuchi harder on this repo, these are the next best upgrades:

1. tune isolated compilation so tail padding matches target more often
2. add a function-specific prompt save workflow directly from Atlas for this repo
3. add a `mizuchi` integrator module that can replace inline asm includes automatically in a worktree
4. optionally add embeddings later once the prompt/build loop is stable enough to justify it

---

## 9. Bottom line

This bootstrap is already enough to make Mizuchi **meaningfully useful** for `wariowareinc` inside pi.

It is not “one command to full automation” yet, but it is absolutely far enough along to support:

- candidate selection
- prompt generation
- iterative decomp experiments
- repeatable repo-specific setup
