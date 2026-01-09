#pragma once

#include <string>
#include <chrono>
#include <cstdint>

namespace eclipse {

// Generate a deterministic seed from a date
class DailySeed {
public:
    // Get seed for today (UTC)
    static uint32_t get_today_seed();
    
    // Get seed for a specific date (YYYY-MM-DD)
    static uint32_t get_seed_for_date(const std::string& date);
    
    // Get today's date string (YYYY-MM-DD UTC)
    static std::string get_today_date();
    
    // Get seed from year, month, day
    static uint32_t get_seed(int year, int month, int day);
    
private:
    // Hash function for date components
    static uint32_t hash_date(int year, int month, int day);
};

} // namespace eclipse

