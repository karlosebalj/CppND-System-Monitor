#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    std::string totalTime;
    long minutes, hours;
    minutes = seconds / 60;
    hours = minutes / 60;
    if (hours < 1) { hours = 00; }
    totalTime = std::to_string(hours) + ":" + std::to_string(minutes % 60) + ":" + std::to_string(seconds % 60);
    return totalTime; 
    }