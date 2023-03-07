#ifndef POWER_H
#define POWER_H

#include <string>
#include <vector>
#include <map>

class Power {
    public:
        /**
         * Static method that thread-safely provides a single
         * instance of the class.
         * @return The single instance of Power class.
        */
        static Power* Instance();

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
        double TodaysTotalEnergyUsage();

        /**
         * Getter method for hours_energy_usages.
         * @return The vector containing energy usage in Wh in every hour.
        */
        std::vector<double> HoursEnergyUsages();

        /**
         * Get the energy usage drawn in the last n days.
         * @param n The last n days.
         * @return The date and corresponding energy usage.
        */
        std::map<std::string, double> LastNDaysEnergyUsage(int n);

    private:
        Power();
        
        /**
         * Reset vector for logging hourly power usage.
        */
        void ResetLogVector();

        /**
         * Format the given date as YYYY/MM/DD.
         * @param year The year of a specific date.
         * @param mon The month of a specific date.
         * @param day The day of a specific date.
         * @return The formatted date in string.
        */
        std::string FormatDate(int year, int mon, int day);

        /**
         * Get the last date that has been logged in the file.
         * @return The last logged date.
        */
        std::string LastLoggedDate();

        /**
         * Append the total energy used in last day to the days logging file.
         * @param date The last logged date.
        */
        void UpdateDaysLogFile(std::string date);

        /**
         * Get the current accumulated energy usage from system.
         * @return The current accumulated energy usage in micro joules.
        */
        long long EnergyUsageInUj();

        /**
         * Update the hours logging file.
         * @param date The current date.
        */
        void UpdateHoursLogFile(std::string date);

        /**
         * Update the hours logging vector.
        */
        void UpdateLogVector();

        static Power* instance;
        std::string hours_log_file = "../data/hours_power_usage.csv";
        std::string days_log_file = "../data/days_power_usage.csv";
        std::string energy_usage_path = "/sys/class/powercap/intel-rapl/intel-rapl:1/energy_uj";
        std::string mex_energy_path = "/sys/class/powercap/intel-rapl/intel-rapl:1/max_energy_range_uj";

        // Record energy usages in every hour in a day
        std::vector<double> hours_energy_usages;

        // The energy amount got from the system will
        // reset to zero when it exceeds this bound
        long long max_energy;
        double curr_hour_energy_usage;
        double curr_power_usage;

};

#endif