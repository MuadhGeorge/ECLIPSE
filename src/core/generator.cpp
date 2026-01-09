#include "generator.h"
#include <algorithm>
#include <vector>

namespace eclipse {

Generator::Generator(const GeneratorConfig& config)
    : config_(config), rng_(config.seed) {}

std::unique_ptr<Puzzle> Generator::generate() {
    // Try multiple times to generate a valid puzzle
    for (int attempt = 0; attempt < 100; ++attempt) {
        auto puzzle = std::make_unique<Puzzle>(config_.grid_size);
        
        // Generate regions first
        puzzle->regions().generate_random_regions(config_.num_regions, rng_());
        
        // Generate solved grid
        if (!generate_solved_grid(*puzzle)) {
            continue;
        }
        
        // Create puzzle by removing cells
        auto solution = std::make_unique<Puzzle>(config_.grid_size);
        solution->grid() = puzzle->grid().clone();
        solution->regions() = puzzle->regions();
        
        Puzzle puzzle_attempt(config_.grid_size);
        puzzle_attempt.regions() = puzzle->regions();
        
        create_puzzle_from_solution(*puzzle, puzzle_attempt);
        
        // Add relationship clues if enabled
        if (config_.use_relationship_clues) {
            int clue_count = 3 + (static_cast<int>(config_.difficulty) * 2);
            add_relationship_clues(puzzle_attempt, clue_count);
        }
        
        // Verify unique solution
        if (has_unique_solution(puzzle_attempt)) {
            return std::make_unique<Puzzle>(puzzle_attempt);
        }
    }
    
    // Fallback: generate simpler puzzle
    auto puzzle = std::make_unique<Puzzle>(config_.grid_size);
    puzzle->regions().generate_random_regions(config_.num_regions, rng_());
    generate_solved_grid(*puzzle);
    
    // Leave more clues for easier solving
    Puzzle simple(config_.grid_size);
    simple.regions() = puzzle->regions();
    
    // Copy half the cells as clues
    int cells_to_fill = (config_.grid_size * config_.grid_size) / 2;
    std::vector<Position> all_cells;
    for (int r = 0; r < config_.grid_size; ++r) {
        for (int c = 0; c < config_.grid_size; ++c) {
            all_cells.push_back({r, c});
        }
    }
    std::shuffle(all_cells.begin(), all_cells.end(), rng_);
    
    for (int i = 0; i < cells_to_fill && i < static_cast<int>(all_cells.size()); ++i) {
        auto pos = all_cells[i];
        simple.grid().set(pos.row, pos.col, puzzle->grid().get(pos.row, pos.col));
    }
    
    return std::make_unique<Puzzle>(simple);
}

bool Generator::generate_solved_grid(Puzzle& puzzle) {
    return fill_grid_random(puzzle);
}

bool Generator::fill_grid_random(Puzzle& puzzle) {
    // Find empty cell
    auto empty_cells = puzzle.grid().get_empty_cells();
    if (empty_cells.empty()) {
        return true;  // Solved
    }
    
    // Pick random empty cell
    std::uniform_int_distribution<size_t> dist(0, empty_cells.size() - 1);
    Position pos = empty_cells[dist(rng_)];
    
    // Try values in random order
    std::vector<Cell> values = {Cell::Sun, Cell::Moon};
    std::shuffle(values.begin(), values.end(), rng_);
    
    for (Cell value : values) {
        if (puzzle.is_valid_placement(pos.row, pos.col, value)) {
            puzzle.grid().set(pos.row, pos.col, value);
            
            if (fill_grid_random(puzzle)) {
                return true;
            }
            
            // Backtrack
            puzzle.grid().set(pos.row, pos.col, Cell::Empty);
        }
    }
    
    return false;
}

void Generator::create_puzzle_from_solution(Puzzle& solution, Puzzle& puzzle) {
    // Start with full solution
    puzzle.grid() = solution.grid().clone();
    
    // Get all positions
    std::vector<Position> positions;
    for (int r = 0; r < config_.grid_size; ++r) {
        for (int c = 0; c < config_.grid_size; ++c) {
            positions.push_back({r, c});
        }
    }
    
    std::shuffle(positions.begin(), positions.end(), rng_);
    
    int target_empty = config_.max_empty_cells;
    int removed = 0;
    
    for (const auto& pos : positions) {
        if (removed >= target_empty) break;
        
        // Try removing this cell
        Cell original = puzzle.grid().get(pos.row, pos.col);
        puzzle.grid().set(pos.row, pos.col, Cell::Empty);
        
        // Check if still unique solution
        Puzzle test_puzzle = puzzle;
        Solver solver(test_puzzle);
        int solutions = solver.count_solutions(2);
        
        if (solutions == 1) {
            // Good, keep it removed
            removed++;
        } else {
            // Restore it
            puzzle.grid().set(pos.row, pos.col, original);
        }
    }
}

void Generator::add_relationship_clues(Puzzle& puzzle, int count) {
    std::vector<std::pair<Position, Position>> candidates;
    
    // Find all adjacent pairs
    for (int r = 0; r < config_.grid_size; ++r) {
        for (int c = 0; c < config_.grid_size; ++c) {
            // Horizontal
            if (c < config_.grid_size - 1) {
                candidates.push_back({{r, c}, {r, c + 1}});
            }
            // Vertical
            if (r < config_.grid_size - 1) {
                candidates.push_back({{r, c}, {r + 1, c}});
            }
        }
    }
    
    std::shuffle(candidates.begin(), candidates.end(), rng_);
    
    int added = 0;
    for (const auto& pair : candidates) {
        if (added >= count) break;
        
        Cell val1 = puzzle.grid().get(pair.first.row, pair.first.col);
        Cell val2 = puzzle.grid().get(pair.second.row, pair.second.col);
        
        // Only add clues where at least one cell is empty
        if (val1 == Cell::Empty || val2 == Cell::Empty) {
            // Determine clue type based on solution
            RelationshipClue clue_type = RelationshipClue::NotEqual;
            if (val1 != Cell::Empty && val2 != Cell::Empty) {
                clue_type = (val1 == val2) ? RelationshipClue::Equal : RelationshipClue::NotEqual;
            }
            
            Clue clue;
            clue.cell1 = pair.first;
            clue.cell2 = pair.second;
            clue.type = clue_type;
            
            puzzle.add_clue(clue);
            added++;
        }
    }
}

int Generator::evaluate_difficulty(const Puzzle& puzzle) const {
    // Count empty cells and analyze constraint tightness
    int empty_count = static_cast<int>(puzzle.grid().get_empty_cells().size());
    
    // More empty cells = harder
    return empty_count * 10;
}

bool Generator::has_unique_solution(Puzzle& puzzle) const {
    Puzzle test = puzzle;
    Solver solver(test);
    return solver.count_solutions(2) == 1;
}

} // namespace eclipse

