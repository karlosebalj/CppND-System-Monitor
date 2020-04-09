#include "processor.h"

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() { 
    float userTime, niceTime, systemTime, idleTime, iowaitTime, irqTime, softirqTime, stealTime, guestTime, guest_niceTime;
    float idlealltime, virtalltime, systemalltime, totaltime;
    float cpu_percentage;
    std::string line;
    std::string key;
    std::string user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
    std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
    if (stream.is_open()) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        while (linestream >> key >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice) {
            if (key == "cpu") {
                userTime = std::stof (user);
                niceTime = std::stof (nice);
                systemTime = std::stof (system);
                idleTime = std::stof (idle);
                iowaitTime = std::stof (iowait);

                irqTime = std::stof (irq);
                softirqTime = std::stof (softirq);
                stealTime = std::stof (steal);
                guestTime = std::stof (guest);
                guest_niceTime = std::stof (guest_nice);

                // Guest time is already accounted in usertime
                userTime = userTime - guestTime;                     // As you see here, it subtracts guest from user time
                niceTime = niceTime - guest_niceTime;                 // and guest_nice from nice time
                // Fields existing on kernels >= 2.6
                // (and RHEL's patched kernel 2.4...)
                idlealltime = idleTime + iowaitTime;                 // ioWait is added in the idleTime
                systemalltime = systemTime + irqTime + softirqTime;
                virtalltime = guestTime + guest_niceTime;
                totaltime = userTime + niceTime + systemalltime + idlealltime + stealTime + virtalltime;

                cpu_percentage = (totaltime - idlealltime)/totaltime;
                stream.close();
                return cpu_percentage;
            }
        }
    }
    stream.close();
    return cpu_percentage;
}
