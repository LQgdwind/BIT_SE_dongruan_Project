//code by zlq
//2022.8.22

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "data.h"
#include "subwindow.h"
#include "client.h"
#include <QApplication>
#include <QSerialPort>
#include <QSerialPortInfo>
#include<QDebug>
#include <QTimer>

enum ConstNum{HEARTRATE = 0,HIGH_PR = 1,LOW_PR = 2,OXYGEN = 3,size_ecg = 500,size_spo2 = 200,size_resp = 100,WAVE = 7};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    client myclient;
    SubWindow myclientservice;

public:
    enum
    {
        cnt = 812,
    };
    //数据帧数组大小
    QSerialPort * qserialport;
    QByteArray Dataset; //处理数据用的数据集
    QByteArray Dataset2; //叠加数据用的数据集
    QByteArray Readdata;//读取数据
    QTimer *myTimer;  //计时器
public:
    int SerialInit(void);//初始化
public slots:
    void readData();//读数据
    void TimerUpdate_qserialport();
signals:
    void my_ready();
    void updatePicture();
private slots:
    void on_confirm_clicked();
    void on_start_clicked();
    void on_pause_clicked();
    void on_draw_clicked();
    void on_exit_draw_clicked();
    void on_open_client_clicked();
    void on_exit_client_clicked();
};

#endif // MAINWINDOW_H
