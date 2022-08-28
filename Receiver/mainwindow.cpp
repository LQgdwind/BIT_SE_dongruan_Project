//code by zlq
//2022.8.22


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"

Data mydata;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->ui->confirm->setEnabled(true);
    this->ui->start->setEnabled(false);
    this->ui->pause->setEnabled(false);
    this->ui->draw->setEnabled(false);
    this->ui->exit_draw->setEnabled(false);
    this->ui->open_client->setEnabled(false);
    this->ui->exit_client->setEnabled(false);
    mydata.mallocMemory(500,200,100);//可以修改这三个数的值从而改变一帧中各个数据的采样量。
}

MainWindow::~MainWindow()
{
    delete ui;
    mydata.deleteMemory();
}

void Data::mallocMemory(unsigned int a,unsigned int b,unsigned int c)
{
    mydata.header = new unsigned int[2];
    mydata.footer = new unsigned int[2];
    mydata.ecgWave = new unsigned int[size_ecg];
    mydata.spo2Wave = new unsigned int[size_spo2];
    mydata.respWave = new unsigned int[size_resp];
    mydata.otherdata = new unsigned int[4];
    mydata.check = new unsigned int[1];//校验
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
    delete[] mydata.check;
    delete[] mydata.package;
}

void MainWindow::TimerUpdate_qserialport()
{
    myTimer->stop();//让定时器停下来
    if(Dataset2.size()>=cnt)
        //cnt是一个枚举量，代表数据帧int数组的元素数
        //如果数据读取的不够一帧就接着读
    {
        Dataset = Dataset2;
        Dataset2.clear();
        Readdata.clear();
        mydata.header[0] = (unsigned int)Dataset[0] << 8 |((unsigned int)Dataset[1] &0x00FF);
        //qDebug()<<header[0];
        mydata.header[1] = (unsigned int)Dataset[2] << 8 |((unsigned int)Dataset[3] &0x00FF);
        //qDebug()<<header[1];
        mydata.footer[0] = (unsigned int)Dataset[2*(cnt-2)] << 8 |((unsigned int)Dataset[2*(cnt-2)+1] &0x00FF);
        //qDebug()<<footer[0];
        mydata.footer[1] = (unsigned int)Dataset[2*(cnt-1)] << 8 |((unsigned int)Dataset[2*(cnt-1)+1] &0x00FF);
        //qDebug()<footer[1];
        mydata.check[0] = 0;
        unsigned int sendcheck = (unsigned int)Dataset[2*(cnt-3)] << 8 |((unsigned int)Dataset[2*(cnt-3)+1] &0x00FF);
        //发送过来的check码
        for(int i = 0;i < (cnt-3) ;i++)
        {
            unsigned int tmp = (unsigned int)Dataset[i*2] << 8 |((unsigned int)Dataset[i*2+1] &0x00FF);
            //qDebug()<<tmp;
            mydata.check[0] ^= tmp;
        }
        //qDebug()<<check[0];
        //qDebug()<<sendcheck;
        while(Dataset.size()>=cnt)
        //不够一帧，退出循环发射信号去读下一个定时器周期的数据
        {
            //qDebug()<<"1";
            if(mydata.header[0] == 171 && mydata.header[1] == 205)
            {
                if(mydata.footer[0] == 220 && mydata.footer[1] == 186)
                {
                    if(mydata.check[0] == sendcheck)
                    {
                        for(int i=0;i<cnt;i++)
                        {
                            unsigned int tmp = (unsigned int)Dataset[i*2] << 8 |((unsigned int)Dataset[i*2+1] &0x00FF);
                            if(mydata.is_generate)
                            {
                                if(i == cnt-3)
                                {
                                    if(tmp!=mydata.package[i])
                                    //如果校验码改变，说明四个值中有值出现了变化
                                    {
                                        mydata.updateTransfer();
                                        mydata.printAll();

                                        //this->ui->exit_client->setEnabled(false);
                                        emit updatePicture();
                                        //当数据成功更新的时候，发射信号。
                                        //emit readyToWrite(ecgWaves,spo2Waves,respWaves,heart_rate,high_pr,low_pr,oxygen);
                                    }
                                }
                            }
                            mydata.package[i]=tmp;
                            //qDebug()<<package[i];
                        }
                        if(mydata.is_generate == false)
                        {
                            mydata.transferToList();
                            mydata.updateTransfer();
                            mydata.is_generate = true;
                            this->ui->open_client->setEnabled(true);
                            //当数据成功接收的时候，打开客户端按钮可以使用。
                            mydata.printAll();
                        }
                        break;
                    }
                    else
                    {
                        Dataset.remove(0,1);
                        //第一个八位字符出模拟队列(很有可能是某个数字的低八位)
                        continue;
                        //如果不匹配，在下一个循环自然会重算
                    }
                }
                Dataset.remove(0,1);
                continue;
                //如果不匹配，在下一个循环自然会重算
                //break;
            }
            else
            {
                Dataset.remove(0,1);//第一个八位字符出队列(很有可能是某个数字的低八位)
                //再算一遍
                if(Dataset.size()<cnt) break;
                //不够一帧，退出循环发射信号去读下一个定时器周期的数据
                mydata.header[0] = (unsigned int)Dataset[0] << 8 |((unsigned int)Dataset[1] &0x00FF);
                mydata.header[1] = (unsigned int)Dataset[2] << 8 |((unsigned int)Dataset[3] &0x00FF);
                mydata.footer[0] = (unsigned int)Dataset[2*(cnt-2)] << 8 |((unsigned int)Dataset[2*(cnt-2)+1] &0x00FF);
                mydata.footer[1] = (unsigned int)Dataset[2*(cnt-1)] << 8 |((unsigned int)Dataset[2*(cnt-1)+1] &0x00FF);
                sendcheck = (unsigned int)Dataset[2*(cnt-3)] << 8 |((unsigned int)Dataset[2*(cnt-3)+1] &0x00FF);
                //发送过来的check码
                for(int i = 0;i < (cnt-3) ;i++)
                {
                    unsigned int tmp = (unsigned int)Dataset[i*2] << 8 |((unsigned int)Dataset[i*2+1] &0x00FF);
                    mydata.check[0] ^= tmp;
                }
            }
        }
    }
    emit this->my_ready();
    //发射信号去读下一帧数据
}

void MainWindow::readData()
{
    myTimer->start(100);//100ms启动定时器
    Dataset2 = Readdata.append(qserialport->readAll());
    //qDebug()<<Dataset2.size();

}

void MainWindow::on_confirm_clicked()
{
    this->ui->confirm->setEnabled(true);
    this->ui->start->setEnabled(true);
    this->ui->pause->setEnabled(false);
    QString title = "提示";
    QString contains = "修改成功";
    QMessageBox::information(this,title,contains,QMessageBox::Ok,QMessageBox::NoButton);

}

void MainWindow::on_start_clicked()
{
    this->qserialport = new QSerialPort();
    //qDebug()<<"111";
    this->qserialport->setPortName(this->ui->portname->toPlainText());
    if(!this->qserialport->open(QIODevice::ReadWrite))
    {
        QString title = "提示";
        QString contains = "打开"+this->ui->portname->toPlainText()+"失败";
        QMessageBox::information(this,title,contains,QMessageBox::Cancel,QMessageBox::NoButton);
        this->ui->confirm->setEnabled(true);
        this->ui->start->setEnabled(true);
        this->ui->pause->setEnabled(false);
    }
    else
    {
        QString title = "提示";
        QString contains = "打开"+this->ui->portname->toPlainText()+"成功";
        QMessageBox::information(this,title,contains,QMessageBox::Ok,QMessageBox::NoButton);
        this->ui->confirm->setEnabled(false);
        this->ui->start->setEnabled(false);
        this->ui->pause->setEnabled(true);
        this->ui->draw->setEnabled(true);
        this->qserialport->setBaudRate(QSerialPort::Baud115200,QSerialPort::AllDirections);
        this->qserialport->setDataBits(QSerialPort::Data8);
        this->qserialport->setFlowControl(QSerialPort::NoFlowControl);
        this->qserialport->setParity(QSerialPort::NoParity);
        this->qserialport->setStopBits(QSerialPort::OneStop);
        myTimer = new QTimer(this); //建立串口读写超时定时器
        myTimer->start(100);//100ms启动定时器
        connect(myTimer,SIGNAL(timeout()),this,SLOT(TimerUpdate_qserialport()));
        connect(this,SIGNAL(my_ready()),this,SLOT(readData()));
    }
}

void MainWindow::on_pause_clicked()
{
    delete this->myTimer;
    this->qserialport->close();
    this->ui->confirm->setEnabled(true);
    this->ui->start->setEnabled(false);
    this->ui->pause->setEnabled(false);
    QString title = "提示";
    QString contains = "关闭"+this->ui->portname->toPlainText()+"成功";
    QMessageBox::information(this,title,contains,QMessageBox::Ok,QMessageBox::NoButton);
}

void MainWindow::on_draw_clicked()
{
    this->ui->draw->setEnabled(false);
    this->ui->exit_draw->setEnabled(true);
    QString title = "提示";
    QString contains = "正在绘图";
    QMessageBox::information(this,title,contains,QMessageBox::Ok,QMessageBox::NoButton);
    //this->hide();
    myclient.ecgBlock->pixmap->fill(Qt::black);
    myclient.spo2Block->pixmap->fill(Qt::black);
    myclient.respBlock->pixmap->fill(Qt::black);
    myclient.ecgBlock->index = 0;
    myclient.spo2Block->index = 0;
    myclient.respBlock->index = 0;
    myclient.show();

}

void MainWindow::on_exit_draw_clicked()
{
    this->ui->draw->setEnabled(true);
    this->ui->exit_draw->setEnabled(false);
    QString title = "提示";
    QString contains = "停止绘图";
    QMessageBox::information(this,title,contains,QMessageBox::Ok,QMessageBox::NoButton);
    myclient.hide();
}

void MainWindow::on_open_client_clicked()
{
    this->ui->open_client->setEnabled(false);
    this->ui->exit_client->setEnabled(true);
    QString title = "提示";
    QString contains = "打开客户端";
    QMessageBox::information(this,title,contains,QMessageBox::Ok,QMessageBox::NoButton);
    this->myclientservice.show();
}

void MainWindow::on_exit_client_clicked()
{
    this->ui->open_client->setEnabled(true);
    this->ui->exit_client->setEnabled(false);
    QString title = "提示";
    QString contains = "关闭客户端";
    QMessageBox::information(this,title,contains,QMessageBox::Ok,QMessageBox::NoButton);
    this->myclientservice.hide();
}
