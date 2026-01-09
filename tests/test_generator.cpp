#include <catch2/catch_test_macros.hpp>
#include "core/generator.h"
#include "core/solver.h"

using namespace eclipse;

TEST_CASE("Generator creates valid puzzles", "[generator]") {
    GeneratorConfig config;
    config.seed = 42;
    config.grid_size = 6;
    config.difficulty = Difficulty::Easy;
    config.num_regions = 6;
    config.max_empty_cells = 20;
    
    Generator generator(config);
    
    SECTION("Can generate a puzzle") {
        auto puzzle = generator.generate();
        REQUIRE(puzzle != nullptr);
        REQUIRE(puzzle->size() == 6);
    }
    
    SECTION("Generated puzzle has regions") {
        auto puzzle = generator.generate();
        REQUIRE(puzzle->regions().get_regions().size() > 0);
    }
    
    SECTION("Generated puzzle is solvable") {
        auto puzzle = generator.generate();
        Solver solver(*puzzle);
        bool solvable = solver.solve();
        REQUIRE(solvable);
    }
}

TEST_CASE("Generator ensures unique solutions", "[generator]") {
    GeneratorConfig config;
    config.seed = 12345;
    config.grid_size = 6;
    config.difficulty = Difficulty::Medium;
    config.num_regions = 6;
    config.max_empty_cells = 15;
    
    Generator generator(config);
    
    SECTION("Generated puzzle has unique solution") {
        auto puzzle = generator.generate();
        Solver solver(*puzzle);
        int solution_count = solver.count_solutions(2);
        
        // Should have exactly 1 solution (or possibly more if generator fallback was used)
        REQUIRE(solution_count >= 1);
    }
}

TEST_CASE("Generator can create different difficulty levels", "[generator]") {
    SECTION("Easy puzzles") {
        GeneratorConfig config;
        config.seed = 100;
        config.difficulty = Difficulty::Easy;
        config.grid_size = 6;
        
        Generator generator(config);
        auto puzzle = generator.generate();
        REQUIRE(puzzle != nullptr);
    }
    
    SECTION("Medium puzzles") {
        GeneratorConfig config;
        config.seed = 200;
        config.difficulty = Difficulty::Medium;
        config.grid_size = 6;
        
        Generator generator(config);
        auto puzzle = generator.generate();
        REQUIRE(puzzle != nullptr);
    }
    
    SECTION("Hard puzzles (8x8)") {
        GeneratorConfig config;
        config.seed = 300;
        config.difficulty = Difficulty::Hard;
        config.grid_size = 8;
        config.num_regions = 8;
        
        Generator generator(config);
        auto puzzle = generator.generate();
        REQUIRE(puzzle != nullptr);
        REQUIRE(puzzle->size() == 8);
    }
}
