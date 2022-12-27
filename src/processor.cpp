#include <vector>
#include <iostream>

#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    std::vector<std::string> cpu_values = LinuxParser::CpuUtilization();

    int prevIdle = previdle + previowait;
    int idle = stoi(cpu_values[3]) + stoi(cpu_values[4]);

    int prevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal;
    int nonIdle = stoi(cpu_values[0]) + stoi(cpu_values[1]) + stoi(cpu_values[2]) + stoi(cpu_values[5]) + stoi(cpu_values[6]) + stoi(cpu_values[7]);

    int prevTotal = prevIdle + prevNonIdle;
    int total = idle + nonIdle;

    int totald = total - prevTotal;
    int idled = idle - prevIdle;

    float returnvalue = (1.0 * (totald - idled)) / (1.0 * totald);

    prevuser = stoi(cpu_values[0]);
    prevnice = stoi(cpu_values[1]);
    prevsystem = stoi(cpu_values[2]);
    previdle = stoi(cpu_values[3]);
    previowait = stoi(cpu_values[4]);
    previrq = stoi(cpu_values[5]);
    prevsoftirq = stoi(cpu_values[6]);
    prevsteal = stoi(cpu_values[7]);
    prevguest = stoi(cpu_values[8]);
    prevguest_nice = stoi(cpu_values[9]);

    return returnvalue; }