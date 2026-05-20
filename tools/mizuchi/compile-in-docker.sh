#!/usr/bin/env bash
set -euo pipefail

if [ "$#" -lt 2 ]; then
  echo "Usage: $0 <c-file> <obj-file> [function-name]" >&2
  exit 2
fi

c_file="$1"
obj_file="$2"
function_name="${3:-candidate}"
repo_root="$(cd "$(dirname "$0")/../.." && pwd)"
local_tmp_dir="$repo_root/.mizuchi-tmp/${function_name}-$$"
container_tmp_dir="/workspace/.mizuchi-tmp/${function_name}-$$"

rm -rf "$local_tmp_dir"
mkdir -p "$local_tmp_dir" "$(dirname "$obj_file")"
cp "$c_file" "$local_tmp_dir/input.c"

cleanup() {
  rm -rf "$local_tmp_dir"
}
trap cleanup EXIT

docker run --rm \
  -v "$repo_root:/workspace" \
  -w /workspace \
  devkitpro/devkitarm:latest \
  bash -lc '
    set -euo pipefail
    export PATH="/opt/devkitpro/devkitARM/bin:/opt/devkitpro/tools/bin:$PATH"
    tmp_asm="$(mktemp /tmp/mizuchi-asm.XXXXXX.s)"
    trap '\''rm -f "$tmp_asm" "$tmp_asm.stripped"'\'' EXIT
    tools/agbcc/bin/agbcc "'"$container_tmp_dir"'/input.c" -o "$tmp_asm" \
      -mthumb-interwork -Wimplicit -Wparentheses -Werror -O2 -g -fhex-asm
    sed "/\\.size/d" "$tmp_asm" > "$tmp_asm.stripped"
    mv "$tmp_asm.stripped" "$tmp_asm"
    arm-none-eabi-as -mcpu=arm7tdmi -mthumb-interwork "$tmp_asm" -o "'"$container_tmp_dir"'/output.o"
  '

cp "$local_tmp_dir/output.o" "$obj_file"
