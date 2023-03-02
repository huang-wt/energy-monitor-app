#include <string>
#include <vector>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <bits/stdc++.h>

#include "bind.h"
#include "view.h"

using namespace std;

View::View(System &sysMonitor) {
    this->sysMonitor = sysMonitor;
}

View::View(System &sysMonitor, const Power &powerMonitor) {
    this->sysMonitor = sysMonitor;
    this->powerMonitor = powerMonitor;
}

void View::displaySystemInfo() {

    for (int i = 0 ; i < 15 ; i++, sleep(1)) {
        system("clear");
        cout << fixed << setprecision(1);
        cout << "\n";
        cout << "\t\t\t -- System Infomation --\n\n";

        cout << "Operation System: " << sysMonitor.getOperatingSystem() << endl;
        cout << "Kernel: " << sysMonitor.getKernel() << endl;
        cout << "Cores number: " << sysMonitor.getCpuCoresCount() << endl;
        cout << endl;
        cout << "Total Processes: " << sysMonitor.getTotalProcesses() << endl;
        cout << "Running Processes: " << sysMonitor.getRunningProcesses() << endl;
        cout << "Up Time: " << sysMonitor.getUpTime() << endl;
        cout << endl;
        cout << "Total Memory: " << sysMonitor.getTotalMemory() / 1024 / 1024  << " Gb\n";
        cout << "Used Memory: " << sysMonitor.getUsedMemory() / 1024 / 1024 << " KB\n";
        cout << "Memory Utilization: " << sysMonitor.getMemoryUtilisation() * 100.0 << "%\n";
        cout << endl;

        vector<float> cpuUtilizations = sysMonitor.getCpuUtilisations();
        cout << "CPU: " << cpuUtilizations[0] * 100.0 << "%" << endl;
        for (int i = 1 ; i < cpuUtilizations.size() ; i++) {
            cout << "Core " << i - 1 << ": " << cpuUtilizations[i] * 100.0 << "%" << endl;
        } 
        cout << endl;
    }

    cin.get();
    cin.get();
}

void View::displayProcesses() {
    for (int i = 0 ; i < 15 ; i++, sleep(1)) {
        system("clear");
        cout << fixed << setprecision(1);
        cout << "\n";
        cout << "\t\t\t -- Processes --\n\n";
        vector<Process> processesSorted = sysMonitor.getSortedProcesses();
        for (int k = 0 ; k < 10 ; k++) {
            cout << processesSorted[k].getPid() << "\t" << processesSorted[k].getCpuUtilization() * 100 << "%\n";
        }
        cout << endl;
    }

    cin.get();
    cin.get();
}

void View::displayTodaysEnergyUsage() {
    for (int i = 0 ; i < 20 ; i++, sleep(1)) {
        system("clear");
        cout << fixed << setprecision(1);
        cout << "\n";
        cout << "\t\t\t -- Hourly Energy Usage --\n\n";
        cout << "Hour\tEnergy (in Wh)\n";
        vector<double> hourlyPowerUsage = powerMonitor.getTodaysHourlyPowerUsage();
        for (int h = 0 ; h < 24 ; h++) {
            cout << h << "\t" << hourlyPowerUsage[h] << endl;
        }
    }

    cin.get();
    cin.get();
}

void View::displayLastWeekEnergyUsage() {
    cout << fixed << setprecision(1);
    cout << "\n";
    cout << "\t\t\t -- Last Week Energy Usage --\n\n";
    cout << "Date\t\tEnergy (in Wh)\n";
    map<string, double> lastSevenDaysPowerUsage = powerMonitor.getLastNDaysPowerUsage(7);
    for (auto const& [key, val] : lastSevenDaysPowerUsage) {
        cout << key << ": " << val << "Wh" << endl;
    }

    cin.get();
    cin.get();
}

void View::displayLivePowerUsage() {
    for (int i = 0 ; i < 20 ; i++, sleep(1)) {
        system("clear");
        cout << fixed << setprecision(1);
        cout << "\n";
        cout << "\t\t\t -- Live Power Usage --\n\n";
        cout << powerMonitor.getCurrPowerUsage() << " watts";
    }

    cin.get();
    cin.get();
}  

void View::powerUsageSelect() {
    int intChoice = 0;
    char charChoice[3];

    system("clear");
    cout << "\n";
    cout << "\t\t\t -- Select Scope -- \n\n";
    cout << "1. Hourly energy usage for today\n";
    cout << "2. Energy usage in last seven days\n";
    cout << "3. Live power usage in watts\n\n";

    cout << "Enter Option: ";

    cin >> ("%s", charChoice);
    stringstream tmpcnvt (charChoice);
    tmpcnvt >> intChoice;

    system("clear");
    switch (intChoice){
        case 1:
            displayTodaysEnergyUsage();
            break;
        case 2:
            displayLastWeekEnergyUsage();
            break;
        case 3:
            displayLivePowerUsage();
            break;
        default:
            cout << "Invalid Choice - Re-Enter / Exit";
            break;
    }
}

// string View::processSelect() {
//     system("clear");
//     bool found = false;
//     cout << "\n";
//     cout << "\t\t\t -- Select Program -- \n\n";
//     string word;
//     cout << "Enter a program to target: ";
//     cin >> word;

//     return word;
// }

// void View::generalCoreSelect() {
//     int int_choice = 0;
//     char char_choice[3];
//     int lCore, eCore, pCore, hyperThreads;
//     string word;
//     vector <string> process_ID;
//     vector <int> logicalCoresBounds;

//     system("clear");
//     cout << ("\n");
//     cout << ("\t\t\t -- Cores Select -- \n\n");
//     cout << ("1. All Cores \n");
//     cout << ("2. P-Cores\n");
//     cout << ("3. E-Cores\n");
//     cout << ("\n\n0. Exit\n");

//     cout << ("Enter Option: ");
//     cin >> ("%s", char_choice);
//     stringstream tmpcnvt (char_choice);
//     tmpcnvt >> int_choice;

//     logicalCoresBounds = Bind::getLogicalCoresBounds();
//     lCore = logicalCoresBounds[0];
//     eCore = logicalCoresBounds[1];
//     pCore = logicalCoresBounds[2];
//     hyperThreads = logicalCoresBounds[3];

    
//     switch(int_choice){
//         case 1:
//             system("clear");
//             word = processSelect();
//             process_ID = Bind::findProcessID(word);
//             Bind::bindProcesses(process_ID, 0, lCore-1);
//             break;
//         case 2:
//             system("clear");
//             word = processSelect();
//             process_ID = Bind::findProcessID(word);
//             Bind::bindProcesses(process_ID, 0, hyperThreads-1);
//             break;
//         case 3:
//             system("clear");
//             word = processSelect();
//             process_ID = Bind::findProcessID(word);
//             Bind::bindProcesses(process_ID, hyperThreads, lCore-1);
//             break;
//         case 0:
//             exit(1);
//             break;
//         default:cout << "Invalid Choice - Re-Enter / Exit";
//         break;
//     } 
// }

void View::powerModeSelect() {
    int int_choice = 0;
    char char_choice[3];
    int lCore, eCore, pCore, hyperThreads;
    vector <int> logicalCoresBounds;

    system("clear");
    cout << ("\n");
    cout << ("\t\t\t -- Performance Mode Select -- \n\n");
    cout << ("1. Balanced \n");
    cout << ("2. High-Performance\n");
    cout << ("3. Power-saver\n");
    cout << ("4. Auto\n");
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

    vector<Process> processes = sysMonitor.getSortedProcesses();
    vector<string> pidsStr;
    for (Process p : processes) {
        if (p.getCpuUtilization() * 100 > 1) {
            pidsStr.push_back(to_string(p.getPid()));
        }
    }

    switch(int_choice){
        case 1:
            system("clear");
            Bind::bindProcesses(pidsStr, 0, lCore-1);
            break;
        case 2:
            system("clear");
            Bind::bindProcesses(pidsStr, 0, hyperThreads-1);
            break;
        case 3:
            system("clear");
            cout << 11111;
            Bind::bindProcesses(pidsStr, hyperThreads, lCore-1);
            break;
        case 4:
            system("clear");
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
        cout << "1. System information\n";
        cout << "2. Running processes\n";
        cout << "3. Energy usage\n";
        cout << "4. General Binds\n";
        cout << "\n\n0. Exit\n";

        cout << "Enter Option: ";
        cin >> ("%s", charChoice);
        stringstream tmpcnvt (charChoice);
        tmpcnvt >> intChoice;

        system("clear");
        switch (intChoice){
            case 1:
                displaySystemInfo();
                break;
            case 2:
                displayProcesses();
                break;
            case 3:
                powerUsageSelect();
                break;
            case 4:
                // generalCoreSelect();
                powerModeSelect();
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