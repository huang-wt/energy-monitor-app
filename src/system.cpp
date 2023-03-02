#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"
#include "system.h"
#include "process.h"
#include "processor.h"
#include "memory.h"

System* System::instancePtr = NULL;

System* System::getInstance() {
    if (instancePtr == NULL) {
        instancePtr = new System();
    }
    return instancePtr;
}

System::System() {
    cpu = Processor();
    memory = Memory();
}

std::string System::getKernel() { 
    return LinuxParser::kernel();
}

std::string System::getOperatingSystem() { 
    return LinuxParser::operatingSystem(); 
}

int System::getRunningProcesses() { 
    return LinuxParser::runningProcesses();
}

int System::getTotalProcesses() { 
    return LinuxParser::totalProcesses(); 
}

long int System::getUpTime() { 
    return LinuxParser::upTime(); 
}

float System::getTotalMemory() {
    return memory.getTotalMemory();
}

float System::getUsedMemory() {
    return memory.getUsedMemory();
}

float System::getMemoryUtilisation() {
    return memory.getUtilisation();
}

int System::getCpuTemp() {
    return cpu.getCpuTemp();
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

void System::bindProcesses(vector<int> pids, int low, int high) {
    string tasksetCMD = "taskset -cp";
    string lowVal, highVal;
    lowVal = to_string(low);
    highVal = to_string(high);
    for (auto &id: pids) {
        string fullcmd = tasksetCMD + " " + lowVal + "-" + highVal + " " + to_string(id);
        // cout << fullcmd << endl;
        CommandResult bindCMD = Command::exec(fullcmd);
    }
}

vector<int> System::getCpuConsumingPids() {
    vector<Process> processes = getSortedProcesses();
    vector<int> pids;
    for (Process p : processes) {
        if (p.getCpuUtilization() * 100 > 1) {
            pids.push_back(p.getPid());
        }
    }

    return pids;
}

void System::bindProcessesToPCores() {
    vector<int> pids = getCpuConsumingPids();
    bindProcesses(pids, 0, cpu.hyperThreadedCores - 1);
}

void System::bindProcessesToAllCores() {
    vector<int> pids = getCpuConsumingPids();
    bindProcesses(pids, 0, cpu.logicalCores - 1);
}

void System::bindProcessesToECores() {
    vector<int> pids = getCpuConsumingPids();
    bindProcesses(pids, cpu.hyperThreadedCores, cpu.logicalCores - 1);
}