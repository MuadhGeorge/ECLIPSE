#include <catch2/catch_test_macros.hpp>
#include "core/constraints.h"

using namespace eclipse;

TEST_CASE("Grid basic operations", "[grid]") {
    Grid grid(6);
    
    SECTION("Grid initializes empty") {
        REQUIRE(grid.size() == 6);
        REQUIRE(grid.is_empty(0, 0));
        REQUIRE(grid.get(0, 0) == Cell::Empty);
    }
    
    SECTION("Can set and get cells") {
        grid.set(2, 3, Cell::Sun);
        REQUIRE(grid.get(2, 3) == Cell::Sun);
        REQUIRE_FALSE(grid.is_empty(2, 3));
    }
    
    SECTION("Grid bounds checking works") {
        REQUIRE(grid.in_bounds(0, 0));
        REQUIRE(grid.in_bounds(5, 5));
        REQUIRE_FALSE(grid.in_bounds(6, 6));
        REQUIRE_FALSE(grid.in_bounds(-1, 0));
    }
    
    SECTION("Can clone grid") {
        grid.set(1, 1, Cell::Moon);
        Grid clone = grid.clone();
        REQUIRE(clone.get(1, 1) == Cell::Moon);
    }
}

TEST_CASE("Constraint validation", "[constraints]") {
    Puzzle puzzle(6);
    puzzle.regions().generate_random_regions(6, 777);
    
    SECTION("Row count constraint") {
        // Fill row with 3 suns (valid for 6x6)
        for (int c = 0; c < 3; ++c) {
            puzzle.grid().set(0, c, Cell::Sun);
        }
        
        // Try to add 4th sun (invalid)
        REQUIRE_FALSE(puzzle.is_valid_placement(0, 3, Cell::Sun));
    }
    
    SECTION("No three adjacent constraint") {
        puzzle.grid().set(0, 0, Cell::Sun);
        puzzle.grid().set(0, 1, Cell::Sun);
        
        // Third sun in a row should be invalid
        REQUIRE_FALSE(puzzle.is_valid_placement(0, 2, Cell::Sun));
        
        // But moon should be valid
        REQUIRE(puzzle.is_valid_placement(0, 2, Cell::Moon));
    }
}

TEST_CASE("Relationship clues", "[constraints]") {
    Puzzle puzzle(6);
    
    SECTION("Can add and retrieve clues") {
        Clue clue;
        clue.cell1 = {0, 0};
        clue.cell2 = {0, 1};
        clue.type = RelationshipClue::Equal;
        
        puzzle.add_clue(clue);
        
        auto retrieved = puzzle.get_clue({0, 0}, {0, 1});
        REQUIRE(retrieved == RelationshipClue::Equal);
    }
    
    SECTION("Equal clue enforces same values") {
        Clue clue;
        clue.cell1 = {0, 0};
        clue.cell2 = {0, 1};
        clue.type = RelationshipClue::Equal;
        puzzle.add_clue(clue);
        
        puzzle.grid().set(0, 0, Cell::Sun);
        
        // Setting adjacent cell to Moon should be invalid
        REQUIRE_FALSE(puzzle.is_valid_placement(0, 1, Cell::Moon));
        
        // Setting to Sun should be valid
        REQUIRE(puzzle.is_valid_placement(0, 1, Cell::Sun));
    }
    
    SECTION("NotEqual clue enforces different values") {
        Clue clue;
        clue.cell1 = {1, 0};
        clue.cell2 = {1, 1};
        clue.type = RelationshipClue::NotEqual;
        puzzle.add_clue(clue);
        
        puzzle.grid().set(1, 0, Cell::Moon);
        
        // Setting adjacent cell to Moon should be invalid
        REQUIRE_FALSE(puzzle.is_valid_placement(1, 1, Cell::Moon));
        
        // Setting to Sun should be valid
        REQUIRE(puzzle.is_valid_placement(1, 1, Cell::Sun));
    }
}

TEST_CASE("Region constraints", "[regions]") {
    RegionManager regions(6);
    
    SECTION("Can generate regions") {
        regions.generate_random_regions(6, 42);
        REQUIRE(regions.get_regions().size() == 6);
        REQUIRE(regions.is_complete());
    }
    
    SECTION("All cells assigned to regions") {
        regions.generate_random_regions(6, 123);
        
        int assigned_count = 0;
        for (int r = 0; r < 6; ++r) {
            for (int c = 0; c < 6; ++c) {
                if (regions.get_region_id(r, c) >= 0) {
                    assigned_count++;
                }
            }
        }
        
        REQUIRE(assigned_count == 36);  // All cells in 6x6
    }
}
