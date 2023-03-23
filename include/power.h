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
         * @return The energy usage in Wh drawn in current hour.
        */
        double CurrHourEnergyUsage();

        /**
         * Getter method for current power usage.
         * @return The real-time power usage in Watts.
        */
        double CurrPowerUsage();

        /**
         * Get the total energy usage drawn in the current date.
         * @return The total energy usage in Wh.
        */
        double TotalEnergyUsage();

        /**
         * Getter method for hourly energy usages in current day.
         * @return The vector containing energy usage in Wh drawn in every hour.
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
         * @param datas The vector containing updated values of energy usages.
        */
        void SetLogVector(vector<double> datas);

        /**
         * Update energy usage in a particular hour.
         * @param hour The given (current) hour.
        */
        void UpdateLogVector(int hour);

        /**
         * Update the total energy usage drawn in previous hours.
        */
        void UpdatePrevHoursEnergy();

        /**
         * Setter method for extra.
         * @param value The value of extra.
        */
        void SetExtra(double value);

        /**
         * Update power and energy usage.
        */
        void UpdatePowerAndEnergyUsage();

    private:
        /**
         * Get the current energy counter from system.
         * @return The current energy counter in micro joules.
        */
        long long EnergyUsageInUj();
        
        // The range of the energy counter
        static const long long MAX_ENERGY;

        vector<double> hours_energy_usages;
        double curr_hour_energy_usage = 0;
        double total_energy_usage = 0; // in a day
        double curr_power_usage = 0;

        // total energy usage (in uj) in the previous hours
        // since the system is booted
        long long prev_hours_energy = 0;

        // total energy usage since the pc is booted
        long long accum_energy_usage = 0;

        // current energy counter extracted from the system file
        long long energy = 0;

        // energy counter extracted in the last logging time
        long long prev_energy = 0;

        // number of times when the capped energy amount
        // is reached
        long long capped_times = 0;

        // The extra energy usage in the current hour 
        // (in case the pc is rebooted)
        double extra = 0;
};

#endif
