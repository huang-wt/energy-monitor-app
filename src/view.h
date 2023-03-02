#ifndef VIEW_H
#define VIEW_H

#include <string>
#include "system.h"
#include "power.h"

class View {

    public:
        View(System &sysMonitor);
        View(System &sysMonitor, const Power &powerMonitor);
        void serviceSelect();

    private:
        System sysMonitor;
        Power powerMonitor;

        void displaySystemInfo();
        void displayProcesses();
        void displayTodaysEnergyUsage();
        void displayLastWeekEnergyUsage();
        void displayLivePowerUsage();
        void powerUsageSelect();
        void powerModeSelect();

};

#endif