#include "date_time.h"

#include "format.h"

time_t DateTime::now = time(0);

int DateTime::Year() {
    now = time(0);
    struct tm *tmp = gmtime(&now);
    return tmp->tm_year + 1900;
}

int DateTime::Month() {
    now = time(0);
    struct tm *tmp = gmtime(&now);
    return tmp->tm_mon + 1;
}

int DateTime::Day() {
    now = time(0);
    struct tm *tmp = gmtime(&now);
    return tmp->tm_mday;
}

int DateTime::Hour() {
    now = time(0);
    struct tm *tmp = gmtime(&now);
    return tmp->tm_hour == 23 ? 0 : tmp->tm_hour + 1;
}

int DateTime::Min() {
    now = time(0);
    struct tm *tmp = gmtime(&now);
    return tmp->tm_min;
}

int DateTime::Sec() {
    now = time(0);
    struct tm *tmp = gmtime(&now);
    return tmp->tm_sec;
}

std::string DateTime::CurrentDate() {
    return Format::Date(Year(), Month(), Day());
}

std::string DateTime::CurrentTime() {
    return Format::Time(Hour(), Min(), Sec());
}
