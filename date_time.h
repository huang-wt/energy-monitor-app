#ifndef DATE_TIME_H
#define DATE_TIME_H

#include <string>

/**
 * This class represents the current date and time.
*/
class DateTime {

    public:
        /**
         * Getter method for current year.
         * @return The current year.
        */
        static int Year();

        /**
         * Getter method for current month.
         * @return The current month.
        */
        static int Month();

        /**
         * Getter method for current day.
         * @return The current day.
        */
        static int Day();

        /**
         * Getter method for current hour.
         * @return The current hour.
        */
        static int Hour();

        /**
         * Getter method for current minute.
         * @return The current minute.
        */
        static int Min();

        /**
         * Getter method for current second.
         * @return The current second.
        */
        static int Sec();

        /**
         * Getter method for current date.
         * @return The formatted current date.
        */
        static std::string CurrentDate();

        /**
         * Getter method for current time.
         * @return The formatted current time.
        */
        static std::string CurrentTime();

    private:
        static time_t now;
};

#endif
