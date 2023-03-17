#include "include/power_dao.h"

#include <fstream>

#include <QStandardPaths>

#include "include/command.h"

using std::string;
using std::vector;
using std::map;
using std::ifstream;
using std::ofstream;

#define HOURS 24

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
    ifstream in(DAYS_LOG_FILE);
    if (!in.good()) {
        ofstream out(DAYS_LOG_FILE);
        out.close();
    }
    in.close();
}

string PowerDAO::LastLoggedDate() {
    string cmd = "head -1 " + HOURS_LOG_FILE + " | tr -d '\\n'";
    string last_logged_date = raymii::Command::exec(cmd).output;
    return last_logged_date;
}

void PowerDAO::UpdateDaysLogFile(string last_logged_date, double total_usage) {
    ofstream out(DAYS_LOG_FILE, std::ios::app);
    out << last_logged_date << ',' << total_usage << '\n';
    out.close();
}

void PowerDAO::UpdateHoursLogFile(string curr_date, vector<double> usages) {
    ofstream out(HOURS_LOG_FILE);
    out << curr_date << '\n';
    int h = 0;
    double usage = 0;
    for (h = 0 ; h < 24 ; h++) {
        usage = usages[h];
        out << h << ',' << usage << '\n';
    }
    out.close();
}

vector<double> PowerDAO::HoursEnergyUsages() {
    ifstream in(HOURS_LOG_FILE);
    vector<double> usages;
    string row;
    if (in.is_open()) {
        getline(in, row);
        for (int h = 0 ; h < HOURS ; h++) {
            getline(in, row);
            usages.push_back(stod(row.substr(row.find(",") + 1)));
        }
    }
    in.close();

    return usages;
}

map<string, double> PowerDAO::LastNDaysEnergyUsage(int n) {
    vector<string> rows;

    // Get rows from log file
    ifstream in(DAYS_LOG_FILE);
    string row;
    while (getline(in, row)) {
        rows.push_back(row);
    }
    in.close();

    int rows_count = rows.size();
    n = rows_count < n ? rows_count : n;

    string date;
    double energy;
    map<string, double> last_n_days_energy;
    for (int i = 0 ; i < n ; i++) {
        row = rows[rows_count - i - 1];
        date = row.substr(0, 10);
        energy = stod(row.substr(11));
        last_n_days_energy[date] = energy;
    }

    return last_n_days_energy;
}
