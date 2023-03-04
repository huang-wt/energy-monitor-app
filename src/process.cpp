#include "process.h"

#include "system_parser.h"

using std::string;
using std::to_string;
using std::vector;

bool Process::operator<(Process const& a) const { 
    return this->cpu_utilisation < a.cpu_utilisation;
}

bool Process::operator>(Process const& a) const { 
    return this->cpu_utilisation > a.cpu_utilisation;
}

int Process::Pid() { 
    return pid; 
}

float Process::CpuUtilisation() { 
    return cpu_utilisation;
}

string Process::Command() { 
    return command; 
}

string Process::Ram() { 
    return ram;
}

string Process::User() { 
    return user; 
}

long Process::UpTime() { 
    return up_time;
}

void Process::SetPid(int pid) {
    this->pid = pid;
}

void Process::SetUser(int pid) {
    user = SystemParser::User(pid);
}

void Process::SetCommand(int pid) {
    command = SystemParser::Command(pid);
}

void Process::SetCpuUtilization(int pid, long curr_total_jiffies) {
    long curr_active_jiffies = SystemParser::ActiveJiffiesP(pid);
    if (prev_active_jiffies == 0 && prev_total_jiffies == 0) {
        cpu_utilisation = 0;
    } else {
        cpu_utilisation = (curr_active_jiffies - prev_active_jiffies + 0.0) /
                          (curr_total_jiffies - prev_total_jiffies);
    }

    prev_active_jiffies = curr_active_jiffies;
    prev_total_jiffies = curr_total_jiffies;
}

void Process::SetRam(int pid) {
    int kbytes = std::atoi(SystemParser::Ram(pid).c_str());
    int mbytes = kbytes / 1024;
    ram = std::to_string(mbytes);
}

void Process::SetUpTime(int pid){
    up_time = SystemParser::UpTime(pid);
}
