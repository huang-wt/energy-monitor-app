#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>

class Processor {
  public:
    static std::vector<float> Utilizations();
    static long Jiffies(int cid);
    
  private:
    inline static std::vector<long> prevActiveJiffies;
    inline static std::vector<long> prevTotalJiffies;
    static long IdleJiffies(int cid);
    static long ActiveJiffies(int cid);
};

#endif