#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QInputDialog>
#include <QWidget>
#include <QLabel>
#include <QtCharts>
#include <QTimer>

#include "include/system.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setWindowStyle();

private slots:
    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current);

    void displayDate();

    void displayTime();

    void displayMemoryChart();

    void displayCpuChart();

    void displayTemperatureChart();

    void displayPowerUsage();

    void displayWeekEnergyUsage();

    void displayEnergyUsage();

    void bindToAllCores();

    void bindToPCores();

    void bindToECores();

    void displayProcesses();

    void clickEnergyReportButtons();

    void displayDayReportGraph();

    void displayAccumDayReportGraph();

    void displayWeekReportGraph();

    void budgetInputReturn();

private:

    void createDonutChart(float value, std::string title, QChartView *chartView, std::string unit, int precision);

    Ui::MainWindow *ui;
    QTimer* timer = new QTimer(this);
    System* system_ = System::Instance();
};

#endif // MAINWINDOW_H
