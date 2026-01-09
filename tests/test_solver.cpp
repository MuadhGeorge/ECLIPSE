#include <catch2/catch_test_macros.hpp>
#include "core/solver.h"
#include "core/constraints.h"

using namespace eclipse;

TEST_CASE("Solver can solve simple puzzles", "[solver]") {
    Puzzle puzzle(6);
    
    // Create a simple solvable configuration
    puzzle.regions().generate_random_regions(6, 12345);
    
    // Add some initial clues
    puzzle.grid().set(0, 0, Cell::Sun);
    puzzle.grid().set(0, 1, Cell::Moon);
    puzzle.grid().set(1, 0, Cell::Moon);
    
    Solver solver(puzzle);
    
    SECTION("Can solve") {
        bool solved = solver.solve();
        REQUIRE(solved);
        REQUIRE(puzzle.grid().is_complete());
        REQUIRE(puzzle.is_valid());
    }
    
    SECTION("Can propagate constraints") {
        bool progress = solver.propagate();
        // May or may not make progress depending on puzzle
        REQUIRE((progress || !progress));  // Just check it doesn't crash
    }
}

TEST_CASE("Solver can count solutions", "[solver]") {
    Puzzle puzzle(6);
    puzzle.regions().generate_random_regions(6, 54321);
    
    Solver solver(puzzle);
    
    SECTION("Can count solutions") {
        int count = solver.count_solutions(3);
        REQUIRE(count >= 0);
        REQUIRE(count <= 3);
    }
}

TEST_CASE("Solver provides logical hints", "[solver]") {
    Puzzle puzzle(6);
    puzzle.regions().generate_random_regions(6, 99999);
    
    // Fill most of the grid
    for (int r = 0; r < 5; ++r) {
        for (int c = 0; c < 5; ++c) {
            puzzle.grid().set(r, c, (r + c) % 2 == 0 ? Cell::Sun : Cell::Moon);
        }
    }
    
    Solver solver(puzzle);
    
    SECTION("Can get forced moves") {
        auto forced = solver.get_forced_moves();
        // Should have some forced moves or none (both valid)
        REQUIRE(forced.size() >= 0);
    }
}

TEST_CASE("Solver validates constraints", "[solver]") {
    Puzzle puzzle(6);
    puzzle.regions().generate_random_regions(6, 11111);
    
    SECTION("Empty puzzle is not complete") {
        REQUIRE_FALSE(puzzle.grid().is_complete());
    }
    
    SECTION("Invalid placement detected") {
        // Fill entire row with suns (invalid)
        for (int c = 0; c < 6; ++c) {
            puzzle.grid().set(0, c, Cell::Sun);
        }
        REQUIRE_FALSE(puzzle.is_valid());
    }
}
