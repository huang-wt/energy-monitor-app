#include "power.h"

#include "command.h"

Power::Power() {
    //Initialise the logging vector and max energy
    for (int i = 0 ; i < 24 ; i++) {
        hours_energy_usages.push_back(0);
    }

    string cmd = "cat " + mex_energy_path;
    max_energy = std::stoll(raymii::Command::exec(cmd).output, 0, 10);
}

double Power::CurrHourEnergyUsage() {
    return curr_hour_energy_usage;
}

double Power::CurrPowerUsage() {
    return curr_power_usage;
}

double Power::TotalEnergyUsage() {
    double total_usage = 0;
    for (double d : hours_energy_usages) {
        total_usage += d;
    }

    return total_usage;
}

vector<double> Power::HoursEnergyUsages() {
    return hours_energy_usages;
}

double Power::HoursEnergyUsages(int hour) {
    return hours_energy_usages[hour];
}

void Power::ResetLogVector() {
    if (hours_energy_usages.size() != 24) {
        return;
    }

    for (int h = 0 ; h < 24 ; h++) {
        hours_energy_usages[h] = 0;
    }
}

void Power::SetLogVector(std::vector<double> datas) {
    for (int h = 0 ; h < 24 ; h++) {
        hours_energy_usages[h] = datas[h];
    }
}

void Power::UpdateLogVector(int hour) {
    hours_energy_usages[hour] = curr_hour_energy_usage;
}

map<string, double> Power::LastNDaysEnergyUsage(vector<string> rows, int n) {
    int rows_count = rows.size();
    n = rows_count < n ? rows_count : n;

    string row;
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

void Power::UpdatePrevHoursEnergy() {
    prev_hours_energy = accum_energy_usage;
}

void Power::SetExtra(double value) {
    extra = value;
}

void Power::UpdatePowerAndEnergyUsage() {

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
    
}

long long Power::EnergyUsageInUj() {
    string cmd = "cat " + energy_usage_path;
    long long energy = std::stoll(raymii::Command::exec(cmd).output, 0, 10);
    return energy;
}
