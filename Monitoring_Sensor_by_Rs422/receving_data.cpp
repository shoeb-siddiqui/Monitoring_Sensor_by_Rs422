#include "receving_data.h"

receving_data::receving_data(QObject *parent) : QThread(parent)
{

}
void receving_data::stop()
{
    stoppedd = true;
    wait();

}

void receving_data::run()
{
    while(!stoppedd){
        if(ptrPort->isOpen()){
             QByteArray data = ptrPort->readAll();


            // if (!data.isEmpty()) {
                qDebug()<<"data_coming_64_byte"<<data.toHex()<<endl;
                 emit dataReceived_imu(data);


             msleep(1);
             data.clear();

       // }

    }
    }

}
