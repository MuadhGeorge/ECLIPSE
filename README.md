# ECLIPSE 🌒☀️

[![CI](https://github.com/MuadhGeorge/ECLIPSE/actions/workflows/ci.yml/badge.svg)](https://github.com/MuadhGeorge/ECLIPSE/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://isocpp.org/)

A production-quality daily logic puzzle game built with modern C++20 and raylib. Solve unique constraint-based puzzles featuring Suns and Moons with guaranteed unique solutions.

![ECLIPSE Gameplay](assets/gameplay.gif)

##  Try It Now

**[🎮 Play Web Demo](https://muadhgeorge.github.io/ECLIPSE/)**

## 🎮 Game Rules

Fill a 6×6 grid (or 8×8 in hard mode) with Suns (☀️) and Moons (🌙) following these constraints:

1. **Balance**: Each row and column must contain equal numbers of Suns and Moons
2. **No Three Adjacent**: Maximum two identical symbols can be orthogonally adjacent
3. **Region Constraint**: Colored regions must contain exactly K Suns (K shown in top-left)
4. **Relationship Clues**: Optional `=` (same) or `≠` (different) between adjacent cells

### Example Puzzle

```
┌───────────────────────────┐
│ [2] S   M  =  _   _   _  │  Region 1: needs 2 Suns
│  M  ≠  S   M   S   M   S  │
│  S   S   M   _  [3] _  M  │  Region 2: needs 3 Suns
│  M   M   S   S   M   S   _│
└───────────────────────────┘
```

## ✨ Features

- 📅 **Daily Puzzles** - Same puzzle worldwide via deterministic seeding
- 🎯 **Difficulty Levels** - Easy/Medium (6×6), Hard (8×8)
- 💡 **Smart Hints** - Highlight → Apply → Reveal
- ⏪ **Undo/Redo** - Unlimited history
- 🔥 **Streak Tracking** - Monitor consecutive days
- 📊 **Statistics** - Average time, hints used, completion rate
- 🌐 **Cross-Platform** - Windows, macOS, Linux, Web

## 🚀 Quick Start

### Prerequisites

- **C++20 Compiler**: MSVC 2019+, GCC 10+, or Clang 12+
- **CMake**: 3.25 or higher
- **Ninja**: (recommended, or your preferred generator)
- **vcpkg**: For dependency management

### 1. Install Dependencies

#### Windows

```powershell
# Install tools
winget install Kitware.CMake
winget install Ninja-build.Ninja
winget install Microsoft.VisualStudio.2022.Community

# During VS install, select "Desktop development with C++"
```

#### macOS

```bash
brew install cmake ninja
# Xcode Command Line Tools will provide the compiler
xcode-select --install
```

#### Linux (Ubuntu/Debian)

```bash
sudo apt update
sudo apt install build-essential cmake ninja-build git
```

### 2. Set Up vcpkg

```bash
# Clone vcpkg (one-time setup)
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg

# Bootstrap vcpkg
./bootstrap-vcpkg.sh  # Linux/macOS
# OR
.\bootstrap-vcpkg.bat  # Windows

# Set environment variable
export VCPKG_ROOT="$(pwd)"  # Linux/macOS
# OR
$env:VCPKG_ROOT = "$PWD"  # Windows PowerShell

cd ..
```

### 3. Clone and Build

```bash
# Clone the repository
git clone https://github.com/MuadhGeorge/ECLIPSE.git
cd ECLIPSE

# Configure (vcpkg will auto-install raylib, sqlite3, catch2)
cmake --preset desktop-release

# Build
cmake --build --preset desktop-release

# Run the game!
./build/desktop-release/eclipse_game  # Linux/macOS
.\build\desktop-release\Release\eclipse_game.exe  # Windows
```

## 🏗️ Building

### Desktop Builds

#### Release Build (Optimized)

```bash
cmake --preset desktop-release
cmake --build --preset desktop-release
```

#### Debug Build (With Symbols)

```bash
cmake --preset desktop-debug
cmake --build --preset desktop-debug
```

### Run the Game

```bash
# Linux/macOS
./build/desktop-release/eclipse_game

# Windows
.\build\desktop-release\Release\eclipse_game.exe
```

### Run Tests

```bash
# Using CTest
cd build/desktop-release
ctest --output-on-failure

# Or run test executable directly
./eclipse_tests  # Linux/macOS
.\Release\eclipse_tests.exe  # Windows
```

### Web Build (Emscripten)

```bash
# Install Emscripten SDK (one-time setup)
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh  # Linux/macOS
# OR
.\emsdk_env.bat  # Windows
cd ..

# Set EMSDK environment variable
export EMSDK="$(pwd)/emsdk"  # Linux/macOS
# OR
$env:EMSDK = "$PWD\emsdk"  # Windows

# Build for web
cd ECLIPSE
cmake --preset web-release
cmake --build --preset web-release

# Serve locally
cd build/web-release
python3 -m http.server 8000
# Open browser to: http://localhost:8000/eclipse_game_web.html
```

## 🎮 Controls

### Keyboard Shortcuts

- **Left Click**: Cycle cell (Empty → Sun → Moon)
- **Ctrl+Z**: Undo
- **Ctrl+Y**: Redo
- **H**: Show hint
- **Esc**: Menu

### Hint System

1. **Level 1**: Highlights a forced cell (yellow)
2. **Level 2**: Applies a forced move automatically
3. **Level 3**: Reveals a cell from the solution

## 🔧 Troubleshooting

### CMake Can't Find vcpkg

**Problem**: `CMAKE_TOOLCHAIN_FILE not found`

```bash
# Solution: Ensure VCPKG_ROOT is set
echo $VCPKG_ROOT  # Should show vcpkg directory

# If not set:
export VCPKG_ROOT="/path/to/vcpkg"  # Linux/macOS
$env:VCPKG_ROOT = "C:\path\to\vcpkg"  # Windows
```

### vcpkg Install Fails

**Problem**: Dependencies fail to install

```bash
# Solution: Update vcpkg
cd $VCPKG_ROOT
git pull
./bootstrap-vcpkg.sh  # or .bat on Windows

# Clear CMake cache
cd /path/to/ECLIPSE
rm -rf build
cmake --preset desktop-release
```

### Compiler Not Found

**Problem**: `No C++ compiler could be found`

```bash
# Windows: Install Visual Studio with C++ workload
winget install Microsoft.VisualStudio.2022.Community

# macOS: Install Xcode tools
xcode-select --install

# Linux: Install build tools
sudo apt install build-essential
```

### CMake Version Too Old

**Problem**: `CMake 3.25 or higher is required`

```bash
# Download latest CMake from cmake.org/download
# Or use package manager:
brew upgrade cmake  # macOS
winget upgrade Kitware.CMake  # Windows
```

### Ninja Not Found

**Problem**: CMake can't find Ninja generator

```bash
# Install Ninja
brew install ninja  # macOS
winget install Ninja-build.Ninja  # Windows
sudo apt install ninja-build  # Linux

# Or use default generator (remove from CMakePresets.json)
```

### First Build Takes Long

This is **normal**. vcpkg compiles dependencies from source:
- First build: 10-20 minutes
- Subsequent builds: 1-3 minutes

### Web Build - Emscripten Issues

**Problem**: `EMSDK not found`

```bash
# Ensure EMSDK environment variable is set
export EMSDK="/path/to/emsdk"  # Linux/macOS
$env:EMSDK = "C:\path\to\emsdk"  # Windows

# Activate Emscripten
cd $EMSDK
./emsdk activate latest
source ./emsdk_env.sh  # Linux/macOS
```

## 🏗️ Architecture & Design

### How It Works

#### Solver Algorithm

**Phase 1: Constraint Propagation**
```
For each empty cell:
  1. Check row/column Sun/Moon counts
  2. Validate no-three-adjacent rule
  3. Verify region Sun requirements
  4. Apply relationship clues
  5. If only one value valid → fill it
  6. Repeat until no progress
```

**Phase 2: Backtracking**
```
1. Find cell with minimum remaining values (MRV heuristic)
2. Try each valid value
3. Recursively solve
4. Backtrack on failure
```

**Uniqueness Verification**: Count solutions up to 2. Valid puzzles have exactly one solution.

#### Puzzle Generation

```
1. Generate random region partition (BFS flood-fill)
2. Fill entire grid randomly with constraint checking
3. Remove cells while maintaining uniqueness:
   - Shuffle all positions
   - Try removing each cell
   - Keep removed if solution remains unique
   - Restore if multiple solutions appear
4. Add strategic relationship clues
5. Final verification: ensure unique solution
```

#### Daily Seed System

Deterministic puzzle generation ensures everyone gets the same puzzle:

```cpp
// FNV-1a hash with MurmurHash mixing
uint32_t seed = hash(YYYY, MM, DD);  // UTC date
Generator gen(seed);
auto puzzle = gen.generate();
```

Same date (UTC) → Same seed → Same puzzle worldwide.

### Project Structure

```
ECLIPSE/
├── src/
│   ├── core/              # Game engine
│   │   ├── grid.*         # Grid representation
│   │   ├── constraints.*  # Constraint validation
│   │   ├── solver.*       # Backtracking solver
│   │   ├── generator.*    # Puzzle generator
│   │   ├── region.*       # Region management
│   │   └── daily_seed.*   # Deterministic seeding
│   └── app/               # UI & Application
│       ├── main.cpp       # Entry point
│       ├── ui.*           # Raylib UI
│       ├── game_state.*   # State management
│       └── persistence.*  # SQLite persistence
├── tests/                 # Unit tests
├── assets/                # Game assets
├── CMakeLists.txt         # Build configuration
├── CMakePresets.json      # CMake presets
└── vcpkg.json            # Dependencies
```

## 🧪 Testing

Run the full test suite:

```bash
# Configure and build tests
cmake --preset desktop-release
cmake --build --preset desktop-release

# Run tests with CTest
ctest --test-dir build/desktop-release --output-on-failure

# Or use the test preset
cmake --build --preset desktop-release --target eclipse_tests
./build/desktop-release/eclipse_tests
```

## 📄 License

MIT License - see [LICENSE](LICENSE) file for details.

## 🙏 Credits

- **Libraries**: [raylib](https://www.raylib.com/), [SQLite](https://www.sqlite.org/), [Catch2](https://github.com/catchorg/Catch2)
- **Inspiration**: LinkedIn daily games (Tango, Queens, Crossclimb)

---

**Made with C++20 | Play your daily ECLIPSE puzzle! ☀️🌑**
