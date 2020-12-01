#ifndef UDPLISTENER_H
#define UDPLISTENER_H

#include <QObject>
#include <QUdpSocket>

class UDPListener : public QObject
{
    Q_OBJECT
public:
    explicit UDPListener(QObject *parent = nullptr);
    ~UDPListener();
    void HelloUDP();

signals:

public slots:
    void readMessage();
    void StartListening();
    void StopListening();

private:
    QUdpSocket *socket;

};

#endif // UDPLISTENER_H
