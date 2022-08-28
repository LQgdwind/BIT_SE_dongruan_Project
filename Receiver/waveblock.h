//coding by zlj
//modifying by zlq
//2022.8.23

#ifndef WAVEBLOCK_H
#define WAVEBLOCK_H

#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <QLabel>
#include <QVBoxLayout>
#include <QLayout>
#include <QBasicTimer>
#include <QString>
#include <QVariant>

//画图工具
class DrawWave : public QWidget
{
    Q_OBJECT
public:
    explicit DrawWave(QWidget *parent = 0, int color_id_ = 0, QVariantList *wavelist = nullptr);
    QBasicTimer timer;
    QPixmap * pixmap;
    QVariantList * datalist;
    int DataIndex;
    int index;
    int color_id;


    virtual void timerEvent(QTimerEvent * event);
    virtual void paintEvent(QPaintEvent * event);

    void drawDataWave(QPainter * painter);
    void drawWaveToPixmap();
};
//数据块布局
class DataBlock : public QWidget
{
    Q_OBJECT
public:
    explicit DataBlock(QWidget *parent = 0, int color_id = 0);
    QLabel * data;

private:
    int color_id;
    QLabel * title;
    QLabel * bmp;
    QLabel * others;
private slots:

};




#endif // WAVEBLOCK_H
