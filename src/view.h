#ifndef VIEW_H
#define VIEW_H

#include <string>
#include "system.h"
#include "power.h"

class View {

    public:
        void serviceSelect();

    private:
        System* sysMonitor = System::getInstance();
        Power* powerMonitor = Power::getInstance();

        void displaySystemInfo();
        void displayProcesses();
        void displayTodaysEnergyUsage();
        void displayLastWeekEnergyUsage();
        void displayLivePowerUsage();
        void powerUsageSelect();
        void powerModeSelect();

};

#endif