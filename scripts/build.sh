#!/bin/bash
# Quick build script for ECLIPSE

set -e

echo "=== ECLIPSE Build Script ==="
echo ""

# Check if vcpkg is set up
if [ -z "$VCPKG_ROOT" ]; then
    echo "⚠️  VCPKG_ROOT not set. Checking for local vcpkg..."
    if [ -d "vcpkg" ]; then
        export VCPKG_ROOT="$PWD/vcpkg"
        echo "✓ Using local vcpkg at $VCPKG_ROOT"
    else
        echo "❌ vcpkg not found. Please set VCPKG_ROOT or run:"
        echo "   git clone https://github.com/Microsoft/vcpkg.git"
        echo "   ./vcpkg/bootstrap-vcpkg.sh"
        echo "   export VCPKG_ROOT=\"\$PWD/vcpkg\""
        exit 1
    fi
fi

# Determine build type (default: Release)
BUILD_TYPE="${1:-release}"

case "$BUILD_TYPE" in
    release|Release)
        PRESET="desktop-release"
        BUILD_DIR="build/release"
        ;;
    debug|Debug)
        PRESET="desktop-debug"
        BUILD_DIR="build/debug"
        ;;
    *)
        echo "❌ Unknown build type: $BUILD_TYPE"
        echo "Usage: $0 [release|debug]"
        exit 1
        ;;
esac

echo "Building: $PRESET"
echo ""

# Configure
echo "=== Configuring ==="
cmake --preset "$PRESET"

# Build
echo ""
echo "=== Building ==="
cmake --build "$BUILD_DIR" -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

# Success
echo ""
echo "================================"
echo "✓ Build successful!"
echo "================================"
echo ""
echo "Run the game:"
echo "  ./$BUILD_DIR/eclipse_app"
echo ""
echo "Run tests:"
echo "  cd $BUILD_DIR && ctest --output-on-failure"
echo ""
