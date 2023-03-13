#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QInputDialog>
#include <QWidget>
#include <QLabel>

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

    void on_pushButton_edit_clicked();

    /**
     * @brief Update current date in format of YYYY/MM/DD.
     */
    void displayDate();

    /**
     * @brief Update pc up time in format of hh::mm::ss.
     */
    void displayTime();

    /**
     * @brief Update live memory usage as a percentage.
     */
    void displayMemoryUsage();

    /**
     * @brief Update real-time CPU temperature in degree Ceisuis.
     */
    void displayCpuTemperature();

    /**
     * @brief Update live power usage in Watts.
     */
    void displayPowerUsage();

    /**
     * @brief Update live cpu utilisation as a percentage.
     */
    void displayCpuUtilisation();

    /**
     * @brief Update total energy cost in the current date.
     */
    void displayTodayEnergyCost();

    /**
     * @brief Update total energy cost in the last week (seven days).
     */
    void displayWeekEnergyCost();

    void bindToAllCores();

    void bindToPCores();

    void bindToECores();

    /**
     * @brief Update most CPU consuming processes in real time.
     */
    void displayProcesses();

    /**
     * @brief Display hourly energy usages in line chart.
     */
    void displayDayReportGraph();

private:
    /**
     * @brief Set data's color to red when the given metric value exceeds threshold;
     *        Otherwise set its color to green.
     * @param label The chosen label.
     * @param value The value about to display.
     * @param threshold The threshold for determining color.
     */
    void colorData(QLabel* label, double value, double threshold);

    Ui::MainWindow *ui;
    System* system_ = System::Instance();
};

#endif // MAINWINDOW_H
