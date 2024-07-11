#ifndef RECEVING_FOR_FUZE_H
#define RECEVING_FOR_FUZE_H

#include <QObject>
#include <QByteArray>
#include <QDebug>
#include <QThread>
#include"global_for_all.h"

class receving_for_fuze : public QThread
{
    Q_OBJECT
public:
    explicit receving_for_fuze(QObject *parent = nullptr);
    bool stopped = false;
    void run () override;
    void stop();

signals:
       void  dataReceived_fuse(const QByteArray &data);

public slots:
};

#endif // RECEVING_FOR_FUZE_H
