#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QInputDialog>
#include <QWidget>
#include <QLabel>
#include <QtCharts>
#include <QTimer>

#include "system.h"

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
    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    /**
     * @brief Update current date in format of YYYY/MM/DD.
     */
    void displayDate();

    /**
     * @brief Update pc up time in format of hh::mm::ss.
     */
    void displayTime();

    void displayMemoryChart();

    void displayCpuChart();

    void displayTemperatureChart();

    /**
     * @brief Update live power usage in Watts.
     */
    void displayPowerUsage();

    void displayWeekEnergyCost();

    void displayEnergyUsage();

    void bindToAllCores();

    void bindToPCores();

    void bindToECores();

    /**
     * @brief Update most CPU consuming processes in real time.
     */
    void displayProcesses();

    void clickEnergyReportButtons();

    /**
     * @brief Display hourly energy usages in line chart.
     */
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
