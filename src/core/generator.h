#pragma once

#include "constraints.h"
#include "solver.h"
#include <random>
#include <memory>

namespace eclipse {

enum class Difficulty {
    Easy,
    Medium,
    Hard
};

struct GeneratorConfig {
    int grid_size = 6;
    Difficulty difficulty = Difficulty::Medium;
    int num_regions = 6;
    unsigned seed = 0;
    
    // Difficulty parameters
    int target_clues = 20;        // Number of initial clues
    int max_empty_cells = 16;     // Maximum empty cells
    bool use_relationship_clues = true;
};

class Generator {
public:
    explicit Generator(const GeneratorConfig& config);
    
    // Generate a complete puzzle with unique solution
    std::unique_ptr<Puzzle> generate();
    
    // Generate a solved grid that satisfies all constraints
    bool generate_solved_grid(Puzzle& puzzle);
    
private:
    GeneratorConfig config_;
    std::mt19937 rng_;
    
    // Fill grid randomly while respecting constraints
    bool fill_grid_random(Puzzle& puzzle);
    
    // Remove cells to create puzzle (while maintaining uniqueness)
    void create_puzzle_from_solution(Puzzle& solution, Puzzle& puzzle);
    
    // Add relationship clues between cells
    void add_relationship_clues(Puzzle& puzzle, int count);
    
    // Evaluate puzzle difficulty
    int evaluate_difficulty(const Puzzle& puzzle) const;
    
    // Check if puzzle has unique solution
    bool has_unique_solution(Puzzle& puzzle) const;
};

} // namespace eclipse

