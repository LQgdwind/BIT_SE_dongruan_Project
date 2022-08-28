//coding by zlq
//2022.08.21

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include "qproperty.h"
#include "data.h"

enum ConstNum{HEARTRATE = 0,HIGH_PR = 1,LOW_PR = 2,OXYGEN = 3};
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QSerialPort *qserialport;
    QTimer * qtimer;
    QProperty self;
    bool update();
    friend void sendData(MainWindow*);
    Ui::MainWindow *ui;
    ~MainWindow();

private slots:
    void on_start_clicked();

    void on_clear_clicked();

    void on_exit_clicked();

    void on_confirm_clicked();

    void on_pause_clicked();

    void send();

};

#endif // MAINWINDOW_H
