
if (-not (Test-Path .\bin)) {
    md .\bin | Out-Null
}

Push-Location .\bin

clang-cl /Zi `
    /std:c99 `
    /I ..\include `
    /o main.exe `
    ..\src\main.c

Pop-Location