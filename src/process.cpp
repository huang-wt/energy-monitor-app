#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

//-----------------------------------------------------------------------------
// Accessors
//-----------------------------------------------------------------------------

// Return this process's ID
int Process::getPid() { 
    return pid_; 
}

// Return this process's CPU utilization
float Process::getCpuUtilization() { 
    return cpu_utilization_;
}

// Return the command that generated this process
string Process::getCommand() { 
    return command_; 
}

// Return this process's memory utilization
string Process::getRam() { 
    return ram_;
}

// Return the user (name) that generated this process
string Process::getUser() { 
    return user_; 
}

// Return the age of this process (in seconds)
long Process::getUpTime() { 
    return up_time_;
}

// Overloads the less operator according to cpu utilization
bool Process::operator<(Process const& a) const { 
    return this->cpu_utilization_ < a.cpu_utilization_;
}

// Overloads the greater operator according to cpu utilization
bool Process::operator>(Process const& a) const { 
    return this->cpu_utilization_ > a.cpu_utilization_;
}

//-----------------------------------------------------------------------------
// Mutators
//-----------------------------------------------------------------------------

void Process::setPid(int pid) {
    pid_ = pid;
}

void Process::setUser(int pid) {
    user_ = LinuxParser::user(pid);
}

void Process::setCommand(int pid) {
    command_ = LinuxParser::command(pid);
}

void Process::setCpuUtilization(int pid, long currTotalJiffies) {
    long currPJiffies = LinuxParser::activeJiffies(pid);
    if (prevPJiffies == 0 && prevTotalJiffies == 0) {
        cpu_utilization_ = 0;
    } else {
        cpu_utilization_ = (static_cast<float>(currPJiffies) - static_cast<float>(prevPJiffies)) /
                           (static_cast<float>(currTotalJiffies) - static_cast<float>(prevTotalJiffies));
    }

    prevPJiffies = currPJiffies;
    prevTotalJiffies = currTotalJiffies;
}

void Process::setRam(int pid) {
    int kbytes = std::atoi(LinuxParser::ram(pid).c_str());
    int mbytes = kbytes / 1000; // in MB
    ram_ = std::to_string(mbytes);
}

void Process::setUpTime(int pid){
    up_time_ = LinuxParser::upTime(pid);
}