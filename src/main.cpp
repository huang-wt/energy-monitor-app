#include <thread>
#include <iostream>
#include <unistd.h>

#include "display.h"

using std::thread;

void UpdatePowerAndEnergyUsage() {
    System* system_ = System::Instance();
    system_->UpdateEnergy();
}

void UpdateCpuAndMemoryUsage() {
    System* system_ = System::Instance();
    system_->UpdateCpuAndMemory();
}

// void UpdateProcesses() {
//     System* system_ = System::Instance();
//     while (true) {
//         sleep(2);
//         system_->UpdateProcesses();
//     } 
// }

int main() {
    thread t(UpdatePowerAndEnergyUsage);
    thread t1(UpdateCpuAndMemoryUsage);
    // thread t2(UpdateProcesses);

    Display display = Display();
    display.DisplayMain();

    t.join();
    t1.join();
    // t2.join();
}