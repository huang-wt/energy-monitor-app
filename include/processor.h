#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>

class Processor {

  	public:
		/**
         * Getter method for physical_cores.
         * @return The number of physical cores of the processor.
        */
		int PhysicalCores();

		/**
         * Getter method for logical_cores.
         * @return The number of logical cores of the processor.
        */
		int LogicalCores();

		/**
         * Getter method for hyperthreaded_cores.
         * @return The number of hyperthreaded cores of the processor.
        */
		int HyperThreadedCores();

		/**
         * Getter method for e_cores.
         * @return The number of efficiency cores of the processor.
        */
		int ECores();

		/**
         * Getter method for e_cores.
         * @return The number of performance cores of the processor.
        */
		int PCores();
		
		/**
         * Getter method for cpu_utilisations.
         * @return The vector containing the utilisation in percentage of each cpu core.
        */
		std::vector<float> Utilisations();

		/**
         * Getter method for temperature.
         * @return The cpu temperature in degree Celsius.
        */
		int Temperature();
		
		/**
		 * Update the utilisation of each cpu core and overall usage in percentage.
		*/
		void UpdateUtilisations();

		/**
		 * Update the cpu temperature.
		*/
		void UpdateTemperature();

		Processor();
    
  	private:
		int physical_cores;
		int logical_cores;
		int hyperthreaded_cores;
		int e_cores;
		int p_cores;
		int temperature;

		// First element represents general/overall cpu utilisation,
		// followed by that of all cores with index cid-1
		std::vector<float> utilisations;

		// Record previous cpu active/total jiffies to calculate
		// the live utilisation of each core in a short period
		std::vector<long> prev_active_jiffies;
		std::vector<long> prev_total_jiffies;
};

#endif