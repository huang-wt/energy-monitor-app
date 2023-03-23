#include "include/mainwindow.h"

#include <vector>
#include <string>
#include <map>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QChartView>
#include <QLineSeries>
#include <QCategoryAxis>
#include <QBarSet>
#include <QBarSeries>
#include <QPieSeries>

#include "include/format.h"
#include "include/date_time.h"
#include "ui_mainwindow.h"

#define INTERVAL 1000
#define PERCENT_CNV_AMT 100
#define THRESHOLD 70
#define PROCESSES_NUM 17
#define ENERGY_COST 34 / 1000
#define HOURS 24
#define PRECISION 1
#define POWER_MAX 999
#define TITLE_FONT_SIZE 16
#define DATA_FONT_SIZE 24
#define DONUT_HOLE_SIZE 0.5
#define SCALE 1.1
#define UNSELECTED_MODE_STYLE "color: rgb(255, 255, 255);\
                               border: transparent;\
                               border-radius:10px;\
                               font:15px;\
                               background-color: #7dadfc;"
#define SELECTED_MODE_STYLE "color: rgb(255, 255, 255);\
                             border: transparent;\
                             border-radius:10px;\
                             font:15px;\
                             font-weight: bold;\
                             background-color: #808cfc;"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("System Monitor");

    // Set up sidebar
    ui->treeWidget->header()->hide();
    ui->treeWidget->topLevelItem(0)->setIcon(0,QIcon(":/dashboard.png"));
    ui->treeWidget->topLevelItem(1)->setIcon(0,QIcon(":/process.png"));
    ui->treeWidget->topLevelItem(2)->setIcon(0,QIcon(":/report.png"));
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
    connect(timer, SIGNAL(timeout()), this, SLOT(displayWeekEnergyUsage()));
    connect(timer, SIGNAL(timeout()), this, SLOT(displayEnergyUsage()));
    connect(timer, SIGNAL(timeout()), this, SLOT(displayProcesses()));

    timer->start(INTERVAL);

    // Connect the clicked signals of the bindAllCore, bindPCore and bindEcore to corresponding slots
    connect(ui->maxTurboButton, &QPushButton::clicked, this, &MainWindow::maxTurboMode);
    connect(ui->highPerformButton, &QPushButton::clicked, this, &MainWindow::highPerformMode);
    connect(ui->powerSaverButton, &QPushButton::clicked, this, &MainWindow::powerSaverMode);
    connect(ui->balancedButton, &QPushButton::clicked, this, &MainWindow::balancedMode);
    connect(ui->dayChartButton, &QRadioButton::clicked, this, &MainWindow::clickEnergyReportButtons);
    connect(ui->accumDayChartButton, &QRadioButton::clicked, this, &MainWindow::clickEnergyReportButtons);
    connect(ui->weekChartButton, &QRadioButton::clicked, this, &MainWindow::clickEnergyReportButtons);

    // Default buttons/modes
    ui->maxTurboButton->click();
    ui->dayChartButton->click();
    ui->budgetInput->setPlaceholderText("set energy cap");
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

void MainWindow::on_treeWidget_currentItemChanged(QTreeWidgetItem *current)
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
    std::string time = DateTime::CurrentTime();

    QLabel* timeLabel = findChild<QLabel*>("time");
    timeLabel->setText(QString::fromStdString(time));
}

void MainWindow::displayMemoryChart()
{
    float utilisation = system_->MemoryUtilisation() * PERCENT_CNV_AMT;
    createDonutChart(utilisation, "RAM Memory", ui->ramChartView, "%", PRECISION);
}

void MainWindow::displayCpuChart()
{
    float utilisation = system_->CpuUtilisations()[0] * PERCENT_CNV_AMT;
    createDonutChart(utilisation, "CPU", ui->cpuChartView, "%", PRECISION);
}

void MainWindow::displayTemperatureChart()
{
    int temperature = system_->CpuTemperature();
    createDonutChart(temperature, "Temperature", ui->tempChartView, "°C", 0);
}

void MainWindow::displayPowerUsage()
{
    double powerUsage = system_->PowerUsage();

    if (powerUsage > POWER_MAX)
    {
        powerUsage = 0;
    }

    QLabel* powerUsageDLabel = findChild<QLabel*>("powerUsage");
    powerUsageDLabel->setText(QString("%1W").arg(powerUsage, 0, 'f', PRECISION));
}

void MainWindow::displayWeekEnergyUsage()
{
    double energyUsage = system_->TotalEnergyUsageLastWeek();
    double cost = energyUsage * ENERGY_COST;

    QLabel* energyUsageWeekLabel = findChild<QLabel*>("usageWeek");
    energyUsageWeekLabel->setText(QString("%1Wh").arg(energyUsage, 0, 'f', PRECISION));
    QLabel* costWeekLabel = findChild<QLabel*>("costWeek");
    costWeekLabel->setText(QString("%1p").arg(cost, 0, 'f', PRECISION));
}

void MainWindow::displayEnergyUsage()
{
    double energyUsage = system_->TotalEnergyUsage();
    double cost = energyUsage * ENERGY_COST;

    QLabel* energyUsageLabel = findChild<QLabel*>("energyUsage");
    energyUsageLabel->setText(QString("%1Wh").arg(energyUsage, 0, 'f', PRECISION));
    QLabel* energyUsageRLabel = findChild<QLabel*>("energyUsageR");
    energyUsageRLabel->setText(QString("%1Wh").arg(energyUsage, 0, 'f', PRECISION));

    QLabel* costTodayLabel = findChild<QLabel*>("costToday");
    costTodayLabel->setText(QString("%1p").arg(cost, 0, 'f', PRECISION));
    QLabel* costTodayRLabel = findChild<QLabel*>("costTodayR");
    costTodayRLabel->setText(QString("%1p").arg(cost, 0, 'f', PRECISION));
}

void MainWindow::budgetInputReturn()
{
    QString userInput = ui->budgetInput->text();
    bool ok;
    int budget = userInput.toInt(&ok);

    if (!ok)
    {
        ui->budgetInput->setText("invalid input!");
    } else
    {
        ui->budgetInput->setText(QString::number(budget));
        system_->SetEnergyCap(budget);
    }
}

void MainWindow::maxTurboMode()
{
    system_->BindToAllCores();

    ui->maxTurboButton->setStyleSheet(SELECTED_MODE_STYLE);
    ui->highPerformButton->setStyleSheet(UNSELECTED_MODE_STYLE);
    ui->powerSaverButton->setStyleSheet(UNSELECTED_MODE_STYLE);
    ui->balancedButton->setStyleSheet(UNSELECTED_MODE_STYLE);
}

void MainWindow::highPerformMode()
{
    system_->BindToPCores();

    ui->highPerformButton->setStyleSheet(SELECTED_MODE_STYLE);
    ui->maxTurboButton->setStyleSheet(UNSELECTED_MODE_STYLE);
    ui->powerSaverButton->setStyleSheet(UNSELECTED_MODE_STYLE);
    ui->balancedButton->setStyleSheet(UNSELECTED_MODE_STYLE);
}

void MainWindow::powerSaverMode()
{
    system_->BindToECores();

    ui->powerSaverButton->setStyleSheet(SELECTED_MODE_STYLE);
    ui->maxTurboButton->setStyleSheet(UNSELECTED_MODE_STYLE);
    ui->highPerformButton->setStyleSheet(UNSELECTED_MODE_STYLE);
    ui->balancedButton->setStyleSheet(UNSELECTED_MODE_STYLE);
}

void MainWindow::balancedMode()
{
    system_->BindToPAndECores();

    ui->balancedButton->setStyleSheet(SELECTED_MODE_STYLE);
    ui->maxTurboButton->setStyleSheet(UNSELECTED_MODE_STYLE);
    ui->highPerformButton->setStyleSheet(UNSELECTED_MODE_STYLE);
    ui->powerSaverButton->setStyleSheet(UNSELECTED_MODE_STYLE);
}

void MainWindow::displayProcesses()
{
    while (ui->tableWidget->rowCount() > 0)
    {
        ui->tableWidget->removeRow(0);
    }

    QStringList header;
    ui->tableWidget->setColumnCount(3);
    header << "PID" << "CPU Utilisation" << "Command";
    ui->tableWidget->setHorizontalHeaderLabels(header);

    std::vector<Process> processes = system_->SortedProcesses();
    Process p;
    int row;
    for (int r = 0 ; r < PROCESSES_NUM ; r++)
    {
        p = processes[r];
        int pid = p.Pid();
        if (pid != 0)
        {
            float cpuUtilisation = p.CpuUtilisation() * PERCENT_CNV_AMT;
            std::string command = p.Command();

            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            row = ui->tableWidget->rowCount() - 1;
            ui->tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(pid)));
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString("%1%").arg(cpuUtilisation, 0, 'f', PRECISION)));
            ui->tableWidget->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(command)));
        }
    }
}

void MainWindow::clickEnergyReportButtons()
{
    QRadioButton *clickedButton = qobject_cast<QRadioButton*>(sender());

    if (clickedButton == ui->dayChartButton)
    {
        disconnect(timer, SIGNAL(timeout()), this, SLOT(displayAccumDayReportGraph()));
        disconnect(timer, SIGNAL(timeout()), this, SLOT(displayWeekReportGraph()));
        connect(timer, SIGNAL(timeout()), this, SLOT(displayDayReportGraph()));
    } else if (clickedButton == ui->accumDayChartButton)
    {
        disconnect(timer, SIGNAL(timeout()), this, SLOT(displayDayReportGraph()));
        disconnect(timer, SIGNAL(timeout()), this, SLOT(displayWeekReportGraph()));
        connect(timer, SIGNAL(timeout()), this, SLOT(displayAccumDayReportGraph()));
    } else if (clickedButton == ui->weekChartButton)
    {
        disconnect(timer, SIGNAL(timeout()), this, SLOT(displayDayReportGraph()));
        disconnect(timer, SIGNAL(timeout()), this, SLOT(displayAccumDayReportGraph()));
        connect(timer, SIGNAL(timeout()), this, SLOT(displayWeekReportGraph()));
    }

}

void MainWindow::displayDayReportGraph()
{
    QBarSet *set = new QBarSet("");
    std::vector<double> energyUsages = system_->HoursEnergyUsages();
    double maxUsage = 0;
    for (int h = 0 ; h < DateTime::Hour() + 1; h++)
    {
        set->append(energyUsages[h]);
        maxUsage = energyUsages[h] > maxUsage ? energyUsages[h] : maxUsage;
    }
    QBarSeries *series = new QBarSeries();
    series->append(set);

    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0, DateTime::Hour());
    axisX->setLabelFormat("%d");
    axisX->setTickCount(DateTime::Hour() + 1);

    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0, maxUsage * SCALE);
    axisY->setLabelFormat("%d");

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->autoFillBackground();
    chart->addSeries(series);
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    // Customize the title font
    QFont font;
    font.setPixelSize(TITLE_FONT_SIZE);
    chart->setTitleFont(font);
    chart->setTitle("Hourly Energy Consumption (Wh)");

    ui->reportGraph->setChart(chart);
    ui->reportGraph->setRenderHint(QPainter::Antialiasing);
}

void MainWindow::displayWeekReportGraph()
{
    QBarSet *set = new QBarSet("");
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    std::map<std::string, double> usages = system_->LastWeekEnergyUsage();
    int x = 0;
    double maxUsage = 0;
    for (auto const& [date, usage] : usages)
    {
        axisX->append(QString::fromStdString(date));
        set->append(usage);
        maxUsage = usage > maxUsage ? usage : maxUsage;
        x++;
    }
    QBarSeries *series = new QBarSeries();
    series->append(set);
    axisX->setGridLineVisible(false);
    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0, maxUsage * SCALE);
    axisY->setLabelFormat("%d");

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
//    chart->createDefaultAxes();
    chart->autoFillBackground();
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    // Customize the title font
    QFont font;
    font.setPixelSize(TITLE_FONT_SIZE);
    chart->setTitleFont(font);
    chart->setTitle("Energy Consumption in Last Week (Wh)");

    ui->reportGraph->setChart(chart);
    ui->reportGraph->setRenderHint(QPainter::Antialiasing);
}

void MainWindow::displayAccumDayReportGraph()
{
    QLineSeries *seriesUp = new QLineSeries();
    QLineSeries *seriesDown = new QLineSeries();
    QLineSeries *lineSeries = new QLineSeries();

    double budget = system_->EnergyCap();
    std::vector<double> energyUsages = system_->HoursEnergyUsages();
    double prevTotalUsage = 0;
    for (int h = 0 ; h < DateTime::Hour() + 1 ; h++)
    {
        seriesUp->append(h, prevTotalUsage + energyUsages[h]);
        seriesDown->append(h, 0);
        lineSeries->append(h, budget);
        prevTotalUsage += energyUsages[h];
    }

    QAreaSeries *areaSeries = new QAreaSeries(seriesUp, seriesDown);
    lineSeries->setName("cap");
    areaSeries->setName("energy usage");
    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0, DateTime::Hour());
    axisX->setLabelFormat("%d");
    axisX->setTickCount(DateTime::Hour() + 1);

    double rangeY = budget > prevTotalUsage ? budget * SCALE : prevTotalUsage * SCALE;
    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0, rangeY);
    axisY->setLabelFormat("%d");

    QChart *chart = new QChart();
    chart->addSeries(areaSeries);
    chart->addSeries(lineSeries);
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    lineSeries->attachAxis(axisX);
    lineSeries->attachAxis(axisY);
    areaSeries->attachAxis(axisX);
    areaSeries->attachAxis(axisY);

    QFont font;
    font.setPixelSize(TITLE_FONT_SIZE);
    chart->setTitleFont(font);
    chart->setTitle("Accumulated Hourly Energy Consumption (Wh)");
    chart->autoFillBackground();

    ui->reportGraph->setChart(chart);
    ui->reportGraph->setRenderHint(QPainter::Antialiasing);

}

void MainWindow::createDonutChart(float value, std::string title, QChartView *chartView, std::string unit, int precision)
{
    QPieSeries *series = new QPieSeries();
    series->setHoleSize(DONUT_HOLE_SIZE);
    series->append("", value);

    for (auto slice : series->slices())
    {
        if (value > THRESHOLD) {
            slice->setBrush(QBrush(QColor(237, 51, 59))); // red
        } else {
            slice->setBrush(QBrush(QColor(87, 227, 137))); // green
        }
    }

    series->append("", 100 - value);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->legend()->hide();
    chart->setMargins({0, 20, 0, 0});
    chart->setTitle(QString::fromStdString(title));
    QFont titleFont = QFont("Ubuntu", TITLE_FONT_SIZE);
    titleFont.setBold(true);
    chart->setTitleFont(titleFont);

    chartView->setChart(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Display number in the center of Donut
    QGraphicsTextItem *label = new QGraphicsTextItem(QString::fromStdString("%1" + unit).arg(value, 0, 'f', precision));
    label->setFont(QFont("Ubuntu", DATA_FONT_SIZE));
    QRectF rect = label->boundingRect();
    QPointF center = chartView->rect().center();
    label->setPos(center - QPointF(rect.width()/2, rect.height()/2 - 20));
    chartView->scene()->addItem(label);
}

