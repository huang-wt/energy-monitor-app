#include <thread>
#include <iostream>
#include <unistd.h>

#include "view.h"

using std::thread;

void UpdatePowerAndEnergyUsage() {
    Power* power_ = Power::Instance();
    power_->UpdatePowerAndEnergyUsage();
}

void updateCpuAndMemoryUsage() {
    System* system_ = System::Instance();
    while (true) {
        sleep(1);
        system_->UpdateCpuAndMemory();
    } 
}

// void updateProcesses() {
//     System* system_ = System::Instance();
//     while (true) {
//         sleep(2);
//         system_->UpdateProcesses();
//     } 
// }

int main() {
    thread t(UpdatePowerAndEnergyUsage);
    thread t1(updateCpuAndMemoryUsage);
    // thread t2(updateProcesses);

    View view = View();
    view.ServiceSelect();

    t.join();
    t1.join();
    // t2.join();
}
