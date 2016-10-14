#ifndef QUATERNIONSOCKET_H
#define QUATERNIONSOCKET_H

#include <QObject>
#include <QContiguousCache>
#include <QTimer>
#include <QHostAddress>

class QUdpSocket;

class MyQuaternion {
public:
    MyQuaternion(float scalar, float xPos, float yPos, float zPos)
        : m_scalar(scalar), m_xPos(xPos), m_yPos(yPos), m_zPos(zPos) {}

public:
    float m_scalar;
    float m_xPos;
    float m_yPos;
    float m_zPos;
};

class QuaternionSocket : public QObject
{
Q_OBJECT
public:
    QuaternionSocket(QObject* parent = 0);
    virtual ~QuaternionSocket();

    bool bind(QHostAddress address, quint16 port, QHostAddress multicastGroup = QHostAddress());

signals:
    void rateChanged(unsigned int rateHz);

public slots:
    void close();
    void updateRate();

private:
    void receiveQuaternions();

private:
    unsigned int m_nPerSecCounter;
    unsigned int m_lastNPerSec;

    QHostAddress m_multicastGroup;
    QUdpSocket* m_socket;
    QTimer* m_perSecTimer;
    QContiguousCache<MyQuaternion> m_qcache;
};

#endif // QUATERNIONSOCKET_H
