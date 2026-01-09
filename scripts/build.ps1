# Quick build script for ECLIPSE
param(
    [string]$BuildType = "release"
)

Write-Host "=== ECLIPSE Build Script ===" -ForegroundColor Cyan
Write-Host ""

# Check if vcpkg is set up
if (-not $env:VCPKG_ROOT) {
    Write-Host "⚠️  VCPKG_ROOT not set. Checking for local vcpkg..." -ForegroundColor Yellow
    if (Test-Path "vcpkg") {
        $env:VCPKG_ROOT = "$PWD\vcpkg"
        Write-Host "✓ Using local vcpkg at $env:VCPKG_ROOT" -ForegroundColor Green
    } else {
        Write-Host "❌ vcpkg not found. Please set VCPKG_ROOT or run:" -ForegroundColor Red
        Write-Host "   git clone https://github.com/Microsoft/vcpkg.git"
        Write-Host "   .\vcpkg\bootstrap-vcpkg.bat"
        Write-Host "   `$env:VCPKG_ROOT = `"`$PWD\vcpkg`""
        exit 1
    }
}

# Determine preset and build directory
switch ($BuildType.ToLower()) {
    "release" {
        $Preset = "desktop-release"
        $BuildDir = "build\release"
    }
    "debug" {
        $Preset = "desktop-debug"
        $BuildDir = "build\debug"
    }
    default {
        Write-Host "❌ Unknown build type: $BuildType" -ForegroundColor Red
        Write-Host "Usage: .\scripts\build.ps1 [release|debug]"
        exit 1
    }
}

Write-Host "Building: $Preset" -ForegroundColor Cyan
Write-Host ""

# Configure
Write-Host "=== Configuring ===" -ForegroundColor Yellow
cmake --preset $Preset

# Build
Write-Host ""
Write-Host "=== Building ===" -ForegroundColor Yellow
cmake --build $BuildDir --config Release -j $env:NUMBER_OF_PROCESSORS

# Success
Write-Host ""
Write-Host "================================" -ForegroundColor Green
Write-Host "✓ Build successful!" -ForegroundColor Green
Write-Host "================================" -ForegroundColor Green
Write-Host ""
Write-Host "Run the game:" -ForegroundColor Yellow
Write-Host "  .\$BuildDir\Release\eclipse_app.exe" -ForegroundColor White
Write-Host ""
Write-Host "Run tests:" -ForegroundColor Yellow
Write-Host "  cd $BuildDir; ctest --output-on-failure -C Release" -ForegroundColor White
Write-Host ""
