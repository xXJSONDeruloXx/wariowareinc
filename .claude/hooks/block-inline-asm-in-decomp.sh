#!/usr/bin/env bash
set -euo pipefail

# Block inline asm in src/decomp/*.c files (the core anti-cheese rule).
# Decompile means writing real C code, not embedding assembly strings.
#
# Allowed exceptions (these patterns are NOT blocked):
#   - __attribute__((naked)) functions — the entire body IS asm by design
#   - GBA BIOS SVC wrappers (asm volatile("svc #N")) — hardware interface
#   - asm volatile("" ...) barrier/nop hints — compiler shaping, not logic
#
# Blocked patterns in non-naked decomp functions:
#   - asm volatile("bl ...")  — must use real C function calls
#   - asm volatile("ldr/str/...")  — must use real C pointer access
#   - asm volatile("sub sp/add sp") — must use real C local variables
#   - asm volatile("mov/bx/pop/push") — must use real C control flow
#   - Any asm volatile containing ARM/Thumb instruction mnemonics

input=$(cat)

file_path=$(echo "$input" | jq -r '.tool_input.file_path // .file_path // ""')

# Only check src/decomp/*.c files
if [[ "$file_path" != *src/decomp/*.c ]]; then
    exit 0
fi

# Read the file content from the tool input
content=$(echo "$input" | jq -r '.tool_input.content // .content // ""')

# If no content (e.g. a Read operation), nothing to block
if [[ -z "$content" ]]; then
    exit 0
fi

# If the file uses __attribute__((naked)), skip all checks — naked functions
# are allowed to contain inline asm as their entire body
if echo "$content" | grep -q '__attribute__((naked))'; then
    exit 0
fi

# Check for blocked inline asm patterns
# We look for asm volatile strings that contain actual ARM/Thumb instructions
# (not just empty barrier strings)

# Extract all asm volatile(...) blocks and check them
# Blocked instruction mnemonics inside asm volatile
BLOCKED_MNEMONICS="bl|bx|ldr|str|strh|strb|ldrh|ldrb|push|pop|add sp|sub sp|mov r|adds r|subs r|cmp|bne|beq|bge|blt|ble|bhi|blo|bpl|bmi|b |svc|swi|stm|ldm|lsls r|lsrs r|asrs r|ands r|orrs r|eors r|bics r|rsbs r|negs r|muls r|mla"

# Find lines with asm volatile that contain instruction-like content
bad_lines=$(echo "$content" | grep -n 'asm volatile(' | grep -ivE 'asm volatile\(\s*""' | grep -ivE '"$' || true)

if [[ -z "$bad_lines" ]]; then
    exit 0
fi

# Check each asm volatile line for blocked mnemonics
while IFS= read -r line; do
    # Skip barrier-only asm volatiles (empty string or just clobber hints)
    if echo "$line" | grep -qE 'asm volatile\(\s*""\s*[:)]'; then
        continue
    fi
    
    # Check if this line contains any blocked instruction mnemonic
    if echo "$line" | grep -qiE "($BLOCKED_MNEMONICS)"; then
        linenum=$(echo "$line" | cut -d: -f1)
        echo "BLOCKED: Inline asm containing ARM/Thumb instructions is not allowed in src/decomp/ files." >&2
        echo "" >&2
        echo "File: $file_path (line $linenum)" >&2
        echo "Line: $line" >&2
        echo "" >&2
        echo "Decompilation means writing REAL C CODE that the compiler translates to matching assembly." >&2
        echo "Embedding assembly instructions in C is not decompilation — it's just hiding the asm." >&2
        echo "" >&2
        echo "Instead:" >&2
        echo "  - Use real C function calls instead of asm volatile(\"bl ...\")" >&2
        echo "  - Use real C pointer dereference instead of asm volatile(\"ldr/str...\")" >&2
        echo "  - Use real C local variables instead of asm volatile(\"sub sp/add sp...\")" >&2
        echo "  - Use real C if/switch/goto instead of asm volatile(\"cmp/bne/beq...\")" >&2
        echo "  - Use __attribute__((naked)) if the function truly cannot be expressed in C" >&2
        echo "" >&2
        echo "See docs/decomp-pattern-library.md for legitimate C shaping techniques." >&2
        exit 2
    fi
done <<< "$bad_lines"

exit 0
