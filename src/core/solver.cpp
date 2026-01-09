#include "solver.h"
#include <algorithm>

namespace eclipse {

Solver::Solver(Puzzle& puzzle) : puzzle_(puzzle) {}

bool Solver::solve() {
    // First apply constraint propagation
    propagate();
    
    // Then use backtracking if needed
    return solve_recursive();
}

bool Solver::solve_recursive() {
    // Check if complete
    if (puzzle_.grid().is_complete()) {
        return puzzle_.is_valid();
    }
    
    // Find best cell to fill (MRV heuristic)
    auto best_cell = find_best_cell();
    if (!best_cell) {
        return false;  // No valid cell to fill
    }
    
    int row = best_cell->row;
    int col = best_cell->col;
    
    // Get possible values
    auto possible = puzzle_.get_possible_values(row, col);
    
    // Try Sun first, then Moon
    for (Cell value : {Cell::Sun, Cell::Moon}) {
        if (!possible[static_cast<int>(value)]) continue;
        
        // Make move
        puzzle_.grid().set(row, col, value);
        
        // Recurse
        if (solve_recursive()) {
            return true;
        }
        
        // Backtrack
        puzzle_.grid().set(row, col, Cell::Empty);
    }
    
    return false;
}

int Solver::count_solutions(int max_count) {
    int count = 0;
    count_solutions_recursive(count, max_count);
    return count;
}

void Solver::count_solutions_recursive(int& count, int max_count) {
    if (count >= max_count) return;
    
    // Check if complete
    if (puzzle_.grid().is_complete()) {
        if (puzzle_.is_valid()) {
            count++;
        }
        return;
    }
    
    // Find best cell
    auto best_cell = find_best_cell();
    if (!best_cell) return;
    
    int row = best_cell->row;
    int col = best_cell->col;
    
    auto possible = puzzle_.get_possible_values(row, col);
    
    for (Cell value : {Cell::Sun, Cell::Moon}) {
        if (!possible[static_cast<int>(value)]) continue;
        
        puzzle_.grid().set(row, col, value);
        count_solutions_recursive(count, max_count);
        puzzle_.grid().set(row, col, Cell::Empty);
        
        if (count >= max_count) return;
    }
}

std::vector<LogicalStep> Solver::get_forced_moves() const {
    std::vector<LogicalStep> forced;
    
    for (int r = 0; r < puzzle_.size(); ++r) {
        for (int c = 0; c < puzzle_.size(); ++c) {
            if (!puzzle_.grid().is_empty(r, c)) continue;
            
            auto possible = puzzle_.get_possible_values(r, c);
            
            // Count how many values are possible (excluding Empty)
            int count = 0;
            Cell forced_value = Cell::Empty;
            if (possible[static_cast<int>(Cell::Sun)]) {
                count++;
                forced_value = Cell::Sun;
            }
            if (possible[static_cast<int>(Cell::Moon)]) {
                count++;
                forced_value = Cell::Moon;
            }
            
            if (count == 1) {
                // Only one possible value - this is forced
                LogicalStep step;
                step.position = {r, c};
                step.value = forced_value;
                step.reason = "Only valid placement";
                forced.push_back(step);
            }
        }
    }
    
    return forced;
}

bool Solver::propagate() {
    bool progress = true;
    bool any_progress = false;
    
    while (progress) {
        progress = false;
        
        for (int r = 0; r < puzzle_.size(); ++r) {
            for (int c = 0; c < puzzle_.size(); ++c) {
                if (propagate_cell(r, c)) {
                    progress = true;
                    any_progress = true;
                }
            }
        }
    }
    
    return any_progress;
}

bool Solver::propagate_cell(int row, int col) {
    if (!puzzle_.grid().is_empty(row, col)) return false;
    
    auto possible = puzzle_.get_possible_values(row, col);
    
    int count = 0;
    Cell forced_value = Cell::Empty;
    
    if (possible[static_cast<int>(Cell::Sun)]) {
        count++;
        forced_value = Cell::Sun;
    }
    if (possible[static_cast<int>(Cell::Moon)]) {
        count++;
        forced_value = Cell::Moon;
    }
    
    if (count == 1) {
        // Only one possible value, fill it
        puzzle_.grid().set(row, col, forced_value);
        return true;
    }
    
    return false;
}

std::optional<Position> Solver::find_best_cell() const {
    std::optional<Position> best;
    int min_choices = 3;  // More than possible
    
    for (int r = 0; r < puzzle_.size(); ++r) {
        for (int c = 0; c < puzzle_.size(); ++c) {
            if (!puzzle_.grid().is_empty(r, c)) continue;
            
            auto possible = puzzle_.get_possible_values(r, c);
            int count = 0;
            if (possible[static_cast<int>(Cell::Sun)]) count++;
            if (possible[static_cast<int>(Cell::Moon)]) count++;
            
            if (count == 0) {
                // No valid values - unsolvable
                return std::nullopt;
            }
            
            if (count < min_choices) {
                min_choices = count;
                best = Position{r, c};
                
                if (count == 1) {
                    // Can't do better than 1 choice
                    return best;
                }
            }
        }
    }
    
    return best;
}

bool Solver::is_solvable() const {
    // Check if any cell has no possible values
    for (int r = 0; r < puzzle_.size(); ++r) {
        for (int c = 0; c < puzzle_.size(); ++c) {
            if (!puzzle_.grid().is_empty(r, c)) continue;
            
            auto possible = puzzle_.get_possible_values(r, c);
            if (!possible[static_cast<int>(Cell::Sun)] && 
                !possible[static_cast<int>(Cell::Moon)]) {
                return false;
            }
        }
    }
    return true;
}

} // namespace eclipse

