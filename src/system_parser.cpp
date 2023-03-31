#include "system_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <string>
#include <vector>
#include <fstream>
#include <regex>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

const std::string SystemParser::kProcDirectory = "/proc/";
const std::string SystemParser::kCmdlineFilename{"/cmdline"};
const std::string SystemParser::kCpuinfoFilename{"/cpuinfo"};
const std::string SystemParser::kStatusFilename{"/status"};
const std::string SystemParser::kStatFilename{"/stat"};
const std::string SystemParser::kUptimeFilename{"/uptime"};
const std::string SystemParser::kMeminfoFilename{"/meminfo"};
const std::string SystemParser::kVersionFilename{"/version"};
const std::string SystemParser::kOSPath{"/etc/os-release"};
const std::string SystemParser::kPasswordPath{"/etc/passwd"};

//-----------------------------------------------------------------------------
// Utils
//-----------------------------------------------------------------------------

// Parse the file to find a value for a given key
string SystemParser::KeyValParser(string key, string path) {
	string value = "n/a";
	bool search = true;
	string line;
	string temp;
	std::ifstream stream(path);
	if(stream.is_open()) {
		while(search == true && stream.peek() != EOF) {
			std::getline(stream, line);
			std::istringstream linestream(line);
			linestream >> temp;
			if(temp == key) {
				linestream >> temp;
				value = temp;
				search = false;
			}
		}
	}
	return value;
}

//-----------------------------------------------------------------------------
// System
//-----------------------------------------------------------------------------

string SystemParser::OperatingSystem() {
	string line;
	string key;
	string value = "n/a";
	std::ifstream filestream(kOSPath);
	if (filestream.is_open()) {
		while (std::getline(filestream, line)) {
			std::replace(line.begin(), line.end(), ' ', '_');
			std::replace(line.begin(), line.end(), '=', ' ');
			std::replace(line.begin(), line.end(), '"', ' ');
			std::istringstream linestream(line);
			while (linestream >> key >> value) {
				if (key == "PRETTY_NAME") {
				std::replace(value.begin(), value.end(), '_', ' ');
				return value;
				}
			}
		}
	}
	return value;
}

string SystemParser::Kernel() {
	string skip;
	string kernel = "n/a";
	string line;
	std::ifstream stream(kProcDirectory + kVersionFilename);
	if (stream.is_open()) {
		std::getline(stream, line);
		std::istringstream linestream(line);
		linestream >> skip >> skip >> kernel;
	}
	return kernel;
}

vector<int> SystemParser::Pids() {
	vector<int> pids;
	DIR* directory = opendir(kProcDirectory.c_str());
	struct dirent* file;
	while ((file = readdir(directory)) != nullptr) {
		if (file->d_type == DT_DIR) {
			string filename(file->d_name);
			if (std::all_of(filename.begin(), filename.end(), isdigit)) {
				int pid = stoi(filename);
				pids.push_back(pid);
			}
		}
	}
	closedir(directory);
	return pids;
}

vector<float> SystemParser::MemoryInfo() { 
	string skip;
	string temp;
	string line;
	vector<float> memory_info;
	std::ifstream stream(kProcDirectory + kMeminfoFilename);
	if (stream.is_open()) {
		for (int i = 0; i < 3; ++i) {
			std::getline(stream, line);
			std::istringstream linestream(line);
			linestream >> skip >> temp >> skip;
			memory_info.push_back(stof(temp));
		}
	}
	return memory_info;
}

float SystemParser::TotalMemory() {
	return MemoryInfo()[0];
}

float SystemParser::AvalMemory() {
	return MemoryInfo()[2];
}

// Reads and returns the total number of processes
int SystemParser::TotalProcesses() { 
	int total_processes = 0;
	string path = kProcDirectory + kStatFilename;
	string result = KeyValParser("processes", path);
	total_processes = std::stoi(result);
	return total_processes;
}

// Reads and returns the number of running processes
int SystemParser::RunningProcesses() { 
	int active_processes = 0;
	string path = kProcDirectory + kStatFilename;
	string result = KeyValParser("procs_running", path);
	active_processes = std::stoi(result);
	return active_processes;
}

// Reads and returns the system uptime
long SystemParser::UpTime() { 
	long uptime = 0.0;
	string temp = "0.0";
	string line;
	std::ifstream stream(kProcDirectory + kUptimeFilename);
	if (stream.is_open()) {
		std::getline(stream, line);
		std::istringstream linestream(line);
		linestream >> temp;
	}
	uptime = std::atoi(temp.c_str());
	return uptime; 
}

//-----------------------------------------------------------------------------
// Processor
//-----------------------------------------------------------------------------

// Reads and returns CPU times info
vector<long> SystemParser::CpuTimes(int cid) { 
	vector<long> timers;
	string timer;
	string line;
	string skip;
	std::ifstream stream(kProcDirectory + kStatFilename);
	if (stream.is_open()) {
		for (int i = -1 ; i < cid + 1 ; i++) {
			std::getline(stream, line);
		}
		
		std::istringstream linestream(line); 
		linestream >> skip;
		for(int i = 0; i < 10; ++i) {
			linestream >> timer;
			timers.push_back(std::stoi(timer));
		}
	}
	return timers; 
}

long SystemParser::TotalJiffies(int cid) {
	vector<long> timers = CpuTimes(cid);
	long total_jiffies = 0;
    for(long timer : timers) {
        total_jiffies += timer;
    }
    return total_jiffies;
}

long SystemParser::IdleJiffiesC(int cid) {
	vector<long> timers = CpuTimes(cid);
    return timers[3] + timers[4];
}

long SystemParser::ActiveJiffiesC(int cid) {
	return TotalJiffies(cid) - IdleJiffiesC(cid);
}

// Reads and returns the number of active jiffies for a PID
long SystemParser::ActiveJiffiesP(int pid) { 
	long active_jiffies = 0;
	string utime;
	string stime;
	string line;
	string skip;
	std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
	if (stream.is_open()) {
		std::getline(stream, line);
		std::istringstream linestream(line); 
		for(int i = 1; i < 14; ++i) {
			linestream >> skip;
		}
		linestream >> utime >> stime;
	}
	active_jiffies = std::atol(utime.c_str()) + std::atol(stime.c_str());
	return active_jiffies;
}

//-----------------------------------------------------------------------------
// Process
//-----------------------------------------------------------------------------

// Reads and returns the user ID associated with a process
string SystemParser::Uid(int pid) { 
	string path = kProcDirectory + "/" + std::to_string(pid) + kStatusFilename;
	return SystemParser::KeyValParser("Uid:", path);
}

// Reads and returns the user associated with a process
string SystemParser::User(int pid) {
	string line;
	string test_user;
	string test_uid;
	string skip;
	string user = "n/a";
	string uid = SystemParser::Uid(pid);
	std::ifstream stream(kPasswordPath);
	bool search = true;
	if (stream.is_open()) {
		while(search && stream.peek() != EOF ) {
			std::getline(stream, line);
			std::replace(line.begin(), line.end(), ':', ' ');
			std::istringstream linestream(line); 
			linestream >> test_user >> skip >> test_uid;
			if(uid == test_uid) {
				user = test_user;
				search = false;
			}
		}
	}
	return user; 
}

// Reads and returns the memory used by a process
string SystemParser::Ram(int pid) { 
	string path = kProcDirectory + "/" + std::to_string(pid) + kStatusFilename;
	return SystemParser::KeyValParser("VmSize:", path);
}

// Reads and returns the uptime of a process
long SystemParser::UpTime(int pid) { 
	long ticks = 0;
	string line;
	string skip;
	std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
	if (stream.is_open()) {
		std::getline(stream, line);
		std::istringstream linestream(line); 
		for(int i = 1; i < 22; ++i) {
			linestream >> skip;
		}
		linestream >> ticks;
	}
	return ticks / sysconf(_SC_CLK_TCK);
}

// Reads and returns the command associated with a process
string SystemParser::Command(int pid) { 
	string line = "n/a";
	std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
	if (stream.is_open()) {
		std::getline(stream, line);
	}
	return line;
}
