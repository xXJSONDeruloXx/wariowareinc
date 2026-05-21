#!/usr/bin/env bash
set -euo pipefail

if [ "$#" -ne 1 ]; then
  echo "Usage: $0 <function-name>" >&2
  exit 2
fi

function_name="$1"
repo_root="$(cd "$(dirname "$0")/../.." && pwd)"
cd "$repo_root"

# Search for asm file containing the function — fall back from rg to grep
asm_path=""
if command -v rg &>/dev/null; then
  asm_path="$({
    rg -l --glob '*.s' "thumb_func_start[[:space:]]+${function_name}\\b" asm || true
    rg -l --glob '*.s' "arm_func_start[[:space:]]+${function_name}\\b" asm || true
    rg -l --glob '*.s' "^glabel[[:space:]]+${function_name}\\b" asm || true
  } | head -n 1)"
else
  # Fallback: grep + find (slower but no dependency)
  while IFS= read -r f; do
    if grep -qE "thumb_func_start[[:space:]]+${function_name}\\b" "$f" 2>/dev/null; then
      asm_path="$f"
      break
    fi
  done < <(find asm -name '*.s' 2>/dev/null)
fi

if [ -z "$asm_path" ]; then
  echo "Could not find asm stub for function: $function_name" >&2
  exit 1
fi

# Find source file that includes this asm stub
source_file=""
if command -v rg &>/dev/null; then
  source_file="$(rg -l -F "#include \"${asm_path}\"" src | head -n 1 || true)"
else
  while IFS= read -r f; do
    if grep -qF "#include \"${asm_path}\"" "$f" 2>/dev/null; then
      source_file="$f"
      break
    fi
  done < <(find src -name '*.c' -o -name '*.h' 2>/dev/null)
fi

if [ -z "$source_file" ]; then
  echo "Could not find source file including stub: $asm_path" >&2
  echo "Tip: If this is a standalone TU (not #included from C), this script won't work." >&2
  echo "Try reading the asm file directly or checking include/ for shared headers." >&2
  exit 1
fi

tmp_dir="$repo_root/.mizuchi-tmp/context-$function_name-$$"
mkdir -p "$tmp_dir"
trap 'rm -rf "$tmp_dir"' EXIT

grep '^#include ' "$source_file" > "$tmp_dir/includes.c"
if [ ! -s "$tmp_dir/includes.c" ]; then
  echo "No includes found in source file: $source_file" >&2
  exit 1
fi

docker run --rm \
  -v "$repo_root:/workspace" \
  -w /workspace \
  devkitpro/devkitarm:latest \
  bash -lc '
    set -euo pipefail
    gcc -E -P -dD -CC \
      -I tools/agbcc \
      -I tools/agbcc/include \
      -I . \
      -I src \
      -iquote include \
      -nostdinc -undef \
      .mizuchi-tmp/context-'"$function_name"'-'"$$"'/includes.c \
    | grep -vE "^#define (__STDC__|__STDC_VERSION__|__STDC_HOSTED__|__STDC_UTF_16__|__STDC_UTF_32__)" \
    | grep -v "asm(\"" \
    | perl -0pe '\''s@/\*.*?\*/@@gs; s@//.*$@@mg'\''
  '
