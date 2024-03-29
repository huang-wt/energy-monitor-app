#include "include/system.h"

#include <algorithm>

#include "include/system_parser.h"
#include "include/command.h"
#include "include/date_time.h"

#define WEEK_DAYS 7
#define CONSUMING_THRESHOLD 0.01

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

    SetUpEnergyDataAndFiles();
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

double System::PowerUsage() {
    return power.CurrPowerUsage();
}

vector<double> System::HoursEnergyUsages() {
    UpdateEnergy();
    return power.HoursEnergyUsages();
}

double System::TotalEnergyUsage() {
    return power.TotalEnergyUsage();
}

map<string, double> System::LastWeekEnergyUsage() {
    return dao.LastNDaysEnergyUsage(WEEK_DAYS);
}

double System::TotalEnergyUsageLastWeek() {
    map<string, double> energy_usages = LastWeekEnergyUsage();
    double total_usage = 0;
    for (auto const& [key, val] : energy_usages) {
        total_usage += val;
    }

    return total_usage;
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

void System::UpdateEnergy() {
    int curr_hour = DateTime::Hour();
    string curr_date = DateTime::CurrentDate();
    string last_logged_date = dao.LastLoggedDate();

    if (curr_date != last_logged_date) {
        dao.UpdateDaysLogFile(last_logged_date, power.TotalEnergyUsage());
        power.ResetLogVector();
    } else {
        power.SetLogVector(dao.HoursEnergyUsages());
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

void System::SetUpEnergyDataAndFiles() {
    // Check if log files have been created
    dao.InitDaysLogFile();
    dao.InitHoursLogFile(DateTime::CurrentDate(), power.HoursEnergyUsages());

    hour = DateTime::Hour();
    string curr_date = DateTime::CurrentDate();
    string last_logged_date = dao.LastLoggedDate();

    // Check if it is a new day
    power.SetLogVector(dao.HoursEnergyUsages());
    if (curr_date != last_logged_date) {
        dao.UpdateDaysLogFile(last_logged_date, power.TotalEnergyUsage());
        power.ResetLogVector();
        dao.UpdateHoursLogFile(curr_date, power.HoursEnergyUsages());
    }
    power.SetExtra(power.HoursEnergyUsages(hour));
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
        if (p.CpuUtilisation() > CONSUMING_THRESHOLD) {
            pids.push_back(p.Pid());
        } else {
            break;
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

void System::BindToPAndECores() {
    vector<int> pids = CpuConsumingProcesses();
    BindProcesses(pids, cpu.HyperThreadedCores() / 2, \
                  (cpu.LogicalCores() + cpu.HyperThreadedCores()) / 2 - 1 );
}

void System::SetEnergyCap(double cap) {
    energy_cap = cap;
}

double System::EnergyCap() {
    return energy_cap;
}
