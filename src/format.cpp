#include "include/format.h"

#include <string>
#include <sstream>
#include <iomanip>

using std::string;

#define HOUR  3600
#define MIN   60
#define TIME_DELIMITER ":"
#define DATE_DELIMITER "/"

string Format::ElapsedTime(long seconds) { 
    int hour = seconds / HOUR;
    int sec = seconds % HOUR;
    int min = sec / MIN;
    sec %= MIN;
    
    return AABBCC(hour, min, sec, TIME_DELIMITER);
}

string Format::Date(int year, int month, int day) {
    return AABBCC(year, month, day, DATE_DELIMITER);
}

string Format::Time(int hour, int min, int sec) {
    return AABBCC(hour, min, sec, TIME_DELIMITER);
}

string Format::Decimal(double value, int precision) {
    std::stringstream stream;
    stream << std::fixed << std::setprecision(precision) << value;
    return stream.str();
}

string Format::Percentage(double percent) {
    string display = std::to_string(percent * 100).substr(0, 4);
    if (percent < 0.1 || percent == 1.0)
        display = " " + std::to_string(percent * 100).substr(0, 3);
    return display;
}

string Format::AABBCC(int aa, int bb, int cc, string delimiter) {
    std::ostringstream out_stream;
    out_stream.width(2);
    out_stream.fill('0');
    out_stream << aa << delimiter;
    out_stream.width(2);
    out_stream.fill('0');
    out_stream << bb << delimiter;
    out_stream.width(2);
    out_stream.fill('0');
    out_stream << cc;
    
    return out_stream.str();
}
