#ifndef VIEW_H
#define VIEW_H

#include <string>

#include "system.h"
#include "power.h"

class View {

    public:
        void ServiceSelect();

    private:
        System* system_ = System::Instance();
        // Power* power_ = Power::Instance();

        void DisplaySystemInfo();
        void DisplayProcesses();
        void DisplayTodaysEnergyUsage();
        void DisplayLastWeekEnergyUsage();
        void DisplayLivePowerUsage();
        void PowerUsageSelect();
        void PowerModeSelect();

};

#endif