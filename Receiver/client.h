//coding by zlj
//modifying by zlq
//2022.8.23

#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include "waveblock.h"

namespace Ui {
class client;
}

class client : public QMainWindow
{
    Q_OBJECT

public:
    explicit client(QWidget *parent = 0);
    ~client();
    Ui::client *ui;
    QWidget * central;
    DrawWave * ecgBlock;
    DrawWave * spo2Block;
    DrawWave * respBlock;
    DataBlock * heartRate;
    DataBlock * highPr;
    DataBlock * lowPr;
    DataBlock * bloodOxygen;
    QTimer *qtimer;
    void iniUI();
private slots:
    void reloadPicture();
};

#endif // CLIENT_H
