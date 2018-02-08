#include<QtNetwork>
#include "broadcastworker.h"

BroadCastWorker::BroadCastWorker(QObject *parent) : QObject(parent)
{
     this->m_is_stop = false;
}

BroadCastWorker::~BroadCastWorker()
{

}

void BroadCastWorker::UdpBoardCast()
{
    receiver = new QUdpSocket(this);
    receiver->bind(LOCAL_PORT, QUdpSocket::ShareAddress);
    connect(receiver, SIGNAL(readyRead()), this, SLOT(processPengingDatagram())); 
}

void BroadCastWorker::processPengingDatagram()
{
    QHostAddress address;
    quint16 port;
    while(receiver->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(receiver->pendingDatagramSize());
        receiver->readDatagram(datagram.data(), datagram.size(), &address, &port);
        emit sendServerBoardCastMsg(datagram.data(), address.toString());
    }
}

void BroadCastWorker::setStop(bool stop)
{
    this->m_is_stop = stop;
}

