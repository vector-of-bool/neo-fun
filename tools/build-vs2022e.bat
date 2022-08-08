@echo off

rem Load the VS environment
echo "Loading VS 2022 enterprise..."
call "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\Common7\Tools\vsdevcmd" -arch=x64 || exit 1

rem Invoke PowerShell for a scripting environment
echo "Running build:"
pwsh -NoLogo -NonInteractive -NoProfile -File "%~dp0/build.ps1" %*
