#!/bin/bash
set -euo pipefail

# Convert asm functions to C, build, and verify.
# If mismatch, binary search to find bad conversions.
# Usage: tools/convert_and_verify.sh

cd "$(git rev-parse --show-toplevel)"

# Build and check
check_match() {
    docker run --rm -v "$PWD:/workspace" -w /workspace devkitpro/devkitarm:latest \
        bash -lc 'set -euo pipefail; make clean >/dev/null 2>&1; make -j4 2>&1 | tail -n 1'
}

echo "Building..."
result=$(check_match)
if echo "$result" | grep -q "OK"; then
    echo "ROM matches!"
    exit 0
else
    echo "ROM does NOT match. Need binary search."
    exit 1
fi
