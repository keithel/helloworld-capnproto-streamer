#ifndef POSITIONRECEIVER_H
#define POSITIONRECEIVER_H

#include <QObject>
#include <QContiguousCache>
#include <QTimer>
#include <QHostAddress>

class QUdpSocket;

class MyPosition {
public:
    MyPosition(float heading, float elevation, float latitude, float longitude, float heightAboveEllipsoid)
        : m_heading(heading), m_elevation(elevation),
          m_latitude(latitude), m_longitude(longitude),
          m_heightAboveEllipsoid(heightAboveEllipsoid)
    {}

public:
    float m_heading;
    float m_elevation;
    float m_latitude;
    float m_longitude;
    float m_heightAboveEllipsoid;
};

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

private:
    unsigned int m_nPerSecCounter;
    unsigned int m_lastNPerSec;

    QHostAddress m_multicastGroup;
    QUdpSocket* m_socket;
    QTimer* m_perSecTimer;
    QContiguousCache<MyPosition> m_positionCache;
};

#endif // POSITIONRECEIVER_H
