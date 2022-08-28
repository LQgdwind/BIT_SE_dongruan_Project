//coding by zlq
//2022.08.21

#include "mainwindow.h"
#include "iostream"

QProperty::QProperty(QString s_portname, int s_heartrate, int s_oxygen, int s_high_pr, int s_low_pr, int s_freq)
    : Portname(s_portname),
      heartrate(s_heartrate),
      freq(s_freq),
      high_pr(s_high_pr),
      low_pr(s_low_pr),
      oxygen(s_oxygen)
{}

QString QProperty::getPortName()const
{
    return this->Portname;
}
int QProperty::getFreq()const
{
    return this->freq;
}
int QProperty::getHeartRate()const
{
    return this->heartrate;
}
int QProperty::getHigh_pr()const
{
    return this->high_pr;
}
int QProperty::getLow_pr()const
{
    return this->low_pr;
}
int QProperty::getOxygen()const
{
    return this->oxygen;
}
void QProperty::setHeartRate(int a)
{
    this->heartrate = a;
}
void QProperty::setHigh_pr(int a)
{
    this->high_pr = a;
}
void QProperty::setLow_pr(int a)
{
    this->low_pr = a;
}
void QProperty::setOxygen(int a)
{
    this->oxygen = a;
}
void QProperty::setFreq(int a)
{
    this->freq= a;
}
void QProperty::setPortname(QString a)
{
    this->Portname = a;
}
