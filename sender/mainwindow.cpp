//coding by zlq
//2022.08.21

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"
#include "QTimer"
#include "QDateTime"

Data mydata;
extern double PI;
bool is_generate;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->ui->confirm->setEnabled(true);
    this->ui->clear->setEnabled(true);
    this->ui->start->setEnabled(false);
    this->ui->pause->setEnabled(false);
    mydata.mallocMemory(500,200,100);//可以修改这三个数的值从而改变一帧中各个数据的采样量。
    mydata.dataGenerate(&this->self);
}

MainWindow::~MainWindow()
{
    delete ui;
    mydata.deleteMemory();
}
void Data::mallocMemory(unsigned int a,unsigned int b,unsigned int c)
{
    mydata.header = new unsigned int[2*2]{171,205};//帧头为00 AB 00 CD
    mydata.footer = new unsigned int[2*2]{220,186};//帧尾为00 DC 00 BA
    mydata.size_ecg = a;
    mydata.size_spo2 = b;
    mydata.size_resp = c;
    mydata.ecgWave = new unsigned int[size_ecg*2];
    mydata.spo2Wave = new unsigned int[size_spo2*2];
    mydata.respWave = new unsigned int[size_resp*2];
    mydata.otherdata = new unsigned int[4*2];
    mydata.Check = new unsigned int[1*2];//校验
    mydata.package = new unsigned int[(5+size_ecg+size_spo2+size_resp+3+4)*2];
}
void Data::deleteMemory()
{
    delete[] mydata.header;
    delete[] mydata.footer;
    delete[] mydata.ecgWave;
    delete[] mydata.spo2Wave;
    delete[] mydata.respWave;
    delete[] mydata.otherdata;
    delete[] mydata.Check;
    delete[] mydata.package;
}
bool MainWindow::update()
{
    if(this->ui->portname->toPlainText()=="")
    {
        QString title = "提示";
        QString contains = "端口名不得为空";
        QMessageBox::information(this,title,contains,QMessageBox::Cancel,QMessageBox::NoButton);
        return false;
    }
    if(this->ui->freq->text().toInt()==0)
    {
        QString title = "提示";
        QString contains = "频率不得为0";
        QMessageBox::information(this,title,contains,QMessageBox::Cancel,QMessageBox::NoButton);
        return false;
    }
    this->self.setPortname(this->ui->portname->toPlainText());
    this->self.setFreq(this->ui->freq->text().toInt());
    this->self.setHeartRate(this->ui->heartrate->text().toInt());
    this->self.setHigh_pr(this->ui->high_pr->text().toInt());
    this->self.setLow_pr(this->ui->low_pr->text().toInt());
    this->self.setOxygen(this->ui->oxygen->text().toInt());
    if(is_generate)
    {
        mydata.eliminateImpact();
        mydata.reImpact(&this->self);
    }
    return true;
}

void MainWindow::on_start_clicked()
{
    this->qserialport = new QSerialPort();
    this->qserialport->setPortName(this->self.getPortName());
    if(!this->qserialport->open(QIODevice::ReadWrite))
    {
        QString title = "提示";
        QString contains = "打开"+this->self.getPortName()+"失败";
        QMessageBox::information(this,title,contains,QMessageBox::Cancel,QMessageBox::NoButton);
        this->ui->confirm->setEnabled(true);
        this->ui->clear->setEnabled(true);
        this->ui->start->setEnabled(true);
        this->ui->pause->setEnabled(false);
    }
    else
    {
        QString title = "提示";
        QString contains = "打开"+this->self.getPortName()+"成功";
        QMessageBox::information(this,title,contains,QMessageBox::Ok,QMessageBox::NoButton);
        this->ui->confirm->setEnabled(false);
        this->ui->clear->setEnabled(true);
        this->ui->start->setEnabled(false);
        this->ui->pause->setEnabled(true);
        this->qserialport->setBaudRate(QSerialPort::Baud115200,QSerialPort::AllDirections);
        this->qserialport->setDataBits(QSerialPort::Data8);
        this->qserialport->setFlowControl(QSerialPort::NoFlowControl);
        this->qserialport->setParity(QSerialPort::NoParity);
        this->qserialport->setStopBits(QSerialPort::OneStop);
        this->qtimer = new QTimer();
        connect(this->qtimer,&QTimer::timeout,this,&MainWindow::send);
        this->qtimer->start(this->self.getFreq());
    }

}

void MainWindow::on_clear_clicked()
{
    this->ui->textBrowser->clear();
}

void MainWindow::on_exit_clicked()
{
    this->close();
}

void MainWindow::on_confirm_clicked()
{
    if(!this->update())
    {
        QString title = "提示";
        QString contains = "修改失败";
        QMessageBox::information(this,title,contains,QMessageBox::Cancel,QMessageBox::NoButton);
    }
    else
    {
        this->ui->confirm->setEnabled(true);
        this->ui->clear->setEnabled(true);
        this->ui->start->setEnabled(true);
        this->ui->pause->setEnabled(false);
        QString title = "提示";
        QString contains = "修改成功";
        QMessageBox::information(this,title,contains,QMessageBox::Ok,QMessageBox::NoButton);
    }
}

void MainWindow::on_pause_clicked()
{
    delete this->qtimer;
    this->qserialport->close();
    this->ui->confirm->setEnabled(true);
    this->ui->clear->setEnabled(true);
    this->ui->start->setEnabled(false);
    this->ui->pause->setEnabled(false);
    QString title = "提示";
    QString contains = "关闭"+this->self.getPortName()+"成功";
    QMessageBox::information(this,title,contains,QMessageBox::Ok,QMessageBox::NoButton);

}

void MainWindow::send()
{
    QString output;
    QDateTime dateTime= QDateTime::currentDateTime();
    QString c_time = dateTime.toString("yyyy-MM-dd hh:mm:ss");
    c_time.append('\n');
    mydata.sendData(this,output);
    //QString heart_rate = "heartrate: "+QString::number(this->self.getHeartRate())+"\n";
    //QString high_pr = "high_pr: "+QString::number(this->self.getHigh_pr())+"\n";
    //QString low_pr = "low_pr: "+QString::number(this->self.getLow_pr())+"\n";
    //QString oxygen = "oxygen: "+QString::number(this->self.getOxygen())+"\n"+"数据帧数据个数: ";
    QString heart_rate = "heartrate: "+QString::number(mydata.package[2+HEARTRATE])+"\n";
    QString high_pr = "high_pr: "+QString::number(mydata.package[2+HIGH_PR])+"\n";
    QString low_pr = "low_pr: "+QString::number(mydata.package[2+LOW_PR])+"\n";
    QString oxygen = "oxygen: "+QString::number(mydata.package[2+OXYGEN])+"\n"+"数据帧数据个数: ";
    this->ui->textBrowser->insertPlainText(c_time);
    this->ui->textBrowser->insertPlainText(heart_rate);
    this->ui->textBrowser->insertPlainText(high_pr);
    this->ui->textBrowser->insertPlainText(low_pr);
    this->ui->textBrowser->insertPlainText(oxygen);
    this->ui->textBrowser->insertPlainText(output);

}
