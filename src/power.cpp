#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>
#include <map>

#include "power.h"
#include "command.h"
// #include "linux_parser.h"

using namespace std;
using namespace raymii;

Power* Power::instancePtr = NULL;

Power* Power::getInstance() {
    if (instancePtr == NULL) {
        instancePtr = new Power();
    }
    return instancePtr;
}

Power::Power() {
    for (int i = 0 ; i < 24 ; i++) {
        hourlyPowerUsage.push_back(0);
    }

    maxPowerUj = std::stoll(Command::exec("cat " + maxPowerUjFile).output, 0, 10); // in micro joules
}

void Power::resetLogVector() {
    if (hourlyPowerUsage.size() != 24) {
        return;
    }

    for (int h = 0 ; h < 24 ; h++) {
        hourlyPowerUsage[h] = 0;
    }
}

// Format the date as YYYY/MM/DD
string Power::formatDate(int year, int mon, int day) {
    string yearStr = to_string(year);
    string monStr = mon > 9 ? to_string(mon) : '0' + to_string(mon);
    string dayStr = day > 9 ? to_string(day) : '0' + to_string(day);
    string formatedDate = yearStr + '/' + monStr + '/' + dayStr;

    return formatedDate;
}

string Power::getLastLoggedDate() {
    string cmd = "head -1 " + hoursLogFile + " | tr -d '\\n'";
    string lastLoggedDate = Command::exec(cmd).output;
    return lastLoggedDate;
}

double Power::getDailyTotalUsage() {
    string cmd = "tail -1 " + hoursLogFile + " | cut -d ',' -f2";
    double totalUsage = stod(Command::exec(cmd).output);
    return totalUsage;
}

void Power::updateDaysLogFile(string date) {
    double totalUsage = getDailyTotalUsage();
    ofstream out(daysLogFile, ios::app);
    out << date << ',' << totalUsage << '\n';
    out.close();
}

long long Power::getEnergyUsageInUj() {
    string cmd = "cat " + powerUsageFile;
    long long energyUsage = std::stoll(Command::exec(cmd).output, 0, 10); // in micro joules
    return energyUsage;
}

void Power::updateHoursLogFile(string currentDate) {
    double totalUsage = 0;
    ofstream out(hoursLogFile);
    out << currentDate << '\n';
    int h = 0;
    for (h = 0 ; h < 24 ; h++) {
        out << h << ',' << hourlyPowerUsage[h] << '\n';
        totalUsage += hourlyPowerUsage[h];
    }
    out << h << ',' << totalUsage;
    out.close();
} 

void Power::updateLogVector() {
    ifstream in(hoursLogFile);
    string row;
    if (in.is_open()) {
        getline(in, row);
        for (int h = 0 ; h < 24 ; h++) {
            getline(in, row);
            hourlyPowerUsage[h] = stod(row.substr(row.find(",") + 1));
        }
    }
    in.close();
}

void Power::logPowerUsage() {
    time_t now;
    struct tm *tmp;
    int secs, hour, day, mon, year;
    string currentDate, lastLoggedDate;

    now = time(0);
    tmp = gmtime(&now);
    hour = tmp->tm_hour;
    day = tmp->tm_mday;
    mon = tmp->tm_mon + 1;
    year = tmp->tm_year + 1900;
    currentDate = formatDate(year, mon, day);
    lastLoggedDate = getLastLoggedDate();

    if (currentDate != lastLoggedDate) {
        // log total power usage of last date
        updateDaysLogFile(lastLoggedDate);
        // init hourly_log_file
        updateHoursLogFile(currentDate);
    } else {
        // store data to RAM
        updateLogVector();
    }

    long long prevHoursEnergyUsage = 0; // in uj
    long long accumEnergyUsage = 0; // in uj
    long long energyUsage = 0;
    long long prevEnergyUsage = 0; // in uj
    long long capTimes = 0;
    currHourEnergyUsage = 0;
    int currHour;
    double extra = hourlyPowerUsage[hour];

    while (true) {
        this_thread::sleep_for(1000ms); //1 second

        now = time(0);
        tmp = gmtime(&now);
        currHour = tmp->tm_hour;
        day = tmp->tm_mday;
        mon = tmp->tm_mon + 1;
        year = tmp->tm_year + 1900;
        currentDate = formatDate(year, mon, day);
        lastLoggedDate = getLastLoggedDate();

        if (currentDate != lastLoggedDate) {
            // log total power usage of last date
            updateDaysLogFile(lastLoggedDate);
            // init hourly_log_file
            resetLogVector();
            updateHoursLogFile(currentDate);
        } else {
            // store data to RAM
            updateLogVector();
        }

        if (currHour != hour) {
            prevHoursEnergyUsage = accumEnergyUsage;
            extra = 0;
            hour = currHour;
        }

        energyUsage = getEnergyUsageInUj();
        if (energyUsage < prevEnergyUsage) {
            capTimes += 1;
            currPowerUsage = (energyUsage - prevEnergyUsage + maxPowerUj) / 1000000.0;
        } else {
            currPowerUsage = (energyUsage - prevEnergyUsage) / 1000000.0;
        }
                                         
        prevEnergyUsage = energyUsage;
        accumEnergyUsage = energyUsage + capTimes * maxPowerUj;

        currHourEnergyUsage = (accumEnergyUsage - prevHoursEnergyUsage) / 1000000 / 3600.0 + extra;
        
        hourlyPowerUsage[hour] = currHourEnergyUsage;
        updateHoursLogFile(currentDate);
    }

}

double Power::getCurrHourEnergyUsage() {
    return currHourEnergyUsage;
}

double Power::getCurrPowerUsage() {
    return currPowerUsage;
}

vector<double> Power::getTodaysHourlyPowerUsage() {
    return hourlyPowerUsage;
}

map<string, double> Power::getLastNDaysPowerUsage(int n) {
    vector<string> rows;

    ifstream in(daysLogFile);
    string row;
    while (getline(in, row)) {
        rows.push_back(row);
    }

    int rowsCount = rows.size();
    n = rowsCount < n ? rowsCount : n;

    string date;
    double powerUsage;
    map<string, double> lastNDaysPowerUsage;
    for (int i = 0 ; i < n ; i++) {
        row = rows[rowsCount - i - 1];
        date = row.substr(0, 10);
        powerUsage = stod(row.substr(11));
        lastNDaysPowerUsage[date] = powerUsage;
    }

    return lastNDaysPowerUsage;
}
