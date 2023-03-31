#include "system.h"

#include <algorithm>

#include "system_parser.h"
#include "command.h"
#include "date_time.h"

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
    power = Power();
    dao = PowerDAO();
}

string System::OperatingSystem() { 
    return operating_system;
}

string System::Kernel() { 
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

vector<float> System::CpuUtilisations() {
    return cpu.Utilisations();
}

void System::UpdateCpuAndMemory() {
    while (true) {
        sleep(1);
        cpu.UpdateUtilisations();
        cpu.UpdateTemperature();
        memory.UpdateUsedMemory();
    }
}

vector<Process> System::SortedProcesses() {
    UpdateProcesses();

    // Sorting in descending order on cpu utilisation
    vector<Process> sorted_processes;
    map<int, Process> :: iterator it;
    for (it = processes.begin() ; it != processes.end() ; it++) {
        sorted_processes.push_back(it->second);
    }
    sort(sorted_processes.begin(), sorted_processes.end(), std::greater<Process>());

    return sorted_processes;
}

void System::UpdateProcesses() {
    vector<int> pids = SystemParser::Pids();
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
        p.SetCpuUtilisation(p.Pid(), totalJiffies);
        processes[j] = p;
    }

}

void System::BindProcesses(vector<int> pids, int low, int high) {
    string taskset_cmd = "taskset -cp";
    for (auto &id: pids) {
        string full_cmd = taskset_cmd + " " + std::to_string(low) + "-" + \
                              std::to_string(high) + " " + std::to_string(id);
        raymii::Command::exec(full_cmd);
    }
}

vector<int> System::CpuConsumingProcesses() {
    vector<Process> processes = SortedProcesses();
    vector<int> pids;
    for (Process p : processes) {
        if (p.CpuUtilisation() * 100 > 1) {
            pids.push_back(p.Pid());
        }
    }

    return pids;
}

void System::BindToPCores() {
    vector<int> pids = CpuConsumingProcesses();
    BindProcesses(pids, 0, cpu.HyperThreadedCores() - 1);
}

void System::BindToAllCores() {
    vector<int> pids = CpuConsumingProcesses();
    BindProcesses(pids, 0, cpu.LogicalCores() - 1);
}

void System::BindToECores() {
    vector<int> pids = CpuConsumingProcesses();
    BindProcesses(pids, cpu.HyperThreadedCores(), cpu.LogicalCores() - 1);
}

double System::PowerUsage() {
    return power.CurrPowerUsage();
}

vector<double> System::HoursEnergyUsages() {
    return power.HoursEnergyUsages();
}

double System::TotalEnergyUsage() {
    return power.TotalEnergyUsage();
}

map<string, double> System::LastWeekEnergyUsage() {
    return power.LastNDaysEnergyUsage(dao.ReadDaysFile(), 7);
}

double System::TotalEnergyUsageLastWeek() {
    map<string, double> energy_usages = LastWeekEnergyUsage();
    double total_usage = 0;
    for (auto const& [key, val] : energy_usages) {
        total_usage += val;
    }

    return total_usage;
}

void System::UpdateEnergy() {
    int hour = DateTime::Hour();
    string curr_date = DateTime::CurrentDate();
    string last_logged_date = dao.LastLoggedDate();

    power.SetLogVector(dao.ReadHoursFile());
    if (curr_date != last_logged_date) {
        dao.UpdateDaysLogFile(last_logged_date, power.TotalEnergyUsage());
        power.ResetLogVector();
        dao.UpdateHoursLogFile(curr_date, power.HoursEnergyUsages());
    }
    
    power.SetExtra(power.HoursEnergyUsages(hour));
    int curr_hour;

    while (true) {
        sleep(1);
        curr_hour = DateTime::Hour();
        curr_date = DateTime::CurrentDate();
        last_logged_date = dao.LastLoggedDate();

        if (curr_date != last_logged_date) {
            dao.UpdateDaysLogFile(last_logged_date, power.TotalEnergyUsage());
            power.ResetLogVector();
        } else {
            power.SetLogVector(dao.ReadHoursFile());
        }

        if (curr_hour != hour) {
            power.UpdatePrevHoursEnergy();
            power.SetExtra(0);
            hour = curr_hour;
        }

        power.UpdatePowerAndEnergyUsage();
        power.UpdateLogVector(hour);
        dao.UpdateHoursLogFile(curr_date, power.HoursEnergyUsages());

    }
}

double System::CurrEnergy() {
    return power.CurrHourEnergyUsage();
}
