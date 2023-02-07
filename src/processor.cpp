#include <string>
#include <vector>

#include "processor.h"
#include "linux_parser.h"

using std::string;
using std::vector;

long Processor::jiffies(int cid) {
  vector<string> jiffies = LinuxParser::cpuTimes(cid);
  long t_jiffies = 0;
  for(string jiffie : jiffies) {
    t_jiffies += std::stoi(jiffie);
  }
  return t_jiffies;
}

long Processor::idleJiffies(int cid) {
  vector<string> jiffies = LinuxParser::cpuTimes(cid);
  long i_jiffies = 0;
  long idle = std::stoi(jiffies[3]);
  long iowait = std::stoi(jiffies[4]);
  i_jiffies = idle + iowait;
  return i_jiffies;
}

long Processor::activeJiffies(int cid) {
  long a_jiffies = 0;
  a_jiffies = jiffies(cid) - idleJiffies(cid);
  return a_jiffies;
}

// Return the aggregate CPU utilization
vector<float> Processor::utilizations() {
  vector<float> cpuUtilizations;
  int cpuNum = LinuxParser::cpuCoresCount();
  long currActiveJiffies;
  long currTotalJiffies;
  bool isFirstTime = prevActiveJiffies.empty();

  for (int cid = -1 ; cid < cpuNum ; cid++) { 
    currActiveJiffies = activeJiffies(cid);
    currTotalJiffies = jiffies(cid);

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
