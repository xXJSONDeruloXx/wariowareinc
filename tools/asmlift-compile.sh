#!/usr/bin/env bash
set -euo pipefail

if [ "$#" -lt 2 ]; then
  echo "Usage: $0 <c-file> <obj-file> [function-name]" >&2
  exit 2
fi

input_file="$1"
output_file="$2"
function_name="${3:-candidate}"
repo_root="$(cd "$(dirname "$0")/.." && pwd)"
wrapped_file="$(mktemp "${TMPDIR:-/tmp}/warioware-asmlift.XXXXXX.c")"

cleanup() {
  rm -f "$wrapped_file"
}
trap cleanup EXIT

# asmlift emits a self-contained candidate, but the project compiler needs the
# same declarations that normal decompilation units get. The adapter supplies
# a comma-separated list through ASMLIFT_HEADERS so callers can select the
# relevant module context without changing the repository-wide compiler setup.
header_list="${ASMLIFT_HEADERS:-global.h}"
IFS=',' read -r -a headers <<< "$header_list"
for header in "${headers[@]}"; do
  header="${header#${header%%[![:space:]]*}}"
  header="${header%${header##*[![:space:]]}}"
  if [ -n "$header" ]; then
    printf '#include "%s"\n' "$header" >> "$wrapped_file"
  fi
done
cat "$input_file" >> "$wrapped_file"

"$repo_root/tools/mizuchi/compile-in-docker.sh" \
  "$wrapped_file" "$output_file" "$function_name"
