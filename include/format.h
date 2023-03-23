#ifndef FORMAT_H
#define FORMAT_H

#include <string>

/**
 * This class helps format data of various types.
*/
class Format {
    public:
        /**
         * Get formatted time given the elapsed times.
         * @param times The given elapsed times.
         * @return The formatted time in hh:mm:ss.
        */
        static std::string ElapsedTime(long times);

        /**
         * Get formatted date given the year, month and day.
         * @param year The given year.
         * @param month The given month.
         * @param day The given day.
         * @return The formatted date in YY/MM/DD.
        */
        static std::string Date(int year, int month, int day);

        /**
         * Get formatted time given the hour, minute and second.
         * @param hour The given hour.
         * @param min The given minute.
         * @param sec The given second.
         * @return The formatted time in hh:mm:ss.
        */
        static std::string Time(int hour, int min, int sec);

        /**
         * Format fractions with some number of decimals.
         * @param value The given fraction number.
         * @param precison The number of decimals reserved.
         * @return The formatted number with n decimals in string.
        */
        static std::string Decimal(double value, int precision);

        /** 
         * Format percentage numbers with 1 or 2 decimals.
         * @param percent The percentage number.
         * @return The formatted percentage number in string.
        */
        static std::string Percentage(double percent);
    
    private:
        static std::string AABBCC(int aa, int bb, int cc, std::string delimiter);
}; 

#endif
