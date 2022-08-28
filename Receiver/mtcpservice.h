//code by lhp
//22.08.22
#ifndef MTCPSERVICE_H
#define MTCPSERVICE_H

#include <QObject>
#include <QHostAddress>
#include <QTcpSocket>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QList>

class MTcpService : public QObject
{
    Q_OBJECT
public:
    explicit MTcpService(QObject *parent = nullptr);
    void startDisconnecting();
    QTcpSocket *server;
private:
    QHostAddress addr;
    quint16 port;
    QString gender;
    QString name;
    int age;
    QVariantList ecg;
    QVariantList spo2;
    QVariantList resp;
    int heartRate;
    int highPr;
    int lowPr;
    int oxygen;
    void sendData();
signals:
    void connectionInfo(QHostAddress hostAddr,quint16 hostPort,QHostAddress localAddr,quint16 localPort);
public slots:
    void configureConnection(QHostAddress addr,quint16 port);
    void resend();
    void connnectionInfoReady();
    void setGender(QString gender);
    void setName(QString name);
    void setAge(QString age);
    void updateParams(QVariantList& ecg,QVariantList& spo2,QVariantList& resp,int heartRate,int highPr,int lowPr,int oxygen);
    void startConnecting();
    void disConnectedCleanUp();
};

#endif // MTCPSERVICE_H
