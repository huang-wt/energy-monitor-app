#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Process {
 public:
  // Accessors
  int getPid();
  std::string getUser();
  std::string getCommand();
  float getCpuUtilization();
  std::string getRam();
  long int getUpTime();
  
  // Mutators
  void setPid(int);
  void setUser(int);
  void setCommand(int);
  void setCpuUtilization(int, long);
  void setRam(int);
  void setUpTime(int);
  
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