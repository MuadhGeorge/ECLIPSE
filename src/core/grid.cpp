#include "grid.h"
#include <stdexcept>

namespace eclipse {

Grid::Grid(int size) : size_(size), cells_(size * size, Cell::Empty) {
    if (size < 4 || size % 2 != 0) {
        throw std::invalid_argument("Grid size must be even and >= 4");
    }
}

Cell Grid::get(int row, int col) const {
    if (!in_bounds(row, col)) {
        throw std::out_of_range("Grid access out of bounds");
    }
    return cells_[index(row, col)];
}

void Grid::set(int row, int col, Cell value) {
    if (!in_bounds(row, col)) {
        throw std::out_of_range("Grid access out of bounds");
    }
    cells_[index(row, col)] = value;
}

bool Grid::is_empty(int row, int col) const {
    return get(row, col) == Cell::Empty;
}

bool Grid::in_bounds(int row, int col) const {
    return row >= 0 && row < size_ && col >= 0 && col < size_;
}

std::vector<Position> Grid::get_empty_cells() const {
    std::vector<Position> empty;
    for (int r = 0; r < size_; ++r) {
        for (int c = 0; c < size_; ++c) {
            if (is_empty(r, c)) {
                empty.push_back({r, c});
            }
        }
    }
    return empty;
}

Grid Grid::clone() const {
    Grid copy(size_);
    copy.cells_ = cells_;
    return copy;
}

bool Grid::is_complete() const {
    return get_empty_cells().empty();
}

std::vector<Cell> Grid::get_row(int row) const {
    std::vector<Cell> result(size_);
    for (int c = 0; c < size_; ++c) {
        result[c] = get(row, c);
    }
    return result;
}

std::vector<Cell> Grid::get_col(int col) const {
    std::vector<Cell> result(size_);
    for (int r = 0; r < size_; ++r) {
        result[r] = get(r, col);
    }
    return result;
}

void Grid::clear() {
    std::fill(cells_.begin(), cells_.end(), Cell::Empty);
}

} // namespace eclipse

