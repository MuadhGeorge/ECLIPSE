# ECLIPSE - Quick Start to Play NOW!

## Current Status
✅ CMake - INSTALLED  
✅ vcpkg - INSTALLED  
✅ All Code - WRITTEN  
❌ C++ Compiler - **NEEDED**

## Fastest Way to Play (Choose ONE)

### Option 1: Visual Studio 2022 (Recommended - Most Reliable)

**Install:**
```powershell
winget install Microsoft.VisualStudio.2022.Community
```

During installation:
- Select **"Desktop development with C++"** workload
- Click Install (takes ~10 minutes)

**Then Build from Developer PowerShell:**
1. Press Windows key, type "Developer PowerShell for VS 2022"
2. Run:
```powershell
cd "C:\Users\muadh\Music\CS 8803\ECLIPSE"
cmake -B build -DCMAKE_TOOLCHAIN_FILE="vcpkg/scripts/buildsystems/vcpkg.cmake"
cmake --build build --config Release
.\build\Release\eclipse_app.exe
```

### Option 2: w64devkit (Faster Install - Portable)

**Install:**
1. Download: https://github.com/skeeto/w64devkit/releases/latest
2. Download `w64devkit-x.x.x.zip`
3. Extract to `C:\w64devkit`
4. Run `C:\w64devkit\w64devkit.exe`

**In w64devkit terminal:**
```bash
cd /c/Users/muadh/Music/CS\ 8803/ECLIPSE
cmake -B build -G "MinGW Makefiles" -DCMAKE_TOOLCHAIN_FILE="vcpkg/scripts/buildsystems/vcpkg.cmake"
cmake --build build
./build/eclipse_app.exe
```

### Option 3: MSYS2 (Package Manager Approach)

**Install:**
```powershell
winget install MSYS2.MSYS2
```

**Then in MSYS2 MINGW64 terminal:**
```bash
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake make
cd /c/Users/muadh/Music/CS\ 8803/ECLIPSE
cmake -B build -G "MinGW Makefiles" -DCMAKE_TOOLCHAIN_FILE="vcpkg/scripts/buildsystems/vcpkg.cmake"
cmake --build build
./build/eclipse_app.exe
```

## What Happens When You Build

1. **First time** (15-20 minutes):
   - vcpkg downloads and compiles raylib
   - vcpkg downloads and compiles SQLite3
   - vcpkg downloads and compiles Catch2
   - Your game code compiles

2. **After that** (2-3 minutes):
   - Only your game code recompiles

## Game Controls (Once Running)

**Main Menu:**
- Click "Play Today's Puzzle" to start

**During Game:**
- **Left Click** on cells to cycle: Empty → Sun (S) → Moon (M) → Empty
- **Ctrl+Z** - Undo
- **Ctrl+Y** - Redo  
- **H** - Get hint
- **Hint Buttons** on right panel:
  - Hint 1: Highlights a forced cell
  - Hint 2: Applies a forced move
  - Hint 3: Reveals a cell

**Goal:**
- Fill 6×6 grid with Suns and Moons
- Equal number of each per row/column
- No three same symbols adjacent
- Each colored region needs specific number of Suns (shown in top-left of region)

## Quick Troubleshooting

**"cmake is not recognized" after install:**
- Close and reopen PowerShell/terminal
- Or restart computer

**Build errors about missing compiler:**
- Make sure you installed the C++ workload (Visual Studio)
- Or extracted w64devkit properly
- Or ran from correct MSYS2 terminal (MINGW64, not MSYS2)

**"vcpkg install failed":**
- Already fixed! vcpkg.json was updated

## Alternative: Play in Browser (No Install)

If you want to skip local installation:

1. Install Emscripten SDK (for web builds)
2. Build for web
3. Play in browser

But local build is recommended for best experience!

---

**Ready to play?** Pick an option above and you'll be solving ECLIPSE puzzles in about 15-30 minutes (including compiler install + first build)! 🌒☀️
