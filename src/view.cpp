#include <string>
#include <vector>
#include <iostream>
#include <thread>
#include <unistd.h>

#include "linux_parser.h"
#include "processor.h"
#include "process.h"
#include "bind.h"
#include "view.h"

using namespace std;


void View::displayStaticData() {
    cout << "\n";
    cout << "\t\t\t -- Static Data --\n\n";

    string os = LinuxParser::operatingSystem();
    cout << "Operation System: " << os << endl;

    string kernel = LinuxParser::kernel();
    cout << "Kernel: " << kernel << endl; 

    int cpuNum = LinuxParser::cpuCoresCount();
    cout << "Cores number: " << cpuNum << endl;

    cin.get();
    cin.get();
}

void View::displayDynamicData() {
    cout << "\n";
    cout << "\t\t\t -- Dynamic Data --\n\n";

    for (int i = 0 ; i < 5 ; i++, sleep(1)) {
        int totalProcesses = LinuxParser::totalProcesses();
        cout << "Total Processes: " << totalProcesses << endl;

        int runningProcesses = LinuxParser::runningProcesses();
        cout << "Running Processes: " << runningProcesses << endl;

        long upTime = LinuxParser::upTime();
        cout << "Up Time: " << upTime << endl;
        // * 100.0 

        cout << endl;
    }

    cin.get();
    cin.get();
}

void View::displayMemoryInfo() {
    cout << "\n";
    cout << "\t\t\t -- Memory Information --\n\n";

    vector<float> memoryInfo;
    float totalMemory, usedMemory;
    for (int i = 0 ; i < 10 ; i++, sleep(1)) {
        memoryInfo = LinuxParser::memoryInfo();
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

void View::displayCpuUtilizations() {
    cout << "\n";
    cout << "\t\t\t -- CPU Utilizations --\n\n";

    for (int i = 0 ; i < 10 ; i++, sleep(1)) {
        vector<float> cpuUtilizations = Processor::utilizations();
        for (int i = 0 ; i < cpuUtilizations.size() ; i++) {
            cout << "Core " << i - 1 << ": " << cpuUtilizations[i] * 100.0 << "%" << endl;

        } 

        cout << endl;
    }

    cin.get();
    cin.get();
}

void View::displayProcesses() {
    cout << "\n";
    cout << "\t\t\t -- Processes --\n\n";

    vector<int> pids;
    map<int, Process> processes;
    for (int i = 0 ; i < 100 ; i++, sleep(1)) {
        // Initialize/Update map
        pids = LinuxParser::pids();
        for (int pid : pids) {
            Process p;
            p.pid(pid);
            p.command(pid);
            processes.insert(make_pair(pid, p));
        }

        // Update cpu utilization for each process
        long totalJiffies = Processor::jiffies(-1);
        for (int j = 0 ; j < processes.size() ; j++) {
            Process p = processes[j];
            p.cpuUtilization(p.pid(), totalJiffies);
            processes[j] = p;
        }

        // Sorting in descending order
        vector<Process> processesVec;
        map<int, Process> :: iterator it;
        for (it = processes.begin() ; it != processes.end() ; it++) {
            processesVec.push_back(it->second);
        }
        std::sort(processesVec.begin(), processesVec.end(), std::greater<Process>());
        for (int k = 0 ; k < 10 ; k++) {
            cout << processesVec[k].pid() << " " << processesVec[k].cpuUtilization() << endl;
        }
        cout << endl;
    }

    cin.get();
    cin.get();
}

void View::systemDataSelect() {
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
        cout << "5. Processes\n";
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
            case 5:
                displayProcesses();
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

string View::processSelect() {
    system("clear");
    bool found = false;
    cout << "\n";
    cout << "\t\t\t -- Select Program -- \n\n";
    string word;
    cout << "Enter a program to target: ";
    cin >> word;

    return word;
}

void View::generalCoreSelect() {
    int int_choice = 0;
    char char_choice[3];
    int lCore, eCore, pCore, hyperThreads;
    string word;
    vector <string> process_ID;
    vector <int> logicalCoresBounds;

    system("clear");
    cout << ("\n");
    cout << ("\t\t\t -- Cores Select -- \n\n");
    cout << ("1. All Cores \n");
    cout << ("2. P-Cores\n");
    cout << ("3. E-Cores\n");
    cout << ("\n\n0. Exit\n");

    cout << ("Enter Option: ");
    cin >> ("%s", char_choice);
    stringstream tmpcnvt (char_choice);
    tmpcnvt >> int_choice;

    logicalCoresBounds = Bind::getLogicalCoresBounds();
    lCore = logicalCoresBounds[0];
    eCore = logicalCoresBounds[1];
    pCore = logicalCoresBounds[2];
    hyperThreads = logicalCoresBounds[3];

    
    switch(int_choice){
        case 1:
             system("clear");
             word = processSelect();
             process_ID = Bind::findProcessID(word);
             Bind::findAndBind(process_ID, 0, lCore-1);
             break;
        case 2:
             system("clear");
             word = processSelect();
             process_ID = Bind::findProcessID(word);
             Bind::findAndBind(process_ID, 0, hyperThreads-1);
             break;
        case 3:
             system("clear");
             word = processSelect();
             process_ID = Bind::findProcessID(word);
             Bind::findAndBind(process_ID, hyperThreads, lCore-1);
             break;
        case 0:
            exit(1);
            break;
        default:cout << "Invalid Choice - Re-Enter / Exit";
        break;
    } 
}

void View::serviceSelect() {
    int intChoice = 0;
    char charChoice[3];

    do {
        system("clear");
        cout << "\n";
        cout << "\t\t\t -- Select Service -- \n\n";
        cout << "1. Monitor system information\n";
        cout << "2. General Binds\n";
        cout << "\n\n0. Exit\n";

        cout << "Enter Option: ";
        cin >> ("%s", charChoice);
        stringstream tmpcnvt (charChoice);
        tmpcnvt >> intChoice;

        system("clear");
        switch (intChoice){
            case 1:
                systemDataSelect();
                break;
            case 2:
                generalCoreSelect();
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