#ifndef QUATERNIONSOCKET_H
#define QUATERNIONSOCKET_H

#include <QObject>
#include <QContiguousCache>
#include <QQuaternion>
#include <QTimer>
#include <QHostAddress>

class QUdpSocket;

class QuaternionSocket : public QObject
{
Q_OBJECT
public:
    QuaternionSocket(QObject* parent = 0);
    virtual ~QuaternionSocket();

    bool bind(QHostAddress address, quint16 port);

public slots:
    void close();
    void printRate();

private:
    void receiveQuaternions();

private:
    unsigned int m_nPerSecCounter;
    unsigned int m_lastNPerSec;

    QUdpSocket* m_socket;
    QTimer* m_perSecTimer;
    QContiguousCache<QQuaternion> m_qcache;
};

#endif // QUATERNIONSOCKET_H
