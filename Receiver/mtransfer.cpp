//code by zlq
//2022.8.22

#include "iostream"
#include "mainwindow.h"

extern Data mydata;

void Data::transferToList()
{
    for(int i = WAVE,j = 0;i<WAVE+size_ecg;i++,j++)
    {
        mydata.ecgWaves.append(package[i]);
    }
    for(int i = WAVE+size_ecg+1,j = 0;i < WAVE+size_ecg+size_spo2+1;i++,j++)
    {
        mydata.spo2Waves.append(package[i]);
    }
    for(int i = WAVE+size_ecg+size_spo2+2,j = 0;i < WAVE+size_ecg+size_spo2+size_resp+2;i++,j++)
    {
        mydata.respWaves.append(package[i]);
    }
}

void Data::updateTransfer()
{
    mydata.heart_rate = mydata.package[2+HEARTRATE];
    mydata.high_pr = mydata.package[2+HIGH_PR];
    mydata.low_pr = mydata.package[2+LOW_PR];
    mydata.oxygen = mydata.package[2+OXYGEN];
}

void Data::printAll()//用于调试
{
    for(auto item:mydata.ecgWaves) qDebug()<<item;
    for(auto item:mydata.spo2Waves) qDebug()<<item;
    for(auto item:mydata.respWaves) qDebug()<<item;
    qDebug()<<"心率"<<mydata.heart_rate;
    qDebug()<<"高压"<<mydata.high_pr;
    qDebug()<<"低压"<<mydata.low_pr;
    qDebug()<<"血氧"<<mydata.oxygen;
}
