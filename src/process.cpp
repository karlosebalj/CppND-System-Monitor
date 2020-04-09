#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// DONE: Return this process's ID
// int Process::Pid() { return _pid; }
Process::Process(int pid) : _pid(pid) {}

int Process::Pid() { return _pid; }

int Process::SetPid() const {
    int setPid = _pid;
    return setPid;
}

//DONE: Return this process's CPU utilization
float Process::CpuUtilization() const {
    std::string line;
    float total_time, seconds, cpu_usage;

    //#1
    long upTime = LinuxParser::UpTime();
    //#14 #15 #16 #17
    int utime = 0.0;
    int stime = 0.0;
    int cutime = 0.0;
    int cstime = 0.0;

    //#22
    float starttime = 0.0;

    const int pid = SetPid();
    std::string pidStr = std::to_string(pid);
    std::ifstream stream(LinuxParser::kProcDirectory + pidStr+ LinuxParser::kStatFilename);
    if (stream.is_open()) {
        while (std::getline(stream, line)) {
            std::replace(line.begin(), line.end(), '(', '_');
            std::replace(line.begin(), line.end(), ')', '_');
            std::replace(line.begin(), line.end(), '-', '_');
            
            std::istringstream linestream(line);
            std::istream_iterator<std::string> beg(linestream), end;
            std::vector<std::string> vec(beg, end);

            utime = std::stoi(vec[13]);
            stime = std::stoi(vec[14]);
            cutime = std::stoi(vec[15]);
            cstime= std::stoi(vec[16]);
            starttime = std::stof(vec[21]);

            total_time = utime + stime+ cutime + cstime;
            seconds = upTime - starttime / sysconf(_SC_CLK_TCK);
            cpu_usage = (total_time / sysconf(_SC_CLK_TCK)) / seconds;
        }
    }
    return cpu_usage;
}

// DONE: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(Pid()); }

// DONE: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(Pid()); }

// DONE: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(Pid()); }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(Pid()); }

// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
  return (this->CpuUtilization() > a.CpuUtilization());
}