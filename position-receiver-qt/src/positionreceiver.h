#ifndef POSITIONRECEIVER_H
#define POSITIONRECEIVER_H

#include <QObject>
#include <QContiguousCache>
#include <QTimer>
#include <QHostAddress>
#include "position.h"

class QUdpSocket;

class PositionReceiver : public QObject
{
Q_OBJECT
public:
    PositionReceiver(QObject* parent = 0, bool enableTesting = false);
    virtual ~PositionReceiver();

    bool bind(QHostAddress address, quint16 port, QHostAddress multicastGroup = QHostAddress());
    inline void setTesting(bool testing) { m_testing = testing; }

signals:
    void rateChanged(unsigned int rateHz);
    void positionReceived(Position pos);

public slots:
    void close();
    void updateRate();

private:
    void receivePositions();
    void inspectPosition(const Position &pos);

private:
    unsigned int m_nPerSecCounter;
    unsigned int m_lastNPerSec;

    QHostAddress m_multicastGroup;
    QUdpSocket* m_socket;
    QTimer* m_perSecTimer;
    QContiguousCache<Position> m_positionCache;
    bool m_testing;
};

#endif // POSITIONRECEIVER_H
