#ifndef DATA_H
#define DATA_H
#include "QVariantList"
class Data
{
public:
    void mallocMemory(unsigned int,unsigned int,unsigned int);
    void deleteMemory();
    void transferToList();
    void updateTransfer();
    void printAll();

public:
    unsigned int* header;
    unsigned int* ecgWave;
    unsigned int* spo2Wave;
    unsigned int* respWave;
    unsigned int* check;
    unsigned int* otherdata;
    unsigned int* footer;
    unsigned int* package;
    bool is_generate;

public:
    QVariantList ecgWaves;
    QVariantList spo2Waves;
    QVariantList respWaves;
    int heart_rate;
    int high_pr;
    int low_pr;
    int oxygen;
};

#endif // DATA_H
