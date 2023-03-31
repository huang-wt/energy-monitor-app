#include "memory.h"

#include "system_parser.h"

const float Memory::TOTAL_MEMORY = SystemParser::TotalMemory();

float Memory::TotalMemory() {
    return TOTAL_MEMORY;
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
