#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <vector>

/**
 * This class represents a computer processor and provides methods
 * to retrieve information about its cores and utilisation.
*/
class Processor {

  	public:
		Processor();

		/**
         * Getter method for physical cores.
         * @return The number of physical cores of the processor.
        */
		int PhysicalCores();

		/**
         * Getter method for logical cores.
         * @return The number of logical cores of the processor.
        */
		int LogicalCores();

		/**
         * Getter method for hyperthreaded cores.
         * @return The number of hyperthreaded cores of the processor.
        */
		int HyperThreadedCores();

		/**
         * Getter method for e cores.
         * @return The number of efficiency cores of the processor.
        */
		int ECores();

		/**
         * Getter method for p cores.
         * @return The number of performance cores of the processor.
        */
		int PCores();
		
		/**
         * Getter method for cpu utilisations.
         * @return The vector containing the utilisation in percentage of each cpu core.
        */
		std::vector<float> Utilisations();

		/**
         * Getter method for temperature.
         * @return The cpu temperature in degree Celsius.
        */
        int Temperature();

        /**
         * Update the utilisation of each cpu core and overall usage.
        */
        void UpdateUtilisations();

  	private:
		static const int PHYSICAL_CORES;
		static const int LOGICAL_CORES;
		static const int HYPERTHREADED_CORES;
		static const int E_CORES;
        static const int P_CORES;

        // utilisation of all cpu cores
		std::vector<float> utilisations;
        // previous jiffies when cpu is not idle
		std::vector<long> prev_active_jiffies;
        // previous jiffies when cpu is in all states
		std::vector<long> prev_total_jiffies;
};

#endif
