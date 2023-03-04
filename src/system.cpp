#include "system.h"

#include <algorithm>

#include "system_parser.h"
#include "command.h"

// Hungry singleton pattern (thread-safe)
System* System::instance = new System();

System* System::Instance() {
    return instance;
}

System::System() {
    // Initialisation
    cpu = Processor();
    memory = Memory();
    operating_system = SystemParser::OperatingSystem(); 
    kernel = SystemParser::Kernel();
}

std::string System::OperatingSystem() { 
    return operating_system;
}

std::string System::Kernel() { 
    return kernel;
}

long int System::UpTime() { 
    return SystemParser::UpTime(); 
}

int System::TotalProcesses() { 
    return SystemParser::TotalProcesses(); 
}

int System::RunningProcesses() { 
    return SystemParser::RunningProcesses();
}

float System::TotalMemory() {
    return memory.TotalMemory();
}

float System::UsedMemory() {
    return memory.UsedMemory();
}

float System::MemoryUtilisation() {
    return memory.Utilisation();
}

int System::CpuTemperature() {
    return cpu.Temperature();
}

std::vector<float> System::CpuUtilisations() {
    return cpu.Utilisations();
}

std::vector<Process> System::SortedProcesses() {
    UpdateProcesses();

    // Sorting in descending order on cpu utilisation
    std::vector<Process> sorted_processes;
    std::map<int, Process> :: iterator it;
    for (it = processes.begin() ; it != processes.end() ; it++) {
        sorted_processes.push_back(it->second);
    }
    sort(sorted_processes.begin(), sorted_processes.end(), std::greater<Process>());

    return sorted_processes;
}

void System::UpdateCpuAndMemory() {
    cpu.UpdateUtilisations();
    cpu.UpdateTemperature();
    memory.UpdateUsedMemory();
}

void System::UpdateProcesses() {
    std::vector<int> pids = SystemParser::Pids();
    for (int pid : pids) {
        Process p;
        p.SetPid(pid);
        p.SetCommand(pid);
        processes.insert(std::make_pair(pid, p));
    }

    // Update cpu utilization of each process
    long totalJiffies = SystemParser::TotalJiffies(-1);
    for (int j = 0 ; j < processes.size() ; j++) {
        Process p = processes[j];
        p.SetCpuUtilization(p.Pid(), totalJiffies);
        processes[j] = p;
    }

}

void System::BindProcesses(std::vector<int> pids, int low, int high) {
    std::string taskset_cmd = "taskset -cp";
    for (auto &id: pids) {
        std::string full_cmd = taskset_cmd + " " + std::to_string(low) + "-" + \
                              std::to_string(high) + " " + std::to_string(id);
        raymii::Command::exec(full_cmd);
    }
}

std::vector<int> System::CpuConsumingProcesses() {
    std::vector<Process> processes = SortedProcesses();
    std::vector<int> pids;
    for (Process p : processes) {
        if (p.CpuUtilisation() * 100 > 1) {
            pids.push_back(p.Pid());
        }
    }

    return pids;
}

void System::BindToPCores() {
    std::vector<int> pids = CpuConsumingProcesses();
    BindProcesses(pids, 0, cpu.HyperThreadedCores() - 1);
}

void System::BindToAllCores() {
    std::vector<int> pids = CpuConsumingProcesses();
    BindProcesses(pids, 0, cpu.LogicalCores() - 1);
}

void System::BindToECores() {
    std::vector<int> pids = CpuConsumingProcesses();
    BindProcesses(pids, cpu.HyperThreadedCores(), cpu.LogicalCores() - 1);
}
