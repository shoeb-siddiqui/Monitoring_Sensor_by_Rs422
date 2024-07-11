#include "receving_for_fuze.h"

receving_for_fuze::receving_for_fuze(QObject *parent) : QThread(parent)
{

}
void receving_for_fuze::stop()
{
    stopped = true;
    wait();

}
void receving_for_fuze::run()
{
    while(!stopped){
        if(ptrPort->isOpen()){
             QByteArray data = ptrPort->readAll();


            // if (!data.isEmpty()) {
                qDebug()<<"data_coming_64_byte"<<data.toHex()<<endl;
                 emit dataReceived_fuse(data);


             msleep(1);
             data.clear();

       // }

    }
    }

}

