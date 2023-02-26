#ifndef MEMORY_H
#define MEMORY_H

class Memory {

public:
    Memory();
    float getTotalMemory();
    float getUsedMemory();
    float getUtilisation();

private:
    float totalMemory = {};

};


#endif