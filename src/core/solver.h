#pragma once

#include "constraints.h"
#include <optional>
#include <vector>
#include <functional>

namespace eclipse {

enum class SolverResult {
    NoSolution,
    UniqueSolution,
    MultipleSolutions
};

struct LogicalStep {
    Position position;
    Cell value;
    std::string reason;
};

class Solver {
public:
    explicit Solver(Puzzle& puzzle);
    
    // Solve the puzzle, returns true if solution found
    bool solve();
    
    // Count solutions up to a maximum (for uniqueness checking)
    int count_solutions(int max_count = 2);
    
    // Get logical next steps (for hints)
    std::vector<LogicalStep> get_forced_moves() const;
    
    // Apply constraint propagation (returns true if progress made)
    bool propagate();
    
    // Check if puzzle is solvable
    bool is_solvable() const;
    
private:
    Puzzle& puzzle_;
    
    // Backtracking solver
    bool solve_recursive();
    void count_solutions_recursive(int& count, int max_count);
    
    // Find cell with minimum remaining values (MRV heuristic)
    std::optional<Position> find_best_cell() const;
    
    // Apply constraint propagation at a cell
    bool propagate_cell(int row, int col);
};

} // namespace eclipse

