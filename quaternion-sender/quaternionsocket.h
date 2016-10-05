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

public slots:
    void sendQuaternion(/*const QQuaternion* qquaternion = nullptr*/);

private:
    QUdpSocket* m_socket;
    QTimer* m_sendTimer;
};

#endif // QUATERNIONSOCKET_H
