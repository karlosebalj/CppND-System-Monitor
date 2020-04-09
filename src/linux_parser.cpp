#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <unistd.h>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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
  float memTotal = 0.0; 
  float memFree = 0.0; 
  float totalUsedMemory = 0.0;
  std::string line;
  std::string key, value;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal") {
          memTotal = std::stof (value);
        }
        if (key == "MemFree") {
          memFree = std::stof (value);
        }
      }
    }  
  }
  totalUsedMemory = (memTotal - memFree)/memTotal;
  stream.close();
  return totalUsedMemory;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() { 
  std::string line;
  std::string time;
  long upTime = 0.0;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> time;
    upTime = std::stol (time);
    return upTime;
  }
  return upTime;
}

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return UpTime() * sysconf(_SC_CLK_TCK); }

// DONE: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  string line, token;
  vector<string> values;
  std::ifstream filestream(LinuxParser::kProcDirectory + to_string(pid) +
                           LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> token) {
      values.push_back(token);
    }
  }
  long jiffies{0};
  if (values.size() > 21) {
    long user = stol(values[13]);
    long kernel = stol(values[14]);
    long children_user = stol(values[15]);
    long children_kernel = stol(values[16]);
    jiffies = user + kernel + children_user + children_kernel;
  }
  return jiffies;
}

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  vector<string> time = CpuUtilization();
  return (stol(time[CPUStates::kUser_]) + stol(time[CPUStates::kNice_]) +
          stol(time[CPUStates::kSystem_]) + stol(time[CPUStates::kIRQ_]) +
          stol(time[CPUStates::kSoftIRQ_]) + stol(time[CPUStates::kSteal_]) +
          stol(time[CPUStates::kGuest_]) + stol(time[CPUStates::kGuestNice_]));
}

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> time = CpuUtilization();
  return (stol(time[CPUStates::kIdle_]) + stol(time[CPUStates::kIOwait_]));
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  std::string line, key, value;
  int procsesses;

  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          procsesses = stoi(value);
          return procsesses;
        }
      }
    }
  }
  return procsesses;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  std::string line, key, value;
  int procsRunning;

  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          procsRunning = stoi(value);
          return procsRunning;
        }
      }
    }
  }
  return procsRunning;
 }

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  std::string line;
  std::string command;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open())
  {
    std::getline (stream, line);
    std::istringstream linestream(line);
    linestream >> command;
    return command;
  }
  return command;  
  }

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  int ram = 0;
  std::string line, key, value;
  std::string vmSize;
  std::ifstream inputStream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (inputStream.is_open()) {
    while (std::getline(inputStream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while(linestream >> key >> value) {
        if (key =="VmSize") {
          ram = std::stof (value) / 1000;
        }
      }
    }
  }  
  return std::to_string(ram);
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  std::string key, value;
  std::string line;
  std::string uid = "";
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while(linestream >> key >> value) {
        if (key =="Uid") {
          uid = value;
          return uid;
        }
      }
    }  
  }
  return uid;
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  std::string key, value, x;
  std::string line;
  std::string uidOfPid = Uid(pid);
  std::ifstream inputStream(kPasswordPath);
  if (inputStream.is_open()) {
    while (std::getline(inputStream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while(linestream >> key >> x >> value) {
        if (value == uidOfPid) {
          return key;
        }
      }
    }  
  }
  return key; 
}

//  DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  std::string line;
  std::string key, value;
  std::string timeCLK;
  std::string time;

  long upTime = 0;
  std::string pidStr = std::to_string(pid);
  std::ifstream inputStream(kProcDirectory + pidStr + kStatFilename);
  if (inputStream.is_open()) {
    std::getline(inputStream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < 22; i++)
    {
      linestream >> timeCLK;
      time = timeCLK;
    }
    upTime = std::stol (time) /sysconf(_SC_CLK_TCK);
    return upTime;
  } 
  return upTime;
}