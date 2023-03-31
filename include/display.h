#ifndef DISPLAY_H
#define DISPLAY_H

#include <curses.h>
#include <string>


#include "system.h"

using std::string;
using std::to_string;

class Display {
    public:
        void DisplayMain();

    private:
        std::string ProgressBar(float percent, int size);
        void DisplayProgramTitle(WINDOW* window);
        void DisplaySystem(WINDOW* window);
        void DisplayMemory(WINDOW* window);
        void DisplayCpu(WINDOW* window);
        void DisplayProcesses(WINDOW* window);
        void DisplaySummary(WINDOW* window);
        void DisplayHourlyEnergyUsages(WINDOW* window);
        void DisplayWeeklyEnergyUsage(WINDOW* window);
        void DisplayModes(WINDOW* window);

        System* system_ = System::Instance();
};

#endif