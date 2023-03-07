#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <string>
#include <vector>

namespace SystemParser {
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
    std::vector<float> MemoryInfo();
    float TotalMemory();
    float AvalMemory();
    int TotalProcesses();
    int RunningProcesses();
    long UpTime();

    // CPU
    std::vector<long> CpuTimes(int cid);
    long TotalJiffies(int cid);
    long IdleJiffiesC(int cid);
    long ActiveJiffiesC(int cid);

    // Processes
    long ActiveJiffiesP(int pid);
    std::string Command(int pid);
    std::string Ram(int pid);
    std::string Uid(int pid);
    std::string User(int pid);
    long int UpTime(int pid);
};

#endif // SYSTEM_PARSER_H
