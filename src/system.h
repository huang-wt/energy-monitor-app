#ifndef SYSTEM_H
#define STSTEM_H

#include <string>
#include <vector>
#include <map>

#include "processor.h"
#include "memory.h"
#include "process.h"

class System {

    public:
        /**
         * Static method that thread-safely provides a single
         * instance of the class.
         * @return The single instance of System class.
        */
        static System* Instance();

        /**
         * Getter method for operating system.
         * @return The type of the operating system.
        */
        std::string OperatingSystem();

        /**
         * Getter method for kernel.
         * @return The kernel of the operating system.
        */
        std::string Kernel();

        /**
         * Getter method for uptime.
         * @return The uptime of the system.
        */
        long UpTime();

        /**
         * Getter method for total processes.
         * @return The number of total processes.
        */
        int TotalProcesses();

        /**
         * Getter method for running processes.
         * @return The number of running processes.
        */
        int RunningProcesses();

        /**
         * Getter method for total memory.
         * @return The amount of system total memory in Kb.
        */
        float TotalMemory();

        /**
         * Getter method for used memory.
         * @return The amount of current used memory in Kb.
        */
        float UsedMemory();

        /**
         * Getter method for memory utilisation as a percentage.
         * @return The live memory utilisation as a percentage.
        */
        float MemoryUtilisation();

        /**
         * Getter method for cpu temperature.
         * @return The cpu temperature in degree Celsius.
        */
        int CpuTemperature();

        /**
         * Getter method for cpu utilisations.
         * @return The vector containing the utilisation of each cpu core.
        */
        std::vector<float> CpuUtilisations();

        /**
         * Getter method for processes in descending order on cpu usage.
         * @return The sorted vector containing all processes.
        */
        std::vector<Process> SortedProcesses();

        /**
         * Update cpu utilisations & temperature and memory usage.
        */
        void UpdateCpuAndMemory();

        /**
         * Update the processes.
        */
        void UpdateProcesses();

        /**
         * Bind the most cpu consuming processes to performance cores.
        */
        void BindToPCores();

        /**
         * Bind the most cpu consuming processes to all cores.
        */
        void BindToAllCores();

        /**
         * Bind the most cpu consuming processes to efficiency cores.
        */
        void BindToECores();

    private:
        System();

        /**
         * Get processes whose cpu utilisation is greater than 1%.
         * @return The ids of eligible processes.
        */
        std::vector<int> CpuConsumingProcesses();

        /**
         * Bind the given processes to some cpu cores.
         * @param pids The ids of selected processes.
         * @param low The lower bound of binded cores.
         * @param high The upper bound of binded cores.
        */
        void BindProcesses(std::vector<int> pids, int low, int high);
    
        static System* instance;
        std::string operating_system;
        std::string kernel;
        Processor cpu;
        Memory memory;
        std::map<int, Process> processes;
};

#endif // SYSTEM_H
