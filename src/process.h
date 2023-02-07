#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Process {
 public:
  // Accessors
  int pid();
  std::string user();
  std::string command();
  float cpuUtilization();
  std::string ram();
  long int upTime();
  
  // Mutators
  void pid(int);
  void user(int);
  void command(int);
  void cpuUtilization(int, long);
  void ram(int);
  void upTime(int);
  
  // Operators overload
  bool operator<(Process const& a) const;
  bool operator>(Process const& a) const;

  float cpu_utilization_{0.0};

 private:
  int pid_{0};
  std::string user_{""};
  std::string command_{""};
  long prevPJiffies{0};
  long prevTotalJiffies{0};
  
  std::string ram_{""};
  long up_time_{0};
};

#endif