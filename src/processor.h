#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>

class Processor {
  public:
    static std::vector<float> utilizations();
    static long jiffies(int cid);
    
  private:
    inline static std::vector<long> prevActiveJiffies;
    inline static std::vector<long> prevTotalJiffies;
    static long idleJiffies(int cid);
    static long activeJiffies(int cid);
};

#endif