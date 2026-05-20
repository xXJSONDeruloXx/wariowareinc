# Macabeus tooling assessment for `wariowareinc`

## Executive summary

If the goal is **practical decomp throughput inside the pi coding agent harness**, the best path is:

1. keep the repo buildable through a **Dockerized devkitARM + agbcc** workflow
2. use **Mizuchi** for:
   - indexing
   - prompt generation / Atlas browsing
   - function-by-function automation
3. treat **Kappa** as:
   - a strong source of ideas
   - a useful manual VS Code extension for a human operator
   - **not** the main automation layer in pi

In short:

- **Mizuchi fits pi well enough to be worth pushing further.**
- **Kappa fits a human-in-VS-Code workflow better than an agent-in-pi workflow.**

---

## What Mizuchi gives you

From the repo and real smoke tests, Mizuchi is trying to solve the right problem for this project:

- automated asm→C iteration
- compile → compare → retry loop
- project indexing
- prompt generation
- atlas-style browsing of candidates
- optional future integration into the real decomp repo

### Why Mizuchi is a good fit here

Because Mizuchi is fundamentally a **CLI/server pipeline**, pi can work with it directly:

- read/edit config files
- run the CLI
- inspect outputs
- run the atlas server in the background
- script repo-specific setup around it

That is exactly the shape of tooling that works well in this harness.

---

## What Kappa gives you

Kappa is good tooling, but it is shaped around **VS Code extension ergonomics**:

- code lenses above asm stubs
- prompt builder commands
- webviews
- Copilot Agent mode
- extension settings
- in-editor workflows

### Why Kappa is less natural in pi

Inside pi, there is **no native “run this VS Code extension command directly” tool surface**.

What pi can do well:

- inspect Kappa source code
- reuse its ideas
- use GUI automation against VS Code if absolutely necessary

What pi cannot do cleanly:

- act like a first-class VS Code extension host API client
- invoke Kappa commands as a stable programmable interface
- use GitHub Copilot Agent mode as if it were a local library

So Kappa is not useless here — far from it — but it is **not the happy path**.

---

## Accessibility matrix in the pi harness

### Mizuchi

| Capability | In pi? | Notes |
|---|---:|---|
| CLI commands | Yes | Natural fit |
| index-codebase | Yes | Works here |
| Decomp Atlas server | Yes | Works here |
| background server process | Yes | Good fit with `process` tool |
| config customization | Yes | Good fit |
| prompt generation | Yes, with repo-specific prep | Needed inline-asm normalization first |
| compile/compare loop | Partly | Works conceptually, but isolated compilation still needs tuning |
| Claude-runner | Depends on credentials | Config is present, but live LLM automation was not relied on in this pass |
| integrator/worktree flow | Likely yes | Not exercised yet |

### Kappa

| Capability | In pi? | Notes |
|---|---:|---|
| read extension source | Yes | Very useful |
| reuse prompt/indexing ideas | Yes | Good reference |
| run VS Code commands directly | No clean API | Only possible indirectly via GUI |
| code lenses / webviews | Not first-class | Human/manual, not agent-native |
| Copilot `#objdiff` tool flow | No | That is VS Code Copilot-specific |
| Agent Mode button flow | Not practical as automation | Possible only via brittle GUI driving |
| decomp.me one-click creation | Not practical as automation | Could be replicated outside Kappa instead |

---

## Repo-specific findings

## 1. `wariowareinc` is not a “clean raw asm only” decomp layout

This repo mixes:

- raw standalone asm translation units
- many inline asm stub files included from C via:
  - `#include "asm/.../asm_xxxxxxxx.s"`
  - where those `.s` files are actually `asm("...")` string wrappers

That matters a lot.

### Why it matters

Mizuchi and Kappa both assume fairly normal decomp layouts more often than not.

For this repo, the inline-stub format creates friction:

- prompt builders can ingest ugly escaped asm text
- indexers may parse only partially
- context generation must resolve **stub file → including C file**

This is why `tools/mizuchi/export-asm.py` was added.

It generates a sanitized mirror under:

- `.mizuchi-asm/asm/...`

so Mizuchi sees raw-looking assembly instead of `asm("...\n\\")` wrapper text.

## 2. Kappa’s current object-file lookup is a bad match for this repo

Kappa’s matched-function lookup logic searches for object files by module basename, effectively expecting patterns like:

- `build/.../main_menu.o`

But this repo produces objects like:

- `build/src/scenes/main_menu.c.o`

That makes Kappa’s current object resolution logic a poor fit here without adaptation.

This is one of the strongest repo-specific reasons to prefer Mizuchi over Kappa for automation work in this codebase.

## 3. Docker is the right build abstraction here

Host-side setup was incomplete for a normal native flow:

- no preinstalled devkitPro rules on host
- `tools/agbcc` is a repo-local requirement
- the repo’s own CI already uses `devkitpro/devkitarm`

So the stable route is:

- keep Node/Mizuchi on the host
- do GBA compilation in Docker

That is exactly how `tools/mizuchi/compile-in-docker.sh` is set up.

---

## What was proven in real testing

## Mizuchi-side wins

### Worked

- built `mizuchi`
- built Decomp Atlas UI
- started Atlas server successfully on `http://localhost:3000`
- indexed this repo with `mizuchi index-codebase --skip-embeddings`
- improved prompt quality by normalizing inline asm wrappers first

### Needed repo-specific adaptation

- asm export / normalization layer
- Dockerized compiler wrapper
- repo-specific context resolver

### Still rough

The isolated Mizuchi-style compile path is **close**, but not fully solved yet.

For tiny wrapper functions, isolated compilation produced a trailing padding difference like:

- compiled: trailing `nop`
- target: trailing `.short 0x0000`

That means the pipeline is not “fire and forget” yet for this repo.

It is still promising, but it needs a little more repo-specific compile/output tuning.

## Kappa-side wins

### Worked as assessment/reference

- extension structure is clear
- prompt builder logic is useful
- indexer structure is useful
- decomp workflow ideas are reusable

### Did not become the recommended pi path

Because the strongest features are bound to:

- VS Code UI
- Copilot chat / Agent mode
- code lenses / extension commands

That is a bad control surface for pi compared to Mizuchi’s CLI/server model.

---

## Recommended happy path

## Phase 1: baseline and candidate selection

1. Build the repo in Docker and confirm the ROM matches.
2. Export normalized asm for Mizuchi:
   - `./tools/mizuchi/export-asm.py`
3. Re-index with Mizuchi:
   - `cd ../mizuchi && npm start -- index-codebase --config /path/to/wariowareinc/mizuchi.yaml --skip-embeddings`
4. Use Atlas or direct file inspection to choose targets.

## Phase 2: target strategy

Prioritize functions in this order:

1. **tiny included asm stubs** in already-C translation units
2. small wrappers and getters/setters
3. small leaf functions with obvious call patterns
4. only then larger logic-heavy scene functions

Good families in this repo:

- `src/graphics_table.c`
- `src/bitmap_font.c`
- `src/beatscript.c`
- selected tiny `src/scenes/main_menu.c` stubs

## Phase 3: actual decomp loop

1. generate prompt/context for a chosen stub
2. draft candidate C
3. full-build or isolated-compile compare
4. integrate when matching
5. keep the main ROM match green after every win

## Phase 4: optional human+VS Code assist

If a human wants richer in-editor help:

- use Kappa in VS Code manually
- use Atlas/Mizuchi in parallel for batch prompt generation and scoring

That is a good **hybrid** workflow.

---

## Bottom line

If I had to choose one primary path for `wariowareinc` **inside pi**, it would be:

- **Mizuchi + Dockerized compile wrappers + repo-specific normalization/context helpers**

If I had to choose one supporting/manual path for a human collaborator, it would be:

- **Kappa inside VS Code**

That combination gives the best balance of:

- automation friendliness
- reproducibility
- decomp iteration speed
- compatibility with this repo’s current structure
