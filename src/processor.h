#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <vector>
#include "command.h"

using namespace std;
using namespace raymii;

class Processor {
  	public:
		int physCores = stoi(Command::exec("lscpu -b -p=Core,Socket | grep -v '^#' | sort -u | wc -l").output);
		int logicalCores = stoi(Command::exec("lscpu -b -p=cpu | grep -v '^#' | sort -u | wc -l").output);
		int hyperThreadedCores = (logicalCores - physCores)*2;
		int eCores = logicalCores - hyperThreadedCores;
		int pCores = hyperThreadedCores / 2;
		int getCpuTemp();
		vector<float> utilizations();
		long jiffies(int cid);
    
  	private:
		vector<long> prevActiveJiffies;
		vector<long> prevTotalJiffies;
		long idleJiffies(int cid);
		long activeJiffies(int cid);
};

#endif