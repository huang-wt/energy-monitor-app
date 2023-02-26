#ifndef SYSTEM_H
#define STSTEM_H

#include <string>
#include <vector>
#include <map>

#include "process.h"
#include "processor.h"
#include "memory.h"

class System {

    public:
        System();
        long getUpTime();
        int getTotalProcesses();
        int getRunningProcesses();
        std::string getKernel();
        std::string getOperatingSystem();
        int getCpuCoresCount();

        float getTotalMemory();
        float getUsedMemory();
        float getMemoryUtilisation();

        std::vector<float> getCpuUtilisations();
        long getTotalJiffies();

        std::vector<Process> getSortedProcesses();

    private:
        Processor cpu = {};
        Memory memory = {};
        std::map<int, Process> processes = {};

};

#endif