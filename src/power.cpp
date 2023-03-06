#include "power.h"

#include <unistd.h>

#include <fstream>
#include <thread>

#include "command.h"

using std::string;
using std::vector;
using std::map;
using std::ifstream;
using std::ofstream;

Power* Power::instance = new Power();

Power* Power::Instance() {
    return instance;
}

Power::Power() {
    //Initialise the logging vector and max energy
    for (int i = 0 ; i < 24 ; i++) {
        hours_energy_usages.push_back(0);
    }

    string cmd = "cat " + mex_energy_path;
    max_energy = std::stoll(raymii::Command::exec(cmd).output, 0, 10);
}

void Power::UpdatePowerAndEnergyUsage() {
    time_t now = time(0);
    struct tm *tmp = gmtime(&now);
    int hour = tmp->tm_hour;
    string current_date = FormatDate(tmp->tm_year + 1900, tmp->tm_mon + 1, tmp->tm_mday);
    string last_logged_date = LastLoggedDate();

    if (current_date != last_logged_date) {
        UpdateDaysLogFile(last_logged_date);
        UpdateHoursLogFile(current_date);
    } else {
        UpdateLogVector();
    }

    // The total energy usage (in uj) in the previous hours since the pc boots
    long long prev_hours_energy = 0;
    // The total energy usage since the pc boots
    long long accum_energy_usage = 0;
    // The current energy amount extracted from the system
    long long energy = 0;
    // The energy amount extracted in the last logging time
    long long prev_energy = 0;
    // The number of times when the capped (max) energy amount is reached
    long long capped_times = 0;
    // The extra energy usage in the current hour (in case the pc reboots)
    double extra = hours_energy_usages[hour];
    int curr_hour;
    
    while (true) {
        sleep(1);

        now = time(0);
        tmp = gmtime(&now);
        curr_hour = tmp->tm_hour;
        current_date = FormatDate(tmp->tm_year + 1900, tmp->tm_mon + 1, tmp->tm_mday);
        last_logged_date = LastLoggedDate();

        if (current_date != last_logged_date) {
            UpdateDaysLogFile(last_logged_date);
            ResetLogVector();
            UpdateHoursLogFile(current_date);
        } else {
            UpdateLogVector();
        }

        if (curr_hour != hour) {
            prev_hours_energy = accum_energy_usage;
            extra = 0;
            hour = curr_hour;
        }

        energy = EnergyUsageInUj();
        if (energy < prev_energy) {
            capped_times += 1;
            curr_power_usage = (energy - prev_energy + max_energy) / 1000000.0;
        } else {
            curr_power_usage = (energy - prev_energy) / 1000000.0;
        }
                                         
        prev_energy = energy;
        accum_energy_usage = energy + capped_times * max_energy;
        curr_hour_energy_usage = (accum_energy_usage - prev_hours_energy) / 1000000 / 3600.0 + extra;
        
        hours_energy_usages[hour] = curr_hour_energy_usage;
        UpdateHoursLogFile(current_date);
    }

}

double Power::CurrHourEnergyUsage() {
    return curr_hour_energy_usage;
}

double Power::CurrPowerUsage() {
    return curr_power_usage;
}

double Power::TodaysTotalEnergyUsage() {
    string cmd = "tail -1 " + hours_log_file + " | cut -d ',' -f2";
    double total_usage = stod(raymii::Command::exec(cmd).output);
    return total_usage;
}

vector<double> Power::HoursEnergyUsages() {
    return hours_energy_usages;
}

map<string, double> Power::LastNDaysEnergyUsage(int n) {
    vector<string> rows;
    ifstream in(days_log_file);
    string row;
    while (getline(in, row)) {
        rows.push_back(row);
    }

    int rows_count = rows.size();
    n = rows_count < n ? rows_count : n;

    string date;
    double energy;
    map<string, double> last_n_days_energy;
    for (int i = 0 ; i < n ; i++) {
        row = rows[rows_count - i - 1];
        date = row.substr(0, 10);
        energy = stod(row.substr(11));
        last_n_days_energy[date] = energy;
    }

    return last_n_days_energy;
}

void Power::ResetLogVector() {
    if (hours_energy_usages.size() != 24) {
        return;
    }

    for (int h = 0 ; h < 24 ; h++) {
        hours_energy_usages[h] = 0;
    }
}

string Power::FormatDate(int year, int mon, int day) {
    string year_str = std::to_string(year);
    string mon_str = mon > 9 ? std::to_string(mon) : '0' + std::to_string(mon);
    string day_str = day > 9 ? std::to_string(day) : '0' + std::to_string(day);
    string formated_date = year_str + '/' + mon_str + '/' + day_str;
    return formated_date;
}

string Power::LastLoggedDate() {
    string cmd = "head -1 " + hours_log_file + " | tr -d '\\n'";
    string last_logged_date = raymii::Command::exec(cmd).output;
    return last_logged_date;
}

void Power::UpdateDaysLogFile(string date) {
    double total_usage = TodaysTotalEnergyUsage();
    ofstream out(days_log_file, std::ios::app);
    out << date << ',' << total_usage << '\n';
    out.close();
}

long long Power::EnergyUsageInUj() {
    string cmd = "cat " + energy_usage_path;
    long long energy = std::stoll(raymii::Command::exec(cmd).output, 0, 10);
    return energy;
}

void Power::UpdateHoursLogFile(string current_date) {
    double total_usage = 0;
    ofstream out(hours_log_file);
    out << current_date << '\n';
    int h = 0;
    for (h = 0 ; h < 24 ; h++) {
        out << h << ',' << hours_energy_usages[h] << '\n';
        total_usage += hours_energy_usages[h];
    }
    out << h << ',' << total_usage;
    out.close();
} 

void Power::UpdateLogVector() {
    ifstream in(hours_log_file);
    string row;
    if (in.is_open()) {
        getline(in, row);
        for (int h = 0 ; h < 24 ; h++) {
            getline(in, row);
            hours_energy_usages[h] = stod(row.substr(row.find(",") + 1));
        }
    }
    in.close();
}
