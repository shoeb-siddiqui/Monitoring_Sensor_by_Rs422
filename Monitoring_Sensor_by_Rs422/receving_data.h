#ifndef RECEVING_DATA_H
#define RECEVING_DATA_H

#include <QObject>
#include <QByteArray>
#include <QDebug>
#include <QThread>
#include"global_for_all.h"

class receving_data : public QThread
{
    Q_OBJECT
public:
    explicit receving_data(QObject *parent = nullptr);
    bool stoppedd = false;
    void run () override;
    void stop();

signals:
    void  dataReceived_imu(const QByteArray &data);

public slots:
};

#endif // RECEVING_DATA_H
