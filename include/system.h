#ifndef SYSTEM_H
#define STSTEM_H

#include <unistd.h>

#include <string>
#include <vector>
#include <map>

#include "processor.h"
#include "memory.h"
#include "process.h"
#include "power.h"

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
         * Update cpu utilisations & temperature and memory usage.
        */
        void UpdateCpuAndMemory();

        /**
         * Getter method for processes in descending order on cpu usage.
         * @return The sorted vector containing all processes.
        */
        vector<Process> SortedProcesses();

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
         * Getter method for energy usage drawn in last week.
         * @return The pairs of date and corresponding energy usage in Wh during last week.
        */
        map<string, double> LastWeekEnergyUsage();

        /**
         * Keep updating and logging energy and power usage.
        */
        void UpdateEnergy();

    private:
        System();

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

        /**
         * Format the given date as YYYY/MM/DD.
         * @param year The year of a specific date.
         * @param mon The month of a specific date.
         * @param day The day of a specific date.
         * @return The formatted date in string.
        */
        string FormatDate(int year, int mon, int day);

        /**
         * Get the last date that has been logged in the file.
         * @return The last logged date.
        */
        string LastLoggedDate();

        /**
         * Append the total energy usage drawn in last day to the logging file.
         * @param last_logged_date The last logged date.
         * @param total_usage The total energy usage in Wh drawn in last day.
        */
        void UpdateDaysLogFile(string last_logged_date, double total_usage);

        /**
         * Update the hours logging file.
         * @param curr_date The current date.
         * @param usages The current status of energy usages in the given day.
        */
        void UpdateHoursLogFile(string curr_date, vector<double> usages);

        /**
         * Read datas from the hours logging file.
         * @return The hourly energy usages read from the logging file.
        */
        std::vector<double> ReadHoursFile();

        /**
         * Read datas from the days logging file.
         * @return The energy usages read from the logging file.
        */
        std::vector<string> ReadDaysFile();

        static System* instance;
        string hours_log_file = "../data/hours_power_usage.csv";
        string days_log_file = "../data/days_power_usage.csv";
        Processor cpu;
        Memory memory;
        map<int, Process> processes;
        Power power;
        string operating_system;
        string kernel;
        int hour;
        string curr_date;
};

#endif // SYSTEM_H
