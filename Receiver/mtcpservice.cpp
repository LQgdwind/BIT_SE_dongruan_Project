//code by lhp
//22.08.22
#include "mtcpservice.h"

MTcpService::MTcpService(QObject *parent) : QObject(parent),server(new QTcpSocket)
{
    connect(server,&QTcpSocket::connected,this,&MTcpService::connnectionInfoReady);
    connect(server,&QTcpSocket::disconnected,this,&MTcpService::disConnectedCleanUp);
    connect(server,&QIODevice::readyRead,this,&MTcpService::resend);
}



void MTcpService::configureConnection(QHostAddress addr, quint16 port)
{
    this->addr=addr;
    this->port=port;
    startConnecting();
}

void MTcpService::resend()
{
    qDebug()<<server->readAll();
    sendData();
}

void MTcpService::connnectionInfoReady()
{
    emit connectionInfo(server->peerAddress(),server->peerPort(),server->localAddress(),server->localPort());
}

void MTcpService::setGender(QString gender)
{
    this->gender=gender;
}

void MTcpService::setName(QString name)
{
    this->name=name;
}

void MTcpService::setAge(QString age)
{
    this->age=age.toInt();
}

void MTcpService::updateParams(QVariantList& ecg,QVariantList& spo2,QVariantList& resp,int heartRate,int highPr,int lowPr,int oxygen)
{
    this->ecg=ecg;
    this->spo2=spo2;
    this->resp=resp;
    this->heartRate=heartRate;
    this->highPr=highPr;
    this->lowPr=lowPr;
    this->oxygen=oxygen;
    if(server->state()==QAbstractSocket::ConnectedState){
        sendData();
    }
}

void MTcpService::startConnecting()
{
    server->connectToHost(addr,port);
}

void MTcpService::startDisconnecting()
{
    server->disconnectFromHost();
}

void MTcpService::sendData()
{
    QJsonArray ecgJson=QJsonArray::fromVariantList(ecg);
    QJsonArray spo2Json=QJsonArray::fromVariantList(spo2);
    QJsonArray respJson=QJsonArray::fromVariantList(resp);
    QJsonObject dataJson;
    dataJson["name"]=name;
    dataJson["gender"]=gender;
    dataJson["age"]=age;
    dataJson["ecg"]=ecgJson;
    dataJson["spo2"]=spo2Json;
    dataJson["resp"]=respJson;
    dataJson["heartRate"]=heartRate;
    dataJson["highPr"]=highPr;
    dataJson["lowPr"]=lowPr;
    dataJson["oxygen"]=oxygen;
    QByteArray data=QJsonDocument(dataJson).toJson(QJsonDocument::Compact);
    server->write(data);
    server->flush();
}

void MTcpService::disConnectedCleanUp()
{
    emit connectionInfo(QHostAddress::Null,0,QHostAddress::Null,0);
}
