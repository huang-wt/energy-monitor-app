#include <thread>

#include "view.h"
#include "power.h"

int main() {

    // std::thread t(Power::logPowerUsage);

    View::serviceSelect();
    
    // t.join();
}
