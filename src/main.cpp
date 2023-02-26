#include <thread>

#include "view.h"
#include "power.h"

void logPowerUsage() {
    Power power = Power();
    power.logPowerUsage();
}

int main() {
    std::thread t(logPowerUsage);

    System sysMonitor = System();
    View view = View(sysMonitor);
    view.serviceSelect();
    
    t.join();
}
