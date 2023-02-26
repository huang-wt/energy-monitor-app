#include <thread>

#include "view.h"
#include "power.h"

int main() {

    // std::thread t(Power::logPowerUsage);

    System sysMonitor = System();
    View view = View(sysMonitor);
    view.serviceSelect();
    
    // t.join();
}
