#include <string>
#include <vector>

#include "processor.h"
#include "linux_parser.h"

using std::string;
using std::vector;

long Processor::Jiffies(int cid) {
  vector<string> jiffies = LinuxParser::CPUTimes(cid);
  long t_jiffies = 0;
  for(string jiffie : jiffies) {
    t_jiffies += std::stoi(jiffie);
  }
  return t_jiffies;
}

long Processor::IdleJiffies(int cid) {
  vector<string> jiffies = LinuxParser::CPUTimes(cid);
  long i_jiffies = 0;
  long idle = std::stoi(jiffies[3]);
  long iowait = std::stoi(jiffies[4]);
  i_jiffies = idle + iowait;
  return i_jiffies;
}

long Processor::ActiveJiffies(int cid) {
  long a_jiffies = 0;
  a_jiffies = Jiffies(cid) - IdleJiffies(cid);
  return a_jiffies;
}

// Return the aggregate CPU utilization
vector<float> Processor::Utilizations() {
  vector<float> cpuUtilizations;
  int cpuNum = LinuxParser::CPUCoresNumber();
  long currActiveJiffies;
  long currTotalJiffies;
  bool isFirstTime = prevActiveJiffies.empty();

  for (int cid = -1 ; cid < cpuNum ; cid++) { 
    currActiveJiffies = ActiveJiffies(cid);
    currTotalJiffies = Jiffies(cid);

    if (!isFirstTime) {
      cpuUtilizations.push_back((float) (currActiveJiffies - prevActiveJiffies[cid + 1]) /
                                        (currTotalJiffies - prevTotalJiffies[cid + 1]));
      prevActiveJiffies[cid + 1] = currActiveJiffies;
      prevTotalJiffies[cid + 1] = currTotalJiffies;
    } else {
      cpuUtilizations.push_back(0);
      prevActiveJiffies.push_back(currActiveJiffies);
      prevTotalJiffies.push_back(currTotalJiffies);
    }
  }

  return cpuUtilizations;
}
