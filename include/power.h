#ifndef POWER_H
#define POWER_H

#include <string>
#include <vector>
#include <map>

using std::string;
using std::vector;
using std::map;

/**
 * This class represents a computer power monitor and provides methods 
 * to retrieve and update information about power and energy usage.
*/
class Power {
    public:
        Power();

        /**
         * Getter method for current hour energy usage.
         * @return The energy usage in Wh in current hour.
        */
        double CurrHourEnergyUsage();

        /**
         * Getter method for current power usage.
         * @return The real-time power usage in watts.
        */
        double CurrPowerUsage();

        /**
         * Get the total energy usage drawn in the current date.
         * @return The total energy usage in Wh.
        */
        double TotalEnergyUsage();

        /**
         * Getter method for hourly energy usages in the current day.
         * @return The vector containing energy usage in Wh in every hour.
        */
        vector<double> HoursEnergyUsages();

        /**
         * Getter method for energy usage in particular hour.
         * @param hour The given hour.
         * @return The amount of energy usage in Wh in the given hour.
        */
        double HoursEnergyUsages(int hour);

        /**
         * Reset vector for logging hourly energy usage.
        */
        void ResetLogVector();

        /**
         * Setter method for hourly energy usages.
         * @param datas The vector that will be assigned to log vector.
        */
        void SetLogVector(vector<double> datas);

        /**
         * Update energy usage in a particular hour.
         * @param hour The given hour.
        */
        void UpdateLogVector(int hour);

        /**
         * Get the energy usage drawn in the last n days.
         * @param rows The unparsed rows read from logging files.
         * @param n The last n days.
         * @return The date and corresponding energy usage.
        */
        map<string, double> LastNDaysEnergyUsage(vector<string> rows, int n);

        /**
         * Update the energy used in previous hours.
        */
        void UpdatePrevHoursEnergy();

        /**
         * Setter method for extra.
         * @param value The value will be assigned to extra.
        */
        void SetExtra(double value);

        /**
         * Update power and energy usage by reading from the system.
        */
        void UpdatePowerAndEnergyUsage();


    private:

        /**
         * Get the current accumulated energy usage from system.
         * @return The current accumulated energy usage in micro joules.
        */
        long long EnergyUsageInUj();
        
        string energy_usage_path = "/sys/class/powercap/intel-rapl/intel-rapl:1/energy_uj";
        string mex_energy_path = "/sys/class/powercap/intel-rapl/intel-rapl:1/max_energy_range_uj";

        // The energy amount got from the system will
        // reset to zero when it exceeds this bound
        long long max_energy;

        // Record energy usages in every hour in a day
        vector<double> hours_energy_usages;

        double curr_hour_energy_usage = 0;
        double total_energy_usage;
        double curr_power_usage = 0;

        // The total energy usage (in uj) in the previous hours 
        // since the pc boots
        long long prev_hours_energy = 0;
        // The total energy usage since the pc boots
        long long accum_energy_usage = 0;
        // The current energy amount extracted from the system
        long long energy = 0;
        // The energy amount extracted in the last logging time
        long long prev_energy = 0;
        // The number of times when the capped energy amount 
        // is reached
        long long capped_times = 0;
        // The extra energy usage in the current hour 
        // (in case the pc reboots)
        double extra = 0;
};

#endif