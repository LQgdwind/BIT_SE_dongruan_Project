//coding by zlj
//modifying by zlq
//2022.8.23
#include "mainwindow.h"
#include "ui_client.h"
#include <QDebug>

extern Data mydata;

extern QString hr_;
extern QString hp_;
extern QString lp_;
extern QString o2_;

client::client(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::client)
{
    ui->setupUi(this);
    iniUI();
    //接收数据信息
    this->qtimer = new QTimer();
    connect(this->qtimer,SIGNAL(timeout()),this,SLOT(reloadPicture()));
    this->qtimer->start(100);
    //modify by zlq 08.23
}

void client::iniUI(){
    setStyleSheet("background-color: black");
    ecgBlock = new DrawWave(0, 0, &mydata.ecgWaves);
    spo2Block = new DrawWave(0, 1, &mydata.spo2Waves);
    respBlock = new DrawWave(0, 2, &mydata.respWaves);

    QLabel * title1 = new QLabel("  ECGII      x1");
    title1->setStyleSheet("background-color: black;color:white;font-size:15px;font:bold");
    title1->setMaximumWidth(610);
    QLabel * title2 = new QLabel("  SPO2");
    title2->setStyleSheet("background-color: black;color:white;font-size:15px;font:bold");
    title2->setMaximumWidth(610);
    QLabel * title3 = new QLabel("  RESP      x1");
    title3->setStyleSheet("background-color: black;color:white;font-size:15px;font:bold");
    title3->setMaximumWidth(610);

    central =new QWidget(this);
    ecgBlock->resize(610,150);
    spo2Block->resize(610,150);
    respBlock->resize(610,150);

    QVBoxLayout *lay_left = new QVBoxLayout;
    lay_left->setSpacing(0);
    lay_left->setMargin(0);
    lay_left->addWidget(title1);
    lay_left->addWidget(ecgBlock);
    lay_left->addWidget(title2);
    lay_left->addWidget(spo2Block);
    lay_left->addWidget(title3);
    lay_left->addWidget(respBlock);

    heartRate = new DataBlock(0, 0);
    bloodOxygen = new DataBlock(0, 1);
    highPr = new DataBlock(0, 2);
    lowPr = new DataBlock(0, 3);

    QVBoxLayout *lay_right = new QVBoxLayout;
    lay_right->setSpacing(0);
    lay_right->setMargin(0);
    lay_right->addWidget(heartRate);
    lay_right->addWidget(bloodOxygen);
    lay_right->addWidget(highPr);
    lay_right->addWidget(lowPr);

    QHBoxLayout *lay = new QHBoxLayout;
    lay->addLayout(lay_left);
    lay->addLayout(lay_right);

    QVBoxLayout *lay_main = new QVBoxLayout();
    lay_main->setSpacing(0);
    lay_main->setMargin(0);
    lay_main->addLayout(lay);

    central->setLayout(lay_main);
    this->setCentralWidget(central);
}

void client::reloadPicture()//code by zlq 08.23
{
    //qDebug()<<"111";
    hr_=QString::number(mydata.heart_rate);
    hp_=QString::number(mydata.high_pr);
    lp_=QString::number(mydata.low_pr);
    o2_=QString::number(mydata.oxygen);
    heartRate->data->setText(hr_);
    bloodOxygen->data->setText(hp_);
    highPr->data->setText(lp_);
    lowPr->data->setText(o2_);
}

client::~client()
{
    delete ui;
}
