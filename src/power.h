#ifndef POWER_H
#define POWER_H

#include <string>
#include <vector>
#include <map>

// using namespace std::;

class Power {
    public:
        Power();
        void logPowerUsage();
        std::vector<double> getTodaysHourlyPowerUsage();
        std::map<std::string, double> getLastNDaysPowerUsage(int n);

    private:
        const std::string hoursLogFile = "../data/hours_power_usage.csv";
        const std::string daysLogFile = "../data/days_power_usage.csv";
        const std::string powerUsageFile = "/sys/class/powercap/intel-rapl/intel-rapl\\:1/energy_uj";
        std::vector<double> hourlyPowerUsage;

        void resetLogVector();
        std::string formatDate(int year, int mon, int day);
        std::string getLastLoggedDate();
        double getDailyTotalUsage();
        void updateDaysLogFile(std::string date);
        long long getEnergyUsageInJoules();
        void updateHoursLogFile(std::string currentDate);
        void updateLogVector(); 

};

#endif