#include "headers/power_dao.h"

#include <fstream>

#include <QStandardPaths>

#include "headers/command.h"

using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;

QString data_dir_path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

const string PowerDAO::HOURS_LOG_FILE = (data_dir_path + "/hours_usage.csv").toStdString();
const string PowerDAO::DAYS_LOG_FILE = (data_dir_path + "/days_usage.csv").toStdString();

void PowerDAO::InitHoursLogFile(string curr_date, vector<double> usages) {
    ifstream in(HOURS_LOG_FILE);
    if (!in.good()) {
        ofstream out(HOURS_LOG_FILE);
        out.close();
        UpdateHoursLogFile(curr_date, usages);
    }
    in.close();
}

void PowerDAO::InitDaysLogFile() {
    ifstream infile(DAYS_LOG_FILE);
    if (!infile.good()) {
        ofstream outfile(DAYS_LOG_FILE);
        outfile.close();
    }
}

string PowerDAO::LastLoggedDate() {
    string cmd = "head -1 " + HOURS_LOG_FILE + " | tr -d '\\n'";
    string last_logged_date = raymii::Command::exec(cmd).output;
    return last_logged_date;
}

void PowerDAO::UpdateDaysLogFile(string last_logged_date, double total_usage) {
    std::ofstream out(DAYS_LOG_FILE, std::ios::app);
    out << last_logged_date << ',' << total_usage << '\n';
    out.close();
}

void PowerDAO::UpdateHoursLogFile(string curr_date, vector<double> usages) {
    std::ofstream out(HOURS_LOG_FILE);
    out << curr_date << '\n';
    int h = 0;
    double usage = 0;
    for (h = 0 ; h < 24 ; h++) {
        usage = usages[h];
        out << h << ',' << usage << '\n';
    }
    out.close();
}

vector<double> PowerDAO::ReadHoursFile() {
    std::ifstream in(HOURS_LOG_FILE);
    vector<double> datas;
    string row;
    if (in.is_open()) {
        getline(in, row);
        for (int h = 0 ; h < 24 ; h++) {
            getline(in, row);
            datas.push_back(stod(row.substr(row.find(",") + 1)));
        }
    }
    in.close();

    return datas;
}

vector<string> PowerDAO::ReadDaysFile() {
    vector<string> rows;
    std::ifstream in(DAYS_LOG_FILE);
    string row;
    while (getline(in, row)) {
        rows.push_back(row);
    }
    in.close();

    return rows;
}
