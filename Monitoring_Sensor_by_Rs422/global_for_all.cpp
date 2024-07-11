#include "global_for_all.h"



//QSerialPort *ptrPort = new QSerialPort;
QSerialPort *ptrPort;
int currentSecond = 0;
int totalSeconds = 0;
qint64 totalDataSize = 0;


global_for_all::global_for_all()
{

}

QStringList global_for_all::getAvailablePorts()
{
    QStringList portNames;

    QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();

    for (const QSerialPortInfo &portInfo : availablePorts) {
        portNames.append(portInfo.portName());
    }

    return portNames;

}


