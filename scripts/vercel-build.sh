#!/bin/bash
# Vercel build script for ECLIPSE web version
set -e

echo "=== ECLIPSE Vercel Build ==="
echo ""

# Pin Emscripten version for reproducibility
EMSDK_VERSION="3.1.51"

echo "Step 1: Installing Emscripten SDK (version $EMSDK_VERSION)..."
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install $EMSDK_VERSION
./emsdk activate $EMSDK_VERSION
source ./emsdk_env.sh
cd ..

echo ""
echo "Step 2: Verifying Emscripten installation..."
emcc --version

echo ""
echo "Step 3: Building web target..."
emcmake cmake -B build-web -DCMAKE_BUILD_TYPE=Release
cmake --build build-web -j$(nproc)

echo ""
echo "Step 4: Preparing dist/ directory..."
mkdir -p dist

# Copy the main HTML file as index.html
if [ -f build-web/eclipse_game_web.html ]; then
    cp build-web/eclipse_game_web.html dist/index.html
    echo "  ✓ Copied eclipse_game_web.html -> dist/index.html"
elif [ -f build-web/eclipse_web.html ]; then
    cp build-web/eclipse_web.html dist/index.html
    echo "  ✓ Copied eclipse_web.html -> dist/index.html"
else
    echo "  ✗ ERROR: No HTML file found in build-web/"
    ls -la build-web/
    exit 1
fi

# Copy JavaScript file
if [ -f build-web/eclipse_game_web.js ]; then
    cp build-web/eclipse_game_web.js dist/
    echo "  ✓ Copied eclipse_game_web.js"
elif [ -f build-web/eclipse_web.js ]; then
    cp build-web/eclipse_web.js dist/
    echo "  ✓ Copied eclipse_web.js"
fi

# Copy WebAssembly file
if [ -f build-web/eclipse_game_web.wasm ]; then
    cp build-web/eclipse_game_web.wasm dist/
    echo "  ✓ Copied eclipse_game_web.wasm"
elif [ -f build-web/eclipse_web.wasm ]; then
    cp build-web/eclipse_web.wasm dist/
    echo "  ✓ Copied eclipse_web.wasm"
fi

# Copy data file if it exists
if [ -f build-web/eclipse_game_web.data ]; then
    cp build-web/eclipse_game_web.data dist/
    echo "  ✓ Copied eclipse_game_web.data"
elif [ -f build-web/eclipse_web.data ]; then
    cp build-web/eclipse_web.data dist/
    echo "  ✓ Copied eclipse_web.data"
fi

# Copy memory file if it exists
if [ -f build-web/eclipse_game_web.mem ]; then
    cp build-web/eclipse_game_web.mem dist/
    echo "  ✓ Copied eclipse_game_web.mem"
fi

# Copy assets directory if it exists
if [ -d assets ]; then
    cp -r assets dist/
    echo "  ✓ Copied assets/"
fi

echo ""
echo "Step 5: Verifying dist/ contents..."
echo "=== dist/ directory ==="
ls -la dist/

echo ""
echo "=== Checking index.html exists ==="
if [ -f dist/index.html ]; then
    echo "✓ index.html found"
    head -5 dist/index.html
else
    echo "✗ index.html NOT found - build failed!"
    exit 1
fi

echo ""
echo "=== Build Complete ==="
echo "dist/ is ready for Vercel deployment"
