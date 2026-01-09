#pragma once

#include <string>
#include <vector>
#include <optional>

namespace eclipse {

struct PlayerStats {
    int total_solved = 0;
    int current_streak = 0;
    int best_streak = 0;
    int total_hints_used = 0;
    double average_solve_time = 0.0;  // seconds
    std::string last_played_date;
};

struct DailyProgress {
    std::string date;
    bool completed = false;
    double solve_time = 0.0;
    int hints_used = 0;
    std::string grid_state;  // Serialized grid
};

class Persistence {
public:
    explicit Persistence(const std::string& db_path);
    ~Persistence();
    
    // Stats operations
    bool load_stats(PlayerStats& stats);
    bool save_stats(const PlayerStats& stats);
    
    // Daily progress
    bool save_daily_progress(const DailyProgress& progress);
    std::optional<DailyProgress> load_daily_progress(const std::string& date);
    
    // Update streak
    void update_streak(const std::string& date);
    
    // Initialize database
    bool initialize();
    
private:
    std::string db_path_;
    void* db_;  // SQLite database handle (using void* to avoid including sqlite3.h in header)
    
    bool execute_sql(const std::string& sql);
};

} // namespace eclipse

