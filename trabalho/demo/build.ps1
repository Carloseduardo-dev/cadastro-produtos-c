$ErrorActionPreference = "Stop"
$sdkEnvironment = Join-Path $env:LOCALAPPDATA "emsdk\emsdk_env.ps1"

if (-not (Test-Path $sdkEnvironment)) {
    throw "Emscripten não encontrado. Instale e ative o SDK antes de compilar."
}

$env:EMSDK_QUIET = "1"
& $sdkEnvironment
if (-not (Get-Command emcc -ErrorAction SilentlyContinue)) {
    throw "O comando emcc não ficou disponível após ativar o SDK."
}

$projectRoot = Split-Path -Parent $PSScriptRoot
$wasmDirectory = Join-Path $PSScriptRoot "wasm"
New-Item -ItemType Directory -Path $wasmDirectory -Force | Out-Null
Push-Location $PSScriptRoot
try {
    emcc -std=c11 -O2 -Wall -Wextra -Wpedantic `
        -I (Join-Path $projectRoot "include") `
        (Join-Path $projectRoot "src\demo_api.c") `
        (Join-Path $projectRoot "src\arquivo_produtos.c") `
        -sMODULARIZE=1 `
        -sEXPORT_NAME=ProdutoDemo `
        -sEXPORT_ES6=1 `
        -sENVIRONMENT=web `
        -sFILESYSTEM=1 `
        -sALLOW_MEMORY_GROWTH=1 `
        -sEXPORTED_FUNCTIONS="['_demo_adicionar','_demo_listar_json','_demo_buscar_json','_demo_calcular_total','_demo_atualizar_quantidade']" `
        -sEXPORTED_RUNTIME_METHODS="['ccall']" `
        --no-entry `
        -o (Join-Path $wasmDirectory "produtos.js")
    if ($LASTEXITCODE -ne 0) {
        throw "A compilação WebAssembly falhou."
    }
}
finally {
    Pop-Location
}