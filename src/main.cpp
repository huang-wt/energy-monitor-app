#include "mainwindow.h"

#include <QApplication>
#include <thread>

void updateCpuAndMemory() {
    System* system_ = System::Instance();
    system_->UpdateCpuAndMemory();
}

void updatePowerAndEnergy() {
    System* system_ = System::Instance();
    system_->UpdateEnergy();
}

int main(int argc, char *argv[])
{
    std::thread t(updateCpuAndMemory);
    std::thread t1(updatePowerAndEnergy);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
