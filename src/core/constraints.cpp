#include "constraints.h"
#include <algorithm>

namespace eclipse {

Puzzle::Puzzle(int size) : grid_(size), regions_(size) {}

void Puzzle::add_clue(const Clue& clue) {
    clues_.push_back(clue);
}

RelationshipClue Puzzle::get_clue(Position pos1, Position pos2) const {
    for (const auto& clue : clues_) {
        if ((clue.cell1 == pos1 && clue.cell2 == pos2) ||
            (clue.cell1 == pos2 && clue.cell2 == pos1)) {
            return clue.type;
        }
    }
    return RelationshipClue::None;
}

bool Puzzle::is_valid_placement(int row, int col, Cell value) const {
    if (value == Cell::Empty) return true;
    
    // Check all constraints
    if (!check_row_col_count(row, col, value)) return false;
    if (!check_no_three_adjacent(row, col, value)) return false;
    if (!check_region_constraint(row, col, value)) return false;
    if (!check_relationship_clues(row, col, value)) return false;
    
    return true;
}

bool Puzzle::check_row_col_count(int row, int col, Cell value) const {
    int size = grid_.size();
    int half = size / 2;
    
    // Count suns and moons in row (excluding current cell)
    int row_suns = 0, row_moons = 0;
    for (int c = 0; c < size; ++c) {
        if (c == col) continue;
        Cell cell = grid_.get(row, c);
        if (cell == Cell::Sun) row_suns++;
        else if (cell == Cell::Moon) row_moons++;
    }
    
    // Check if adding this value would exceed half
    if (value == Cell::Sun && row_suns >= half) return false;
    if (value == Cell::Moon && row_moons >= half) return false;
    
    // Count in column
    int col_suns = 0, col_moons = 0;
    for (int r = 0; r < size; ++r) {
        if (r == row) continue;
        Cell cell = grid_.get(r, col);
        if (cell == Cell::Sun) col_suns++;
        else if (cell == Cell::Moon) col_moons++;
    }
    
    if (value == Cell::Sun && col_suns >= half) return false;
    if (value == Cell::Moon && col_moons >= half) return false;
    
    return true;
}

bool Puzzle::check_no_three_adjacent(int row, int col, Cell value) const {
    if (value == Cell::Empty) return true;
    
    int size = grid_.size();
    
    // Check horizontal (left and right)
    // Pattern: XX? or ?XX
    if (col >= 2) {
        Cell left1 = grid_.get(row, col - 1);
        Cell left2 = grid_.get(row, col - 2);
        if (left1 == value && left2 == value) return false;
    }
    if (col <= size - 3) {
        Cell right1 = grid_.get(row, col + 1);
        Cell right2 = grid_.get(row, col + 2);
        if (right1 == value && right2 == value) return false;
    }
    // Pattern: X?X (current in middle)
    if (col >= 1 && col < size - 1) {
        Cell left = grid_.get(row, col - 1);
        Cell right = grid_.get(row, col + 1);
        if (left == value && right == value) return false;
    }
    
    // Check vertical
    if (row >= 2) {
        Cell up1 = grid_.get(row - 1, col);
        Cell up2 = grid_.get(row - 2, col);
        if (up1 == value && up2 == value) return false;
    }
    if (row <= size - 3) {
        Cell down1 = grid_.get(row + 1, col);
        Cell down2 = grid_.get(row + 2, col);
        if (down1 == value && down2 == value) return false;
    }
    if (row >= 1 && row < size - 1) {
        Cell up = grid_.get(row - 1, col);
        Cell down = grid_.get(row + 1, col);
        if (up == value && down == value) return false;
    }
    
    return true;
}

bool Puzzle::check_region_constraint(int row, int col, Cell value) const {
    if (value == Cell::Empty) return true;
    
    int region_id = regions_.get_region_id(row, col);
    if (region_id == -1) return true;  // No region constraint
    
    const Region* region = regions_.get_region(region_id);
    if (!region) return true;
    
    // Count suns in this region (excluding current cell)
    int sun_count = 0;
    for (const auto& pos : region->cells) {
        if (pos.row == row && pos.col == col) continue;
        if (grid_.get(pos.row, pos.col) == Cell::Sun) {
            sun_count++;
        }
    }
    
    // Check if adding this value would exceed the required count
    if (value == Cell::Sun && sun_count >= region->required_suns) {
        return false;
    }
    
    // Also check if we have enough space for remaining suns
    int empty_count = 0;
    for (const auto& pos : region->cells) {
        if (grid_.get(pos.row, pos.col) == Cell::Empty) {
            empty_count++;
        }
    }
    
    if (value == Cell::Moon) {
        // If we place a moon, we need enough empty cells for remaining suns
        int remaining_suns = region->required_suns - sun_count;
        if (remaining_suns > empty_count - 1) {  // -1 because we're filling current cell
            return false;
        }
    }
    
    return true;
}

bool Puzzle::check_relationship_clues(int row, int col, Cell value) const {
    if (value == Cell::Empty) return true;
    
    Position current{row, col};
    
    // Check all adjacent cells for clues
    std::vector<Position> neighbors = {
        {row - 1, col}, {row + 1, col},
        {row, col - 1}, {row, col + 1}
    };
    
    for (const auto& neighbor : neighbors) {
        if (!grid_.in_bounds(neighbor.row, neighbor.col)) continue;
        
        RelationshipClue clue = get_clue(current, neighbor);
        if (clue == RelationshipClue::None) continue;
        
        Cell neighbor_value = grid_.get(neighbor.row, neighbor.col);
        if (neighbor_value == Cell::Empty) continue;
        
        if (clue == RelationshipClue::Equal) {
            if (value != neighbor_value) return false;
        } else if (clue == RelationshipClue::NotEqual) {
            if (value == neighbor_value) return false;
        }
    }
    
    return true;
}

bool Puzzle::is_valid() const {
    for (int r = 0; r < grid_.size(); ++r) {
        for (int c = 0; c < grid_.size(); ++c) {
            Cell value = grid_.get(r, c);
            if (value != Cell::Empty) {
                // Temporarily clear cell to check if placement is valid
                Grid temp = grid_.clone();
                const_cast<Grid&>(grid_).set(r, c, Cell::Empty);
                bool valid = is_valid_placement(r, c, value);
                const_cast<Grid&>(grid_).set(r, c, value);
                if (!valid) return false;
            }
        }
    }
    return true;
}

std::bitset<3> Puzzle::get_possible_values(int row, int col) const {
    std::bitset<3> possible;
    possible[static_cast<int>(Cell::Empty)] = true;  // Always can be empty
    
    if (!grid_.is_empty(row, col)) {
        // Already filled
        possible[static_cast<int>(grid_.get(row, col))] = true;
        return possible;
    }
    
    // Try each value
    possible[static_cast<int>(Cell::Sun)] = is_valid_placement(row, col, Cell::Sun);
    possible[static_cast<int>(Cell::Moon)] = is_valid_placement(row, col, Cell::Moon);
    
    return possible;
}

} // namespace eclipse

