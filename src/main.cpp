#include <thread>
#include <iostream>
#include <unistd.h>

#include "view.h"

void logPowerUsage() {
    Power* powerMonitor = Power::getInstance();
    powerMonitor->logPowerUsage();
}

int main() {
    thread t(logPowerUsage);

    View view = View();
    view.serviceSelect();

    t.join();
}
