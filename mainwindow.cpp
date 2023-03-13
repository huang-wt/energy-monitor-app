#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QCategoryAxis>

#include <vector>
#include <string>

#include "format.h"
#include "date_time.h"

#define THRESHOLD 70
#define POWER_THRESHOLD 35
#define PROCESSES_NUM 10
#define ENERGY_COST 34
#define HOURS 24

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("System Monitoring");

    ui->treeWidget->header()->hide();
    ui->treeWidget->topLevelItem(0)->setIcon(0,QIcon(":/dashboard.png"));
    ui->treeWidget->topLevelItem(1)->setIcon(0,QIcon(":/power.png"));
    ui->treeWidget->topLevelItem(2)->setIcon(0,QIcon(":/process.png"));
    ui->treeWidget->topLevelItem(3)->setIcon(0,QIcon(":/report.png"));
    ui->treeWidget->topLevelItem(4)->setIcon(0,QIcon(":/setting.png"));
    ui->treeWidget->setCurrentItem(ui->treeWidget->topLevelItem(0));

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->stackedWidget->setCurrentIndex(0);

    setWindowStyle();

    // Create QTimer object for updating live data
    QTimer* timer = new QTimer(this);
    timer->start(1000);

    // Connect timer to slots for continuously updating system information
    connect(timer, SIGNAL(timeout()), this, SLOT(displayDate()));
    connect(timer, SIGNAL(timeout()), this, SLOT(displayTime()));
    connect(timer, SIGNAL(timeout()), this, SLOT(displayMemoryUsage()));
    connect(timer, SIGNAL(timeout()), this, SLOT(displayCpuTemperature()));
    connect(timer, SIGNAL(timeout()), this, SLOT(displayPowerUsage()));
    connect(timer, SIGNAL(timeout()), this, SLOT(displayCpuUtilisation()));
    connect(timer, SIGNAL(timeout()), this, SLOT(displayTodayEnergyCost()));
    connect(timer, SIGNAL(timeout()), this, SLOT(displayWeekEnergyCost()));
    connect(timer, SIGNAL(timeout()), this, SLOT(displayProcesses()));
    connect(timer, SIGNAL(timeout()), this, SLOT(displayDayReportGraph()));

    // Connect the clicked signals of the bindAllCore, bindPCore and bindEcore to corresponding slots
    connect(ui->bindAllCorePushButton, &QPushButton::clicked, this, &MainWindow::bindToAllCores);
    connect(ui->bindPCorePushButton, &QPushButton::clicked, this, &MainWindow::bindToPCores);
    connect(ui->bindECorePushButton, &QPushButton::clicked, this, &MainWindow::bindToECores);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setWindowStyle()
{
    QString qss;
    QFile file(":/qss/silvery.css");
    if (file.open(QFile::ReadOnly))
    {
        QStringList list;
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line;
            in >> line;
            list << line;
        }
        qss = list.join("\n");

        QString paletteColor = qss.mid(20, 7);
        this->setPalette(QPalette(QColor(paletteColor)));
        this->setStyleSheet(qss);
        file.close();
    }
}

void MainWindow::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    int index = ui->treeWidget->indexOfTopLevelItem(current);
    ui->stackedWidget->setCurrentIndex(index);
}

void MainWindow::on_pushButton_edit_clicked()
{
    QInputDialog::getText(this,"edit","please enter");
}

void MainWindow::displayDate()
{
    std::string date = DateTime::CurrentDate();

    QLabel* dateLabel = findChild<QLabel*>("date");
    dateLabel->setText(QString::fromStdString(date));
}

void MainWindow::displayTime()
{
    std::string up_time = Format::ElapsedTime(system_->UpTime());

    QLabel* upTimeLabel = findChild<QLabel*>("upTime");
    upTimeLabel->setText(QString::fromStdString(up_time));
}

void MainWindow::displayMemoryUsage()
{
    float utilisation = system_->MemoryUtilisation() * 100;

    QLabel* memoryUsageLabel = findChild<QLabel*>("memoryUsage");
    memoryUsageLabel->setText(QString("%1%").arg(utilisation, 0, 'f', 0));

    colorData(memoryUsageLabel, utilisation, THRESHOLD);
}

void MainWindow::displayCpuTemperature()
{
    int temperature = system_->CpuTemperature();

    QLabel* cpuTemperatureLabel = findChild<QLabel*>("cpuTemp");
    cpuTemperatureLabel->setText(QString("%1°C").arg(temperature));

    colorData(cpuTemperatureLabel, temperature, THRESHOLD);
}

void MainWindow::displayPowerUsage()
{
    double powerUsage = system_->PowerUsage();

    QLabel* powerUsageDLabel = findChild<QLabel*>("powerUsage");
    powerUsageDLabel->setText(QString("%1W").arg(powerUsage, 0, 'f', 1));

//    QLabel* currentUsageLabel2 = findChild<QLabel*>("currentUsageLabel2");
//    currentUsageLabel2->setText(QString("%1W").arg(powerUsage, 0, 'f', 1));

    colorData(powerUsageDLabel, powerUsage, POWER_THRESHOLD);
}

void MainWindow::displayCpuUtilisation()
{
    float cpu_utilisation = system_->CpuUtilisations()[0] * 100;

    QLabel* cpuUsageLabel = findChild<QLabel*>("cpuUsage");
    cpuUsageLabel->setText(QString("%1%").arg(cpu_utilisation, 0, 'f', 0));

    colorData(cpuUsageLabel, cpu_utilisation, THRESHOLD);
}

void MainWindow::displayTodayEnergyCost()
{
    double total_cost = system_->TotalEnergyUsage() * ENERGY_COST / 1000;

    QLabel* costTodayLabel = findChild<QLabel*>("costToday");
    costTodayLabel->setText(QString("%1p").arg(total_cost, 0, 'f', 1));
}

void MainWindow::displayWeekEnergyCost()
{
    double total_cost = system_->TotalEnergyUsageLastWeek() * ENERGY_COST / 1000;

    QLabel* costWeekLabel = findChild<QLabel*>("costWeek");
    costWeekLabel->setText(QString("%1p").arg(total_cost, 0, 'f', 1));
}

void MainWindow::bindToAllCores()
{
    system_->BindToAllCores();

    ui->bindAllCorePushButton->setStyleSheet("color: black; font-weight: bold;");
    ui->bindPCorePushButton->setStyleSheet("");
    ui->bindECorePushButton->setStyleSheet("");
}

void MainWindow::bindToPCores()
{
    system_->BindToPCores();

    ui->bindPCorePushButton->setStyleSheet("color: black; font-weight: bold;");
    ui->bindAllCorePushButton->setStyleSheet("");
    ui->bindECorePushButton->setStyleSheet("");
}

void MainWindow::bindToECores()
{
    system_->BindToECores();

    ui->bindECorePushButton->setStyleSheet("color: black; font-weight: bold;");
    ui->bindAllCorePushButton->setStyleSheet("");
    ui->bindPCorePushButton->setStyleSheet("");
}

void MainWindow::colorData(QLabel* label, double value, double threshold)
{
    if (value > threshold) {
        label->setStyleSheet("QLabel {color: rgb(246, 97, 81); font: 24pt;}");
    } else {
        label->setStyleSheet("QLabel {color: rgb(87, 227, 137); font: 24pt;}");
    }
}

void MainWindow::displayProcesses()
{
    while (ui->tableWidget->rowCount() > 0) {
        ui->tableWidget->removeRow(0);
    }

    QStringList header;
    ui->tableWidget->setColumnCount(3);
    header << "PID" << "CPU Utilisation" << "Command";
    ui->tableWidget->setHorizontalHeaderLabels(header);

    std::vector<Process> processes = system_->SortedProcesses();
    int row;
    for (int r = 0 ; r < PROCESSES_NUM ; r++) {
        Process p = processes[r];
        int pid = p.Pid();
        float cpuUtilisation = p.CpuUtilisation() * 100;
        std::string command = p.Command();

        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        row = ui->tableWidget->rowCount() - 1;
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(pid)));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString("%1%").arg(cpuUtilisation, 0, 'f', 1)));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(command)));
    }
}

void MainWindow::displayDayReportGraph() {
    QLineSeries *series = new QLineSeries();
    QCategoryAxis *axisX = new QCategoryAxis();

    std::vector<double> energyUsages = system_->HoursEnergyUsages();
    for (int h = 0 ; h < HOURS; h++) {
        axisX->append(QString::number(h),h);
        series->append(h, energyUsages[h]);
    }

    // Create chart, add data, hide legend, and add axis
    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();

    // Customize the title font
    QFont font;
    font.setPixelSize(18);
    chart->setTitleFont(font);
    chart->setTitleBrush(QBrush(Qt::black));
    chart->setTitle("Barry Bonds HRs as Pirate");

    // Change the line color and weight
    QPen pen(QRgb(0x000000));
    pen.setWidth(5);
    series->setPen(pen);

    chart->setAnimationOptions(QChart::AllAnimations);

    // Change the x axis categories
    chart->setAxisX(axisX, series);

    chart->autoFillBackground();

    // Used to display the chart
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    chartView->setParent(ui->reportGraphFrame);
}

