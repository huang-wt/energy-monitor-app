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
        int getCpuTemp();
        std::vector<float> getCpuUtilisations();
        std::vector<Process> getSortedProcesses();
        void bindProcessesToPCores();
        void bindProcessesToAllCores();
        void bindProcessesToECores();

    private:
        Processor cpu = {};
        Memory memory = {};
        std::map<int, Process> processes = {};
        long getTotalJiffies();
        void bindProcesses(vector<int> pids, int low, int high);
        std::vector<int> getCpuConsumingPids();

};

#endif