# ECLIPSE - Setup Guide for Windows

## Prerequisites Installation

### 1. Install CMake

Download and install CMake from the official website:
- **URL**: https://cmake.org/download/
- **Recommended**: cmake-3.28.1-windows-x86_64.msi (or latest)
- **During installation**: Check "Add CMake to system PATH"

Or use winget (Windows Package Manager):
```powershell
winget install Kitware.CMake
```

Or use Chocolatey:
```powershell
choco install cmake
```

### 2. Install Visual Studio Build Tools

You need a C++20 compiler. Install Visual Studio 2022 (Community Edition is free):
- **URL**: https://visualstudio.microsoft.com/downloads/
- **Workload**: "Desktop development with C++"
- **Components**: Make sure C++20 support is included

Or install just the Build Tools:
```powershell
winget install Microsoft.VisualStudio.2022.BuildTools
```

### 3. vcpkg (Already Done!)

✅ vcpkg is already cloned and bootstrapped in the project directory!

## Building ECLIPSE

Once CMake is installed, run these commands:

### Step 1: Configure the Project
```powershell
cd "C:\Users\muadh\Music\CS 8803\ECLIPSE"
cmake -B build -DCMAKE_TOOLCHAIN_FILE="vcpkg/scripts/buildsystems/vcpkg.cmake"
```

### Step 2: Build the Project
```powershell
cmake --build build --config Release
```

This will:
- Download and compile raylib, SQLite3, and Catch2 via vcpkg
- Compile all source files
- Create the executable: `build\Release\eclipse_app.exe`

### Step 3: Run the Game
```powershell
.\build\Release\eclipse_app.exe
```

### Step 4: Run Tests
```powershell
cd build
ctest -C Release --output-on-failure
```

## Quick Start (After Prerequisites)

```powershell
# One-command build
cmake -B build -DCMAKE_TOOLCHAIN_FILE="vcpkg/scripts/buildsystems/vcpkg.cmake" && cmake --build build --config Release

# Run the game
.\build\Release\eclipse_app.exe
```

## Troubleshooting

### "cmake is not recognized"
- CMake is not in your PATH. Restart PowerShell/terminal after installation, or add CMake to PATH manually.

### vcpkg dependency installation takes long
- First build will take 10-20 minutes as vcpkg compiles all dependencies from source.
- Subsequent builds will be much faster (cached).

### Compiler errors
- Ensure Visual Studio 2022 is installed with C++20 support.
- Try running from "Developer PowerShell for VS 2022" or "x64 Native Tools Command Prompt".

### Missing DLLs when running
- The executable should be self-contained. If issues occur, run from the build directory.

## Development Workflow

### Edit and Rebuild
```powershell
# After making code changes
cmake --build build --config Release

# Or for faster debug builds
cmake --build build --config Debug
```

### Clean Build
```powershell
Remove-Item build -Recurse -Force
cmake -B build -DCMAKE_TOOLCHAIN_FILE="vcpkg/scripts/buildsystems/vcpkg.cmake"
cmake --build build --config Release
```

### Run Tests After Changes
```powershell
cd build
ctest -C Release --output-on-failure
```

## Project Structure Summary

```
ECLIPSE/
├── vcpkg/                 # Package manager (already set up!)
├── build/                 # Build output (created by cmake)
├── src/
│   ├── core/             # Game logic (complete)
│   │   ├── grid.h/cpp
│   │   ├── constraints.h/cpp
│   │   ├── solver.h/cpp
│   │   ├── generator.h/cpp
│   │   ├── region.h/cpp
│   │   └── daily_seed.h/cpp
│   └── app/              # UI (complete)
│       ├── main.cpp
│       ├── ui.h/cpp
│       ├── game_state.h/cpp
│       └── persistence.h/cpp
├── tests/                # Unit tests (complete)
│   ├── test_solver.cpp
│   ├── test_generator.cpp
│   └── test_constraints.cpp
├── assets/               # Game assets
├── CMakeLists.txt        # Build configuration
├── vcpkg.json           # Dependencies manifest
├── README.md            # Full documentation
└── SETUP.md             # This file
```

## What's Been Implemented

✅ **Complete Core Engine**
- Grid and cell management
- Constraint system (row/col balance, no three adjacent, regions, clues)
- Constraint solver with propagation and backtracking
- Puzzle generator with uniqueness verification
- Daily deterministic seed system

✅ **Complete UI**
- raylib-based desktop interface
- Main menu, gameplay, stats, completion screens
- Click-to-cycle cell interaction
- Undo/redo support
- Three-tier hint system
- Timer and statistics tracking

✅ **Persistence**
- SQLite database for stats and progress
- Streak tracking
- Daily puzzle progress saving

✅ **Testing**
- Comprehensive Catch2 unit tests
- Solver correctness tests
- Generator uniqueness tests
- Constraint validation tests

✅ **CI/CD**
- GitHub Actions workflows
- Multi-platform builds (Windows/Linux/macOS)

## Next Steps

1. **Install CMake** (see above)
2. **Build the project** (one cmake command)
3. **Play ECLIPSE!** 🌒☀️

Enjoy your daily puzzle!
