#ifndef BIND_H
#define BIND_H

#include <string>
#include <vector>

namespace Bind {

    void tokenize(std::string const &str, const char delim, std::vector<std::string> &out);
    std::vector<std::string> findProcessID(std::string word);
    void findAndBind(std::vector<std::string> process_ID, int low, int high);
    std::string exec(const char* cmd);
    std::vector<int> getLogicalCoresBounds();

}

#endif