#include <iostream>
#include <cstdio>
#include <string>
#include <memory>
#include <stdexcept>
#include <array>
#include <iterator>
#include <map>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <stdlib.h>
#include <vector>
#include <sched.h>
#include <cstring>
#include <sys/types.h>
#include <unistd.h>
#include "command.h"
#include "bind.h"

using namespace std;
using namespace raymii;

void Bind::tokenize(string const &str, const char delim, vector<string> &out) {
    stringstream ss(str);
    string s;
    while(std::getline(ss, s, delim)) {
        out.push_back(s);
    }
}

vector<string> Bind::findProcessID(string word) {
    bool found = false;
    string concatCmd, baseCMD = "pidof ";
    //transform word to lowercase for accepted cmd format
    transform(word.begin(), word.end(), word.begin(), ::tolower);
    concatCmd = baseCMD + word;

    CommandResult data = Command::exec(concatCmd);
    // Assume data is valid
    const char delim = ' ';
    vector<std::string> out;
    tokenize(data.output, delim, out);
    
    return out;
}

void Bind::findAndBind(vector<string> process_ID, int low, int high) {
    string tasksetCMD = "taskset -cp";
    string lowVal, highVal;
    lowVal = to_string(low);
    highVal = to_string(high);
    for (auto &s: process_ID) {
        string fullcmd = tasksetCMD +" "+ lowVal + "-" + highVal+" " + s;
        cout << fullcmd << endl;
        CommandResult bindCMD = Command::exec(fullcmd);
    }
    cin.get();
    cin.get();
}

// string Bind::exec(const char* cmd) {
//     array<char, 128> buffer;
//     string result;
//     shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
//     if (!pipe) throw runtime_error("popen() failed!");
//     while (!feof(pipe.get())) {
//         if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
//             result += buffer.data();
//     }
//     return result;
// }

vector<int> Bind::getLogicalCoresBounds() {
    vector<int> logicalCoresBounds;
    int tPhysical, tLogical, eCores, pCores, threads;

    string physCores = Command::exec("lscpu -b -p=Core,Socket | grep -v '^#' | sort -u | wc -l").output; //12
    string logicalCores = Command::exec("lscpu -b -p=cpu | grep -v '^#' | sort -u | wc -l").output; //16

    stringstream tmp1(physCores);
    stringstream tmp2(logicalCores);
    tmp1 >> tPhysical;
    tmp2 >> tLogical;

    threads = ((tLogical - tPhysical)*2); //8

    eCores = tLogical - threads; //8
    pCores = threads / 2; //4

    logicalCoresBounds.push_back(tLogical);
    logicalCoresBounds.push_back(eCores);
    logicalCoresBounds.push_back(pCores);
    logicalCoresBounds.push_back(threads);

    return logicalCoresBounds;
}