#ifndef QUATERNIONSOCKET_H
#define QUATERNIONSOCKET_H

#include <QObject>
#include <QContiguousCache>
#include <QQuaternion>
#include <QTimer>

class QUdpSocket;

class QuaternionSocket : public QObject
{
Q_OBJECT
public:
    QuaternionSocket(QObject* parent = 0);

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
