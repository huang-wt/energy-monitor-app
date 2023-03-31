#include "display.h"

#include <string>
#include <vector>
#include <map>
#include <thread>
#include <chrono>

#include "format.h"
#include "date_time.h"

void Display::DisplayMain() {
    initscr();      // start ncurses
    noecho();       // do not print input values
    cbreak();       // terminate ncurses on ctrl + c
    start_color();  // enable color

    int x_max = getmaxx(stdscr);
    WINDOW* title_window = newwin(3, x_max - 1, 0, 0);
    WINDOW* system_window = newwin(7, x_max / 2, 3, 0);
    WINDOW* memory_window = newwin(7, x_max / 2 - 1, 3, x_max / 2);
    WINDOW* cpu_window = newwin(12, x_max - 1, 10, 0);
    WINDOW* process_window = newwin(8, x_max - 1, 22, 0);
    WINDOW* summary_window = newwin(15, x_max / 3 - 1, 30, 0);
    WINDOW* energy_window = newwin(15, x_max / 3, 30, x_max / 3 - 1);
    WINDOW* week_energy_window = newwin(15, x_max / 3, 30, x_max / 3 * 2 - 1);
    WINDOW* mode_window = newwin(4, x_max - 1, 45, 0);

    while (1) {
        init_pair(1, COLOR_RED, COLOR_BLACK);
        init_pair(2, COLOR_GREEN, COLOR_BLACK);
        box(title_window, 0, 0);
        box(system_window, 0, 0);
        box(memory_window, 0, 0);
        box(cpu_window, 0, 0);
        box(process_window, 0, 0);
        box(energy_window, 0, 0);
        box(week_energy_window, 0, 0);
        box(summary_window, 0, 0);
        box(mode_window, 0, 0);

        DisplayProgramTitle(title_window);
        DisplaySystem(system_window);
        DisplayMemory(memory_window);
        DisplayCpu(cpu_window);
        DisplayProcesses(process_window);
        DisplayHourlyEnergyUsages(energy_window);
        DisplayWeeklyEnergyUsage(week_energy_window);
        DisplaySummary(summary_window);
        DisplayModes(mode_window);

        refresh();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    endwin();
}

void Display::DisplayProgramTitle(WINDOW* window) {
    mvwprintw(window, 1, 2, "%s", "Linux Energy Monitor - Test Interface");
    wrefresh(window);
}

void Display::DisplaySystem(WINDOW* window) {
    int row = 0;
    mvwprintw(window, ++row, 2, "%s", ("OS: " + system_->OperatingSystem()).c_str());
    mvwprintw(window, ++row, 2, "%s", ("Kernel: " + system_->Kernel()).c_str());
    mvwprintw(window, ++row, 2, "%s", ("Total Processes: " + to_string(system_->TotalProcesses())).c_str());
    mvwprintw(window, ++row, 2, "%s", ("Running Processes: " + to_string(system_->RunningProcesses())).c_str());
    mvwprintw(window, ++row, 2, "%s", ("Up Time: " + Format::ElapsedTime(system_->UpTime())).c_str());
    wrefresh(window);
}

void Display::DisplayMemory(WINDOW* window) {
    int row = 0;
    mvwprintw(window, ++row, 2, "%s", ("Total Memory: " + 
        Format::Decimal(system_->TotalMemory() / 1024 / 1024) + "Gb").c_str());
    mvwprintw(window, ++row, 2, "%s", ("Used Memory: " + 
        Format::Decimal(system_->UsedMemory() / 1024 / 1024) + "Gb").c_str());

    mvwprintw(window, ++row, 2, "%s", "Usage: ");
    double usage = system_->MemoryUtilisation();
    int color_op = usage > 0.8 ? 1 : 2;
    wattron(window, COLOR_PAIR(color_op));
    mvwprintw(window, row, 9, "%s", "");
    wprintw(window, "%s", (ProgressBar(usage, 25)).c_str());
    wattroff(window, COLOR_PAIR(color_op));

    wrefresh(window);
}

void Display::DisplayCpu(WINDOW* window) {
    int row = 0;
    int temperature = system_->CpuTemperature();
    vector<float> utilisations = system_->CpuUtilisations();
    int color_op = 1;

    mvwprintw(window, ++row, 2, "%s", "CPU Temperature: ");
    color_op = temperature > 70 ? 1 : 2;
    if (temperature < 99) {
        wattron(window, COLOR_PAIR(color_op));
        mvwprintw(window, row, 20, "%s", (to_string(temperature) + "°C").c_str());
        wattroff(window, COLOR_PAIR(color_op));
    }
    
    mvwprintw(window, ++row, 2, "%s", "CPU: ");
    color_op = utilisations[0] > 0.7 ? 1 : 2;
    wattron(window, COLOR_PAIR(color_op));
    mvwprintw(window, row, 7, "%s", "");
    wprintw(window, "%s", (ProgressBar(utilisations[0], 50)).c_str());
    wattroff(window, COLOR_PAIR(color_op));
    for (int i = 1 ; i < utilisations.size() / 2 + 1 ; i++) {
        mvwprintw(window, ++row, 2, "%s", ("C" + to_string(i - 1) + ": ").c_str());
        color_op = utilisations[i] > 0.7 ? 1 : 2;
        wattron(window, COLOR_PAIR(color_op));
        mvwprintw(window, row, 7, "%s", "");
        wprintw(window, "%s", (ProgressBar(utilisations[i], 25)).c_str());
        wattroff(window, COLOR_PAIR(color_op));

        mvwprintw(window, row, 52, "%s", ("C" + to_string(i - 1 + utilisations.size() / 2) + ": ").c_str());
        color_op = utilisations[i + utilisations.size() / 2] > 0.7 ? 1 : 2;
        wattron(window, COLOR_PAIR(color_op));
        mvwprintw(window, row, 57, "%s", "");
        wprintw(window, "%s", (ProgressBar(utilisations[i + utilisations.size() / 2], 25)).c_str());
        wattroff(window, COLOR_PAIR(color_op));
    }
    wrefresh(window);
}

void Display::DisplayProcesses(WINDOW* window) {
    int row = 0;
    int const pid_column = 2;
    int const cpu_column = 11;
    int const command_column = 21;
    mvwprintw(window, ++row, pid_column, "%s", "PID");
    mvwprintw(window, row, cpu_column, "%s", "CPU[%%]");
    mvwprintw(window, row, command_column, "%s", "COMMAND");
    vector<Process> processes = system_->SortedProcesses();
    std::string command;
    for (int i = 0; i < 5; ++i) {
        mvwprintw(window, ++row, pid_column, "%s", (to_string(processes[i].Pid())).c_str());
        float cpu = processes[i].CpuUtilisation() * 100;
        wattron(window, COLOR_PAIR(2));
        mvwprintw(window, row, cpu_column, "%s", (to_string(cpu).substr(0, 4)).c_str());
        wattroff(window, COLOR_PAIR(2));
        command = processes[i].Command();
        if (command.size() < 30) {
            mvwprintw(window, row, command_column, "%s", (command.c_str()));
        } else {
            mvwprintw(window, row, command_column, "%s", (command.substr(0, 30) + "...").c_str());
        }
        
    }
    wrefresh(window);
}

void Display::DisplaySummary(WINDOW* window) {
    int row = 0;
    double usage_day = system_->TotalEnergyUsage();
    double usage_week = system_->TotalEnergyUsageLastWeek();
    double power = system_->PowerUsage();
    mvwprintw(window, ++row, 2, "%s", ("Date: " + DateTime::CurrentDate()).c_str());
    row++;
    mvwprintw(window, ++row, 2, "%s", "Today");
    mvwprintw(window, ++row, 2, "%s", ("    Energy Usage: " + Format::Decimal(usage_day) + " Wh").c_str());
    mvwprintw(window, ++row, 2, "%s", ("    Costs: " + Format::Decimal(usage_day * 34 / 1000) + "p").c_str());
    row++;
    mvwprintw(window, ++row, 2, "%s", "Week");
    mvwprintw(window, ++row, 2, "%s", ("    Energy Usage: " + Format::Decimal(usage_week) + " Wh").c_str());
    mvwprintw(window, ++row, 2, "%s", ("    Costs: " + Format::Decimal(usage_week * 34 / 1000) + "p").c_str());
    row++;
    mvwprintw(window, ++row, 2, "%s", "Live Power Energy: ");
    int color_op = power > 40 ? 1 : 2;
    wattron(window, COLOR_PAIR(color_op));
    if (power < 9999) {
        mvwprintw(window, row, 22, "%s", (Format::Decimal(power) + " W").c_str()); // bug
    }
    wattroff(window, COLOR_PAIR(color_op));
    
    wrefresh(window);
}

void Display::DisplayHourlyEnergyUsages(WINDOW* window) {
    int row = 0;
    mvwprintw(window, ++row, 2, "%s", "Daily Energy Usage in Wh");
    vector<double> usages = system_->HoursEnergyUsages();
    int curr_hour = DateTime::Hour();
    for (int i = 0 ; i < 12 ; i++) {
        mvwprintw(window, ++row, 2, "%s", (to_string(i) + ": ").c_str());
        if (i == curr_hour) {
            wattron(window, COLOR_PAIR(2));
            mvwprintw(window, row, 6, "%s", (Format::Decimal(usages[i])).c_str());
            wattroff(window, COLOR_PAIR(2));
        } else {
            mvwprintw(window, row, 6, "%s", (Format::Decimal(usages[i])).c_str());
        }
        
        mvwprintw(window, row, 20, "%s", (to_string(i + 12) + ": ").c_str());
        if (i + 12 == curr_hour) {
            wattron(window, COLOR_PAIR(2));
            mvwprintw(window, row, 25, "%s", (Format::Decimal(usages[i + 12])).c_str());
            wattroff(window, COLOR_PAIR(2));
        } else {
            mvwprintw(window, row, 25, "%s", (Format::Decimal(usages[i + 12])).c_str());
        }
    }
    wrefresh(window);

}

void Display::DisplayWeeklyEnergyUsage(WINDOW* window) {
    int row = 0;
    mvwprintw(window, ++row, 2, "%s", "Weekly Energy Usage in Wh");
    map<string, double> usages = system_->LastWeekEnergyUsage();
    for (auto const& [key, val] : usages) {
        mvwprintw(window, ++row, 2, "%s", (key + ": " + Format::Decimal(val)).c_str());
    }
    wrefresh(window);
}

void Display::DisplayModes(WINDOW* window) {
    mvwprintw(window, 1, 2, "%s", "All Cores [a]");
    mvwprintw(window, 1, 22, "%s", "P Cores [p]");
    mvwprintw(window, 1, 42, "%s", "E Cores [e]");

    keypad(window, true);
    timeout(0);
    int c = getch();
    string prompt = "Binding processes to ";
    if (c == 'a') {
        system_->BindToAllCores();
        prompt += "All Cores";
    } else if (c == 'p') {
        system_->BindToPCores();
        prompt += "P Cores";
    } else if (c == 'e') {
        system_->BindToECores();
        prompt += "E cores";
    }

    mvwprintw(window, 2, 2, "%s", prompt.c_str());

    wrefresh(window);
}

std::string Display::ProgressBar(float percent, int size) {
    std::string result = "0%";
    float bars = percent >= 0 ? percent * size : 0;

    for (int i = 0; i < size; ++i) {
        result += i <= bars ? '|' : ' ';
    }

    string display = Format::Percentage(percent);
    return result + " " + display + "/100%";
}
