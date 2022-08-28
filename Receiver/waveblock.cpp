//code by zlj
//modify by zlq
//22.8.23

#include "mainwindow.h"


extern Data mydata;

QString hr_=QString::number(mydata.heart_rate);
QString hp_=QString::number(mydata.high_pr);
QString lp_=QString::number(mydata.low_pr);
QString o2_=QString::number(mydata.oxygen);

DataBlock::DataBlock(QWidget *parent, int color_id):QWidget(parent)
{
    //数据模块，可以设置传输过来的心率、血压等信息
    QVBoxLayout * lay = new QVBoxLayout(this);
    QFont ft;
    ft.setPointSize(20);
    if(color_id == 0){
        title = new QLabel("heart_rate");
        hr_=QString::number(mydata.heart_rate);
        title->setStyleSheet("background-color: grey;color:white;font-size:15px;font:bold");
        data = new QLabel(hr_);
        //modify by zlq 08.23
        data->setFont(ft);
        data->setStyleSheet("background-color: black;color: green;");
        bmp = new QLabel("bmp");
        bmp->setStyleSheet("background-color: black;color: green");
        others = new QLabel("ST1 -?-\nST2 -?-\nPVCs -?-");
        others->setStyleSheet("background-color: black;color: green;");
    }
    else if(color_id == 1){
        title = new QLabel("high_pr");
        hp_=QString::number(mydata.high_pr);
        title->setStyleSheet("background-color: grey;color:white;font-size:15px;font:bold");
        data = new QLabel(hp_);
        //modify by zlq 08.23
        data->setFont(ft);
        data->setStyleSheet("background-color: black;color: red");
        bmp = new QLabel("bmp");
        bmp->setStyleSheet("background-color: black ;color: red");
        others = new QLabel("ST1 -?-\nST2 -?-\nPVCs -?-");
        others->setStyleSheet("background-color: black;color: red;");
    }
    else if(color_id == 2){
        title = new QLabel("low_pr");
        lp_=QString::number(mydata.low_pr);
        title->setStyleSheet("background-color: grey;color:white;font-size:15px;font:bold");
        data = new QLabel(lp_);
        //modify by zlq 08.23
        data->setFont(ft);
        data->setStyleSheet("background-color: black;color: yellow");
        bmp = new QLabel("bmp");
        bmp->setStyleSheet("background-color: black ;color: yellow");
        others = new QLabel("ST1 -?-\nST2 -?-\nPVCs -?-");
        others->setStyleSheet("background-color: black;color: yellow;");
    }
    else {
        title = new QLabel("oxygen");
        o2_=QString::number(mydata.oxygen);
        title->setStyleSheet("background-color: grey;color:white;font-size:15px;font:bold");
        data = new QLabel(o2_);
        //modify by zlq 08.23
        data->setFont(ft);
        data->setStyleSheet("background-color: black;color: blue");
        bmp = new QLabel("bmp");
        bmp->setStyleSheet("background-color: black ;color: blue");
        others = new QLabel("ST1 -?-\nST2 -?-\nPVCs -?-");
        others->setStyleSheet("background-color: black;color: blue;");
    }
    QHBoxLayout *lay2 = new QHBoxLayout();
    lay2->addWidget(data);
    lay2->addWidget(bmp);
    lay2->addWidget(others);

    lay->addWidget(title);
    lay->addLayout(lay2);
    lay->setStretch(0,1);
    lay->setStretch(1,5);
}

DrawWave::DrawWave(QWidget * parent, int color_id_, QVariantList * datalist_):QWidget(parent)
{
    datalist = new QVariantList;
//    datalist = datalist_;
    qreal height = this->height();
    datalist->append(2000);
    //modify by zlq 08.23 //加入初始值，防止空指针异常
    color_id = color_id_;
    index = 1;
    DataIndex = 0;
    if(color_id == 0)timer.start(16,this);
    else if(color_id == 1)timer.start(8,this);
    else timer.start(24,this);
    pixmap = new QPixmap(this->width(),this->height());
    pixmap->fill(Qt::black);

}

void DrawWave::drawWaveToPixmap()
{
    //画波形工具
    QPainter pixPainter;
    pixPainter.begin(this->pixmap);
    drawDataWave(&pixPainter);
    pixPainter.end();
}

void DrawWave::timerEvent(QTimerEvent *event)
{
    //接收数据
    if(mydata.is_generate)
    {
        switch (this->color_id) {
        case 0: datalist = &mydata.ecgWaves;  break;
        case 1: datalist = &mydata.spo2Waves; break;
        case 2: datalist = &mydata.respWaves; break;
        default: break;
        }
    }
    //modify by zlq 08.23
    drawWaveToPixmap();
    this->update();//强制更新
}



void DrawWave::paintEvent(QPaintEvent * event){
    QPainter * painter = new QPainter();
    painter->begin(this);
    painter->drawPixmap(0,0,*pixmap);
    painter->end();
}

void DrawWave::drawDataWave(QPainter * painter)
{
    QPen pen;
    pen.setWidth(2);
    if(color_id == 0)pen.setColor(Qt::green);
    else if(color_id == 1)pen.setColor(Qt::red);
    else pen.setColor(Qt::yellow);
    painter->setPen(pen);

    //更新绘图参数
    index = index + 1;
    if(index>this->width())
        index = 1;
    //擦除之前的痕迹
    painter->save();//保存当前的设置
    pen.setColor(Qt::black);//设置成背景色
    pen.setWidth(5);
    painter->setPen(pen);
    painter->drawLine(index + 1,0,index + 1,this->height());
    painter->drawLine(index+6,0,index+6,this->height());
    painter->drawLine(index+11,0,index+11,this->height());

    painter->restore();//恢复了保存之前的设置
    //绘图
    qreal height = this->height();
    QPoint lineStart;
    QPoint lineEnd;
    lineStart.setX(index - 1);
    lineEnd.setX(index);

    int a = datalist->at(DataIndex).toInt();
    qreal value = a;
    int y;
    if(color_id == 0)y = round(height/2 -((value - 2000)/600)*height/2);
    else y = round(height/2 -((value - 150)/130)*height/2.8);
    //modify by zlq 08.23
    lineStart.setY(y);

    DataIndex +=3;
    if(DataIndex >= datalist->size())
        DataIndex = 0;
    value = datalist->at(DataIndex).toInt();
    if(color_id == 0)y = round(height/2 -((value - 2000)/600)*height/2);
    else y = round(height/2 -((value - 150)/130)*height/2.8);
    //modify by zlq 08.23
    lineEnd.setY(y);
    painter->drawLine(lineStart,lineEnd);
}

