#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QCategoryAxis>
#include <QBarSet>
#include <QBarSeries>
#include <QPieSeries>

#include <vector>
#include <string>
#include <map>

#include "format.h"
#include "date_time.h"

#define THRESHOLD 70
#define PROCESSES_NUM 10
#define ENERGY_COST 34
#define HOURS 24
#define PRECISION 1
#define POWER_MAX 999

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("System Monitor");

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

    // Connect timer to slots for continuously updating system information
    connect(timer, SIGNAL(timeout()), this, SLOT(displayDate()));
    connect(timer, SIGNAL(timeout()), this, SLOT(displayTime()));
    connect(timer, SIGNAL(timeout()), this, SLOT(displayCpuChart()));
    connect(timer, SIGNAL(timeout()), this, SLOT(displayTemperatureChart()));
    connect(timer, SIGNAL(timeout()), this, SLOT(displayMemoryChart()));
    connect(timer, SIGNAL(timeout()), this, SLOT(displayPowerUsage()));
    connect(timer, SIGNAL(timeout()), this, SLOT(displayWeekEnergyCost()));
    connect(timer, SIGNAL(timeout()), this, SLOT(displayEnergyUsage()));
    connect(timer, SIGNAL(timeout()), this, SLOT(displayProcesses()));

    timer->start(1000);

    // Connect the clicked signals of the bindAllCore, bindPCore and bindEcore to corresponding slots
    connect(ui->bindAllCorePushButton, &QPushButton::clicked, this, &MainWindow::bindToAllCores);
    connect(ui->bindPCorePushButton, &QPushButton::clicked, this, &MainWindow::bindToPCores);
    connect(ui->bindECorePushButton, &QPushButton::clicked, this, &MainWindow::bindToECores);
    connect(ui->dayChartButton, &QRadioButton::clicked, this, &MainWindow::clickEnergyReportButtons);
    connect(ui->accumDayChartButton, &QRadioButton::clicked, this, &MainWindow::clickEnergyReportButtons);
    connect(ui->weekChartButton, &QRadioButton::clicked, this, &MainWindow::clickEnergyReportButtons);

    ui->dayChartButton->click();

    ui->budgetInput->setPlaceholderText("set daily budget");
    connect(ui->budgetInput, &QLineEdit::returnPressed, this, &MainWindow::budgetInputReturn);

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

void MainWindow::displayMemoryChart() {
    float utilisation = system_->MemoryUtilisation() * 100;
    createDonutChart(utilisation, "RAM Memory", ui->ramChartView, "%", PRECISION);
}

void MainWindow::displayCpuChart() {
    float utilisation = system_->CpuUtilisations()[0] * 100;
    createDonutChart(utilisation, "CPU", ui->cpuChartView, "%", PRECISION);
}

void MainWindow::displayTemperatureChart() {
    int temperature = system_->CpuTemperature();
    createDonutChart(temperature, "Temperature", ui->tempChartView, "°C", 0);
}

void MainWindow::displayPowerUsage()
{
    double powerUsage = system_->PowerUsage();

    if (powerUsage > POWER_MAX) {
        powerUsage = 0;
    }

    QLabel* powerUsageDLabel = findChild<QLabel*>("powerUsage");
    powerUsageDLabel->setText(QString("%1W").arg(powerUsage, 0, 'f', PRECISION));

    QLabel* powerUsageRLabel = findChild<QLabel*>("powerUsageR");
    powerUsageRLabel->setText(QString("%1W").arg(powerUsage, 0, 'f', PRECISION));
}

void MainWindow::displayWeekEnergyCost()
{
    double energyUsage = system_->TotalEnergyUsageLastWeek();
    double cost = energyUsage * ENERGY_COST / 1000;

    QLabel* energyUsageWeekLabel = findChild<QLabel*>("usageWeek");
    energyUsageWeekLabel->setText(QString("%1Wh").arg(energyUsage, 0, 'f', PRECISION));
    QLabel* costWeekLabel = findChild<QLabel*>("costWeek");
    costWeekLabel->setText(QString("%1p").arg(cost, 0, 'f', PRECISION));
}

void MainWindow::displayEnergyUsage()
{
    double energyUsage = system_->TotalEnergyUsage();
    double cost = energyUsage * ENERGY_COST / 1000;

    QLabel* energyUsageLabel = findChild<QLabel*>("energyUsage");
    energyUsageLabel->setText(QString("%1Wh").arg(energyUsage, 0, 'f', PRECISION));
    QLabel* energyUsageRLabel = findChild<QLabel*>("energyUsageR");
    energyUsageRLabel->setText(QString("%1Wh").arg(energyUsage, 0, 'f', PRECISION));

    QLabel* costTodayLabel = findChild<QLabel*>("costToday");
    costTodayLabel->setText(QString("%1p").arg(cost, 0, 'f', PRECISION));
    QLabel* costTodayRLabel = findChild<QLabel*>("costTodayR");
    costTodayRLabel->setText(QString("%1p").arg(cost, 0, 'f', PRECISION));
}

void MainWindow::budgetInputReturn() {
    QString userInput = ui->budgetInput->text();
    bool ok;
    int budget = userInput.toInt(&ok);

    if (!ok) {
        ui->budgetInput->setText("invalid input!");
    } else {
        ui->budgetInput->setText(QString::number(budget));
        system_->SetBudget(budget);
    }
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
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString("%1%").arg(cpuUtilisation, 0, 'f', PRECISION)));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(command)));
    }
}

void MainWindow::clickEnergyReportButtons()
{
    QRadioButton *clickedButton = qobject_cast<QRadioButton*>(sender());

    if (clickedButton == ui->dayChartButton) {
        disconnect(timer, SIGNAL(timeout()), this, SLOT(displayAccumDayReportGraph()));
        disconnect(timer, SIGNAL(timeout()), this, SLOT(displayWeekReportGraph()));
        connect(timer, SIGNAL(timeout()), this, SLOT(displayDayReportGraph()));
    } else if (clickedButton == ui->accumDayChartButton) {
        disconnect(timer, SIGNAL(timeout()), this, SLOT(displayDayReportGraph()));
        disconnect(timer, SIGNAL(timeout()), this, SLOT(displayWeekReportGraph()));
        connect(timer, SIGNAL(timeout()), this, SLOT(displayAccumDayReportGraph()));
    } else if (clickedButton == ui->weekChartButton) {
        disconnect(timer, SIGNAL(timeout()), this, SLOT(displayDayReportGraph()));
        disconnect(timer, SIGNAL(timeout()), this, SLOT(displayAccumDayReportGraph()));
        connect(timer, SIGNAL(timeout()), this, SLOT(displayWeekReportGraph()));
    }

}

void MainWindow::displayDayReportGraph() {
    QBarSet *set = new QBarSet("");
    QBarCategoryAxis *axisX = new QBarCategoryAxis();

    std::vector<double> energyUsages = system_->HoursEnergyUsages();
    for (int h = 0 ; h < DateTime::Hour() + 1; h++) {
        axisX->append(QString::number(h));
        set->append(energyUsages[h]);
    }

    QBarSeries *series = new QBarSeries();
    series->append(set);

    // Create chart, add data, hide legend, and add axis
    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();

    // Customize the title font
    QFont font;
    font.setPixelSize(18);
    chart->setTitleFont(font);
//    chart->setTitleBrush(QBrush(Qt::black));
    chart->setTitle("Hourly Energy Consumption (Wh)");
    chart->setAxisX(axisX, series);
    chart->autoFillBackground();

    ui->reportGraph->setChart(chart);
    ui->reportGraph->setRenderHint(QPainter::Antialiasing);
}

void MainWindow::displayWeekReportGraph() {
    QBarSet *set = new QBarSet("");
    QBarCategoryAxis *axisX = new QBarCategoryAxis();

    std::map<std::string, double> usages = system_->LastWeekEnergyUsage();
    int x = 0;
    for (auto const& [key, val] : usages) {
        axisX->append(QString::fromStdString(key));
        set->append(val);
        x++;
    }

    QBarSeries *series = new QBarSeries();
    series->append(set);

    // Create chart, add data, hide legend, and add axis
    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();

    // Customize the title font
    QFont font;
    font.setPixelSize(18);
    chart->setTitleFont(font);
//    chart->setTitleBrush(QBrush(Qt::black));
    chart->setTitle("Energy Consumption in Last Week (Wh)");
    chart->setAxisX(axisX, series);
    chart->autoFillBackground();

    ui->reportGraph->setChart(chart);
    ui->reportGraph->setRenderHint(QPainter::Antialiasing);
}

void MainWindow::displayAccumDayReportGraph() {
    QLineSeries *series0 = new QLineSeries();
    QLineSeries *series1 = new QLineSeries();
    QLineSeries *lineSeries = new QLineSeries();
    QCategoryAxis *axisX = new QCategoryAxis();

    double budget = system_->Budget();
    std::vector<double> energyUsages = system_->HoursEnergyUsages();
    double prevTotalUsage = 0;
    for (int h = 0 ; h < DateTime::Hour() + 1 ; h++) {
        series0->append(h, prevTotalUsage + energyUsages[h]);
        series1->append(h, 0);
        lineSeries->append(h, budget);
        axisX->append(QString::number(h),h);
        prevTotalUsage += energyUsages[h];
    }

    QAreaSeries *series = new QAreaSeries(series0, series1);
    lineSeries->setName("budget");
    series->setName("energy usage");

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->addSeries(lineSeries);
    chart->createDefaultAxes();

    QFont font;
    font.setPixelSize(18);
    chart->setTitleFont(font);
    chart->setTitle("Accumulated Hourly Energy Consumption (Wh)");
    chart->setAxisX(axisX, series);
    chart->autoFillBackground();

    if (budget > prevTotalUsage) {
        chart->axes(Qt::Vertical).first()->setRange(0, budget + 100);
    } else {
        chart->axes(Qt::Vertical).first()->setRange(0, prevTotalUsage + 100);
    }

    ui->reportGraph->setChart(chart);
    ui->reportGraph->setRenderHint(QPainter::Antialiasing);

}

void MainWindow::createDonutChart(float value, std::string title, QChartView *chartView, std::string unit, int precision) {
    QPieSeries *series = new QPieSeries();
    series->setHoleSize(0.5);
    series->append("", value);

    for (auto slice : series->slices()) {
        if (value > THRESHOLD) {
            slice->setBrush(QBrush(QColor(237, 51, 59)));
        } else {
            slice->setBrush(QBrush(QColor(87, 227, 137)));
        }
        //rgb(143, 240, 164);rgb(246, 97, 81)
        //rgb(87, 227, 137);rgb(237, 51, 59)
    }

    series->append("", 100 - value);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->legend()->hide();
    chart->setMargins({0, 20, 0, 0});
    chart->setTitle(QString::fromStdString(title));
    QFont titleFont = QFont("Ubuntu", 16);
    titleFont.setBold(true);
    chart->setTitleFont(titleFont);

    chartView->setChart(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QGraphicsTextItem *label = new QGraphicsTextItem(QString::fromStdString("%1" + unit).arg(value, 0, 'f', precision));
    label->setFont(QFont("Ubuntu", 24));
    QRectF rect = label->boundingRect();
    QPointF center = chartView->rect().center();
    label->setPos(center - QPointF(rect.width()/2, rect.height()/2 - 20));
    chartView->scene()->addItem(label);
}
