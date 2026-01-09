#include "persistence.h"
#include <sqlite3.h>
#include <iostream>
#include <sstream>

namespace eclipse {

Persistence::Persistence(const std::string& db_path)
    : db_path_(db_path), db_(nullptr) {
    initialize();
}

Persistence::~Persistence() {
    if (db_) {
        sqlite3_close(static_cast<sqlite3*>(db_));
    }
}

bool Persistence::initialize() {
    if (sqlite3_open(db_path_.c_str(), reinterpret_cast<sqlite3**>(&db_)) != SQLITE_OK) {
        std::cerr << "Failed to open database: " << db_path_ << std::endl;
        return false;
    }
    
    // Create tables
    const char* create_stats_table = R"(
        CREATE TABLE IF NOT EXISTS stats (
            id INTEGER PRIMARY KEY CHECK (id = 1),
            total_solved INTEGER DEFAULT 0,
            current_streak INTEGER DEFAULT 0,
            best_streak INTEGER DEFAULT 0,
            total_hints_used INTEGER DEFAULT 0,
            average_solve_time REAL DEFAULT 0.0,
            last_played_date TEXT DEFAULT ''
        );
    )";
    
    const char* create_progress_table = R"(
        CREATE TABLE IF NOT EXISTS daily_progress (
            date TEXT PRIMARY KEY,
            completed INTEGER DEFAULT 0,
            solve_time REAL DEFAULT 0.0,
            hints_used INTEGER DEFAULT 0,
            grid_state TEXT DEFAULT ''
        );
    )";
    
    if (!execute_sql(create_stats_table) || !execute_sql(create_progress_table)) {
        return false;
    }
    
    // Insert default stats if not exists
    execute_sql("INSERT OR IGNORE INTO stats (id) VALUES (1);");
    
    return true;
}

bool Persistence::execute_sql(const std::string& sql) {
    char* err_msg = nullptr;
    if (sqlite3_exec(static_cast<sqlite3*>(db_), sql.c_str(), nullptr, nullptr, &err_msg) != SQLITE_OK) {
        std::cerr << "SQL error: " << (err_msg ? err_msg : "unknown") << std::endl;
        if (err_msg) sqlite3_free(err_msg);
        return false;
    }
    return true;
}

bool Persistence::load_stats(PlayerStats& stats) {
    const char* sql = "SELECT * FROM stats WHERE id = 1;";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(static_cast<sqlite3*>(db_), sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }
    
    bool found = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        stats.total_solved = sqlite3_column_int(stmt, 1);
        stats.current_streak = sqlite3_column_int(stmt, 2);
        stats.best_streak = sqlite3_column_int(stmt, 3);
        stats.total_hints_used = sqlite3_column_int(stmt, 4);
        stats.average_solve_time = sqlite3_column_double(stmt, 5);
        const char* date_str = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
        stats.last_played_date = date_str ? date_str : "";
        found = true;
    }
    
    sqlite3_finalize(stmt);
    return found;
}

bool Persistence::save_stats(const PlayerStats& stats) {
    std::ostringstream sql;
    sql << "UPDATE stats SET "
        << "total_solved = " << stats.total_solved << ", "
        << "current_streak = " << stats.current_streak << ", "
        << "best_streak = " << stats.best_streak << ", "
        << "total_hints_used = " << stats.total_hints_used << ", "
        << "average_solve_time = " << stats.average_solve_time << ", "
        << "last_played_date = '" << stats.last_played_date << "' "
        << "WHERE id = 1;";
    
    return execute_sql(sql.str());
}

bool Persistence::save_daily_progress(const DailyProgress& progress) {
    std::ostringstream sql;
    sql << "INSERT OR REPLACE INTO daily_progress "
        << "(date, completed, solve_time, hints_used, grid_state) VALUES ("
        << "'" << progress.date << "', "
        << (progress.completed ? 1 : 0) << ", "
        << progress.solve_time << ", "
        << progress.hints_used << ", "
        << "'" << progress.grid_state << "');";
    
    return execute_sql(sql.str());
}

std::optional<DailyProgress> Persistence::load_daily_progress(const std::string& date) {
    std::ostringstream sql;
    sql << "SELECT * FROM daily_progress WHERE date = '" << date << "';";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(static_cast<sqlite3*>(db_), sql.str().c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return std::nullopt;
    }
    
    std::optional<DailyProgress> result;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        DailyProgress progress;
        progress.date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        progress.completed = sqlite3_column_int(stmt, 1) != 0;
        progress.solve_time = sqlite3_column_double(stmt, 2);
        progress.hints_used = sqlite3_column_int(stmt, 3);
        const char* grid_str = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        progress.grid_state = grid_str ? grid_str : "";
        result = progress;
    }
    
    sqlite3_finalize(stmt);
    return result;
}

void Persistence::update_streak(const std::string& date) {
    PlayerStats stats;
    load_stats(stats);
    
    if (stats.last_played_date.empty()) {
        stats.current_streak = 1;
    } else {
        // Parse dates and check if consecutive
        // Simplified: just check if different
        if (stats.last_played_date != date) {
            // In a real implementation, check if dates are consecutive
            // For now, just increment
            stats.current_streak++;
        }
    }
    
    if (stats.current_streak > stats.best_streak) {
        stats.best_streak = stats.current_streak;
    }
    
    stats.last_played_date = date;
    save_stats(stats);
}

} // namespace eclipse

