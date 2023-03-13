#include "mainwindow.h"
#include "ui_mainwindow.h"

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
