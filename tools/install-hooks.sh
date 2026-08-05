#!/usr/bin/env bash
set -euo pipefail

repo_root="$(cd "$(dirname "$0")/.." && pwd)"
cd "$repo_root"
git config core.hooksPath .githooks
echo "Installed WarioWare hooks at .githooks (core.hooksPath=$(git config --get core.hooksPath))"
