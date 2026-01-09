#include "game_state.h"
#include "core/generator.h"
#include <sstream>

namespace eclipse {

GameState::GameState(std::unique_ptr<Puzzle> puzzle)
    : puzzle_(std::move(puzzle)) {
    // Solve to get solution for hints
    solution_ = std::make_unique<Puzzle>(*puzzle_);
    Solver solver(*solution_);
    solver.solve();
}

void GameState::set_cell(int row, int col, Cell value) {
    Cell old_value = puzzle_->grid().get(row, col);
    if (old_value == value) return;
    
    Move move{{row, col}, old_value, value};
    record_move(move);
    
    puzzle_->grid().set(row, col, value);
}

Cell GameState::get_cell(int row, int col) const {
    return puzzle_->grid().get(row, col);
}

void GameState::record_move(const Move& move) {
    undo_stack_.push(move);
    // Clear redo stack when new move is made
    while (!redo_stack_.empty()) {
        redo_stack_.pop();
    }
}

void GameState::undo() {
    if (undo_stack_.empty()) return;
    
    Move move = undo_stack_.top();
    undo_stack_.pop();
    
    puzzle_->grid().set(move.position.row, move.position.col, move.old_value);
    redo_stack_.push(move);
}

void GameState::redo() {
    if (redo_stack_.empty()) return;
    
    Move move = redo_stack_.top();
    redo_stack_.pop();
    
    puzzle_->grid().set(move.position.row, move.position.col, move.new_value);
    undo_stack_.push(move);
}

std::optional<Position> GameState::get_hint_position(HintLevel level) {
    Solver solver(*puzzle_);
    auto forced_moves = solver.get_forced_moves();
    
    if (!forced_moves.empty()) {
        return forced_moves[0].position;
    }
    
    if (level == HintLevel::Reveal) {
        // Find first empty cell and return it
        auto empty_cells = puzzle_->grid().get_empty_cells();
        if (!empty_cells.empty()) {
            return empty_cells[0];
        }
    }
    
    return std::nullopt;
}

void GameState::apply_hint(HintLevel level) {
    hints_used_++;
    
    if (level == HintLevel::Highlight) {
        // Just highlighting, no action needed here
        return;
    }
    
    Solver solver(*puzzle_);
    auto forced_moves = solver.get_forced_moves();
    
    if (!forced_moves.empty() && level == HintLevel::Apply) {
        // Apply first forced move
        const auto& step = forced_moves[0];
        set_cell(step.position.row, step.position.col, step.value);
        return;
    }
    
    if (level == HintLevel::Reveal) {
        // Reveal a cell from solution
        auto empty_cells = puzzle_->grid().get_empty_cells();
        if (!empty_cells.empty()) {
            Position pos = empty_cells[0];
            Cell solution_value = solution_->grid().get(pos.row, pos.col);
            set_cell(pos.row, pos.col, solution_value);
        }
    }
}

bool GameState::is_solved() const {
    if (!puzzle_->grid().is_complete()) return false;
    return puzzle_->is_valid();
}

bool GameState::is_valid() const {
    return puzzle_->is_valid();
}

void GameState::start_timer() {
    if (!timer_running_) {
        start_time_ = std::chrono::steady_clock::now();
        timer_running_ = true;
    }
}

void GameState::pause_timer() {
    if (timer_running_) {
        auto now = std::chrono::steady_clock::now();
        elapsed_time_ += std::chrono::duration<double>(now - start_time_).count();
        timer_running_ = false;
    }
}

double GameState::get_elapsed_time() const {
    double total = elapsed_time_;
    if (timer_running_) {
        auto now = std::chrono::steady_clock::now();
        total += std::chrono::duration<double>(now - start_time_).count();
    }
    return total;
}

std::string GameState::generate_share_text(const std::string& date) const {
    std::ostringstream oss;
    oss << "ECLIPSE " << date << "\n";
    
    int minutes = static_cast<int>(get_elapsed_time() / 60);
    int seconds = static_cast<int>(get_elapsed_time()) % 60;
    oss << "Time: " << minutes << "m " << seconds << "s\n";
    oss << "Hints: " << hints_used_ << "\n\n";
    
    // Generate emoji grid
    for (int r = 0; r < puzzle_->size(); ++r) {
        for (int c = 0; c < puzzle_->size(); ++c) {
            Cell cell = puzzle_->grid().get(r, c);
            if (cell == Cell::Sun) {
                oss << "🟨";  // Sun (yellow square)
            } else if (cell == Cell::Moon) {
                oss << "🌑";  // Moon (new moon)
            } else {
                oss << "⬛";  // Empty (black square)
            }
        }
        oss << "\n";
    }
    
    return oss.str();
}

} // namespace eclipse

