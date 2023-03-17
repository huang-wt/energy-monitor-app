#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <string>
#include <vector>

class SystemParser {

    public: 
        // System
        static std::string OperatingSystem();
        static std::string Kernel();
        static std::vector<int> Pids();
        static std::vector<float> MemoryInfo();
        static float TotalMemory();
        static float AvalMemory();
        static int TotalProcesses();
        static int RunningProcesses();
        static long UpTime();

        // CPU
        static std::vector<long> CpuTimes(int cid);
        static long TotalJiffies(int cid);
        static long IdleJiffiesC(int cid);
        static long ActiveJiffiesC(int cid);

        // Processes
        static long ActiveJiffiesP(int pid);
        static std::string Command(int pid);
        static std::string Ram(int pid);
        static std::string Uid(int pid);
        static std::string User(int pid);
        static long int UpTime(int pid);


    private:
        // Utils
        static std::string KeyValParser(std::string, std::string);

        // Paths
        static const std::string kProcDirectory;
        static const std::string kCmdlineFilename;
        static const std::string kCpuinfoFilename;
        static const std::string kStatusFilename;
        static const std::string kStatFilename;
        static const std::string kUptimeFilename;
        static const std::string kMeminfoFilename;
        static const std::string kVersionFilename;
        static const std::string kOSPath;
        static const std::string kPasswordPath;
};

#endif // SYSTEM_PARSER_H
