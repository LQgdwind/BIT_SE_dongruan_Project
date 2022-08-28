//coding by zlq
//2022.08.21

#include "mainwindow.h"
#include "QDebug"

extern Data mydata;
double PI = 3.14156926;
int cnt;
extern bool is_generate;


extern int ecgWaveProvided[500];

void Data::eliminateImpact()
{
    mydata.Check[0]=0;
}

void Data::reImpact(QProperty* mthis)
{
    mydata.otherdata[HEARTRATE]=mthis->getHeartRate();
    mydata.otherdata[HIGH_PR]=mthis->getHigh_pr();
    mydata.otherdata[LOW_PR]=mthis->getLow_pr();
    mydata.otherdata[OXYGEN]=mthis->getOxygen();
    for(int i=0;i<=3;i++)
    {
        mydata.package[2+i]=mydata.otherdata[i];
    }
    for(int i = 0;i<=2+4+mydata.size_ecg+mydata.size_spo2+mydata.size_resp+2;i++)
    {
       mydata.Check[0]^=mydata.package[i];//重新算校验位
    }
    mydata.package[2+4+mydata.size_ecg+mydata.size_spo2+mydata.size_resp+3]=mydata.Check[0];
    qDebug()<<mydata.package[2]<<mydata.package[3]<<mydata.package[4]<<mydata.package[5]<<"";
    qDebug()<<mydata.Check[0];
}


void Data::dataGenerate(QProperty* mthis)
{
    cnt = 0;
    for(int i=0;i<size_ecg;i++)
        mydata.ecgWave[i]=ecgWaveProvided[i];
    mydata.get_tri_tab(size_spo2,50);
    //生成三角波
    mydata.get_cos_tab(size_resp,50);
    //生成余弦波
    mydata.otherdata[0]=mthis->getHeartRate();
    mydata.otherdata[1]=mthis->getHigh_pr();
    mydata.otherdata[2]=mthis->getLow_pr();
    mydata.otherdata[3]=mthis->getOxygen();
    for(int i = 0;i<2;i++)
        mydata.package[cnt++]=mydata.header[i];
    for(int i=0;i<4;i++)
        mydata.package[cnt++]=mydata.otherdata[i];
    mydata.package[cnt++] = mydata.size_ecg;
    for(int i = 0;i<mydata.size_ecg;i++)
        mydata.package[cnt++]=ecgWave[i];
    mydata.package[cnt++] = mydata.size_spo2;
    for(int i = 0;i<mydata.size_spo2;i++)
        mydata.package[cnt++]=mydata.spo2Wave[i];
    mydata.package[cnt++] = mydata.size_resp;
    for(int i = 0;i<mydata.size_resp;i++)
        mydata.package[cnt++]=mydata.respWave[i];
    mydata.eliminateImpact();
    mydata.reImpact(mthis);//计算校验位
    cnt++;
    for(int i = 0;i<2;i++)
        mydata.package[cnt++] = mydata.footer[i];
    is_generate=true;
    //qDebug()<<cnt;
    for(int i=0;i<cnt;i++) qDebug()<<mydata.package[i];
}
unsigned int Data::randGenerate(unsigned int a,unsigned int b)
{
    return a + (int)b * rand() / (RAND_MAX + 1);
}

void Data::sendData(MainWindow* mthis,QString& text)
{
    using Ui::MainWindow;
    int * dataBuf = (int *)mydata.package;
    char chh,chl;
    for(int i=0;i<cnt;i++)
    {
        chh = (char)(dataBuf[i]>>8);
        chl = (char)(dataBuf[i] & 0x00FF);
        mthis->qserialport->write(&chh,1);
        mthis->qserialport->write(&chl,1);
    }
    text = QString::number(cnt);
    text.append('\n');
}
void Data::get_sin_tab(unsigned int point, unsigned int peek)
//正弦波模拟
{
    unsigned int i = 0, j = 0;
    double c_rad = 0.0;       //弧度
    double c_val = 0.0;         //峰值
    unsigned int tem = 0;
    j = peek ;
    c_rad = PI * 2 / point;
    for( i = 0; i < point; i++ )
    {
        c_val = j * sin( c_rad * i ) + j;
        tem = (unsigned int)c_val;
        unsigned int noise = mydata.randGenerate(tem+(unsigned int)(peek*0.25),tem+(unsigned int)(peek*0.85));
        //随机数生成噪声
        mydata.spo2Wave[i] = tem+noise;
    }
}
void Data::get_cos_tab(unsigned int point, unsigned int peek)
//余弦波模拟
{
    unsigned int i = 0, j = 0;
    double c_rad = 0.0;       //弧度
    double c_val = 0.0;         //峰值
    unsigned int tem = 0;
    j = peek ;
    c_rad = PI * 2 / point;
    for( i = 0; i < point; i++ )
    {
        c_val = 1.2*j * cos( c_rad * i ) + 1.2*j;
        tem = (unsigned int)c_val;
        unsigned int noise = mydata.randGenerate(tem+(unsigned int)(peek*0.05),tem+(unsigned int)(peek*0.15));
        //随机数生成噪声
        mydata.respWave[i] = tem+noise;
    }
}
void Data::get_tri_tab(unsigned int point, unsigned int peek)
//三角波模拟
{
    unsigned int i = 0, j = 0;
    double c_val = 0.0;         //峰值
    unsigned int tem = 0;
    j = peek;
    for( i = 0; i < point / 2; i++ )
    {
        c_val = 4 * j * ( 0.005 * i );
        tem = (unsigned int)c_val;
        unsigned int noise = mydata.randGenerate(tem+(unsigned int)(peek*0.05),tem+(unsigned int)(peek*0.15));
        //随机数生成噪声
        mydata.spo2Wave[i] = tem+noise;
    }
    int k = i;
    for(;i<point;i++,k--)
    {
        c_val = 4 * j*( 0.005 * k);
        tem = (unsigned int)c_val;
        unsigned int noise = mydata.randGenerate(tem+(unsigned int)(peek*0.05),tem+(unsigned int)(peek*0.15));
        //随机数生成噪声
        mydata.spo2Wave[i] = tem+noise;
    }
}
