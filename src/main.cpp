#include <string>
#include <vector>
#include <iostream>
#include <thread>
#include <unistd.h>

#include "linux_parser.h"
#include "processor.h"

using namespace std;


void displayStaticData() {
    cout << "\t\t\t -- Static Data --\n\n";

    string os = LinuxParser::OperatingSystem();
    cout << "Operation System: " << os << endl;

    string kernel = LinuxParser::Kernel();
    cout << "Kernel: " << kernel << endl; 

    int cpuNum = LinuxParser::CpuCoresCount();
    cout << "Cores number: " << cpuNum << endl;

    cin.get();
    cin.get();
}

void displayDynamicData() {
    cout << "\t\t\t -- Dynamic Data --\n\n";

    for (int i = 0 ; i < 5 ; i++, sleep(1)) {
        int totalProcesses = LinuxParser::TotalProcesses();
        cout << "Total Processes: " << totalProcesses << endl;

        int runningProcesses = LinuxParser::RunningProcesses();
        cout << "Running Processes: " << runningProcesses << endl;

        long upTime = LinuxParser::UpTime();
        cout << "Up Time: " << upTime << endl;
        // * 100.0 

        cout << endl;
    }

    cin.get();
    cin.get();
}

void displayMemoryInfo() {
    cout << "\t\t\t -- Memory Information --\n\n";

    vector<float> memoryInfo;
    float totalMemory, usedMemory;
    for (int i = 0 ; i < 10 ; i++, sleep(1)) {
        memoryInfo = LinuxParser::MemoryInfo();
        totalMemory = memoryInfo[0];
        usedMemory = totalMemory - memoryInfo[2];
        cout << "Total Memory: " << totalMemory << " KB\n";
        cout << "Used Memory: " << usedMemory << " KB\n";
        cout << "Memory Utilization: " << usedMemory / totalMemory * 100.0 << "%\n";

        cout << endl;
    }

    cin.get();
    cin.get();
}

void displayCpuUtilizations() {
    cout << "\t\t\t -- CPU Utilizations --\n\n";

    for (int i = 0 ; i < 10 ; i++, sleep(1)) {
        vector<float> cpuUtilizations = Processor::Utilizations();
        for (int i = 0 ; i < cpuUtilizations.size() ; i++) {
            cout << "Core " << i - 1 << ": " << cpuUtilizations[i] * 100.0 << "%" << endl;

        } 

        cout << endl;
    }

    cin.get();
    cin.get();
}

void systemDataSelect() {
    int intChoice = 0;
    char charChoice[3];

    do {
        system("clear");
        cout << "\n";
        cout << "\t\t\t -- Select System Data -- \n\n";
        cout << "1. Static data\n";
        cout << "2. Dynamic data\n";
        cout << "3. Memory information\n";
        cout << "4. CPU utilization\n";
        cout << "\n\n0. Exit\n";

        cout << "Enter Option: ";
        cin >> ("%s", charChoice);
        stringstream tmpcnvt (charChoice);
        tmpcnvt >> intChoice;

        system("clear");
        switch (intChoice){
            case 1:
                displayStaticData();
                break;
            case 2:
                displayDynamicData();
                break;
            case 3:
                displayMemoryInfo();
                break;
            case 4:
                displayCpuUtilizations();
                break;
            case 0:
                exit(1);
                break;
            default:
                cout << "Invalid Choice - Re-Enter / Exit";
                break;
        }


    } while (intChoice != 0);

}

int main() {

    systemDataSelect();
    
}
