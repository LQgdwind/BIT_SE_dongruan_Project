//code by lhp
//22.08.22

#ifndef SUBWINDOW_H
#define SUBWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QInputDialog>
#include <QMessageBox>
#include <QHostAddress>
#include "mtcpservice.h"
#include <QTcpSocket>

namespace Ui {
class SubWindow;
}

class SubWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SubWindow(QWidget *parent = 0);
    ~SubWindow();
signals:
    void connectionConfigured(QHostAddress addr,quint16 port);
    void readyToWrite(QVariantList& ecg,QVariantList& spo2,QVariantList& resp,int heartRate,int highPr,int lowPr,int oxygen);
public slots:
    void createConnection();
    void handleStatusChange(QAbstractSocket::SocketState socketState);
    void handleConnectionError(QAbstractSocket::SocketError socketError);
    void updateConnectionInfo(QHostAddress hostAddr,quint16 hostPort,QHostAddress localAddr,quint16 localPort);
    void startDisconnect();
    void startReconnect();

    void testSend();
private:
    Ui::SubWindow *ui;
    bool statusFlag{true};
    MTcpService *tcpService;
    void getReadyForDisconnect();
    void getReadyForReconnect();
};

#endif // SUBWINDOW_H
