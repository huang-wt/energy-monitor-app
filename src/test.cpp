#include <filesystem>
#include <iostream>

int main() {
    time_t now = time(0);
    struct tm *tmp = gmtime(&now);
    std::cout << tmp->tm_hour;
    
}