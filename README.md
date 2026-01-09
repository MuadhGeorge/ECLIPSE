# ECLIPSE 🌒☀️

A production-quality daily logic puzzle game built with C++20 and raylib. ECLIPSE is an addictive LinkedIn-games style puzzle featuring Suns and Moons with unique constraint-based gameplay.

![ECLIPSE Game](assets/screenshot.png)

## Game Rules

### Objective
Fill a 6×6 grid (or 8×8 in hard mode) with Suns (S) and Moons (M) following these constraints:

1. **Balance**: Each row and column must contain an equal number of Suns and Moons
2. **No Three in a Row**: No more than two identical symbols can be adjacent orthogonally (no "SSS" or "MMM")
3. **Region Constraint**: The board is divided into colored regions. Each region must contain exactly K Suns, where K is displayed in the region (typically K = region_size/2)
4. **Relationship Clues** (optional): Some adjacent cells have clues:
   - `=` means both cells must have the same symbol
   - `≠` means cells must have different symbols

### Example Puzzle

```
Region 1 (Blue, needs 2 Suns):    Region 2 (Green, needs 2 Suns):
[2] [ ] [ ] = [ ]                 [2] [ ]
[ ] ≠ [ ] [ ]                     [ ] [ ]
```

## Features

### 🎮 Gameplay
- **Daily Puzzle**: New deterministic puzzle every day (same for everyone worldwide)
- **Three Difficulty Levels**: Easy (6×6), Medium (6×6), Hard (8×8)
- **Undo/Redo**: Full move history with unlimited undo/redo
- **Three-Tier Hint System**:
  1. Highlight a forced cell
  2. Apply a forced move automatically
  3. Reveal a cell from the solution

### 📊 Progress Tracking
- **Streak Counter**: Track consecutive days played
- **Statistics**: Total puzzles solved, average solve time, hint usage
- **Persistent Storage**: SQLite database for stats and progress

### 🎨 User Experience
- Clean, minimalist raylib UI
- Intuitive click-to-cycle controls (Empty → Sun → Moon)
- Keyboard shortcuts (Ctrl+Z/Y for undo/redo, H for hint)
- Share results as emoji grid (🟨☀️ = Sun, 🌑 = Moon)

### 🔧 Technical Features
- **Constraint Solver**: Advanced propagation + backtracking algorithm
- **Uniqueness Verification**: Ensures each puzzle has exactly one solution
- **Smart Puzzle Generation**: Deterministic daily seeds using date-based hashing
- **Cross-Platform**: Desktop (Windows/macOS/Linux) and Web (Emscripten)

## Building the Game

### Prerequisites

#### Desktop Build
- **C++20 Compiler**: MSVC 2019+, GCC 10+, or Clang 12+
- **CMake**: 3.20 or higher
- **vcpkg**: Package manager for dependencies

#### Web Build
- **Emscripten SDK**: For WebAssembly compilation

### Dependencies (via vcpkg)
- `raylib` - Graphics and UI
- `sqlite3` - Persistence
- `catch2` - Unit testing

### Installation

1. **Clone the repository**:
```bash
git clone https://github.com/yourusername/eclipse.git
cd eclipse
```

2. **Install vcpkg** (if not already installed):
```bash
# Windows
git clone https://github.com/Microsoft/vcpkg.git
.\vcpkg\bootstrap-vcpkg.bat

# Linux/macOS
git clone https://github.com/Microsoft/vcpkg.git
./vcpkg/bootstrap-vcpkg.sh
```

3. **Build Desktop Version**:
```bash
# Configure with vcpkg
cmake -B build -DCMAKE_TOOLCHAIN_FILE=[path-to-vcpkg]/scripts/buildsystems/vcpkg.cmake

# Build
cmake --build build --config Release

# Run
./build/eclipse_app  # Linux/macOS
.\build\Release\eclipse_app.exe  # Windows
```

4. **Run Tests**:
```bash
cd build
ctest --output-on-failure
```

### Web Build (Emscripten)

1. **Install Emscripten**:
```bash
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh  # Or emsdk_env.bat on Windows
```

2. **Build for Web**:
```bash
# Configure
emcmake cmake -B build-web

# Build
cmake --build build-web

# Serve locally
cd build-web
python3 -m http.server 8000
# Open browser to http://localhost:8000/eclipse_web.html
```

## How It Works

### Daily Seed Algorithm
Each day's puzzle is generated using a deterministic seed based on the UTC date:

```cpp
seed = hash(YYYY-MM-DD)
```

The hash function uses FNV-1a followed by MurmurHash-style mixing, ensuring:
- Same puzzle worldwide on the same day
- Excellent distribution of random values
- Reproducible for testing/debugging

### Solver Architecture

The solver uses a two-phase approach:

1. **Constraint Propagation**: Apply logical deductions
   - Check row/column counts
   - Validate no-three-adjacent rule
   - Verify region constraints
   - Apply relationship clues

2. **Backtracking Search**: For cells with multiple possibilities
   - MRV (Minimum Remaining Values) heuristic
   - Efficient bitset representation
   - Early termination when counting solutions

### Generator Algorithm

1. Generate random region partition using BFS
2. Fill grid randomly with constraint checking
3. Remove cells iteratively while maintaining uniqueness
4. Add relationship clues strategically
5. Validate final puzzle has unique solution

## Project Structure

```
ECLIPSE/
├── src/
│   ├── core/              # Core game logic
│   │   ├── grid.h/cpp     # Grid representation
│   │   ├── constraints.h/cpp   # Puzzle constraints
│   │   ├── solver.h/cpp   # Constraint solver
│   │   ├── generator.h/cpp     # Puzzle generator
│   │   ├── region.h/cpp   # Region management
│   │   └── daily_seed.h/cpp    # Deterministic seeding
│   └── app/               # UI and application
│       ├── main.cpp       # Entry point
│       ├── ui.h/cpp       # Raylib UI
│       ├── game_state.h/cpp    # Game state management
│       └── persistence.h/cpp   # SQLite persistence
├── tests/                 # Unit tests
│   ├── test_solver.cpp
│   ├── test_generator.cpp
│   └── test_constraints.cpp
├── assets/                # Game assets (if any)
├── CMakeLists.txt         # Build configuration
├── vcpkg.json            # Package manifest
├── LICENSE               # MIT License
└── README.md             # This file
```

## Development

### Code Style
- C++20 features encouraged
- Modern STL containers and algorithms
- Clear variable names, comments where needed
- No giant files (each class in its own file)

### Testing
All core logic is unit tested with Catch2:
```bash
./build/eclipse_tests
```

### Continuous Integration
GitHub Actions automatically:
- Builds desktop version (Windows/Linux/macOS)
- Runs all unit tests
- Reports coverage and build status

## Contributing

Contributions welcome! Please:
1. Fork the repository
2. Create a feature branch
3. Add tests for new features
4. Ensure all tests pass
5. Submit a pull request

## License

MIT License - see [LICENSE](LICENSE) file for details.

## Credits

- **Game Design & Implementation**: Built as a production-quality showcase
- **Inspiration**: LinkedIn-style daily puzzle games (Tango, Queens, Crossclimb)
- **Libraries**: raylib, SQLite, Catch2

## Screenshots

### Main Menu
![Main Menu](assets/menu.png)

### Gameplay
![Gameplay](assets/gameplay.png)

### Statistics
![Stats](assets/stats.png)

---

**Enjoy your daily ECLIPSE puzzle! ☀️🌑**
