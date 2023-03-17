#include "include/power.h"

#include "include/command.h"

#define ENERGY_CMD "cat /sys/class/powercap/intel-rapl/intel-rapl:1/energy_uj"
#define MAX_ENERGY_CMD "cat /sys/class/powercap/intel-rapl/intel-rapl:1/max_energy_range_uj"
#define HOURS 24
#define POWER_CNV_AMT 1 / 1000000.0
#define ENERGY_CNV_AMT 1 / (1000000 * 3600.0)

const long long Power::MAX_ENERGY = std::stoll(raymii::Command::exec(MAX_ENERGY_CMD).output, 0, 10);

Power::Power() {
    //Initialise the logging vector and max energy
    for (int i = 0 ; i < HOURS ; i++) {
        hours_energy_usages.push_back(0);
    }
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
    if (hours_energy_usages.size() != HOURS) {
        return;
    }

    for (int h = 0 ; h < HOURS ; h++) {
        hours_energy_usages[h] = 0;
    }
}

void Power::SetLogVector(std::vector<double> datas) {
    if (hours_energy_usages.size() != HOURS ||
            datas.size() != HOURS) {
        return;
    }

    for (int h = 0 ; h < HOURS ; h++) {
        hours_energy_usages[h] = datas[h];
    }
}

void Power::UpdateLogVector(int hour) {
    if (hour >= HOURS) {
        return;
    }

    hours_energy_usages[hour] = curr_hour_energy_usage;
}

void Power::UpdatePrevHoursEnergy() {
    prev_hours_energy = accum_energy_usage;
}

void Power::SetExtra(double value) {
    extra = value;
}

void Power::UpdatePowerAndEnergyUsage() {
    energy = EnergyUsageInUj();

    // Update power consumption
    if (energy < prev_energy) {
        capped_times += 1;
        curr_power_usage = (energy - prev_energy + MAX_ENERGY) * POWER_CNV_AMT;
    } else {
        curr_power_usage = (energy - prev_energy) * POWER_CNV_AMT;
    }

    // Update energy consumption
    prev_energy = energy;
    accum_energy_usage = energy + capped_times * MAX_ENERGY;
    curr_hour_energy_usage = (accum_energy_usage - prev_hours_energy) * ENERGY_CNV_AMT + extra;
}

long long Power::EnergyUsageInUj() {
    long long energy = std::stoll(raymii::Command::exec(ENERGY_CMD).output, 0, 10);
    return energy;
}
