#include "memory.h"
#include "linux_parser.h"

Memory::Memory() {
    this->totalMemory = LinuxParser::totalMemory();
}

float Memory::getTotalMemory() {
    return totalMemory;
}

float Memory::getUsedMemory() {
    return totalMemory - LinuxParser::avalMemory();
}

float Memory::getUtilisation() {
    return getUsedMemory() / getTotalMemory();
}