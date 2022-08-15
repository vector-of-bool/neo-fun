[CmdletBinding()]
param (
    # The toolchain to git to the bpt build
    [Parameter(Mandatory)]
    [string]
    $Toolchain
)

$ErrorActionPreference = "Stop"

$this_file = $MyInvocation.MyCommand.Source
$tools_dir = Split-Path -Parent $this_file
$root = Split-Path -Parent $tools_dir

$build_dir = Join-Path $root "_build"
$null = New-Item -ItemType Directory $build_dir -Force

$bpt_exe = Join-Path $build_dir "bpt.exe"
if (!(Test-Path $bpt_exe)) {
    Invoke-WebRequest -UseBasicParsing -Uri "https://github.com/vector-of-bool/bpt/releases/download/1.0.0-beta.1/bpt-win-x64.exe" `
        -OutFile $bpt_exe
}

& $bpt_exe build --project=$root --toolchain=$Toolchain --tweaks-dir=$root/conf

if ($LASTEXITCODE -ne 0) {
    throw "Execution of bpt build failed [$LASTEXITCODE]"
}
