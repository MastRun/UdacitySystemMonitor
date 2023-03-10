#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    processes_.clear();
    vector <int> pids = LinuxParser::Pids();
    for(int pid :  pids) {
        Process proc(pid);
        proc.cpuUsage_ = LinuxParser::ProcessCpuUtilization(pid);
        proc.command_ = LinuxParser::Command(pid);
        proc.ram_ = LinuxParser::Ram(pid);
        proc.uid_ = LinuxParser::Uid(pid);
        proc.user_ = LinuxParser::User(proc.uid_);
        processes_.push_back(proc);
    }
    return processes_; }

// DONE: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// DONE: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// DONE: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// DONE: Return the number of seconds since the system started running
long int System::UpTime() { 
    upTime_ =  LinuxParser::UpTime();
    return upTime_; }