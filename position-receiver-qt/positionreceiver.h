#ifndef POSITIONRECEIVER_H
#define POSITIONRECEIVER_H

#include <QObject>
#include <QContiguousCache>
#include <QTimer>
#include <QHostAddress>
#include "myposition.h"

class QUdpSocket;

class PositionReceiver : public QObject
{
Q_OBJECT
public:
    PositionReceiver(QObject* parent = 0);
    virtual ~PositionReceiver();

    bool bind(QHostAddress address, quint16 port, QHostAddress multicastGroup = QHostAddress());

signals:
    void rateChanged(unsigned int rateHz);

public slots:
    void close();
    void updateRate();

private:
    void receivePositions();
    void inspectPosition(MyPosition& pos);

private:
    unsigned int m_nPerSecCounter;
    unsigned int m_lastNPerSec;

    QHostAddress m_multicastGroup;
    QUdpSocket* m_socket;
    QTimer* m_perSecTimer;
    QContiguousCache<MyPosition> m_positionCache;
};

#endif // POSITIONRECEIVER_H
