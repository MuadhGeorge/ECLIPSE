#pragma once

#include "grid.h"
#include <vector>
#include <cstdint>

namespace eclipse {

// A region is a colored group of cells with a constraint
struct Region {
    int id;
    uint32_t color; // RGB color for display
    std::vector<Position> cells;
    int required_suns;  // Number of suns required in this region
    
    Region(int id, uint32_t color) : id(id), color(color), required_suns(0) {}
};

// Manages regions for the puzzle
class RegionManager {
public:
    explicit RegionManager(int grid_size);
    
    // Generate random regions (for puzzle generation)
    void generate_random_regions(int num_regions, unsigned seed);
    
    // Add a region
    void add_region(const Region& region);
    
    // Get region for a cell
    int get_region_id(int row, int col) const;
    const Region* get_region(int region_id) const;
    
    // Get all regions
    const std::vector<Region>& get_regions() const { return regions_; }
    
    // Clear all regions
    void clear();
    
    // Check if every cell is assigned to a region
    bool is_complete() const;
    
    int grid_size() const { return grid_size_; }
    
private:
    int grid_size_;
    std::vector<Region> regions_;
    std::vector<int> cell_to_region_;  // Maps cell index to region ID
    
    int index(int row, int col) const { return row * grid_size_ + col; }
};

} // namespace eclipse

