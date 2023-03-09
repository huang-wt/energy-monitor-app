#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QToolButton>
#include <QWidget>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


//protected:
//    bool eventFilter(QObject *object, QEvent *event);

private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked();

    void on_dashBoardButton_clicked();

    void on_powerModeButton_clicked();

    void on_processesButton_clicked();

    void on_reportsButton_clicked();



private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
