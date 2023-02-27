#ifndef VIEW_H
#define VIEW_H

#include <string>
#include "system.h"
#include "power.h"

class View {

    public:
        View(System &sysMonitor);
        View(System &sysMonitor, Power &powerMonitor);
        void serviceSelect();

    private:
        System sysMonitor;
        Power powerMonitor;

        void displaySystemInfo();
        void displayProcesses();
        void displayTodaysPowerUsage();
        void displayLastWeekPowerUsage();
        void powerUsageSelect();
        std::string processSelect();
        void generalCoreSelect();

};

#endif