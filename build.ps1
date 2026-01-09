# ECLIPSE Build Script
Write-Host "=== ECLIPSE Game Build Script ===" -ForegroundColor Cyan
Write-Host ""

# Find VS installation
$vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"

if (Test-Path $vswhere) {
    $vsPath = & $vswhere -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
    
    if ($vsPath) {
        Write-Host "Found Visual Studio at: $vsPath" -ForegroundColor Green
        
        $vcvarsall = Join-Path $vsPath "VC\Auxiliary\Build\vcvarsall.bat"
        
        if (Test-Path $vcvarsall) {
            Write-Host "Setting up Visual Studio environment..." -ForegroundColor Green
            
            if (Test-Path "build") {
                Remove-Item build -Recurse -Force
            }
            
            $buildCmd = "call `"$vcvarsall`" x64`r`ncd /d `"$PWD`"`r`ncmake -B build -DCMAKE_TOOLCHAIN_FILE=`"vcpkg\scripts\buildsystems\vcpkg.cmake`"`r`ncmake --build build --config Release"
            
            $buildCmd | Out-File -FilePath "temp_build.bat" -Encoding ASCII
            
            Write-Host ""
            Write-Host "=== Starting Build (15-20 minutes first time) ===" -ForegroundColor Cyan
            Write-Host ""
            
            cmd /c temp_build.bat
            
            Remove-Item "temp_build.bat" -ErrorAction SilentlyContinue
            
            if (Test-Path "build\Release\eclipse_app.exe") {
                Write-Host ""
                Write-Host "BUILD SUCCESSFUL!" -ForegroundColor Green  
                Write-Host ""
                Write-Host "To play: .\build\Release\eclipse_app.exe" -ForegroundColor Yellow
            }
        }
    } else {
        Write-Host "Visual Studio C++ tools not found" -ForegroundColor Red
        Write-Host "Install with: winget install Microsoft.VisualStudio.2022.Community" -ForegroundColor Yellow
    }
} else {
    Write-Host "vswhere.exe not found" -ForegroundColor Red
}
