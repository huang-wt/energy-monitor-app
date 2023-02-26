#ifndef VIEW_H
#define VIEW_H

#include <string>
#include "system.h"

class View {

    public:
        View(System &sysMonitor);
        // void displayStaticData();
        // void displayDynamicData();
        // void displayMemoryInfo();
        // void displayCpuUtilizations();
        void displaySystemInfo();
        void displayProcesses();
        // void systemDataSelect();
        std::string processSelect();
        void generalCoreSelect();
        void serviceSelect();

    private:
        System sysMonitor;
};

#endif