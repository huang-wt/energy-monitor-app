#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>

class Processor {
  	public:
		std::vector<float> utilizations();
		long jiffies(int cid);
    
  	private:
		std::vector<long> prevActiveJiffies;
		std::vector<long> prevTotalJiffies;
		long idleJiffies(int cid);
		long activeJiffies(int cid);
};

#endif