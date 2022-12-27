#include <string>

#include "format.h"

using std::string;
using std::to_string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    int hours, minutes;
    minutes = seconds / 60;
    hours = minutes / 60;
    string output = to_string(hours) + ":" + to_string(minutes%60) + ":" + to_string(seconds%60);
    return output; 
}