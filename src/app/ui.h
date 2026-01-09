#pragma once

#include "game_state.h"
#include "persistence.h"
#include <raylib.h>
#include <memory>
#include <optional>

namespace eclipse {

enum class UIState {
    MainMenu,
    Playing,
    Paused,
    Completed,
    Stats
};

class UI {
public:
    UI();
    ~UI();
    
    // Initialize the UI
    bool init();
    
    // Run the main game loop
    void run();
    
private:
    // Screen dimensions
    const int screen_width_ = 1000;
    const int screen_height_ = 800;
    
    // UI state
    UIState state_ = UIState::MainMenu;
    std::unique_ptr<GameState> game_state_;
    std::unique_ptr<Persistence> persistence_;
    PlayerStats stats_;
    
    // Daily puzzle info
    std::string current_date_;
    
    // UI elements
    int cell_size_ = 60;
    int board_offset_x_ = 100;
    int board_offset_y_ = 150;
    
    // Interaction state
    std::optional<Position> selected_cell_;
    std::optional<Position> hint_highlight_;
    
    // Drawing functions
    void draw();
    void draw_main_menu();
    void draw_game();
    void draw_board();
    void draw_cell(int row, int col, int x, int y);
    void draw_regions();
    void draw_clues();
    void draw_ui_panel();
    void draw_stats_screen();
    void draw_completed_screen();
    
    // Input handling
    void handle_input();
    void handle_cell_click(int mouse_x, int mouse_y);
    void handle_keyboard();
    
    // Game management
    void start_new_game();
    void load_daily_puzzle();
    void complete_puzzle();
    
    // Utility
    Color get_region_color(int region_id) const;
    Rectangle get_cell_rect(int row, int col) const;
    
    // Button helper
    bool draw_button(const char* text, int x, int y, int width, int height);
};

} // namespace eclipse

