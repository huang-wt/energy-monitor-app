#ifndef VIEW_H
#define VIEW_H

#include<string>

namespace View {

void displayStaticData();
void displayDynamicData();
void displayMemoryInfo();
void displayCpuUtilizations();
void displayProcesses();
void systemDataSelect();
std::string processSelect();
void generalCoreSelect();
void serviceSelect();

}

#endif