#include <string>
#include <vector>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <bits/stdc++.h>

#include "view.h"

using namespace std;

void View::DisplaySystemInfo() {
    for (int i = 0 ; i < 15 ; i++, sleep(1)) {
        system("clear");
        cout << fixed << setprecision(1);
        cout << "\n";
        cout << "\t\t\t -- System Infomation --\n\n";

        cout << "Operation System: " << system_->OperatingSystem() << endl;
        cout << "Kernel: " << system_->Kernel() << endl;
        cout << endl;
        cout << "Total Processes: " << system_->TotalProcesses() << endl;
        cout << "Running Processes: " << system_->RunningProcesses() << endl;
        cout << "Up Time: " << system_->UpTime() << endl;
        cout << endl;
        cout << "Total Memory: " << system_->TotalMemory() / 1024 / 1024  << " Gb\n";
        cout << "Used Memory: " << system_->UsedMemory() / 1024 / 1024 << " Gb\n";
        cout << "Memory Utilization: " << system_->MemoryUtilisation() * 100.0 << "%\n";
        cout << endl;

        std::vector<float> cpu_utilisations = system_->CpuUtilisations();
        if (!cpu_utilisations.empty()) {
            cout << "CPU: " << cpu_utilisations[0] * 100.0 << "%" << "\t" << system_->CpuTemperature() << "C" << endl;
            for (int i = 1 ; i < cpu_utilisations.size() ; i++) {
                cout << "Core " << i - 1 << ": " << cpu_utilisations[i] * 100.0 << "%" << endl;
            } 
        }
        cout << endl;
    }

    cin.get();
    cin.get();
}

void View::DisplayProcesses() {
    for (int i = 0 ; i < 15 ; i++, sleep(2)) {
        system("clear");
        cout << fixed << setprecision(1);
        cout << "\n";
        cout << "\t\t\t -- Processes --\n\n";
        std::vector<Process> sorted_processes = system_->SortedProcesses();
        if (!sorted_processes.empty()) {
            for (int k = 0 ; k < 10 ; k++) {
                cout << sorted_processes[k].Pid() << "\t" << sorted_processes[k].CpuUtilisation() * 100 << "%\n";
            }
        }
        cout << endl;
    }

    cin.get();
    cin.get();
}

void View::DisplayTodaysEnergyUsage() {
    for (int i = 0 ; i < 20 ; i++, sleep(1)) {
        system("clear");
        cout << fixed << setprecision(1);
        cout << "\n";
        cout << "\t\t\t -- Hourly Energy Usage --\n\n";
        cout << "Hour\tEnergy (in Wh)\n";
        std::vector<double> hours_energy_usage = power_->HoursEnergyUsages();
        for (int h = 0 ; h < 24 ; h++) {
            cout << h << "\t" << hours_energy_usage[h] << endl;
        }
    }

    cin.get();
    cin.get();
}

void View::DisplayLastWeekEnergyUsage() {
    cout << fixed << setprecision(1);
    cout << "\n";
    cout << "\t\t\t -- Last Week Energy Usage --\n\n";
    cout << "Date\t\tEnergy (in Wh)\n";
    map<string, double> last_seven_days_energy_usage = power_->LastNDaysEnergyUsage(7);
    for (auto const& [key, val] : last_seven_days_energy_usage) {
        cout << key << ": " << val << "Wh" << endl;
    }

    cin.get();
    cin.get();
}

void View::DisplayLivePowerUsage() {
    for (int i = 0 ; i < 20 ; i++, sleep(1)) {
        system("clear");
        cout << fixed << setprecision(1);
        cout << "\n";
        cout << "\t\t\t -- Live Power Usage --\n\n";
        cout << power_->CurrPowerUsage() << " watts" << endl;
    }

    cin.get();
    cin.get();
}  

void View::PowerUsageSelect() {
    int choice = 0;
    char choice_char[3];

    system("clear");
    cout << "\n";
    cout << "\t\t\t -- Select -- \n\n";
    cout << "1. Hourly energy usage for today\n";
    cout << "2. Energy usage in last seven days\n";
    cout << "3. Live power usage\n\n";

    cout << "Enter Option: ";

    cin >> ("%s", choice_char);
    stringstream tmpcnvt (choice_char);
    tmpcnvt >> choice;

    system("clear");
    switch (choice){
        case 1:
            DisplayTodaysEnergyUsage();
            break;
        case 2:
            DisplayLastWeekEnergyUsage();
            break;
        case 3:
            DisplayLivePowerUsage();
            break;
        default:
            cout << "Invalid Choice - Re-Enter / Exit";
            break;
    }
}

void View::PowerModeSelect() {
    int int_choice = 0;
    char char_choice[3];

    system("clear");
    cout << ("\n");
    cout << ("\t\t\t -- Performance Mode Select -- \n\n");
    cout << ("1. All cores \n");
    cout << ("2. P cores\n");
    cout << ("3. E cores\n");
    cout << ("\n\n0. Exit\n");

    cout << ("Enter Option: ");
    cin >> ("%s", char_choice);
    stringstream tmpcnvt (char_choice);
    tmpcnvt >> int_choice;

    switch(int_choice){
        case 1:
            system("clear");
            system_->BindToAllCores();
            cout << "Balanced Mode (Binding to all cores...)" << endl;
            break;
        case 2:
            system("clear");
            system_->BindToPCores();
            cout << "High Performance Mode (Binding to P-cores...)" << endl;
            break;
        case 3:
            system("clear");
            system_->BindToECores();
            cout << "Power Saver Mode (Binding to E-cores...)" << endl;
            break;
        case 0:
            exit(1);
            break;
        default:cout << "Invalid Choice - Re-Enter / Exit";
        break;
    }

    cin.get();
    cin.get();
}

void View::ServiceSelect() {
    int choice = 0;
    char choice_char[3];

    do {
        system("clear");
        cout << "\n";
        cout << "\t\t\t -- Select Service -- \n\n";
        cout << "1. System information\n";
        cout << "2. Running processes\n";
        cout << "3. Energy usage\n";
        cout << "4. Bind processes to cores\n";
        cout << "\n\n0. Exit\n";

        cout << "Enter Option: ";
        cin >> ("%s", choice_char);
        stringstream tmpcnvt (choice_char);
        tmpcnvt >> choice;

        system("clear");
        switch (choice){
            case 1:
                DisplaySystemInfo();
                break;
            case 2:
                DisplayProcesses();
                break;
            case 3:
                PowerUsageSelect();
                break;
            case 4:
                PowerModeSelect();
                break;
            case 0:
                exit(1);
                break;
            default:
                cout << "Invalid Choice - Re-Enter / Exit";
                break;
        }

    } while (choice != 0);

}