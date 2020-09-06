#pragma once
#include <sstream> 
#include <chrono>  
#include <iomanip>

inline std::string GetCurrentDateTimeAsFormatedString() {
    auto in_time_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    return (std::stringstream{} << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X")).str();
}