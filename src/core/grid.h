#pragma once

#include <vector>
#include <cstdint>
#include <optional>

namespace eclipse {

enum class Cell : uint8_t {
    Empty = 0,
    Sun = 1,
    Moon = 2
};

enum class RelationshipClue : uint8_t {
    None = 0,
    Equal = 1,    // =
    NotEqual = 2  // ≠
};

struct Position {
    int row;
    int col;

    bool operator==(const Position& other) const {
        return row == other.row && col == other.col;
    }
};

// A grid represents the puzzle state
class Grid {
public:
    explicit Grid(int size = 6);

    int size() const { return size_; }
    
    Cell get(int row, int col) const;
    void set(int row, int col, Cell value);
    
    bool is_empty(int row, int col) const;
    bool in_bounds(int row, int col) const;
    
    // Get all empty cells
    std::vector<Position> get_empty_cells() const;
    
    // Copy
    Grid clone() const;
    
    // Check if grid is complete (no empty cells)
    bool is_complete() const;
    
    // Get row/col as vectors
    std::vector<Cell> get_row(int row) const;
    std::vector<Cell> get_col(int col) const;
    
    // Clear the grid
    void clear();
    
private:
    int size_;
    std::vector<Cell> cells_;  // Flattened 2D array
    
    int index(int row, int col) const { return row * size_ + col; }
};

} // namespace eclipse

