# `wariowareinc` decomp progress audit and smoke-test log

## Scope

This audit was done against:

- repo: `xXJSONDeruloXx/wariowareinc`
- branch: `docs/macabeus-tooling-assessment`
- ROM: `WarioWare, Inc. - Mega Microgame$! (USA).gba`
- verified SHA1: `3f556448d290fa5406d6ed367fee16cc02387ad3`

---

## 0. Latest verified checkpoint (2026-05-20)

A new standalone-asm conversion batch was verified successfully on `docs/macabeus-tooling-assessment`.

### Verification commands

```bash
docker run --rm -v "$PWD:/workspace" -w /workspace devkitpro/devkitarm:latest \
  bash -lc 'set -euo pipefail; make clean >/dev/null 2>&1; make -j4 2>&1 | tail -n 5'
```

Success signal:

- `wariowareinc.gba: OK`

```bash
docker run --rm -v "$PWD:/workspace" -w /workspace devkitpro/devkitarm:latest \
  bash -lc 'set -euo pipefail; make report 2>&1 | tail -n 5'
python3 tools/gen_objdiff.py
```

### Latest verified metrics

- `matched_functions`: **1036 / 5961**
- `matched_functions_percent`: **17.379635%**
- `matched_code_percent`: **5.866135%**
- `tools/gen_objdiff.py`: **589 C / 6098 asm-only units**
- previous verified baseline used by this checkpoint: **1031 / 5961**, **584 C / 6103 asm-only units**
- accepted delta for this batch: **+5 matched functions**, **+5 C units**, **-5 asm-only units**

### Files accepted in this batch

Verified C conversions kept:

- `src/decomp/asm_08053264.c`
- `src/decomp/asm_080c61d0.c`
- `src/decomp/asm_080d24a8.c`
- `src/decomp/asm_080d2890.c`
- `src/decomp/asm_080d7738.c`

Original asm files were moved to `asm/converted/`:

- `asm/converted/asm_08053264.s`
- `asm/converted/asm_080c61d0.s`
- `asm/converted/asm_080d24a8.s`
- `asm/converted/asm_080d2890.s`
- `asm/converted/asm_080d7738.s`

### What worked

- A sibling-rich family of shift-based byte setters converted cleanly and again moved both major metric families.
- All five functions shared the same core shape:

```c
void func_xxx(void) {
    u8 *p = (u8 *)gCurrentSceneVariable;
    p[(IMM << SHIFT)] = VALUE;
}
```

Examples from the accepted batch:

```c
void func_08053264(void) { u8 *p = (u8 *)gCurrentSceneVariable; p[(0xAA << 1)] = 5; }
void func_080C61D0(void) { u8 *p = (u8 *)gCurrentSceneVariable; p[(0x92 << 1)] = 0; }
void func_080D24A8(void) { u8 *p = (u8 *)gCurrentSceneVariable; p[(0xF7 << 2)] = 0; }
```

### Durable workflow lesson reinforced

Object-level preflight is paying off.

For this batch, compiling just the candidate objects before touching the linker confirmed:

- identical instruction sequences
- identical `.text` sizes

across all five siblings.

That let the whole batch be promoted at once with high confidence.

### Strategy update from this batch

This is now a clearly productive class of targets:

- short sibling families where the asm differs only by immediates
- especially when the same C spelling pattern can be copied across every sibling

These are currently better value than exploratory one-off functions.

### Prior traps carried forward

The earlier repair lessons still matter:

- direct `((u8 *)&gBeatscriptScene)[N]` expressions can compile as symbol-plus-offset literal relocations instead of base-plus-immediate accesses
- for large byte offsets, direct total-offset spelling can alter Thumb address splitting (`+0x26` / `[+0]` instead of `+8` / `[+0x1E]`)
- when a small conversion unexpectedly breaks the ROM, compare both disassembly and TU `.text` size before deciding it is “close enough”

## 1. Real build baseline

A real build baseline was established with Docker, following the repo’s CI shape rather than guessing at a host-native setup.

### Container path used

- image: `devkitpro/devkitarm:latest`
- repo-local `tools/agbcc` installed from `pret/agbcc`
- `baserom.gba` copied into repo root

### Result

The repo built successfully and produced a matching ROM:

- `wariowareinc.gba: OK`

That matters because every later assessment is grounded in a **known-good, reproducible, matching baseline**.

---

## 2. Progress numbers: what they mean and what they do not mean

## `make report` result

Running `make report` produced:

- `matched_code_percent`: **5.4086823%**
- `matched_functions`: **471**
- `total_functions`: **5961**
- `matched_functions_percent`: **7.901359%**

### Important interpretation

This is **not** a pure “how much source is now decompiled into C” number.

Why:

- this repo still contains many asm stubs included from C translation units
- matched code in objdiff-style reporting can still come from inline asm, not from newly written C

So `~5.41%` is a good **matched-code** number, but it should **not** be read as “5.41% of the game is already decompiled into handwritten C”.

---

## 3. Heuristic decomp estimate from current source layout

A rough repo-local heuristic after the smoke-test replacements:

- C function definitions found in `src/**/*.c`: **165**
- inline asm stub includes still present in `src/**/*.c`: **306**
- total functions from report: **5961**

### Heuristic ratio

- `165 / 5961 = 2.7680%`

### My read

That puts the repo’s current **explicit C-defined function coverage** closer to:

- **~2.77% by function count**

while the objdiff matched-code report sits around:

- **~5.41% by matched code bytes**

### Best practical summary

If someone says:

- “the repo is around 5% decomped”

my answer would be:

- **that is fair if they mean matched-code progress**
- **it is optimistic if they mean pure C replacement coverage**

---

## 4. Real asm→C smoke tests completed

Two real asm stubs were replaced with C in the repo, rebuilt, and verified against the matching baseline.

## Smoke test 1: `mem_heap_alloc`

### File

- `src/memory_heap.c`

### Original state

- inline asm include stub:
  - `#include "asm/memory_heap/asm_08006174.s"`

### Replacement

```c
void *func_08006184(u16 heapId, u32 size);

void *mem_heap_alloc(u32 size) {
    return func_08006184(0, size);
}
```

### Verification

- full Docker build still matched the ROM
- symbol disassembly matched the saved target object for the function body:

```asm
00000000 <mem_heap_alloc>:
   0:    b500        push    {lr}
   2:    1c01        adds    r1, r0, #0
   4:    2000        movs    r0, #0
   6:    f7ff fffe   bl      10 <func_08006184>
   a:    bc02        pop     {r1}
   c:    4708        bx      r1
```

## Smoke test 2: `func_0800A270`

### File

- `src/beatscript.c`

### Original state

- inline asm include stub:
  - `#include "asm/beatscript/asm_0800a270.s"`

### Replacement

```c
void write_save_main(void);

void func_0800A270(void) {
    write_save_main();
}
```

### Verification

- full Docker build still matched the ROM
- symbol disassembly matched the saved target object for the function body:

```asm
00000a9c <func_0800A270>:
 a9c:    b500        push    {lr}
 a9e:    f7ff fffe   bl      0 <write_save_main>
 aa2:    bc01        pop     {r0}
 aa4:    4700        bx      r0
```

---

## 5. Important finding about what does and does not move `make report`

After replacing the first two included asm stubs with matching C, `make report` stayed effectively unchanged.

That is not a contradiction.

It means the report is primarily capturing **matched binary/code status**, not “this used to be inline asm inside a C unit and is now a real C function”.

However, converting a **standalone asm-only leaf function** outside the existing C scaffold did move the official report slightly:

- `matched_code_percent`: `5.4082794%` -> `5.4086823%`
- `matched_functions`: `470` -> `471`
- `tools/gen_objdiff.py`: `18 C / 6669 asm-only stubs` -> `19 C / 6668 asm-only stubs`

That is a useful warning for future progress tracking:

- **binary match metrics** and **decomped-C metrics** are not the same metric in this repo
- but **standalone asm-only conversions do show up in the official report**

---

## 5.1 Standalone asm-only proof point: `func_080F26D0`

A tiny standalone asm file outside the pre-existing C translation units was converted successfully.

### Original state

- file: `asm/asm_080f26d0.s`
- body:

```asm
STRB R1, [R0, #1]
BX LR
```

### Replacement

- new file: `src/asm_080f26d0.c`

```c
#include "global.h"

void func_080F26D0(u8 *arg0, u8 arg1) {
    arg0[1] = arg1;
}
```

### Build-system changes needed

Because this function was not an inline include stub, replacing it required:

- filtering `asm/asm_080f26d0.s` out of `SFILES` in `Makefile`
- swapping the linker-script entry in `wariowareinc.ld` from:
  - `build/asm/asm_080f26d0.s.o(.text*);`
  - to `build/src/asm_080f26d0.c.o(.text*);`

### Result

- full Docker build still matched the ROM
- this did move the official report slightly

This is the clearest proof so far that **progress outside the currently-C-scaffolded portion of the repo is absolutely achievable**, just a bit more invasive per function.

---

## 6. Mizuchi smoke tests

## 6.1 Built and launched successfully

Completed:

- `mizuchi` install/build
- Decomp Atlas UI build
- Atlas server launch on `http://localhost:3000`

## 6.2 First problem found: inline asm wrapper format polluted prompts

Before adding normalization, Atlas prompt output for inline asm stubs contained C-string wrapper artifacts such as:

- escaped `\n\\`
- trailing `");`

That was a real repo-compatibility issue.

## 6.3 Fix applied: normalized asm export mirror

Added:

- `tools/mizuchi/export-asm.py`

This generates:

- `.mizuchi-asm/asm/...`

and converts inline wrapper files like:

```c
asm(".syntax unified \n\
thumb_func_start func_0800A038 \n\
...");
```

into clean raw asm text like:

```asm
.syntax unified

thumb_func_start func_0800A038
/* 0800A038 */ LDR R0, =gBeatscriptScene
/* 0800A03A */ LDRH R0, [R0, #0XC]
/* 0800A03C */ BX LR
```

After that change, Atlas prompts became substantially cleaner.

## 6.4 Indexing result

After normalization and re-indexing with:

- `mizuchi index-codebase --skip-embeddings`

result was:

- total indexed by Mizuchi DB: **679**
- matched: **164**
- unmatched: **515**

### Interpretation

This is useful, but it is **not** the same total as objdiff’s `5961` function count.

So today I would treat Mizuchi’s DB for this repo as:

- a useful working subset for prompting and browsing
- not yet a perfect whole-project accounting layer

## 6.5 Isolated compile smoke test result

The added helper path for isolated compilation is promising but not perfect yet.

For a tiny empty function candidate (`func_0800BC0C`), isolated compilation produced:

```asm
00000000 <func_0800BC0C>:
   0:    4770        bx      lr
   2:    46c0        nop
```

while the current target object shows:

```asm
00000490 <func_0800BC0C>:
 490:    4770        bx      lr
 492:    0000        .short  0x0000
```

Likewise, an isolated compile of the simple `func_0800A270` wrapper produced a trailing `nop` where the target object shows trailing `0x0000` padding.

### What that means

- the **Dockerized isolated compile path works**, in the sense that it produces near-target code and can assemble objects successfully
- but there is still a **tiny padding/alignment mismatch** at the function tail for some small functions

That makes Mizuchi **usable but not fully dialed in** for this repo today.

---

## 7. Shortlist of good next targets

Based on current stub size and likely ease, these are strong early candidates:

### Best first-tier candidates

- `src/graphics_table.c`
  - `func_08002468`
  - `func_080025F8`
  - `func_0800260C`
- `src/bitmap_font.c`
  - `func_0800BC0C`
  - `func_0800BF34`
- `src/beatscript.c`
  - `func_0800A038`
  - `func_0800A044`
  - `func_0800A050`
  - `func_0800A128`
  - `func_0800A218`
- `src/scenes/main_menu.c`
  - many tiny wrappers exist, but that file is crowded, so it is a slightly messier editing surface

### Why these are good

They are small enough that:

- you can hand-audit the asm quickly
- you can validate exact output rapidly
- they give fast confidence that the workflow is sound

---

## 8. Bottom line

### Proven today

- the repo builds and matches
- small asm→C replacements are easy wins here
- the user’s “~5%” intuition is reasonable for matched-code framing
- actual C replacement coverage is probably lower than that
- Mizuchi can be made meaningfully useful with repo-local glue
- Kappa is more useful as a manual/human VS Code companion than as a pi-native automation path

### My current practical estimate

- **matched-code progress:** ~5.40868%
- **explicit C-defined function progress (heuristic):** ~2.77%

That is the clearest honest summary I can give after real builds and real smoke tests.
