#include <thread>

#include "view.h"
#include "power.h"

Power power = Power();

void logPowerUsage() {
    power.logPowerUsage();
}

int main() {
    std::thread t(logPowerUsage);

    System sysMonitor = System();
    View view = View(sysMonitor, power);
    view.serviceSelect();
    
    t.join();
}
