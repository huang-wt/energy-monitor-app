#ifndef POWER_DAO_H
#define POWER_DAO_H

#include <string>
#include <vector>

/**
 * This class is responsible for accessing data in power logging files.
*/
class PowerDAO {

    public:
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
         * Read datas from the hours logging file.
         * @return The hourly energy usages read from the logging file.
        */
        std::vector<double> ReadHoursFile();

        /**
         * Read datas from the days logging file.
         * @return The energy usages read from the logging file.
        */
        std::vector<std::string> ReadDaysFile();

    private:
        static const std::string HOURS_LOG_FILE;
        static const std::string DAYS_LOG_FILE;
};

#endif