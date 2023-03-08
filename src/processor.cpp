#include "processor.h"

#include "system_parser.h"
#include "command.h"

Processor::Processor() {
    // Initialise cpu cores number of the following types
    physical_cores = std::stoi(raymii::Command::exec("lscpu -b -p=Core,Socket | grep -v '^#' | sort -u | wc -l").output);
	logical_cores = std::stoi(raymii::Command::exec("lscpu -b -p=cpu | grep -v '^#' | sort -u | wc -l").output);
	hyperthreaded_cores = (logical_cores - physical_cores) * 2;
	e_cores = logical_cores - hyperthreaded_cores;
    p_cores = hyperthreaded_cores / 2;

    // Initialise utilisations with all 0 in length of the number of logical cores + 1
    for (int i = 0 ; i < logical_cores + 1 ; i++) {
        utilisations.push_back(0);
    }
}

int Processor::PhysicalCores() {
    return physical_cores;
}

int Processor::LogicalCores() {
    return logical_cores;
}

int Processor::HyperThreadedCores() {
    return hyperthreaded_cores;
}

int Processor::ECores() {
    return e_cores;
}

int Processor::PCores() {
    return p_cores;
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
