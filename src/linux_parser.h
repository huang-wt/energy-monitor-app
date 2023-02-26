#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

// Utils
std::string keyValParser(std::string, std::string);

// System
std::string operatingSystem();
std::string kernel();
std::vector<int> pids();
int cpuCoresCount();
std::vector<float> memoryInfo();
float totalMemory();
float avalMemory();
int totalProcesses();
int runningProcesses();
long upTime();

// CPU
std::vector<std::string> cpuTimes(int cid);
long activeJiffies(int pid);

// Processes
std::string command(int pid);
std::string ram(int pid);
std::string uid(int pid);
std::string user(int pid);
long int upTime(int pid);
};

#endif