#include "region.h"
#include <random>
#include <queue>
#include <algorithm>

namespace eclipse {

RegionManager::RegionManager(int grid_size)
    : grid_size_(grid_size), cell_to_region_(grid_size * grid_size, -1) {}

void RegionManager::generate_random_regions(int num_regions, unsigned seed) {
    clear();
    
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> coord_dist(0, grid_size_ - 1);
    
    // Create color palette
    std::vector<uint32_t> colors;
    for (int i = 0; i < num_regions; ++i) {
        // Generate distinct colors
        float hue = (i * 360.0f / num_regions);
        // Simple HSV to RGB conversion (S=0.6, V=0.9)
        float s = 0.6f, v = 0.9f;
        float c = v * s;
        float x = c * (1 - std::abs(std::fmod(hue / 60.0f, 2.0f) - 1));
        float m = v - c;
        
        float r, g, b;
        if (hue < 60) { r = c; g = x; b = 0; }
        else if (hue < 120) { r = x; g = c; b = 0; }
        else if (hue < 180) { r = 0; g = c; b = x; }
        else if (hue < 240) { r = 0; g = x; b = c; }
        else if (hue < 300) { r = x; g = 0; b = c; }
        else { r = c; g = 0; b = x; }
        
        uint8_t ri = static_cast<uint8_t>((r + m) * 255);
        uint8_t gi = static_cast<uint8_t>((g + m) * 255);
        uint8_t bi = static_cast<uint8_t>((b + m) * 255);
        
        colors.push_back((ri << 16) | (gi << 8) | bi);
    }
    
    // Pick random starting seeds for BFS
    std::vector<Position> seeds;
    for (int i = 0; i < num_regions; ++i) {
        int attempts = 0;
        while (attempts++ < 100) {
            int r = coord_dist(rng);
            int c = coord_dist(rng);
            if (cell_to_region_[index(r, c)] == -1) {
                seeds.push_back({r, c});
                regions_.emplace_back(i, colors[i]);
                cell_to_region_[index(r, c)] = i;
                regions_[i].cells.push_back({r, c});
                break;
            }
        }
    }
    
    // BFS to grow regions
    std::vector<std::queue<Position>> queues(num_regions);
    for (int i = 0; i < num_regions; ++i) {
        queues[i].push(seeds[i]);
    }
    
    std::uniform_int_distribution<int> region_dist(0, num_regions - 1);
    
    while (true) {
        // Pick a random region to grow
        std::vector<int> active_regions;
        for (int i = 0; i < num_regions; ++i) {
            if (!queues[i].empty()) {
                active_regions.push_back(i);
            }
        }
        
        if (active_regions.empty()) break;
        
        int region_id = active_regions[region_dist(rng) % active_regions.size()];
        auto& queue = queues[region_id];
        
        if (queue.empty()) continue;
        
        Position current = queue.front();
        queue.pop();
        
        // Try to expand to neighbors
        std::vector<Position> neighbors = {
            {current.row - 1, current.col},
            {current.row + 1, current.col},
            {current.row, current.col - 1},
            {current.row, current.col + 1}
        };
        
        std::shuffle(neighbors.begin(), neighbors.end(), rng);
        
        for (const auto& neighbor : neighbors) {
            if (neighbor.row >= 0 && neighbor.row < grid_size_ &&
                neighbor.col >= 0 && neighbor.col < grid_size_) {
                int idx = index(neighbor.row, neighbor.col);
                if (cell_to_region_[idx] == -1) {
                    cell_to_region_[idx] = region_id;
                    regions_[region_id].cells.push_back(neighbor);
                    queue.push(neighbor);
                }
            }
        }
    }
    
    // Calculate required suns for each region (half of region size)
    for (auto& region : regions_) {
        region.required_suns = static_cast<int>(region.cells.size()) / 2;
    }
}

void RegionManager::add_region(const Region& region) {
    regions_.push_back(region);
    for (const auto& pos : region.cells) {
        cell_to_region_[index(pos.row, pos.col)] = region.id;
    }
}

int RegionManager::get_region_id(int row, int col) const {
    if (row < 0 || row >= grid_size_ || col < 0 || col >= grid_size_) {
        return -1;
    }
    return cell_to_region_[index(row, col)];
}

const Region* RegionManager::get_region(int region_id) const {
    for (const auto& region : regions_) {
        if (region.id == region_id) {
            return &region;
        }
    }
    return nullptr;
}

void RegionManager::clear() {
    regions_.clear();
    std::fill(cell_to_region_.begin(), cell_to_region_.end(), -1);
}

bool RegionManager::is_complete() const {
    for (int val : cell_to_region_) {
        if (val == -1) return false;
    }
    return true;
}

} // namespace eclipse

