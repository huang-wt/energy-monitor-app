#ifndef POWER_H
#define POWER_H

#include <string>
#include <vector>
#include <map>

using namespace std;

namespace Power {
    const string hoursLogFile = "../data/hours_power_usage.csv";
    const string daysLogFile = "../date/days_power_usage.csv";
    const string powerUsageFile = "/sys/class/power_supply/BAT1/power_now";
    vector<double> hourlyPowerUsage;

    void initLogVector();
    void resetLogVector();
    string formatDate(int year, int mon, int day);
    string getLastLoggedDate();
    double getDailyTotalUsage();
    void updateDaysLogFile(string date);
    int getPowerUsage();
    void updateHoursLogFile(string currentDate);
    void updateLogVector();
    void logPowerUsage();
    vector<double> getTodaysHourlyPowerUsage();
    map<string, double> getLastNDaysPowerUsage(int n);

}

#endif