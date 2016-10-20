#ifndef POSITIONSENDER_H
#define POSITIONSENDER_H

#include <QObject>
#include <QHostAddress>

class QUdpSocket;
class QTimer;

class PositionSender : public QObject
{
Q_OBJECT
public:
    PositionSender(QObject* parent = 0);
    void setRate(int rateHz);
    void setDestination(QHostAddress dest, quint16 destPort);

public slots:
    void start();
    void stop();
    void sendPosition();

private:
    int m_delayMsec;
    QHostAddress m_dest;
    quint16 m_destPort;

    QUdpSocket* m_socket;
    QTimer* m_sendTimer;
};

#endif // POSITIONSENDER_H
