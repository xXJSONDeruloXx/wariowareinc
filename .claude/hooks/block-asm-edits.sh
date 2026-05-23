#!/usr/bin/env bash
set -euo pipefail

# Block direct edits to asm/ directory files (except asm/converted/ moves which
# are handled by apply_conversion). These files are auto-generated from the
# original ROM and must not be manually edited.

input=$(cat)
file_path=$(echo "$input" | jq -r '.tool_input.file_path // .file_path // ""')

# Block writes/edits to asm/ files (but allow asm/converted/ which is where
# moved stubs go, and allow reading asm/ for reference)
if [[ "$file_path" == *asm/* ]] && [[ "$file_path" != *asm/converted/* ]]; then
    echo "BLOCKED: Files in asm/ (except asm/converted/) are auto-generated from the original ROM." >&2
    echo "They must not be edited directly. Use apply_conversion to move stubs to asm/converted/." >&2
    exit 2
fi

exit 0
