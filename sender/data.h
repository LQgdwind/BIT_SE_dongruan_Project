#ifndef DATA_H
#define DATA_H
class MainWindow;
class Data
{
public:
    void dataGenerate(QProperty*);
    void eliminateImpact();
    void reImpact(QProperty*);
    void mallocMemory(unsigned int,unsigned int,unsigned int);
    void deleteMemory();
    void sendData(MainWindow*,QString&);
    void get_sin_tab(unsigned int,unsigned int);
    void get_cos_tab(unsigned int,unsigned int);
    void get_tri_tab(unsigned int,unsigned int);
    unsigned int randGenerate(unsigned int,unsigned int);

public:
    unsigned int size_ecg;
    unsigned int size_spo2;
    unsigned int size_resp;
    unsigned int* header;
    unsigned int* ecgWave;
    unsigned int* spo2Wave;
    unsigned int* respWave;
    unsigned int* Check;
    unsigned int* otherdata;
    unsigned int* footer;
    unsigned int* package;
};

#endif // DATA_H
