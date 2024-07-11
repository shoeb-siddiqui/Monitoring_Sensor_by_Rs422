#ifndef GLOBAL_FOR_ALL_H
#define GLOBAL_FOR_ALL_H
#include <QtSerialPort/QSerialPort>
#include <QSerialPortInfo>
#include <QString>
#include <QList>
#include <QTimer>
#include <QDebug>



extern QSerialPort *ptrPort;


extern int currentSecond;
extern int totalSeconds ;
extern qint64 totalDataSize;



class global_for_all
{
public:
    global_for_all();
     static QStringList getAvailablePorts();
};

#endif // GLOBAL_FOR_ALL_H
