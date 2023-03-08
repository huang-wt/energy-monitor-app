#ifndef POWER_H
#define POWER_H

#include <iostream>
#include <string>
#include <vector>
#include <map>

class Power {
    public:
        Power();

        /**
         * Keep updating and logging power and energy usage.
        */
        void UpdatePowerAndEnergyUsage();

        /**
         * Getter method for curr_hour_energy_usage.
         * @return The energy usage in Wh in current hour.
        */
        double CurrHourEnergyUsage();

        /**
         * Getter method for curr_power_usage.
         * @return The real-time power usage in watts.
        */
        double CurrPowerUsage();

        /**
         * Get the total energy usage that is drawn in the current date.
         * @return The total energy usage in Wh.
        */
        double TotalEnergyUsage();

        /**
         * Getter method for hours_energy_usages.
         * @return The vector containing energy usage in Wh in every hour.
        */
        std::vector<double> HoursEnergyUsages();

        double HoursEnergyUsages(int hour);

        /**
         * Reset vector for logging hourly power usage.
        */
        void ResetLogVector();

        void SetLogVector(std::vector<double> datas);

        void UpdateLogVector(int hour);

        /**
         * Get the energy usage drawn in the last n days.
         * @param n The last n days.
         * @return The date and corresponding energy usage.
        */
        std::map<std::string, double> LastNDaysEnergyUsage(std::vector<std::string> rows, int n);

        void UpdatePrevHoursEnergy();

        void SetExtra(double value);


    private:

        /**
         * Get the current accumulated energy usage from system.
         * @return The current accumulated energy usage in micro joules.
        */
        long long EnergyUsageInUj();
        
        std::string energy_usage_path = "/sys/class/powercap/intel-rapl/intel-rapl:1/energy_uj";
        std::string mex_energy_path = "/sys/class/powercap/intel-rapl/intel-rapl:1/max_energy_range_uj";

        // The energy amount got from the system will
        // reset to zero when it exceeds this bound
        long long max_energy;

        // Record energy usages in every hour in a day
        std::vector<double> hours_energy_usages;

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