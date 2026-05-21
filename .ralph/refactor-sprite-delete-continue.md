Continue decompilation by selecting the next batch from the queue (item 1: more conditional byte-check + BL wrappers). Converted one conditional byte-check + BL wrapper: asm_0800200c. Build succeed. Next iteration: continue with more wrappers.

## Progress Summary

### Completed
- Fixed linker section discard issue for `func_08002038_c` by adding entry to `wariowareinc.ld`
- Added `build/src/decomp/asm_08002038_c.c.o(.text*)` to linker script
- Committed: `f2fc857b` - "fix: keep func_08002038_c section from being discarded"
- Build succeeds with 864 C decompiled functions (up from 854)
- ROM builds successfully

### Current Status
- 864 C functions decompiled
- 5824 asm-only stubs
- Next batch: More conditional byte-check + BL wrappers

## Current Issues Resolving
- `wariowareinc.ld` file appears to not contain expected build/src/decomp entries after previous edits
- Need to verify linker script has proper entries and commit changes