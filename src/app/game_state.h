#pragma once

#include "core/constraints.h"
#include "core/solver.h"
#include "persistence.h"
#include <vector>
#include <stack>
#include <memory>
#include <chrono>

namespace eclipse {

struct Move {
    Position position;
    Cell old_value;
    Cell new_value;
};

enum class HintLevel {
    Highlight,  // Highlight a forced cell
    Apply,      // Apply a forced move
    Reveal      // Reveal a cell from solution
};

class GameState {
public:
    explicit GameState(std::unique_ptr<Puzzle> puzzle);
    
    // Game actions
    void set_cell(int row, int col, Cell value);
    Cell get_cell(int row, int col) const;
    
    // Undo/Redo
    void undo();
    void redo();
    bool can_undo() const { return !undo_stack_.empty(); }
    bool can_redo() const { return !redo_stack_.empty(); }
    
    // Hints
    std::optional<Position> get_hint_position(HintLevel level);
    void apply_hint(HintLevel level);
    int hints_used() const { return hints_used_; }
    
    // Check if puzzle is solved
    bool is_solved() const;
    bool is_valid() const;
    
    // Timer
    void start_timer();
    void pause_timer();
    double get_elapsed_time() const;
    
    // Access puzzle
    const Puzzle& puzzle() const { return *puzzle_; }
    Puzzle& puzzle() { return *puzzle_; }
    
    // Generate share text
    std::string generate_share_text(const std::string& date) const;
    
private:
    std::unique_ptr<Puzzle> puzzle_;
    std::unique_ptr<Puzzle> solution_;  // Store solution for hints
    
    std::stack<Move> undo_stack_;
    std::stack<Move> redo_stack_;
    
    int hints_used_ = 0;
    
    std::chrono::steady_clock::time_point start_time_;
    double elapsed_time_ = 0.0;
    bool timer_running_ = false;
    
    void record_move(const Move& move);
};

} // namespace eclipse

