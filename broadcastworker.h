#ifndef BROADCASTWORKER_H
#define BROADCASTWORKER_H

#include <QObject>
#include <QCoreApplication>
#include <QHostAddress>
#include <QDebug>
#include <stdio.h>

#define LOCAL_PORT 39510
#define DEST_PORT 39510
#define GET_HOST_COMMAND "GetIPAddr"
#define TRY_TIMES 1

class QUdpSocket;

class BroadCastWorker : public QObject
{
    Q_OBJECT
public:
    explicit BroadCastWorker(QObject *parent = 0);
    ~BroadCastWorker();
    void UdpBoardCast();
    void setStop(bool stop);
    void BroadcastGetIpCommand();

signals:
     void BoardCastDone();

public slots:
private slots:
        void processPengingDatagram();
private:
     int m_is_stop;
     QUdpSocket *receiver;
     QUdpSocket *sender;
private:
     BroadCastWorker(const BroadCastWorker &);
     const BroadCastWorker & operator = (const BroadCastWorker &);
};

#endif // BROADCASTWORKER_H
