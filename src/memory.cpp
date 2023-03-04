#include "memory.h"

#include "system_parser.h"

float Memory::TotalMemory() {
    return total_memory;
}

float Memory::UsedMemory() {
    return used_memory;
}

float Memory::Utilisation() {
    return UsedMemory() / TotalMemory();
}

void Memory::UpdateUsedMemory() {
    used_memory = TotalMemory() - SystemParser::AvalMemory();
}

Memory::Memory() {
    // Initialise totalMemory
    total_memory = SystemParser::TotalMemory();
}
