# Windows Tooling Notes — MSYS2 / Docker / Node.js path issues

This document records the Windows-specific tooling issues encountered while running the WarioWare decomp pipeline on a Windows machine with MSYS2 (Git for Windows Bash), and their fixes.

## Core issue: MSYS2 path translation

MSYS2 automatically translates between Unix-style paths (`/c/Users/kurt/...`) and Windows paths (`C:\Users\kurt\...`). This translation breaks Docker volume mounts because Docker (on WSL2/Desktop) does not understand the MSYS2-converted paths.

**Symptom:** `docker: Error response from daemon: the working directory 'C:/Program Files/Git/workspace' is invalid`

**Root cause:** When running `bash` from Node.js on Windows, MSYS2 intercepts POSIX-style paths passed as arguments and converts them. `/workspace` (a Docker volume mount path) was being translated to `C:/Program Files/Git/workspace` (inside Git's MSYS2 runtime). Docker rejects this as invalid.

## Fix 1: MSYS_NO_PATHCONV environment variable

Set `MSYS_NO_PATHCONV=1` in the environment when executing bash scripts from Node.js:

```js
// In runScript():
function runScript(script, args = [], opts = {}) {
  const env = { ...process.env };
  env.MSYS_NO_PATHCONV = "1";  // prevent Docker path mangling
  // ...
}
```

**Where this applies:**
- `compile_and_view_asm` tool (via `compile-in-docker.sh`)
- `get_function_context` tool (via `get-context.sh`)
- Any other `.sh` script execution through `runScript()`

**Effect:** MSYS2 stops translating any `/x/...` paths in arguments. Docker volume mounts work correctly.

## Fix 2: Forward-slash path conversion for shell scripts

When `resolveScript()` wraps a `.sh` file for bash execution, it must convert Windows backslash paths to forward-slash MSYS2 paths:

```js
// In resolveScript():
if (isWindows() && ext === ".sh") {
  const msysPath = unixPath
    .replace(/[\/\\]/g, "/")
    .replace(/^([A-Za-z]):\//, (m, drive) => "/" + drive.toLowerCase() + "/");
  return { bash: "bash", args: [msysPath] };
}
```

**Why:** `path.join()` on Windows produces `C:\Users\kurt\...` which MSYS2 bash cannot execute directly. The regex converts it to `/c/Users/kurt/...`.

**Note:** The drive letter must be lowercase for MSYS2 mount points.

## Fix 3: m2c Python binary detection on Windows

m2c runs via the local Python venv, not Docker. Node.js on Windows CAN execute native Windows paths directly — the MSYS2 path mangling only applies to `/c/...` Unix-style paths passed through bash.

```js
function findM2cPython(mizuchiRoot) {
  // Try Unix venv first (Linux/macOS)
  const unixVenv = path.join(mizuchiRoot, "vendor/m2c/.venv/bin/python3");
  if (fs.existsSync(unixVenv)) return unixVenv;

  // Windows venv (native path — works fine with execFileSync)
  const winVenv = path.join(mizuchiRoot, "vendor/m2c/.venv/Scripts/python.exe");
  if (fs.existsSync(winVenv)) return winVenv;

  return null;
}
```

**Key insight:** `execFileSync(mizuchiRoot + "\\vendor\\m2c\\.venv\\Scripts\\python.exe", [...])` works because Node.js passes Windows-native paths directly to `CreateProcess()` — no MSYS2 translation occurs.

**Docker fallback:** Not needed on Windows if `findM2cPython()` finds the venv. The Docker fallback was previously attempted but is unnecessary since the Windows venv Python executes fine via Node.js's native path handling.

## Fix 4: objdiff-cli health check on Windows

The executable bit check (`chmod`-style) doesn't work on Windows. Check for both the Unix script and `.exe` variant:

```js
check(
  "tools/objdiff-cli (executable)",
  fs.existsSync(path.join(repoRoot, "tools/objdiff-cli")) ||
    fs.existsSync(path.join(repoRoot, "tools/objdiff-cli.exe")),
  isWindows()
    ? "Download objdiff-cli from GitHub releases"
    : "chmod +x tools/objdiff-cli",
);
```

## Testing on Windows

Always verify tools end-to-end before committing:

```bash
cd /c/Users/kurt/wariowareinc
node -e "
const fs = require('fs');
const path = require('path');
const { execFileSync } = require('child_process');

// 1. Test compile-in-docker.sh
const script = require('./.pi/extensions/warioware-decomp-tools.js');
const result = execFileSync('bash', [
  path.resolve('./tools/mizuchi/compile-in-docker.sh'),
  '/tmp/test.c', '/tmp/out.o', 'test_func'
], { env: {...process.env, MSYS_NO_PATHCONV: '1'}, cwd: process.cwd(), stdio: 'pipe' });
console.log('compile:', result.status === 0 ? 'OK' : 'FAIL');

// 2. Test m2c
const m2cPython = 'C:\\\\Users\\\\kurt\\\\mizuchi\\\\vendor\\\\m2c\\\\.venv\\\\Scripts\\\\python.exe';
const output = execFileSync(m2cPython, [
  path.join(m2cRoot, 'vendor/m2c/m2c.py'), '-t', 'gba', '/tmp/test.s'
], { cwd: process.cwd(), stdio: 'pipe' });
console.log('m2c:', output.status === 0 ? 'OK' : 'FAIL');
"
```

## Troubleshooting

### Docker volume mount errors
If Docker complains about invalid working directories:
1. Check if `MSYS_NO_PATHCONV=1` is set in the execution environment
2. Verify the bash wrapper passes forward-slash paths
3. Test manually: `MSYS_NO_PATHCONV=1 docker run --rm -v /c/Users/kurt/repo:/workspace --workdir /workspace devkitpro/devkitarm:latest pwd`

### m2c fails with "can't open file"
The Python venv path is being mangled by MSYS2. Either:
- Use the Windows-native `.exe` path (Node.js handles it natively)
- Or run m2c via Docker: `docker run --rm -v /c/Users/kurt/mizuchi:/m2c python:3.11-slim python3 /m2c/vendor/m2c/m2c.py ...`

### Scripts hang or error on paths
Set `MSYS_NO_PATHCONV=1` and use forward-slash paths. Test with a minimal script first.

## Summary table

| Tool / Script | Issue | Fix |
|---------------|-------|-----|
| `compile-in-docker.sh` | Docker path mangling | `MSYS_NO_PATHCONV=1` + forward-slash paths |
| `get-context.sh` | Same Docker issue | `MSYS_NO_PATHCONV=1` + forward-slash paths |
| `m2c_decompile` | Python path not found | `findM2cPython()` detects `.venv/Scripts/python.exe` |
| `objdiff-cli` check | No executable bit on Windows | Check for `.exe` variant |
| `resolveScript()` | Backslash paths in bash | Convert `\` → `/`, `C:` → `/c` |
