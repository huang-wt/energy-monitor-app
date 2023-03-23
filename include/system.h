#ifndef SYSTEM_H
#define STSTEM_H

#include <unistd.h>

#include <string>
#include <vector>
#include <map>

#include "include/processor.h"
#include "include/memory.h"
#include "include/process.h"
#include "include/power.h"
#include "include/power_dao.h"

using std::string;
using std::vector;
using std::map;

/**
 * This class represents a computer system, integrated with essential
 * components such as processor, memory and power. It also provides 
 * methods for retrieving essential information about the system.
*/
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
        string OperatingSystem();

        /**
         * Getter method for kernel.
         * @return The kernel of the operating system.
        */
        string Kernel();

        /**
         * Getter method for up time.
         * @return The up time of the system.
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
         * Getter method for memory utilisation.
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
        vector<float> CpuUtilisations();

        /**
         * Getter method for processes in descending order on cpu usage.
         * @return The sorted vector containing all processes.
        */
        vector<Process> SortedProcesses();

        /**
         * Getter method for live power usage.
         * @return The real-time power usage in watts.
        */
        double PowerUsage();

        /**
         * Getter method for hourly energy usages in a day.
         * @return The vector containing energy usage in Wh in every hour.
        */
        vector<double> HoursEnergyUsages();

        /**
         * Getter method for today's total energy usage.
         * @return The total energy usage in Wh drawn today.
        */
        double TotalEnergyUsage();

        /**
         * Getter method for energy usages drawn in last week.
         * @return The date and corresponding energy usage in Wh during last week.
        */
        map<string, double> LastWeekEnergyUsage();

        /**
         * Getter method for total energy usage drawn in last week.
         * @return The total energy usage in Wh in last week.
        */
        double TotalEnergyUsageLastWeek();

        /**
         * Bind the most cpu-consuming processes to performance cores.
        */
        void BindToPCores();

        /**
         * Bind the most cpu-consuming processes to all cores.
        */
        void BindToAllCores();

        /**
         * Bind the most cpu-consuming processes to efficiency cores.
        */
        void BindToECores();

        /**
         * Bind the most cpu-consuming processes to 1/2 p-cores and 1/2 e-cores.
         */
        void BindToPAndECores();

        /**
         * Setter method for energy cap.
         * @param cap The value of energy cap.
         */
        void SetEnergyCap(double cap);

        /**
         * Getter method for energy cap.
         * @return The value of energy cap in Wh.
         */
        double EnergyCap();

    private:
        System();

        /**
         * Update the processes.
        */
        void UpdateProcesses();

        void UpdateEnergy();

        void SetUpEnergyDataAndFiles();

        /**
         * Get processes whose cpu utilisation is greater than 1%.
         * @return The ids of eligible processes.
        */
        vector<int> CpuConsumingProcesses();

        /**
         * Bind the given processes to some cpu cores.
         * @param pids The ids of selected processes.
         * @param low The lower bound of binded cores.
         * @param high The upper bound of binded cores.
        */
        void BindProcesses(vector<int> pids, int low, int high);

        static System* instance;

        Processor cpu;
        Memory memory;
        map<int, Process> processes;
        string operating_system;
        string kernel;

        Power power;
        PowerDAO dao;
        double energy_cap = 500; // by default in Wh

        int hour;
};

#endif // SYSTEM_H
