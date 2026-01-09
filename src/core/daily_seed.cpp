#include "daily_seed.h"
#include <sstream>
#include <iomanip>

namespace eclipse {

uint32_t DailySeed::get_today_seed() {
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    std::tm* utc_tm = std::gmtime(&time_t_now);
    
    return get_seed(utc_tm->tm_year + 1900, utc_tm->tm_mon + 1, utc_tm->tm_mday);
}

uint32_t DailySeed::get_seed_for_date(const std::string& date) {
    // Parse YYYY-MM-DD
    int year, month, day;
    char dash1, dash2;
    std::istringstream iss(date);
    iss >> year >> dash1 >> month >> dash2 >> day;
    
    if (dash1 != '-' || dash2 != '-') {
        return get_today_seed();  // Fallback
    }
    
    return get_seed(year, month, day);
}

std::string DailySeed::get_today_date() {
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    std::tm* utc_tm = std::gmtime(&time_t_now);
    
    std::ostringstream oss;
    oss << (utc_tm->tm_year + 1900) << "-"
        << std::setfill('0') << std::setw(2) << (utc_tm->tm_mon + 1) << "-"
        << std::setfill('0') << std::setw(2) << utc_tm->tm_mday;
    
    return oss.str();
}

uint32_t DailySeed::get_seed(int year, int month, int day) {
    return hash_date(year, month, day);
}

uint32_t DailySeed::hash_date(int year, int month, int day) {
    // Simple but effective hash mixing
    uint32_t hash = 2166136261u;  // FNV offset basis
    
    hash ^= static_cast<uint32_t>(year);
    hash *= 16777619u;  // FNV prime
    
    hash ^= static_cast<uint32_t>(month);
    hash *= 16777619u;
    
    hash ^= static_cast<uint32_t>(day);
    hash *= 16777619u;
    
    // Mix bits
    hash ^= hash >> 16;
    hash *= 0x85ebca6b;
    hash ^= hash >> 13;
    hash *= 0xc2b2ae35;
    hash ^= hash >> 16;
    
    return hash;
}

} // namespace eclipse

