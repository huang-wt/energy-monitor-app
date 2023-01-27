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
std::string KeyValParser(std::string, std::string);

// System
std::string OperatingSystem();
std::string Kernel();
std::vector<int> Pids();
int CpuCoresCount();
float MemoryUtilization();
std::vector<float> MemoryInfo();
int TotalProcesses();
int RunningProcesses();
long UpTime();

// CPU
std::vector<std::string> CpuTimes(int cid);
long ActiveJiffies(int pid);

// Processes
std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long int UpTime(int pid);
};

#endif