# ECLIPSE 🌒☀️

[![CI](https://github.com/MuadhGeorge/ECLIPSE/actions/workflows/ci.yml/badge.svg)](https://github.com/MuadhGeorge/ECLIPSE/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://isocpp.org/)

A production-quality daily logic puzzle game built with modern C++20 and raylib. ECLIPSE combines the addictive appeal of LinkedIn-style puzzles with unique constraint-based gameplay featuring Suns and Moons.

![ECLIPSE Gameplay](assets/gameplay.gif)

## 🎮 Game Rules

Fill a 6×6 grid (or 8×8 in hard mode) with Suns (☀️) and Moons (🌙) following these constraints:

1. **Balance**: Each row and column must contain equal numbers of Suns and Moons
2. **No Three Adjacent**: Maximum two identical symbols can be orthogonally adjacent
3. **Region Constraint**: Colored regions must contain exactly K Suns (K shown in top-left of each region)
4. **Relationship Clues**: Optional `=` (same) or `≠` (different) clues between adjacent cells

### Example Puzzle

```
┌───────────────────────────┐
│ [2] S   M  =  _   _   _  │  Region 1 (blue): needs 2 Suns
│  M  ≠  S   M   S   M   S  │
│  S   S   M   _  [3] _  M  │  Region 2 (green): needs 3 Suns
│  M   M   S   S   M   S   _│
│  S   _   M   M   S   M   S│
│ [2] M   S   S   M   _   M │
└───────────────────────────┘
```

## ✨ Features

### Gameplay
- 📅 **Daily Puzzles** - Same puzzle worldwide via deterministic seeding
- 🎯 **Three Difficulty Levels** - Easy/Medium (6×6), Hard (8×8)
- 💡 **Smart Hint System** - Highlight → Apply → Reveal
- ⏪ **Undo/Redo** - Unlimited history with keyboard shortcuts
- ⚡ **Fast Solver** - Constraint propagation + backtracking

### Progress Tracking
- 🔥 **Streak System** - Track consecutive days played
- 📊 **Statistics** - Average time, hints used, completion rate
- 💾 **SQLite Storage** - Persistent local data
- 🎉 **Share Results** - Copy emoji grid to clipboard

### Technical
- 🎨 **Clean Modern UI** - raylib-based minimalist interface
- ✅ **Unique Solutions** - Generator verifies solvability by logic only
- 🧪 **Comprehensive Tests** - Catch2 unit tests for core logic
- 🌐 **Cross-Platform** - Windows, macOS, Linux, Web (Emscripten)

## 🚀 Quick Start

### Prerequisites

1. **C++20 Compiler**: MSVC 2019+, GCC 10+, or Clang 12+
2. **CMake**: 3.20 or higher
3. **vcpkg**: For dependency management

### Installation

#### Windows

```powershell
# Clone repository
git clone https://github.com/MuadhGeorge/ECLIPSE.git
cd ECLIPSE

# Install dependencies (if not already installed)
winget install Kitware.CMake
winget install Microsoft.VisualStudio.2022.Community  # Select "C++ Desktop Development"

# Set vcpkg root (if using existing vcpkg installation)
$env:VCPKG_ROOT = "C:\path\to\vcpkg"

# Or clone vcpkg locally
git clone https://github.com/Microsoft/vcpkg.git
.\vcpkg\bootstrap-vcpkg.bat
$env:VCPKG_ROOT = "$PWD\vcpkg"

# Configure and build
cmake --preset desktop-release
cmake --build build/release --config Release

# Run the game
.\build\release\Release\eclipse_app.exe
```

#### macOS

```bash
# Clone repository
git clone https://github.com/MuadhGeorge/ECLIPSE.git
cd ECLIPSE

# Install dependencies
brew install cmake

# Setup vcpkg
git clone https://github.com/Microsoft/vcpkg.git
./vcpkg/bootstrap-vcpkg.sh
export VCPKG_ROOT="$PWD/vcpkg"

# Configure and build
cmake --preset desktop-release
cmake --build build/release

# Run the game
./build/release/eclipse_app
```

#### Linux (Ubuntu/Debian)

```bash
# Clone repository
git clone https://github.com/MuadhGeorge/ECLIPSE.git
cd ECLIPSE

# Install dependencies
sudo apt update
sudo apt install -y build-essential cmake git

# Setup vcpkg
git clone https://github.com/Microsoft/vcpkg.git
./vcpkg/bootstrap-vcpkg.sh
export VCPKG_ROOT="$PWD/vcpkg"

# Configure and build
cmake --preset desktop-release
cmake --build build/release

# Run the game
./build/release/eclipse_app
```

## 🏗️ Building

### Desktop Release Build

```bash
# Configure
cmake --preset desktop-release

# Build
cmake --build build/release

# Run
./build/release/eclipse_app  # Linux/macOS
.\build\release\Release\eclipse_app.exe  # Windows
```

### Debug Build

```bash
cmake --preset desktop-debug
cmake --build build/debug
```

### Run Tests

```bash
# After building
cd build/release  # or build/debug
ctest --output-on-failure

# Or with preset
cmake --build build/release --target eclipse_tests
./build/release/eclipse_tests  # Linux/macOS
.\build\release\Release\eclipse_tests.exe  # Windows
```

### Web Build (Emscripten)

```bash
# Install Emscripten SDK
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh  # or emsdk_env.bat on Windows
cd ..

# Configure and build
cmake --preset web-release
cmake --build build/web

# Serve locally
cd build/web
python3 -m http.server 8000
# Open http://localhost:8000/eclipse_web.html
```

## 🎮 Controls

### Keyboard Shortcuts
- **Left Click**: Cycle cell (Empty → Sun → Moon)
- **Ctrl+Z**: Undo
- **Ctrl+Y**: Redo
- **H**: Show hint
- **Esc**: Menu

### Game Interface
- **Hint 1**: Highlights a forced cell (yellow)
- **Hint 2**: Applies a forced move automatically
- **Hint 3**: Reveals a cell from the solution

## 🏗️ Architecture

### Project Structure

```
ECLIPSE/
├── src/
│   ├── core/              # Game engine
│   │   ├── grid.h/cpp           # Grid representation
│   │   ├── constraints.h/cpp    # Constraint validation
│   │   ├── solver.h/cpp         # Backtracking solver
│   │   ├── generator.h/cpp      # Puzzle generator
│   │   ├── region.h/cpp         # Region management
│   │   └── daily_seed.h/cpp     # Deterministic seeding
│   └── app/               # UI & Application
│       ├── main.cpp             # Entry point
│       ├── ui.h/cpp             # Raylib UI
│       ├── game_state.h/cpp     # State management
│       └── persistence.h/cpp    # SQLite persistence
├── tests/                 # Unit tests
├── assets/                # Game assets
├── CMakeLists.txt        # Build configuration
├── CMakePresets.json     # CMake presets
└── vcpkg.json           # Dependency manifest
```

### System Architecture

```
┌─────────────────────────────────────────────┐
│              UI Layer (raylib)              │
│  ┌────────────┬──────────┬────────────┐    │
│  │ Main Menu  │  Game UI │ Statistics │    │
│  └────────────┴──────────┴────────────┘    │
└─────────────────┬───────────────────────────┘
                  │
┌─────────────────▼───────────────────────────┐
│           Game State Manager                │
│  ┌───────────┬──────────┬──────────────┐   │
│  │ Undo/Redo │  Timer   │  Hint System │   │
│  └───────────┴──────────┴──────────────┘   │
└─────────────────┬───────────────────────────┘
                  │
┌─────────────────▼───────────────────────────┐
│            Core Engine                      │
│  ┌──────────────┬──────────────────────┐   │
│  │   Solver     │      Generator        │   │
│  │ • Propagate  │  • Fill grid          │   │
│  │ • Backtrack  │  • Remove cells       │   │
│  │ • Count sols │  • Verify uniqueness  │   │
│  └──────────────┴──────────────────────┘   │
│  ┌──────────────────────────────────────┐  │
│  │    Constraint System                  │  │
│  │  • Row/Col balance                    │  │
│  │  • No three adjacent                  │  │
│  │  • Region constraints                 │  │
│  │  • Relationship clues                 │  │
│  └──────────────────────────────────────┘  │
└─────────────────┬───────────────────────────┘
                  │
┌─────────────────▼───────────────────────────┐
│         Persistence (SQLite)                │
│   • Player statistics                       │
│   • Daily progress                          │
│   • Streak tracking                         │
└─────────────────────────────────────────────┘
```

### Solver Algorithm

**Phase 1: Constraint Propagation**
```
For each empty cell:
  1. Check row/column Sun/Moon counts
  2. Validate no-three-adjacent rule
  3. Verify region Sun requirements
  4. Apply relationship clues
  5. If only one valid value → fill it
  6. Repeat until no progress
```

**Phase 2: Backtracking Search**
```
1. Find cell with minimum remaining values (MRV heuristic)
2. Try each valid value
3. Recursively solve
4. Backtrack on failure
```

**Uniqueness Verification**: Count solutions up to 2. Valid puzzles have exactly one solution.

### Puzzle Generation

```
1. Generate random region partition (BFS flood-fill)
2. Fill entire grid randomly with constraint checking
3. Create initial puzzle:
   • Shuffle all cell positions
   • For each cell:
     - Try removing it
     - Check if solution remains unique
     - Keep removed if unique, restore if not
4. Add relationship clues strategically
5. Final verification: ensure unique solution exists
```

### Daily Seed System

Deterministic puzzle generation ensures everyone gets the same puzzle:

```cpp
// FNV-1a hash with MurmurHash mixing
uint32_t seed = hash(year, month, day);
Generator gen(seed);
auto puzzle = gen.generate();
```

Same date (UTC) → Same seed → Same puzzle worldwide.

## 🔧 Troubleshooting

### vcpkg Issues

**Problem**: `vcpkg install failed`
```bash
# Solution: Update vcpkg
cd vcpkg
git pull
./bootstrap-vcpkg.sh  # or .bat on Windows

# Clear cache if needed
rm -rf build
```

**Problem**: `CMAKE_TOOLCHAIN_FILE not found`
```bash
# Solution: Set VCPKG_ROOT environment variable
export VCPKG_ROOT="/path/to/vcpkg"  # Linux/macOS
$env:VCPKG_ROOT = "C:\path\to\vcpkg"  # Windows PowerShell
```

### Compiler Issues

**Problem**: `No C++ compiler found`
```bash
# Windows
winget install Microsoft.VisualStudio.2022.Community
# Select "Desktop development with C++" during installation

# macOS
xcode-select --install

# Linux
sudo apt install build-essential
```

**Problem**: `C++20 features not supported`
- Ensure compiler version: GCC 10+, Clang 12+, MSVC 2019+
- Update your compiler if needed

### CMake Issues

**Problem**: `CMake version too old`
```bash
# Minimum required: CMake 3.20

# Windows
winget install Kitware.CMake

# macOS
brew install cmake

# Linux
# Download from https://cmake.org/download/
```

**Problem**: `Generator mismatch`
```bash
# Solution: Remove build directory
rm -rf build
cmake --preset desktop-release
```

### Build Errors

**Problem**: Long build times (first time)
- Expected: 15-20 minutes for first build (vcpkg compiles dependencies)
- Subsequent builds: 2-3 minutes

**Problem**: `raylib` or `sqlite3` not found
```bash
# Ensure vcpkg installed dependencies
./vcpkg/vcpkg install raylib sqlite3 catch2
```

### Runtime Issues

**Problem**: Game window doesn't open
- Check graphics drivers are up to date
- Try running from terminal to see error messages

**Problem**: Database errors
- Game creates `eclipse.db` in user data directory
- Delete it to reset: 
  - Windows: `%APPDATA%\ECLIPSE\eclipse.db`
  - macOS/Linux: `~/.eclipse/eclipse.db`

## 📊 Development

### Running Tests

```bash
# Build tests
cmake --build build/release --target eclipse_tests

# Run all tests
cd build/release
ctest --output-on-failure

# Run specific test file
./eclipse_tests "test_solver"
```

### Code Style

- Modern C++20 features (concepts, ranges, modules where appropriate)
- RAII and smart pointers (no raw `new`/`delete`)
- Clear naming conventions
- Comments for complex algorithms

### Contributing

1. Fork the repository
2. Create a feature branch
3. Add tests for new features
4. Ensure all tests pass
5. Submit a pull request

## 📸 Screenshots

### Main Menu
![Main Menu](assets/menu.png)

### Gameplay
![Gameplay](assets/gameplay.png)

### Statistics
![Statistics](assets/stats.png)

## 📄 License

MIT License - see [LICENSE](LICENSE) file for details.

## 🙏 Credits

- **Game Design**: Original constraint-based puzzle mechanics
- **Inspiration**: LinkedIn daily games (Tango, Queens, Crossclimb)
- **Libraries**: [raylib](https://www.raylib.com/), [SQLite](https://www.sqlite.org/), [Catch2](https://github.com/catchorg/Catch2)

## 🏷️ Topics

`raylib` `cpp20` `puzzle-game` `constraint-solver` `daily-puzzle` `vcpkg` `cmake` `game-development` `logic-puzzle` `cross-platform`

---

**Made with ❤️ using Modern C++20**

Play your daily ECLIPSE puzzle! ☀️🌑
