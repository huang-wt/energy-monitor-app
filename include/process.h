#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Process {
    public:
        /**
         * Overloads the less operator according to cpu utilisation
         * @param a Another Process object.
         * @return True if this process is less than a; otherwise false.
        */
        bool operator<(Process const& a) const;

        /**
         * Overloads the greater operator according to cpu utilisation
         * @param a Another Process object.
         * @return True if this process is greater than a; otherwise false.
        */
        bool operator>(Process const& a) const;

        /**
         * Getter method for pid.
         * @return The process id.
        */
        int Pid();

        /**
         * Getter method for user.
         * @return The user (name) who runs the process.
        */
        std::string User();

        /**
         * Getter method for command.
         * @return The command that generated the process.
        */
        std::string Command();

        /**
         * Getter method for cpu utilisation.
         * @return The cpu utilisation of the process as a percentage.
        */
        float CpuUtilisation();

        /**
         * Getter method for memory.
         * @return The memory used by the process in Mb.
        */
        std::string Ram();

        /**
         * Getter method for up time.
         * @return The age of the process in seconds.
        */
        long int UpTime();
    
        /**
         * Setter method for process id.
         * @param pid The process id.
        */
        void SetPid(int pid);

        /**
         * Setter method for process user.
         * @param pid The process id.
        */
        void SetUser(int pid);

        /**
         * Setter method for process command.
         * @param pid The process id.
        */
        void SetCommand(int pid);

        /**
         * Setter method for cpu utilisation.
         * @param pid The process id.
         * @param curr_total_jiffies The current total cpu jiffies.
        */
        void SetCpuUtilization(int pid, long curr_total_jiffies);

        /**
         * Setter method for memory usage.
         * @param pid The process id.
        */
        void SetRam(int pid);

        /**
         * Setter method for up time of the process.
         * @param pid The process id.
        */
        void SetUpTime(int pid);

    private:
        int pid;
        std::string user;
        std::string command;
        float cpu_utilisation;
        std::string ram;
        long up_time;

        // Record previous cpu active/total jiffies to calculate
		// the live utilisation of each process in a short period
        long prev_active_jiffies{0};
        long prev_total_jiffies{0};
};

#endif // PROCESS_H
