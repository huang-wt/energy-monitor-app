#include <iostream>
#include <fstream>
#include <ctime>
#include <unistd.h>
#include <vector>

#include "power_monitor.h"
#include "command.h"

using namespace std;
using namespace raymii;

int getUsage() {
    string cmd = "cat /sys/class/power_supply/BAT1/power_now";
    string res = Command::exec(cmd).output;
    return stod(res);
}

int main() {

    string fileName = "power_usage.csv";

    time_t now = time(0);
    struct tm *tmp = gmtime(&now);
    int hour = tmp->tm_hour;
    int day = tmp->tm_mday;
    int mon = tmp->tm_mon + 1;
    int year = tmp->tm_year + 1900;

    vector<double> hourlyPowerUsage;

    ifstream inputFile;
    inputFile.open(fileName);
    string row;
    if (inputFile.is_open()) {
        getline(inputFile, row);
        while (getline(inputFile, row)) {
            hourlyPowerUsage.push_back(stod(row.substr(row.find(",") + 1)));
        }
    }

    double prevHoursUsage = 0;
    double accumUsage = 0;
    double currHourUsage = 0;
    int currHour = hour;
    double extra = hourlyPowerUsage[hour];
    double total = 0;

    while (1) {
        sleep(5 * 60);
        now = time(0);
        currHour = gmtime(&now)->tm_hour;

        if (currHour != hour) {
            prevHoursUsage = accumUsage;
            extra = 0;
            hour = currHour;
        }

        accumUsage = getUsage();
        currHourUsage = accumUsage - prevHoursUsage + extra;
        hourlyPowerUsage[hour] = currHourUsage;

        ofstream outputFile;
        outputFile.open(fileName);
        int i = 0;
        for (i = 0 ; i < hourlyPowerUsage.size() ; i++) {
            total += hourlyPowerUsage[i];
            outputFile << i << ',' << hourlyPowerUsage[i] << '\n';
        }
        outputFile << i << ',' << total << '\n';
    }
}
