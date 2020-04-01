#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 

    std::string totalTime;
    std::string strHour, strMinute, strSecond;
    long minutes, hours;

    minutes = seconds / 60;
    seconds = seconds % 60;

    hours = minutes / 60;
    minutes= minutes % 60;

    strHour = std::to_string(hours);
    strMinute = std::to_string(minutes);
    strSecond = std::to_string(seconds);

    if (hours < 10 ) {
        strHour = "0" + strHour;
    }

    if (minutes < 10) {
        strMinute = "0" + strMinute;
    }
    
    if (seconds < 10) {
        strSecond = "0" + strSecond;
    }

    if (strSecond == "60") {
        strSecond = "00";
    }

    if (strMinute == "60") {
        strMinute = "00";
    }

    totalTime =  strHour + ":" + strMinute + ":" + strSecond;
    return totalTime; 
}