@echo off
setlocal
set MSYS_NO_PATHCONV=1
set "SCRIPT_DIR=%~dp0"
bash "%SCRIPT_DIR%compile-in-docker.bash" %*
