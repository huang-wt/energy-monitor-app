#include "system.h"

#include <algorithm>
#include <fstream>

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
    power = Power();
    operating_system = SystemParser::OperatingSystem(); 
    kernel = SystemParser::Kernel();
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
        p.SetCpuUtilization(p.Pid(), totalJiffies);
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
    return power.LastNDaysEnergyUsage(ReadDaysFile(), 7);
}

void System::UpdateEnergy() {
    time_t now = time(0);
    struct tm *tmp = gmtime(&now);
    hour = tmp->tm_hour;
    curr_date = FormatDate(tmp->tm_year + 1900, tmp->tm_mon + 1, tmp->tm_mday);
    string last_logged_date = LastLoggedDate();

    power.SetLogVector(ReadHoursFile());
    if (curr_date != last_logged_date) {
        UpdateDaysLogFile(last_logged_date, power.TotalEnergyUsage());
        power.ResetLogVector();
    }
    
    power.SetExtra(power.HoursEnergyUsages(hour));
    int curr_hour;

    while (true) {
        sleep(1);
        now = time(0);
        tmp = gmtime(&now);
        curr_hour = tmp->tm_hour;
        curr_date = FormatDate(tmp->tm_year + 1900, tmp->tm_mon + 1, tmp->tm_mday);
        last_logged_date = LastLoggedDate();

        if (curr_date != last_logged_date) {
            UpdateDaysLogFile(last_logged_date, power.TotalEnergyUsage());
            power.ResetLogVector();
        } else {
            power.SetLogVector(ReadHoursFile());
        }

        if (curr_hour != hour) {
            power.UpdatePrevHoursEnergy();
            power.SetExtra(0);
            hour = curr_hour;
        }

        power.UpdatePowerAndEnergyUsage();
        power.UpdateLogVector(hour);
        UpdateHoursLogFile(curr_date, power.HoursEnergyUsages());

    }
}

string System::FormatDate(int year, int mon, int day) {
    string year_str = std::to_string(year);
    string mon_str = mon > 9 ? std::to_string(mon) : '0' + std::to_string(mon);
    string day_str = day > 9 ? std::to_string(day) : '0' + std::to_string(day);
    string formated_date = year_str + '/' + mon_str + '/' + day_str;
    return formated_date;
}

string System::LastLoggedDate() {
    string cmd = "head -1 " + hours_log_file + " | tr -d '\\n'";
    string last_logged_date = raymii::Command::exec(cmd).output;
    return last_logged_date;
}

void System::UpdateDaysLogFile(string last_logged_date, double total_usage) {
    std::ofstream out(days_log_file, std::ios::app);
    out << last_logged_date << ',' << total_usage << '\n';
    out.close();
}

void System::UpdateHoursLogFile(string curr_date, vector<double> usages) {
    std::ofstream out(hours_log_file);
    out << curr_date << '\n';
    int h = 0;
    double usage = 0;
    for (h = 0 ; h < 24 ; h++) {
        usage = usages[h];
        out << h << ',' << usage << '\n';
    }
    out.close();
}

vector<double> System::ReadHoursFile() {
    std::ifstream in(hours_log_file);
    vector<double> datas;
    string row;
    if (in.is_open()) {
        getline(in, row);
        for (int h = 0 ; h < 24 ; h++) {
            getline(in, row);
            datas.push_back(stod(row.substr(row.find(",") + 1)));
        }
    }
    in.close();

    return datas;
}

vector<string> System::ReadDaysFile() {
    vector<string> rows;
    std::ifstream in(days_log_file);
    string row;
    while (getline(in, row)) {
        rows.push_back(row);
    }
    in.close();

    return rows;
}
