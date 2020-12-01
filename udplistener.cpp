#include "udplistener.h"

#include<udplistener.h>
#include<QUdpSocket>

UDPListener::UDPListener(QObject *parent) : QObject(parent)
{
    // create a QUDP socket
    socket = new QUdpSocket(this);
}

UDPListener::~UDPListener()
{
    socket->close();
    socket->deleteLater();
}

void UDPListener::HelloUDP()
{
    QByteArray Data;
    Data.append("Hello from UDP");

    // Sends the datagram datagram
    // to the host address and at port.
    // qint64 QUdpSocket::writeDatagram(const QByteArray & datagram,
    //                      const QHostAddress & host, quint16 port)
    socket->writeDatagram(Data, QHostAddress::LocalHost,45617);
}

void UDPListener::readMessage()
{
    // when data comes in
    QByteArray buffer;
    buffer.resize(socket->pendingDatagramSize());

    QHostAddress sender;
    quint16 senderPort;

    // qint64 QUdpSocket::readDatagram(char * data, qint64 maxSize,
    //                 QHostAddress * address = 0, quint16 * port = 0)
    // Receives a datagram no larger than maxSize bytes and stores it in data.
    // The sender's host address and port is stored in *address and *port
    // (unless the pointers are 0).

    socket->readDatagram(buffer.data(), buffer.size(),
                         &sender, &senderPort);

    qDebug() << "Message from: " << sender.toString();
    qDebug() << "Message port: " << senderPort;
    qDebug() << "Message: " << buffer;
}

void UDPListener::StartListening()
{
    qDebug()<<"Start";
    socket->bind(QHostAddress::LocalHost,45617);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readMessage()));

}

void UDPListener::StopListening()
{
    socket->close();
}




