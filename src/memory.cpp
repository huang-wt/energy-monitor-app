#include "include/memory.h"

#include "include/system_parser.h"

Memory::Memory() {
    // Initialise totalMemory
    total_memory = SystemParser::TotalMemory();
}

float Memory::TotalMemory() {
    return total_memory;
}

float Memory::UsedMemory() {
    return TotalMemory() - SystemParser::AvalMemory();
}

float Memory::Utilisation() {
    return UsedMemory() / TotalMemory();
}
