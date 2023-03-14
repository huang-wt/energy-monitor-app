#include "processor.h"

#include "system_parser.h"
#include "command.h"

const int Processor::PHYSICAL_CORES = std::stoi(raymii::Command::exec("lscpu -b -p=Core,Socket | grep -v '^#' | sort -u | wc -l").output);
const int Processor::LOGICAL_CORES = std::stoi(raymii::Command::exec("lscpu -b -p=cpu | grep -v '^#' | sort -u | wc -l").output);
const int Processor::HYPERTHREADED_CORES = (LOGICAL_CORES - PHYSICAL_CORES) * 2;
const int Processor::E_CORES = LOGICAL_CORES - HYPERTHREADED_CORES;
const int Processor::P_CORES = HYPERTHREADED_CORES / 2;

Processor::Processor() {
    // Initialise utilisations with all 0 in length of the number of logical cores + 1
    for (int i = 0 ; i < LOGICAL_CORES + 1 ; i++) {
        utilisations.push_back(0);
    }
}

int Processor::PhysicalCores() {
    return PHYSICAL_CORES;
}

int Processor::LogicalCores() {
    return LOGICAL_CORES;
}

int Processor::HyperThreadedCores() {
    return HYPERTHREADED_CORES;
}

int Processor::ECores() {
    return E_CORES;
}

int Processor::PCores() {
    return P_CORES;
}

std::vector<float> Processor::Utilisations() {
    return utilisations;
}

int Processor::Temperature() {
    return temperature;
}

void Processor::UpdateUtilisations() {
    long curr_active_jiffies;
    long curr_total_jiffies;
    bool is_first_time = prev_active_jiffies.empty();

    for (int cid = -1 ; cid < LogicalCores() ; cid++) { 
        curr_active_jiffies = SystemParser::ActiveJiffiesC(cid);
        curr_total_jiffies = SystemParser::TotalJiffies(cid);

        if (!is_first_time) {
            utilisations[cid + 1] = (float) (curr_active_jiffies - prev_active_jiffies[cid + 1]) /
                                            (curr_total_jiffies - prev_total_jiffies[cid + 1]);
            prev_active_jiffies[cid + 1] = curr_active_jiffies;
            prev_total_jiffies[cid + 1] = curr_total_jiffies;
        } else {
            utilisations[cid + 1] = 0;
            prev_active_jiffies.push_back(curr_active_jiffies);
            prev_total_jiffies.push_back(curr_total_jiffies);
        }
    }
}

void Processor::UpdateTemperature() {
    std::string cmd = "cat /sys/class/thermal/thermal_zone0/temp";
    temperature = stoi(raymii::Command::exec(cmd).output) / 1000;
}
