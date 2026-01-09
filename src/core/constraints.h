#pragma once

#include "grid.h"
#include "region.h"
#include <vector>
#include <map>
#include <bitset>

namespace eclipse {

// Relationship clue between two adjacent cells
struct Clue {
    Position cell1;
    Position cell2;
    RelationshipClue type;
};

// Puzzle contains all the constraints
class Puzzle {
public:
    explicit Puzzle(int size = 6);
    
    Grid& grid() { return grid_; }
    const Grid& grid() const { return grid_; }
    
    RegionManager& regions() { return regions_; }
    const RegionManager& regions() const { return regions_; }
    
    // Clue management
    void add_clue(const Clue& clue);
    const std::vector<Clue>& get_clues() const { return clues_; }
    RelationshipClue get_clue(Position pos1, Position pos2) const;
    
    // Check if a value violates constraints
    bool is_valid_placement(int row, int col, Cell value) const;
    
    // Check if entire grid is valid
    bool is_valid() const;
    
    // Get possible values for a cell (based on constraints)
    std::bitset<3> get_possible_values(int row, int col) const;
    
    int size() const { return grid_.size(); }
    
private:
    Grid grid_;
    RegionManager regions_;
    std::vector<Clue> clues_;
    
    // Helper constraint checkers
    bool check_row_col_count(int row, int col, Cell value) const;
    bool check_no_three_adjacent(int row, int col, Cell value) const;
    bool check_region_constraint(int row, int col, Cell value) const;
    bool check_relationship_clues(int row, int col, Cell value) const;
};

} // namespace eclipse

