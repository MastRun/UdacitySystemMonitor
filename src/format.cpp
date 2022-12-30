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

    string min = to_string(minutes%60);
    min.insert(0,2-min.length(), '0');
    string hrs = to_string(hours);
    hrs.insert(0,2-hrs.length(), '0');
    string sec = to_string(seconds%60);
    sec.insert(0,2-sec.length(), '0');

    //string output = to_string(hours) + ":" + to_string(minutes%60) + ":" + to_string(seconds%60);
    return hrs + ":" + min + ":" + sec; 
}