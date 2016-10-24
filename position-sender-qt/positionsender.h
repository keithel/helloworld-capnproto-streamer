#ifndef POSITIONSENDER_H
#define POSITIONSENDER_H

#include <QObject>
#include <QHostAddress>
#include <QList>
#include <QByteArray>

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
    void initPositions();

    int m_delayMsec;
    QHostAddress m_dest;
    quint16 m_destPort;

    QUdpSocket* m_socket;
    QTimer* m_sendTimer;

    QList<QByteArray> m_positionBuffers;
};

#endif // POSITIONSENDER_H
