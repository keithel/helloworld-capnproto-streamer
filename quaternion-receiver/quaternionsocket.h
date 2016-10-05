#ifndef QUATERNIONSOCKET_H
#define QUATERNIONSOCKET_H

#include <QObject>

class QUdpSocket;
class QTimer;

class QuaternionSocket : public QObject
{
Q_OBJECT
public:
    QuaternionSocket(QObject* parent = 0);
    QQuaternion* receiveQuaternion();

private:
    void receivePrintQuaternions();

    QUdpSocket* m_socket;
};

#endif // QUATERNIONSOCKET_H
