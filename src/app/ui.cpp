#include "ui.h"
#include "core/daily_seed.h"
#include "core/generator.h"
#include <raylib.h>
#include <cmath>
#include <filesystem>

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#endif

namespace eclipse {

UI::UI() {
    // Get user data directory
#ifdef _WIN32
    std::string data_dir = std::string(getenv("APPDATA")) + "/ECLIPSE";
#else
    std::string data_dir = std::string(getenv("HOME")) + "/.eclipse";
#endif
    std::filesystem::create_directories(data_dir);
    
    persistence_ = std::make_unique<Persistence>(data_dir + "/eclipse.db");
    persistence_->load_stats(stats_);
}

UI::~UI() {
    CloseWindow();
}

bool UI::init() {
    InitWindow(screen_width_, screen_height_, "ECLIPSE - Daily Puzzle");
    SetTargetFPS(60);
    
    current_date_ = DailySeed::get_today_date();
    
    return true;
}

void UI::run() {
#ifdef PLATFORM_WEB
    emscripten_set_main_loop_arg([](void* arg) {
        UI* ui = static_cast<UI*>(arg);
        if (!WindowShouldClose()) {
            ui->handle_input();
            ui->draw();
        }
    }, this, 0, 1);
#else
    while (!WindowShouldClose()) {
        handle_input();
        draw();
    }
#endif
}

void UI::draw() {
    BeginDrawing();
    ClearBackground(Color{245, 245, 245, 255});
    
    switch (state_) {
        case UIState::MainMenu:
            draw_main_menu();
            break;
        case UIState::Playing:
        case UIState::Paused:
            draw_game();
            break;
        case UIState::Completed:
            draw_completed_screen();
            break;
        case UIState::Stats:
            draw_stats_screen();
            break;
    }
    
    EndDrawing();
}

void UI::draw_main_menu() {
    // Title
    const char* title = "ECLIPSE";
    int title_width = MeasureText(title, 80);
    DrawText(title, (screen_width_ - title_width) / 2, 100, 80, Color{50, 50, 50, 255});
    
    const char* subtitle = "Daily Logic Puzzle";
    int subtitle_width = MeasureText(subtitle, 30);
    DrawText(subtitle, (screen_width_ - subtitle_width) / 2, 200, 30, Color{100, 100, 100, 255});
    
    // Buttons
    if (draw_button("Play Today's Puzzle", screen_width_/2 - 150, 300, 300, 60)) {
        load_daily_puzzle();
    }
    
    if (draw_button("View Stats", screen_width_/2 - 150, 380, 300, 60)) {
        state_ = UIState::Stats;
    }
    
    // Show today's date
    DrawText(current_date_.c_str(), screen_width_/2 - 50, 480, 20, GRAY);
    
    // Quick stats
    char stats_text[256];
    snprintf(stats_text, sizeof(stats_text), "Streak: %d | Solved: %d", 
             stats_.current_streak, stats_.total_solved);
    int stats_width = MeasureText(stats_text, 20);
    DrawText(stats_text, (screen_width_ - stats_width) / 2, 520, 20, DARKGRAY);
}

void UI::draw_game() {
    if (!game_state_) return;
    
    // Title
    DrawText("ECLIPSE", 20, 20, 40, Color{50, 50, 50, 255});
    DrawText(current_date_.c_str(), 20, 70, 20, GRAY);
    
    // Timer
    int elapsed = static_cast<int>(game_state_->get_elapsed_time());
    int minutes = elapsed / 60;
    int seconds = elapsed % 60;
    char timer_text[32];
    snprintf(timer_text, sizeof(timer_text), "%02d:%02d", minutes, seconds);
    DrawText(timer_text, screen_width_ - 150, 20, 40, Color{50, 50, 50, 255});
    
    // Board
    draw_regions();
    draw_board();
    draw_clues();
    
    // UI Panel
    draw_ui_panel();
}

void UI::draw_board() {
    const auto& puzzle = game_state_->puzzle();
    int size = puzzle.size();
    
    for (int row = 0; row < size; ++row) {
        for (int col = 0; col < size; ++col) {
            int x = board_offset_x_ + col * cell_size_;
            int y = board_offset_y_ + row * cell_size_;
            
            draw_cell(row, col, x, y);
        }
    }
}

void UI::draw_cell(int row, int col, int x, int y) {
    Rectangle rect = {static_cast<float>(x), static_cast<float>(y), 
                     static_cast<float>(cell_size_), static_cast<float>(cell_size_)};
    
    // Highlight selected cell
    if (selected_cell_ && selected_cell_->row == row && selected_cell_->col == col) {
        DrawRectangleRec(rect, Color{200, 220, 255, 255});
    }
    
    // Highlight hint
    if (hint_highlight_ && hint_highlight_->row == row && hint_highlight_->col == col) {
        DrawRectangleRec(rect, Color{255, 255, 150, 255});
    }
    
    // Cell border
    DrawRectangleLinesEx(rect, 2, Color{100, 100, 100, 255});
    
    // Draw symbol
    Cell value = game_state_->get_cell(row, col);
    if (value == Cell::Sun) {
        DrawText("S", x + cell_size_/2 - 15, y + cell_size_/2 - 20, 40, Color{255, 200, 0, 255});
    } else if (value == Cell::Moon) {
        DrawText("M", x + cell_size_/2 - 15, y + cell_size_/2 - 20, 40, Color{100, 100, 200, 255});
    }
}

void UI::draw_regions() {
    const auto& puzzle = game_state_->puzzle();
    const auto& regions = puzzle.regions().get_regions();
    
    for (const auto& region : regions) {
        for (const auto& pos : region.cells) {
            int x = board_offset_x_ + pos.col * cell_size_;
            int y = board_offset_y_ + pos.row * cell_size_;
            
            Color region_color = get_region_color(region.id);
            region_color.a = 80;  // Semi-transparent
            DrawRectangle(x + 2, y + 2, cell_size_ - 4, cell_size_ - 4, region_color);
        }
        
        // Draw required suns count (on first cell of region)
        if (!region.cells.empty()) {
            const auto& first = region.cells[0];
            int x = board_offset_x_ + first.col * cell_size_;
            int y = board_offset_y_ + first.row * cell_size_;
            
            char count_text[8];
            snprintf(count_text, sizeof(count_text), "%d", region.required_suns);
            DrawText(count_text, x + 5, y + 5, 15, BLACK);
        }
    }
}

void UI::draw_clues() {
    const auto& puzzle = game_state_->puzzle();
    const auto& clues = puzzle.get_clues();
    
    for (const auto& clue : clues) {
        int x1 = board_offset_x_ + clue.cell1.col * cell_size_ + cell_size_ / 2;
        int y1 = board_offset_y_ + clue.cell1.row * cell_size_ + cell_size_ / 2;
        int x2 = board_offset_x_ + clue.cell2.col * cell_size_ + cell_size_ / 2;
        int y2 = board_offset_y_ + clue.cell2.row * cell_size_ + cell_size_ / 2;
        
        int mid_x = (x1 + x2) / 2;
        int mid_y = (y1 + y2) / 2;
        
        const char* symbol = (clue.type == RelationshipClue::Equal) ? "=" : "≠";
        DrawText(symbol, mid_x - 8, mid_y - 10, 20, RED);
    }
}

void UI::draw_ui_panel() {
    int panel_x = board_offset_x_ + game_state_->puzzle().size() * cell_size_ + 50;
    int panel_y = board_offset_y_;
    
    // Hints used
    char hints_text[64];
    snprintf(hints_text, sizeof(hints_text), "Hints: %d", game_state_->hints_used());
    DrawText(hints_text, panel_x, panel_y, 20, DARKGRAY);
    
    // Hint buttons
    if (draw_button("Hint 1\n(Highlight)", panel_x, panel_y + 40, 120, 50)) {
        hint_highlight_ = game_state_->get_hint_position(HintLevel::Highlight);
        game_state_->apply_hint(HintLevel::Highlight);
    }
    
    if (draw_button("Hint 2\n(Apply)", panel_x, panel_y + 100, 120, 50)) {
        game_state_->apply_hint(HintLevel::Apply);
        hint_highlight_ = std::nullopt;
    }
    
    if (draw_button("Hint 3\n(Reveal)", panel_x, panel_y + 160, 120, 50)) {
        game_state_->apply_hint(HintLevel::Reveal);
        hint_highlight_ = std::nullopt;
    }
    
    // Undo/Redo
    if (draw_button("Undo", panel_x, panel_y + 240, 120, 40)) {
        game_state_->undo();
    }
    
    if (draw_button("Redo", panel_x, panel_y + 290, 120, 40)) {
        game_state_->redo();
    }
    
    // Menu
    if (draw_button("Menu", panel_x, panel_y + 360, 120, 40)) {
        game_state_->pause_timer();
        state_ = UIState::MainMenu;
    }
    
    // Instructions
    DrawText("Click: Cycle", panel_x, screen_height_ - 150, 16, DARKGRAY);
    DrawText("Empty->S->M", panel_x, screen_height_ - 130, 16, DARKGRAY);
}

void UI::draw_stats_screen() {
    DrawText("STATISTICS", screen_width_/2 - 100, 100, 40, Color{50, 50, 50, 255});
    
    char stats_lines[512];
    snprintf(stats_lines, sizeof(stats_lines),
             "Total Solved: %d\n\n"
             "Current Streak: %d\n\n"
             "Best Streak: %d\n\n"
             "Total Hints Used: %d\n\n"
             "Average Solve Time: %.1f seconds",
             stats_.total_solved,
             stats_.current_streak,
             stats_.best_streak,
             stats_.total_hints_used,
             stats_.average_solve_time);
    
    DrawText(stats_lines, screen_width_/2 - 150, 200, 24, DARKGRAY);
    
    if (draw_button("Back", screen_width_/2 - 75, 550, 150, 50)) {
        state_ = UIState::MainMenu;
    }
}

void UI::draw_completed_screen() {
    draw_game();
    
    // Overlay
    DrawRectangle(0, 0, screen_width_, screen_height_, Color{0, 0, 0, 180});
    
    // Completion message
    DrawText("PUZZLE SOLVED!", screen_width_/2 - 150, 200, 40, WHITE);
    
    int minutes = static_cast<int>(game_state_->get_elapsed_time() / 60);
    int seconds = static_cast<int>(game_state_->get_elapsed_time()) % 60;
    char time_text[64];
    snprintf(time_text, sizeof(time_text), "Time: %dm %ds", minutes, seconds);
    DrawText(time_text, screen_width_/2 - 80, 260, 24, WHITE);
    
    char hints_text[64];
    snprintf(hints_text, sizeof(hints_text), "Hints Used: %d", game_state_->hints_used());
    DrawText(hints_text, screen_width_/2 - 80, 300, 24, WHITE);
    
    if (draw_button("Share", screen_width_/2 - 75, 360, 150, 50)) {
        std::string share_text = game_state_->generate_share_text(current_date_);
        SetClipboardText(share_text.c_str());
    }
    
    if (draw_button("Menu", screen_width_/2 - 75, 430, 150, 50)) {
        state_ = UIState::MainMenu;
    }
}

void UI::handle_input() {
    if (state_ == UIState::Playing) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            handle_cell_click(GetMouseX(), GetMouseY());
        }
        
        handle_keyboard();
        
        // Check if solved
        if (game_state_ && game_state_->is_solved()) {
            complete_puzzle();
        }
    }
}

void UI::handle_cell_click(int mouse_x, int mouse_y) {
    if (!game_state_) return;
    
    int size = game_state_->puzzle().size();
    
    for (int row = 0; row < size; ++row) {
        for (int col = 0; col < size; ++col) {
            Rectangle rect = get_cell_rect(row, col);
            if (CheckCollisionPointRec({static_cast<float>(mouse_x), static_cast<float>(mouse_y)}, rect)) {
                selected_cell_ = Position{row, col};
                
                // Cycle through values
                Cell current = game_state_->get_cell(row, col);
                Cell next = Cell::Empty;
                
                if (current == Cell::Empty) next = Cell::Sun;
                else if (current == Cell::Sun) next = Cell::Moon;
                else next = Cell::Empty;
                
                game_state_->set_cell(row, col, next);
                hint_highlight_ = std::nullopt;
                return;
            }
        }
    }
}

void UI::handle_keyboard() {
    if (!game_state_) return;
    
    // Undo/Redo
    if (IsKeyPressed(KEY_Z) && (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_LEFT_SUPER))) {
        game_state_->undo();
    }
    if (IsKeyPressed(KEY_Y) && (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_LEFT_SUPER))) {
        game_state_->redo();
    }
    
    // Hints
    if (IsKeyPressed(KEY_H)) {
        hint_highlight_ = game_state_->get_hint_position(HintLevel::Highlight);
        game_state_->apply_hint(HintLevel::Highlight);
    }
}

void UI::start_new_game() {
    load_daily_puzzle();
}

void UI::load_daily_puzzle() {
    // Check if already played today
    auto progress = persistence_->load_daily_progress(current_date_);
    
    // Generate puzzle
    uint32_t seed = DailySeed::get_today_seed();
    GeneratorConfig config;
    config.seed = seed;
    config.grid_size = 6;
    config.difficulty = Difficulty::Medium;
    config.num_regions = 6;
    config.max_empty_cells = 20;
    
    Generator generator(config);
    auto puzzle = generator.generate();
    
    game_state_ = std::make_unique<GameState>(std::move(puzzle));
    game_state_->start_timer();
    
    state_ = UIState::Playing;
    selected_cell_ = std::nullopt;
    hint_highlight_ = std::nullopt;
}

void UI::complete_puzzle() {
    game_state_->pause_timer();
    state_ = UIState::Completed;
    
    // Update stats
    stats_.total_solved++;
    stats_.total_hints_used += game_state_->hints_used();
    
    // Update average solve time
    double total_time = stats_.average_solve_time * (stats_.total_solved - 1) + game_state_->get_elapsed_time();
    stats_.average_solve_time = total_time / stats_.total_solved;
    
    persistence_->update_streak(current_date_);
    persistence_->load_stats(stats_);  // Reload to get updated streak
    persistence_->save_stats(stats_);
    
    // Save daily progress
    DailyProgress progress;
    progress.date = current_date_;
    progress.completed = true;
    progress.solve_time = game_state_->get_elapsed_time();
    progress.hints_used = game_state_->hints_used();
    persistence_->save_daily_progress(progress);
}

Color UI::get_region_color(int region_id) const {
    // Generate distinct colors based on region ID
    float hue = (region_id * 137.5f);  // Golden angle
    while (hue >= 360.0f) hue -= 360.0f;
    
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
    
    return Color{
        static_cast<uint8_t>((r + m) * 255),
        static_cast<uint8_t>((g + m) * 255),
        static_cast<uint8_t>((b + m) * 255),
        255
    };
}

Rectangle UI::get_cell_rect(int row, int col) const {
    return Rectangle{
        static_cast<float>(board_offset_x_ + col * cell_size_),
        static_cast<float>(board_offset_y_ + row * cell_size_),
        static_cast<float>(cell_size_),
        static_cast<float>(cell_size_)
    };
}

bool UI::draw_button(const char* text, int x, int y, int width, int height) {
    Rectangle rect = {static_cast<float>(x), static_cast<float>(y), 
                     static_cast<float>(width), static_cast<float>(height)};
    
    bool hovered = CheckCollisionPointRec(GetMousePosition(), rect);
    bool clicked = hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    
    Color button_color = hovered ? Color{100, 150, 200, 255} : Color{70, 120, 180, 255};
    
    DrawRectangleRounded(rect, 0.2f, 8, button_color);
    
    // Center text
    int text_width = MeasureText(text, 20);
    DrawText(text, x + (width - text_width) / 2, y + (height - 20) / 2, 20, WHITE);
    
    return clicked;
}

} // namespace eclipse
