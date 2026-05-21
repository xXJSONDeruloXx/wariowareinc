# `wariowareinc` decomp progress audit and smoke-test log

> Historical archive: this file captures the earlier audit / smoke-test phase.
> For the live autonomous workflow, current metrics, pattern library, and batch history, start at `docs/README.md` and `docs/wariowareinc-decomp-scaleup.md`.


## Scope

This audit was done against:

- repo: `xXJSONDeruloXx/wariowareinc`
- branch: `docs/macabeus-tooling-assessment`
- ROM: `WarioWare, Inc. - Mega Microgame$! (USA).gba`
- verified SHA1: `3f556448d290fa5406d6ed367fee16cc02387ad3`

---

## 0. Latest verified checkpoint (2026-05-20)

A small-body function batch (dec-counter, store-advance, struct-field-arithmetic, mul-acc) was verified successfully, following a batch of byte-extract/combine/struct-init functions.

### Latest verified metrics

- `matched_functions`: **1121 / 5961** = **18.805569%**
- `matched_code_percent`: **5.986366%**
- `tools/gen_objdiff.py`: **669 C / 6018 asm-only units**
- previous verified baseline: **1111 / 5961**, **659 C / 6028 asm-only units**
- accepted delta: **+10 matched functions** (two 5-function batches), **+10 C units**, **-10 asm-only units**

### Files accepted

- `src/decomp/asm_080c4794.c` (byte-extract: LSLS#16, LSRS#24)
- `src/decomp/asm_080f2c44.c` (byte-combine: LDRB, LSLS#8, LDRB, ORRS)
- `src/decomp/asm_080cd708.c` (dual-zero-word: MOVS#0, STR offset 0x28, STR offset 0x2C)
- `src/decomp/asm_080f2780.c` (struct init: byte[6], word[8], byte[7] zeroed)
- `src/decomp/asm_08002fb4.c` (struct init: word[0], byte[5], byte[4] zeroed)
- `src/decomp/asm_080039b4.c` (dec-counter: LDR, SUBS#1, STR, LDRB)
- `src/decomp/asm_0800397c.c` (store-advance: LDR, STRB, ADDS#1, STR)
- `src/decomp/asm_08062f00.c` (field-arithmetic: LDR, ADDS#0x40, STR, LDR, ADDS, STR)
- `src/decomp/asm_080f2774.c` (struct init: MOVS R2=0, MOVS R1=1, STRB word[6]=1, STR word[8]=0, STRB byte[7]=0)
- `src/decomp/asm_080bb344.c` (mul-acc: LDR, MULS, ASRS, ADDS, STR)

### What worked

Self-contained small-body functions (3-7 instructions, no extern references) remain the highest-yield target class. Key C spellings validated:
- `*counter -= 1; return *(u8 *)(*counter)` for dec-pointer patterns
- `u8 *p = *pp; *p = val; *pp = p + 1` for store-advance (avoids reload from memory)
- `fields[N] += imm; fields[M] += fields[N]` for field arithmetic
- `(s32)val * (s32)mul) >> 8` for MULS + ASRS signed multiply-shift
- `u32 zero = 0;` shared variable avoids redundant MOVS between mixed-width zero stores

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
- raw-pointer entry setters of the form `((u8 *)*(void **)((u8 *)arg0 + 0x18))[arg1 * 0x20 + off] = value;` can match cleanly across sibling families
- raw-pointer entry setters with `*(u16 *)&... = arg2 << 8;` can also match cleanly for halfword fields in the same family
- pair-add helpers of the form `u32 *arg0; arg0[1] += arg0[a]; arg0[2] += arg0[b];` can match cleanly across sibling families with different source indices
- tiny sound wrappers can match cleanly with absolute-address spellings like `stop_sound((struct SongHeader *)0x083FF348);` and `func_0800C7CC((void *)0x083FDB88);` when those ROM data symbols are not exported as normal C symbols
- `if (D_03006520 == IMM) func_target();` wrappers can match cleanly across sibling families when `D_03006520` comes from `src/beatscript.h`
- `LDRH`-based compare-and-call wrappers around `D_03006520` continue to be a strong sibling family when the compare immediate and callee are the only differences
- two-call `D_03006520` guards of the form `if (D_03006520 == IMM) { func1(); func2(); }` can match cleanly
- large-immediate `D_03006520` guards like `if (D_03006520 == 500) func();` can match cleanly since agbcc correctly emits the `MOVS R0, #0xFA; LSLS R0, #1; CMP R1, R0` sequence for Thumb immediates exceeding #imm8 range
- **Bitfield ops via shift-pair DO NOT match AND masks**: `LSLS R0,R0,#20; LSRS R0,R0,#20` (extract bits 0-3) compiles differently from `AND #0xF` — the shift-pair is what agbcc emits for bitfield extraction, not the AND form
- **`~N` compiles as immediate, not as `MOVS + RSBS`**: `*p &= ~2` compiles to `MOVS R1, #253` (one instruction) instead of `MOVS R1, #2; RSBS R1, R1, #0` (two instructions) — these are NOT codegen-equivalent
- **`-1` via `MOVS #1; RSBS` vs literal pool**: `MOVS R2,#1; RSBS R2,R2,#0; ADDS R0,R2,#0` (3 instructions, no literal) differs from `LDR R2,[pc,#8]=0xFFFF; ADDS R0,R2,#0` (2 instructions + literal pool) — different code size and layout
- Simple `(*p)--` and triple-store `STR R1,[R0,#0xC]; STR R2,[R0,#0x10]; STR R3,[R0,#0x14]` patterns DO match cleanly
- preflighting candidate C spellings as object files before linker edits is effective for short standalone TU batches

## Current Known Traps / Non-Matching Patterns
- grouping multiple functions in one C file breaks ROM matching
- leaving converted `.s` files in place causes wildcard / linker collisions
- bare `extern` declarations for g-symbols can conflict with existing types
- `POP {R1}; BX R1` wrappers are not safe under agbcc’s normal output
- `((u8 *)&gBeatscriptScene)[N]` may compile as a **symbol+offset literal relocation** instead of loading the base symbol and using `[base, #N]`
- even when function bytes look close, TU-level padding/alignment can still break the final ROM; compare `.text` section sizes when a full-ROM mismatch survives seemingly matched code
- raw `.text` bytes from relocatable C-vs-asm object preflights can differ for extern-backed address loads because the C object may keep an `R_ARM_ABS32` relocation where the hand asm already bakes the literal; use instruction flow, `.text` size, and final linked-ROM verification as the real gate
- for large byte offsets, writing the full offset directly can change Thumb address splitting (`+0x26` / `[+0]` instead of `+8` / `[+0x1E]`); shape the pointer expression to preserve the original immediate split

## Next Candidate Queue
1. explore more complex `D_03006520` wrappers that load `gCurrentSceneVariable` or `gGraphicsBuffer` before the BL call (e.g., `asm_0801f2a0`, `asm_08022b28`, `asm_08024450`, `asm_08021ab0`) — these need careful C spelling to match codegen
2. continue mining BX LR empty stubs and simple return-constant functions as filler
3. continue mining sibling-rich one-call wrapper families, especially `scene_set_current_thread` groups that differ only by store offset/value or object-field offset
4. continue mining short `gCurrentSceneVariable` sibling families where one validated spelling can fan out to multiple siblings
5. mine short `LDR global; LDR/LDRB/LDRH` getters and bitfield extracts that already have matched siblings

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
- **Iteration 10**
  - Candidate set: a 5-function raw-pointer struct-entry setter family — `asm_080f25d8`, `asm_080f25e4`, `asm_080f25f0`, `asm_080f25fc`, `asm_080f26b0`
  - Result: **match**
  - Metric delta vs previous verified baseline: `1046 -> 1051 matched functions` (**+5**), `matched_code_percent 5.890302% -> 5.901177%`, linker/unit coverage `599 C / 6088 asm-only -> 604 C / 6083 asm-only`
  - Verification: clean Docker build returned `wariowareinc.gba: OK`; `make report` refreshed `build/report.json`; `python3 tools/gen_objdiff.py` reported `604 C / 6083 asm-only units`
  - Learnings: raw pointer arithmetic can be safer than guessed named structs for decomping isolated setters; the family matched cleanly with `((u8 *)*(void **)((u8 *)arg0 + 0x18))[arg1 * 0x20 + off] = ...` and the halfword variant `*(u16 *)&... = arg2 << 8` after object-preflight confirmed byte-for-byte-equivalent codegen
  - Next action: commit code + docs together, push immediately, then keep mining sibling-rich families that reuse the same raw-pointer or one-call-wrapper spellings
- **Iteration 11**
  - Candidate set: a 5-function pair-add helper family — `asm_0804ef64`, `asm_08089668`, `asm_080b3328`, `asm_080c950c`, `asm_080cd710`
  - Result: **match**
  - Metric delta vs previous verified baseline: `1051 -> 1056 matched functions` (**+5**), `matched_code_percent 5.901177% -> 5.906414%`, linker/unit coverage `604 C / 6083 asm-only -> 609 C / 6078 asm-only`
  - Verification: clean Docker build returned `wariowareinc.gba: OK`; `make report` refreshed `build/report.json`; `python3 tools/gen_objdiff.py` reported `609 C / 6078 asm-only units`
  - Learnings: simple arithmetic helper families can be excellent targets when their asm differs only by source-slot offsets; the spelling `u32 *arg0; arg0[1] += arg0[a]; arg0[2] += arg0[b];` matched cleanly across all five preflighted siblings
  - Next action: commit code + docs together, push immediately, then keep mining sibling-rich arithmetic and wrapper families before resorting to more speculative one-offs
- **Iteration 12**
  - Candidate set: a 5-function tiny sound-wrapper batch — `asm_080c6940`, `asm_080d8e8c`, `asm_080d8e9c`, `asm_080eac04`, `asm_080eac14`
  - Result: **match** after switching from undefined named `D_083...` symbols to absolute-address spellings
  - Metric delta vs previous verified baseline: `1056 -> 1061 matched functions` (**+5**), `matched_code_percent 5.906414% -> 5.915476%`, linker/unit coverage `609 C / 6078 asm-only -> 614 C / 6073 asm-only`
  - Verification: clean Docker build returned `wariowareinc.gba: OK`; `make report` refreshed `build/report.json`; `python3 tools/gen_objdiff.py` reported `614 C / 6073 asm-only units`
  - Learnings: when ROM-resident `D_083...` song data is not exported as a normal C symbol, direct absolute-address casts like `(struct SongHeader *)0x083FF348` and `(void *)0x083FDB88` can preserve the exact literal-pool behavior and still match cleanly after object preflight
  - Next action: commit code + docs together, push immediately, then continue mining small sibling-rich wrapper and helper families

- **Iteration 13**
  - Candidate set: a 5-function `D_03006520` compare-and-call wrapper family — `asm_08016a9c`, `asm_08016b34`, `asm_0801badc`, `asm_0801c368`, `asm_0801cfd8`
  - Result: **match**
  - Metric delta vs previous verified baseline: `1061 -> 1071 matched functions` (**+10**), `matched_code_percent 5.915476% -> 5.935615%`, linker/unit coverage `614 C / 6073 asm-only -> 619 C / 6068 asm-only`
  - Verification: clean Docker build returned `wariowareinc.gba: OK`; `make report` refreshed `build/report.json`; `python3 tools/gen_objdiff.py` reported `619 C / 6068 asm-only units`
  - Accepted commit: `0acf010e` (`feat: add D_03006520 wrapper batch`), pushed to `origin/docs/macabeus-tooling-assessment`
  - Learnings: small state-guard wrappers of the form `if (D_03006520 == IMM) func_target();` are a productive sibling family; object preflight still worked here, but the source-built objects kept an `R_ARM_ABS32` relocation for `D_03006520` while the original asm already contained the fixed literal, so instruction flow + `.text` size + final linked-ROM verification were the right acceptance checks
  - Next action: continue the remaining `D_03006520` compare-wrapper siblings before pivoting back to lower-yield one-offs

- **Iteration 14**
  - Candidate set: a 5-function `D_03006520` compare-and-call wrapper family — `asm_0801d2f0`, `asm_0801eca0`, `asm_08020968`, `asm_08022090`, `asm_08022938`
  - Result: **match**
  - Metric delta vs previous verified baseline: `1071 -> 1076 matched functions` (**+5**), `matched_code_percent 5.935615% -> 5.9476986%`, linker/unit coverage `619 C / 6068 asm-only -> 624 C / 6063 asm-only`
  - Verification: clean Docker build returned `wariowareinc.gba: OK`; `make report` refreshed `build/report.json`; `python3 tools/gen_objdiff.py` reported `624 C / 6063 asm-only units`
  - Accepted commits: `d4aaeb5f` (`feat: add D_03006520 wrapper siblings`) and `82fa1497` (`chore: move D_03006520 asm files`), pushed to `origin/docs/macabeus-tooling-assessment`
  - Learnings: the same `LDRH D_03006520; CMP #imm; BL func` spelling remains reusable across more siblings when only the compare constant and callee change, so the family is still a strong target class
  - Next action: continue mining the remaining `D_03006520` siblings and then pivot to the next best wrapper/helper family

- **Iteration 15**
  - Candidate set: a 5-function `D_03006520` wrapper batch — three single-BL wrappers (`asm_0802295c`, `asm_08022980`, `asm_08024208`) plus one large-immediate variant (`asm_080233b8`, CMP #0x1F4 via shift) and one two-call variant (`asm_08021748`)
  - Result: **match**
  - Metric delta vs previous verified baseline: `1076 -> 1081 matched functions` (**+5**), `matched_code_percent 5.9476986% -> 5.960588%`, linker/unit coverage `624 C / 6063 asm-only -> 629 C / 6058 asm-only`
  - Verification: clean Docker build returned `wariowareinc.gba: OK`; `make report` refreshed `build/report.json`; `python3 tools/gen_objdiff.py` reported `629 C / 6058 asm-only units`
  - Learnings: the `D_03006520` family extends to (a) two-call guards `if (D_03006520 == IMM) { func1(); func2(); }` and (b) large-immediate guards `if (D_03006520 == 500) func();` — agbcc correctly emits `MOVS R0, #0xFA; LSLS R0, #1; CMP R1, R0` for the Thumb-encoding case
  - Next action: continue the two-call `D_03006520` guard family (`asm_08021338`, `asm_08021540`) and then explore more complex `D_03006520` wrappers that load gCurrentSceneVariable or gGraphicsBuffer before the BL call
  - Accepted commit: `2ec447f7` (`feat: add D_03006520 wrapper batch (two-call + large-immediate variants)`), pushed to `origin/docs/macabeus-tooling-assessment`

- **Iteration 16**
  - Candidate set: a mixed 5-function batch — two remaining two-call `D_03006520` guards (`asm_08021338`, `asm_08021540`) plus three BX LR empty stubs (`asm_08016f58`, `asm_0801749c`, `asm_080179dc`)
  - Result: **match**
  - Metric delta vs previous verified baseline: `1081 -> 1086 matched functions` (**+5**), `matched_code_percent 5.960588% -> 5.9668307%`, linker/unit coverage `629 C / 6058 asm-only -> 634 C / 6053 asm-only`
  - Verification: clean Docker build returned `wariowareinc.gba: OK`; `make report` refreshed `build/report.json`; `python3 tools/gen_objdiff.py` reported `634 C / 6053 asm-only units`
  - Learnings: the two-call `D_03006520` guard family is now fully exhausted (all 3 siblings matched); mixing two proven families in one batch works fine
  - Accepted commit: `d4483c91` (`feat: add D_03006520 two-call guards and BX LR stubs`), pushed to `origin/docs/macabeus-tooling-assessment`
  - Next action: explore more complex `D_03006520` wrappers that load gCurrentSceneVariable or gGraphicsBuffer, and mine more BX LR / return-constant filler

- **Iteration 17**
  - Candidate set: 5 BX LR empty stubs — `asm_08008130`, `asm_0801684c`, `asm_08016f5c`, `asm_080174a0`, `asm_080179e0`
  - Result: **match**
  - Metric delta vs previous verified baseline: `1086 -> 1091 matched functions` (**+5**), `matched_code_percent 5.9668307% -> 5.967838%`, linker/unit coverage `634 C / 6053 asm-only -> 639 C / 6048 asm-only`
  - Verification: clean Docker build returned `wariowareinc.gba: OK`; `make report` refreshed `build/report.json`; `python3 tools/gen_objdiff.py` reported `639 C / 6048 asm-only units`
  - Next action: explore more complex `D_03006520` wrappers or pivot to bitfield/return-constant families

- **Iteration 18**
  - Candidate set: 5 BX LR empty stubs — `asm_08017910`, `asm_0801792c`, `asm_08017a2c`, `asm_08017a54`, `asm_080180c8`
  - Result: **match**
  - Metric delta vs previous verified baseline: `1091 -> 1096 matched functions` (**+5**), `639 C / 6048 asm-only -> 644 C / 6043 asm-only`
  - Accepted commit: `b38bf7d3`, pushed

### Reflection (after iteration 18)

- **Velocity is good** but BX LR stubs have diminishing returns per matched_function (they add to C unit count but each stub is tiny, so `matched_code_percent` barely moves)
- **Pivot needed**: The BX LR stubs are easy but don't move `matched_code_percent` meaningfully. The remaining ~20+ BX LR stubs should still be picked up opportunistically, but the main focus should shift to functions with actual body logic that improve `matched_code_percent`
- **Promising next families**: The simple non-BL BX LR functions (`asm_08003d1c` bitfield-clear, `asm_08006410` dec-and-store, `asm_08006734` bitfield-extract, `asm_0800d75c` triple-store) have small bodies and should improve `matched_code_percent` more than empty stubs
- **D_03006520 complex wrappers** with gCurrentSceneVariable loads (`asm_0801f2a0`, `asm_08022b28`, `asm_08024450`) are the next frontier — they have real body logic but need careful C spelling to match codegen
- **Running total**: 15 iterations × 5 functions = ~75 functions decompiled since iteration 12 baseline of 1061, now at 1096. Need 4768 target → still 3672 to go
