#ifndef QUATERNIONSOCKET_H
#define QUATERNIONSOCKET_H

#include <QObject>
#include <QHostAddress>

class QUdpSocket;
class QTimer;

class QuaternionSocket : public QObject
{
Q_OBJECT
public:
    QuaternionSocket(QObject* parent = 0);
    void setRate(int rateHz);
    void setDestination(QHostAddress dest, quint16 destPort);

public slots:
    void start();
    void stop();
    void sendQuaternion();

private:
    int m_delayMsec;
    QHostAddress m_dest;
    quint16 m_destPort;

    QUdpSocket* m_socket;
    QTimer* m_sendTimer;
};

#endif // QUATERNIONSOCKET_H
