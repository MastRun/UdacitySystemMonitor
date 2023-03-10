#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

template <typename T>
T ParserHelper(string path, string searchterm, T &returnvalue) {
  string line, key;
  std::ifstream stream(path);
  T default_return{};
  if(stream.is_open()) {
    while(std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> returnvalue;
      if(key==searchterm) { return returnvalue;}
    }
  }
  return default_return;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  long memTotal, memFree, value = 0;
  string line, key, unit;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if(stream.is_open()) {
    while (std::getline(stream, line))
    {
      std::istringstream linestream(line);
      linestream >> key >> value >> unit;
      if(key=="MemTotal:") { memTotal = value; }
      if(key=="MemFree:") { memFree = value; }
    }
    
  }
  return (1.0 * memFree) / (1.0 * memTotal); 
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() { 
  long uptime, idletime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if(stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idletime;
    return uptime;
  } 
  return 0; 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<std::string> LinuxParser::CpuUtilization() { 
  string key, value, line = {};
  vector<std::string> results = {};
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()) {
    while(std::getline(stream, line)) {;
    std::istringstream linestream(line);
    linestream >> key;
    if(key=="cpu") {
      while(linestream >> value) {
        results.push_back(value);
      }     
    }
  } 
  }
  return results; 
  }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  int returnvalue = 0;
  return ParserHelper<int>(kProcDirectory + kStatFilename, "processes", returnvalue);
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  int returnvalue = 0;
  return ParserHelper<int>(kProcDirectory + kStatFilename, "procs_running", returnvalue);
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { 
  string command;
  std::ifstream stream(kProcDirectory + "/" + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, command);
  }
  return command; 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string returnvalue = {""};
  returnvalue = ParserHelper(kProcDirectory + "/" + to_string(pid) + kStatusFilename, "VmSize:", returnvalue);
  if(returnvalue == "") {
    return string();
  } 
  return to_string(stoi(returnvalue) / 1024);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { 
  string returnvalue{};
  return ParserHelper(kProcDirectory + "/" + to_string(pid) + kStatusFilename, "Uid:", returnvalue);
}

string Get_Substring(string line, string start_match, string end_match) {
  int start_idx, end_idx;
  start_idx = line.find(start_match);
  end_idx = line.find(end_match, start_idx + start_match.length());
  if(start_idx == 0 || end_idx == 0) {
    return string();
  }
  string substring = line.substr(start_idx + start_match.length(), end_idx - (start_idx + start_match.length()));
  return substring;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(string searched_uid) { 
  string line, key;
  std::ifstream stream(kPasswordPath);
  if(stream.is_open()) {
    while(std::getline(stream, line)) {
      string user = Get_Substring(line, "\n", ":x:");
      string uid = Get_Substring(line, ":x:", ":");
      if (uid == searched_uid) {
        return user;
      }
    }
  }
  return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string key, value, line = {};
  vector<std::string> results = {};
  std::ifstream stream(kProcDirectory + "/" + to_string(pid) + kStatFilename);
  if(stream.is_open()) {
  std::getline(stream, line);
  std::istringstream linestream(line);
    while(linestream >> value) {
      results.push_back(value);
    }     
  }
  if(results.size() == 0) {return 0;}
  if(results[21] == "") {return 0;
  }
  return atol(results[21].c_str()); 
}

std::vector<std::string> LinuxParser::CPU_Stuff(int pid) { 
  string key, value, line = {};
  vector<std::string> results = {};
  std::ifstream stream(kProcDirectory + "/" + to_string(pid) + kStatFilename);
  if(stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
      while(linestream >> value) {
        results.push_back(value);
      }     
  }
  return results; 
}

float LinuxParser::ProcessCpuUtilization(int pid) { 
    std::vector<std::string> results{};
    results = CPU_Stuff(pid);
    if(results.size() == 0) {return 0.0;}
    if(results[13] == "" || results[14] == "" || results[21] == "" || results[15] == "" || results[16] == "") {return 0.0;}
    float systemUpTime = 1.0 * LinuxParser::UpTime();
    long total_time = atol(results[13].c_str()) + atol(results[14].c_str()) + atol(results[15].c_str()) + atol(results[16].c_str());
    float seconds = systemUpTime - (atol(results[21].c_str())/sysconf(_SC_CLK_TCK));
    if(seconds == 0) {return 0.0;}
    float cpuUsage = (total_time / sysconf(_SC_CLK_TCK)) / seconds;
    return cpuUsage; 
}