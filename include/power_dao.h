#ifndef POWER_DAO_H
#define POWER_DAO_H

#include <string>
#include <vector>
#include <map>

/**
 * This class is responsible for accessing data in power logging files.
*/
class PowerDAO {

    public:
        /**
         * Create and initiate the hours log file when the application is used the first time.
         * @param curr_date The current date.
         * @param usages The hourly energy usages.
         */
        void InitHoursLogFile(std::string curr_date, std::vector<double> usages);

        /**
         * Create and initiate the days log file when the application is used the first time.
         */
        void InitDaysLogFile();

        /**
         * Get the last date that has been logged in the file.
         * @return The last logged date.
        */
        std::string LastLoggedDate();

        /**
         * Append the total energy usage drawn in last day to the logging file.
         * @param last_logged_date The last logged date.
         * @param total_usage The total energy usage in Wh drawn in last day.
        */
        void UpdateDaysLogFile(std::string last_logged_date, double total_usage);

        /**
         * Update the hours logging file.
         * @param curr_date The current date.
         * @param usages The current status of energy usages in the given day.
        */
        void UpdateHoursLogFile(std::string curr_date, std::vector<double> usages);

        /**
         * Read datas from the hours log file.
         * @return The hourly energy usages read from the logging file.
        */
        std::vector<double> HoursEnergyUsages();

        /**
         * Read datas from the days log file.
         * @param n The last n days.
         * @return The date and corresponding energy usage.
        */
        std::map<std::string, double> LastNDaysEnergyUsage(int n);

    private:
        static const std::string HOURS_LOG_FILE;
        static const std::string DAYS_LOG_FILE;
};

#endif
