#include <string>
#include <vector>
#include <iostream>
#include <thread>
#include <unistd.h>

#include "linux_parser.h"

using namespace std;


int main() {

    //---------static----------
    string os = LinuxParser::OperatingSystem();
    cout << "Operation System: " << os << endl;

    string kernel = LinuxParser::Kernel();
    cout << "Kernel: " << kernel << endl; 

    int cpuNum = LinuxParser::CPUCoresNumber();
    cout << "Cores number: " << cpuNum << endl;

    //---------dynamic----------
    float memUtilization = LinuxParser::MemoryUtilization();
    cout << "Memory Utilization: " << memUtilization * 100.0 << "%" << endl;

    int totalProcesses = LinuxParser::TotalProcesses();
    cout << "Total Processes: " << totalProcesses << endl;

    int runningProcesses = LinuxParser::RunningProcesses();
    cout << "Running Processes: " << runningProcesses << endl;

    long upTime = LinuxParser::UpTime();
    cout << "Up Time: " << upTime << endl;

    //---------CPU Utilizations---------
    for (int i = 0 ; i < 10 ; i++, sleep(1)) {
        vector<float> cpuUtilizations = LinuxParser::CpuUtilizations();
        for (int i = 0 ; i < cpuUtilizations.size() ; i++) {
            cout << "CPU core " << i - 1 << ": " << cpuUtilizations[i] * 100.0 << "%" << endl;

        } 
    }






}