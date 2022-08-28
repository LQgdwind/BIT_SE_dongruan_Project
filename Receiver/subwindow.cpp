//code by lhp
//modify by zlq
//22.08.22
#include "mainwindow.h"
#include "subwindow.h"
#include "ui_subwindow.h"
#include <QDebug>
#include <QAbstractSocket>
#include <QTimer>

extern Data mydata;

SubWindow::SubWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SubWindow),
    tcpService(new MTcpService)
{
  ui->setupUi(this);
  connect(ui->connectionCreater,&QPushButton::clicked,this,&SubWindow::createConnection);
  connect(this,&SubWindow::connectionConfigured,tcpService,&MTcpService::configureConnection);
  connect(tcpService->server,&QTcpSocket::stateChanged,this,&SubWindow::handleStatusChange);
  connect(ui->ageEdit,&QLineEdit::textEdited,tcpService,&MTcpService::setAge);
  connect(ui->genderEdit,&QLineEdit::textEdited,tcpService,&MTcpService::setGender);
  connect(ui->nameEdit,&QLineEdit::textEdited,tcpService,&MTcpService::setName);
  connect(tcpService,&MTcpService::connectionInfo,this,&SubWindow::updateConnectionInfo);
  connect(tcpService->server,QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),this,SubWindow::handleConnectionError);
  connect(this,&SubWindow::readyToWrite,tcpService,&MTcpService::updateParams);
  connect(ui->testSend,&QPushButton::clicked,this,&SubWindow::testSend);
}

SubWindow::~SubWindow()
{
    delete ui;
}

void SubWindow::testSend()
{
    QVariantList samp;
    emit readyToWrite(mydata.ecgWaves,mydata.spo2Waves,mydata.respWaves,mydata.heart_rate,mydata.high_pr,mydata.low_pr,mydata.oxygen);
    //modify by zlq 08.23
}

void SubWindow::createConnection(){
    if(ui->ageEdit->text()==""||ui->genderEdit->text()==""||ui->nameEdit->text()==""){
        QMessageBox::critical(this,"错误提示","缺少必要信息");
        return;
    }
    bool ok,valid{true};
    QHostAddress addr;
    quint16 port;
    QString text = QInputDialog::getText(this,"服务器ip与端口","ip:port", QLineEdit::Normal,"127.0.0.1:8080", &ok);
    if(ok){
        QStringList list=text.split(":");
        valid=addr.setAddress(list[0]);
        if(!(list.size()==2&&valid)){
            QMessageBox::critical(this,"错误提示","格式错误");
            return;
        }
        port=list[1].toUShort(&valid);
        if(!valid){
            QMessageBox::critical(this,"错误提示","格式错误");
            return;
        }
    }
    emit connectionConfigured(addr,port);
    getReadyForDisconnect();
}

void SubWindow::handleStatusChange(QAbstractSocket::SocketState socketState)
{
    if(statusFlag){
    //qDebug()<<socketState;
    QString msg{"连接状态:"};
    switch (socketState) {
    case QAbstractSocket::UnconnectedState:
        msg.append("未连接");
        break;
    case QAbstractSocket::HostLookupState:
        msg.append("正在查找服务端");
        break;
    case QAbstractSocket::ConnectingState:
        msg.append("尝试连接服务端");
        break;
    case QAbstractSocket::ConnectedState:
        msg.append("正常连接");
        break;
    case QAbstractSocket::ClosingState:
        msg.append("正在断开连接");
        break;
    case QAbstractSocket::BoundState:
        msg.append("未知状态");
    default:
        break;
    }
    ui->connectionStatus->item(0)->setText(msg);
    }
}

void SubWindow::handleConnectionError(QAbstractSocket::SocketError socketError)
{
    statusFlag=false;
    //qDebug()<<socketError;
    QString msg{"连接状态:"};
    switch (socketError) {
    case QAbstractSocket::ConnectionRefusedError:
        msg.append("服务端拒绝连接");
        break;
    case QAbstractSocket::RemoteHostClosedError:
        msg.append("服务端关闭了连接");
        break;
    case QAbstractSocket::HostNotFoundError:
        msg.append("未找到服务端");
        break;
    case QAbstractSocket::NetworkError:
        msg.append("网络异常");
        break;
    case QAbstractSocket::UnknownSocketError:
        qDebug("unkown here");
    default:
        msg.append("未知错误，请重试");
        break;
    }
    ui->connectionStatus->item(0)->setText(msg);
    getReadyForReconnect();
}

void SubWindow::updateConnectionInfo(QHostAddress hostAddr, quint16 hostPort, QHostAddress localAddr, quint16 localPort)
{
    ui->connectionStatus->item(1)->setText(QString("服务端ip:%1").arg(hostAddr.toString()));
    if(hostPort!=0){
        ui->connectionStatus->item(2)->setText(QString("服务端端口:%1").arg(hostPort));
    }else ui->connectionStatus->item(2)->setText("服务端端口:");
    ui->connectionStatus->item(3)->setText(QString("本机ip:%1").arg(localAddr.toString()));
    if(localPort!=0){
        ui->connectionStatus->item(4)->setText(QString("本机端口:%1").arg(localPort));
    }else ui->connectionStatus->item(4)->setText("本机端口:");
}

void SubWindow::startDisconnect()
{
    tcpService->startDisconnecting();
    getReadyForReconnect();
}

void SubWindow::startReconnect()
{
    statusFlag=true;
    tcpService->startConnecting();
    getReadyForDisconnect();
}

void SubWindow::getReadyForDisconnect()
{
    disconnect(ui->connectionCreater, nullptr, nullptr, nullptr);
    ui->connectionCreater->setText("断开连接");
    connect(ui->connectionCreater,&QPushButton::clicked,this,&SubWindow::startDisconnect);
}

void SubWindow::getReadyForReconnect()
{
    disconnect(ui->connectionCreater, nullptr, nullptr, nullptr);
    ui->connectionCreater->setText("重新连接");
    connect(ui->connectionCreater,&QPushButton::clicked,this,&SubWindow::startReconnect);
}
