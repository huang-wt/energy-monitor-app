#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
//#include "memory.h"
//#include "processor.h"
//#include "power.h"




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // Set up the user interface
    ui->setupUi(this);

    // Create QTimer object for updating live data
    QTimer* timer = new QTimer(this);

    // Connect timer to slots for updating memory usage, CPU temperature, and power usage
    connect(timer, SIGNAL(timeout()), this, SLOT(updateMemoryUsage()));
    connect(timer, SIGNAL(timeout()), this, SLOT(updateCpuTemperature()));
    connect(timer, SIGNAL(timeout()), this, SLOT(updatePowerUsage()));

    // Connect the clicked signals of the bindAllCore, bindPCore and bindEcore to corresponding slots
    connect(ui->bindAllCorePushButton, &QPushButton::clicked, this, &MainWindow::bindToAllCores);
    connect(ui->bindPCorePushButton, &QPushButton::clicked, this, &MainWindow::bindToPCores);
    connect(ui->bindECorePushButton, &QPushButton::clicked, this, &MainWindow::bindToECores);
    connect(ui->applyButton, &QPushButton::clicked, this, &MainWindow::applySettings);




    // Start timer with interval of 1 second (1000 ms)
    timer->start(1000);

    // Set default dashboard color
    ui->dashBoardButton->setStyleSheet("background-color: #fdfdfd; color: #000000");

    // Connect button clicks to their respective slots
    connect(ui->dashBoardButton, &QPushButton::clicked, this, &MainWindow::on_dashBoardButton_clicked);
    connect(ui->powerModeButton, &QPushButton::clicked, this, &MainWindow::on_powerModeButton_clicked);
    connect(ui->processesButton, &QPushButton::clicked, this, &MainWindow::on_processesButton_clicked);
    connect(ui->reportsButton, &QPushButton::clicked, this, &MainWindow::on_reportsButton_clicked);

    // Install event filter on buttons to detect hover and click events
    ui->dashBoardButton->installEventFilter(this);
    ui->powerModeButton->installEventFilter(this);
    ui->processesButton->installEventFilter(this);
    ui->reportsButton->installEventFilter(this);
}


MainWindow::~MainWindow()
{
    delete ui;
}


//Close 'x' button
void MainWindow::on_pushButton_3_clicked()
{
    close();
}


//Minimuse '-' button
void MainWindow::on_pushButton_2_clicked()
{
    showMinimized();
}


//Minimise Maximise square Button
void MainWindow::on_pushButton_4_clicked()
{
    if (isMaximized()) {
        showNormal();
    } else {
        showMaximized();
    }

}


void MainWindow::on_dashBoardButton_clicked()
{
    ui->stackedWidget_2->setCurrentWidget(ui->page_3);

    // Set the background color of the dashBoardButton to#white and other buttons to their default color
    ui->dashBoardButton->setStyleSheet("background-color: #fdfdfd; color: #000000");
    ui->powerModeButton->setStyleSheet("");
    ui->processesButton->setStyleSheet("");
    ui->reportsButton->setStyleSheet("");
}


void MainWindow::on_powerModeButton_clicked()
{
    ui->stackedWidget_2->setCurrentWidget(ui->page_4);

    // Set the background color of the powerModeButton to #white, font to black and other buttons to their default color
    ui->powerModeButton->setStyleSheet("background-color: #fdfdfd; color: #000000");
    ui->dashBoardButton->setStyleSheet("");
    ui->processesButton->setStyleSheet("");
    ui->reportsButton->setStyleSheet("");
}


void MainWindow::on_processesButton_clicked()
{
    int pageIndex = ui->stackedWidget_2->indexOf(ui->page_5);
    if (pageIndex != -1) {
        ui->stackedWidget_2->setCurrentIndex(pageIndex);
    }

   // Set the background color of the powerModeButton to #white, font to black and other buttons to their default color
    ui->processesButton->setStyleSheet("background-color: #fdfdfd; color: #000000");
    ui->dashBoardButton->setStyleSheet("");
    ui->powerModeButton->setStyleSheet("");
    ui->reportsButton->setStyleSheet("");
}


void MainWindow::on_reportsButton_clicked()
{
    int pageIndex = ui->stackedWidget_2->indexOf(ui->page_6);
    if (pageIndex != -1) {
        ui->stackedWidget_2->setCurrentIndex(pageIndex);
    }

    // Set the background color of the powerModeButton to #white, font to black and other buttons to their default color
    ui->reportsButton->setStyleSheet("background-color: #fdfdfd; color: #000000");
    ui->dashBoardButton->setStyleSheet("");
    ui->powerModeButton->setStyleSheet("");
    ui->processesButton->setStyleSheet("");
}


//WIP: HOVER EFFECT ON BUTTON
//bool MainWindow::eventFilter(QObject *object, QEvent *event)
//{
//    if (event->type() == QEvent::Enter) {
//        if (object == ui->dashBoardButton) {
//            ui->dashBoardButton->setStyleSheet("background-color: purple");
//        } else if (object == ui->powerModeButton) {
//            ui->powerModeButton->setStyleSheet("background-color: red");
//        } else if (object == ui->processesButton) {
//            ui->processesButton->setStyleSheet("background-color: blue");
//        } else if (object == ui->reportsButton) {
//            ui->reportsButton->setStyleSheet("background-color: green");
//        }
//    } else if (event->type() == QEvent::Leave) {
//        if (object == ui->dashBoardButton) {
//            ui->dashBoardButton->setStyleSheet("");
//        } else if (object == ui->powerModeButton) {
//            ui->powerModeButton->setStyleSheet("");
//        } else if (object == ui->processesButton) {
//            ui->processesButton->setStyleSheet("");
//        } else if (object == ui->reportsButton) {
//            ui->reportsButton->setStyleSheet("");
//        }
//    }
//    return false;
//}


//****INTEGRATION BACKEND TO FRONTEND****//

//***To upate memory Usage on Dash Board Page
//void MainWindow::updateMemoryUsage()
//{
//    Memory memory;

//    // update used memory
//    memory.UpdateUsedMemory();

//    // get utilization as a percentage
//    float utilization = memory.Utilisation() * 100;

//    // update memory utilization label text
//    QLabel* memoryUtilizationLabel = findChild<QLabel*>("memoryUtilizationLabel");
//    memoryUtilizationLabel->setText(QString("%1%").arg(utilization));
//}


//***To upate CPU Temperature on Dash Board Page
//void MainWindow::updateCpuTemperature()
//{
//    Processor processor;

//    // update CPU temperature
//    processor.UpdateTemperature();

//    // get CPU temperature value
//    int temperature = processor.Temperature();

//    // update CPU temperature label text
//    QLabel* cpuTemperatureLabel = findChild<QLabel*>("cpuTemperatureLabel");
//    cpuTemperatureLabel->setText(QString("%1°C").arg(temperature));
//}

//***To upate current Power Usage on Dash Board Page and Reports Page
//void MainWindow::updatePowerUsage()
//{
//    Power power;

//    // update power and energy usage
//    power.UpdatePowerAndEnergyUsage();

//    // get power usage value
//    double powerUsage = power.CurrPowerUsage();

//    // update current usage labels
//    QLabel* currentUsageLabel = findChild<QLabel*>("currentUsageLabel");
//    currentUsageLabel->setText(QString("%1").arg(powerUsage, 0, 'f', 2));

//    QLabel* currentUsageLabel2 = findChild<QLabel*>("currentUsageLabel2");
//    currentUsageLabel2->setText(QString("%1").arg(powerUsage, 0, 'f', 2));
//}


//***To handle Core Binding[ALL, P, E] on power page
//void MyMainWindow::bindToAllCores() {
//    system.BindToAllCores();

//    // Set the text color of the bindAllCorePushButton to black.
//    ui->bindAllCorePushButton->setStyleSheet("color: black; font-weight: bold;");

//    // Reset the text color of the other buttons to their original state.
//    ui->bindPCorePushButton->setStyleSheet("");
//    ui->bindECorePushButton->setStyleSheet("");
//}

//void MyMainWindow::bindToPCores() {
//    system.BindToPCores();

//    // Set the text color of the bindAllCorePushButton to black.
//    ui->bindPCorePushButton->setStyleSheet("color: black; font-weight: bold;");


//    // Reset the text color of the other buttons to their original state.
//    ui->bindAllCorePushButton->setStyleSheet("");
//    ui->bindECorePushButton->setStyleSheet("");
//}

//void MyMainWindow::bindToECores() {
//    system.BindToECores();

//    // Set the text color of the bindAllCorePushButton to black.
//    ui->bindECorePushButton->setStyleSheet("color: black; font-weight: bold;");



//    // Reset the text color of the other buttons to their original state.
//    ui->bindAllCorePushButton->setStyleSheet("");
//    ui->bindPCorePushButton->setStyleSheet("");
//}

//void MyMainWindow::applySettings() {
//    if (ui->bindAllCorePushButton->isChecked()) {
//        system.BindToAllCores();
//    } else if (ui->bindPCorePushButton->isChecked()) {
//        system.BindToPCores();
//    } else if (ui->bindECorePushButton->isChecked()) {
//        system.BindToECores();
//    }
//}










