#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"
#include "system.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid(pid) {};

// TODO: Return this process's ID
int Process::Pid() { return pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    return cpuUsage_; 
    
}

// TODO: Return the command that generated this process
string Process::Command() { 
return command_; 
}

string Process::Ram() { 
    string ram = LinuxParser::Ram(pid);
    if(ram == "") {return string();}
    return to_string(stoi(ram) / 1024); 
}

// TODO: Return the user (name) that generated this process
string Process::User() { 
    string user = "Else";
    int uid = stoi(LinuxParser::Uid(pid));
    if (uid == 0) {
        user = "root";
    }
    if (uid == 1000) {
        user = "malteub";
    }
    return user; 
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
    long int time = LinuxParser::UpTime(pid);
    long int result = time/sysconf(_SC_CLK_TCK);
    return result; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    return cpuUsage_ < a.cpuUsage_ ? true : false;
    }