#ifndef MEMORY_H
#define MEMORY_H

/**
 * This class represents a computer memory and provides methods
 * to retrieve information about its usage.
*/
class Memory {

    public:
        Memory();

        /**
         * Getter method for total memory.
         * @return The amount of total memory in Kb.
        */
        float TotalMemory();

        /**
         * Getter method for used_memory.
         * @return The amount of used memory in Kb.
         */
        float UsedMemory();

        /**
         * Calculates and returns the memory utilization as a percentage.
         * @return The memory utilization as a percentage.
         */
        float Utilisation();

    private:
        float total_memory;

};

#endif // MEMORY_H
