#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"
#include "system.h"
#include "process.h"
#include "processor.h"
#include "memory.h"

System::System() {
    cpu = Processor();
    memory = Memory();
}

// Returns the system's kernel identifier (string)
std::string System::getKernel() { 
    return LinuxParser::kernel();
}

// Returns the operating system name
std::string System::getOperatingSystem() { 
    return LinuxParser::operatingSystem(); 
}

// Returns the number of processes actively running on the system
int System::getRunningProcesses() { 
    return LinuxParser::runningProcesses();
}

// Returns the total number of processes on the system
int System::getTotalProcesses() { 
    return LinuxParser::totalProcesses(); 
}

// Returns the number of seconds since the system started running
long int System::getUpTime() { 
    return LinuxParser::upTime(); 
}

int System::getCpuCoresCount() {
    return LinuxParser::cpuCoresCount();
}

// in Kb
float System::getTotalMemory() {
    return memory.getTotalMemory();
}

float System::getUsedMemory() {
    return memory.getUsedMemory();
}

float System::getMemoryUtilisation() {
    return memory.getUtilisation();
}

std::vector<float> System::getCpuUtilisations() {
    return cpu.utilizations();
}

long System::getTotalJiffies() {
    return cpu.jiffies(-1);
}

std::vector<Process> System::getSortedProcesses() {
  // Initialize/Update map
    std::vector<int> pids = LinuxParser::pids();
    
    for (int pid : pids) {
        Process p;
        p.setPid(pid);
        p.setCommand(pid);
        processes.insert(std::make_pair(pid, p));
    }

    // Update cpu utilization for each process
    long totalJiffies = getTotalJiffies();
    for (int j = 0 ; j < processes.size() ; j++) {
        Process p = processes[j];
        p.setCpuUtilization(p.getPid(), totalJiffies);
        processes[j] = p;
    }

    // Sorting in descending order
    std::vector<Process> processesSorted;
    std::map<int, Process> :: iterator it;
    for (it = processes.begin() ; it != processes.end() ; it++) {
        processesSorted.push_back(it->second);
    }
    std::sort(processesSorted.begin(), processesSorted.end(), std::greater<Process>());

    return processesSorted;
}
