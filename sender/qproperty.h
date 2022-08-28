//coding by zlq
//2022.08.21

#ifndef QPROPERTY_H
#define QPROPERTY_H
#include <QString>
class QProperty
{
private:
    QString Portname;
    int heartrate;
    int high_pr;
    int low_pr;
    int oxygen;
    int freq;
public:
    QProperty(QString = "",int = 0,int = 0,int = 0,int = 0,int = 0);
    int getHeartRate()const;
    int getHigh_pr()const;
    int getLow_pr()const;
    int getOxygen()const;
    int getFreq()const;
    QString getPortName()const;
    void setPortname(QString);
    void setHeartRate(int);
    void setHigh_pr(int);
    void setLow_pr(int);
    void setOxygen(int);
    void setFreq(int);
};

#endif // QPROPERTY_H
