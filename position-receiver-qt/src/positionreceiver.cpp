#include "positionreceiver.h"
#include <QUdpSocket>
#include <capnp/message.h>
#include <capnp/serialize.h>
#include "position.capnp.h"
#include <QtConcurrent/QtConcurrent>

using ::capnp::word;

PositionReceiver::PositionReceiver(QObject* parent, bool enableTesting)
    : QObject(parent)
    , m_nPerSecCounter(0)
    , m_lastNPerSec(0)
    , m_socket(new QUdpSocket(this))
    , m_perSecTimer(new QTimer(this))
    , m_positionCache(500)
    , m_testing(enableTesting)
{
}

PositionReceiver::~PositionReceiver()
{
    close();
}

bool PositionReceiver::bind(QHostAddress address, quint16 port, QHostAddress multicastGroup)
{
    if (address.isNull())
        return false;

    if (m_socket->state() == QAbstractSocket::ConnectingState
        || m_socket->state() == QAbstractSocket::ConnectedState)
    {
        m_socket->disconnectFromHost();
    }
    if (m_socket->state() == QAbstractSocket::BoundState)
    {
        m_socket->close();
    }

    if (!m_socket->bind(address, port))
        return false;

    if (!multicastGroup.isNull())
    {
        m_socket->joinMulticastGroup(multicastGroup);
        m_multicastGroup = multicastGroup;
    }

    if (!m_perSecTimer->isActive())
    {
        connect(m_socket, &QUdpSocket::readyRead, this, &PositionReceiver::receivePositions);
        connect(m_perSecTimer, &QTimer::timeout, this, &PositionReceiver::updateRate);
        m_perSecTimer->start(1000);
    }

    return true;
}

void PositionReceiver::close()
{
    if (!m_multicastGroup.isNull())
    {
        m_socket->leaveMulticastGroup(m_multicastGroup);
        m_multicastGroup.clear();
    }

    if (m_socket->state() == QAbstractSocket::ConnectingState
        || m_socket->state() == QAbstractSocket::ConnectedState)
    {
        m_socket->disconnectFromHost();
    }
    if (m_socket->state() == QAbstractSocket::BoundState)
    {
        m_socket->close();
    }

    if (m_perSecTimer->isActive())
    {
        m_perSecTimer->stop();
        disconnect(m_perSecTimer, &QTimer::timeout, this, &PositionReceiver::updateRate);
        disconnect(m_socket, &QUdpSocket::readyRead, this, &PositionReceiver::receivePositions);
    }
}

void PositionReceiver::updateRate()
{
    if (m_nPerSecCounter != m_lastNPerSec)
    {
        m_lastNPerSec = m_nPerSecCounter;
        emit rateChanged(m_lastNPerSec);
    }
    m_nPerSecCounter = 0;
}

void PositionReceiver::receivePositions()
{
    while(m_socket->pendingDatagramSize() > 0)
    {
#if defined(SHOW_HASPENDINGDATAGRAMS_BUG)
        if (!m_socket->hasPendingDatagrams())
            qWarning() << qPrintable("socket->hasPendingDatagrams() false but pendingDatagramSize nonzero (") << m_socket->pendingDatagramSize() << qPrintable(")");
#endif
        QByteArray buffer(m_socket->pendingDatagramSize(), (char)0);
        m_socket->readDatagram(buffer.data(), buffer.size());

        QtConcurrent::run([=] {
            ::capnp::FlatArrayMessageReader reader(::kj::ArrayPtr<const word>((const word*)buffer.data(), buffer.size()));
            L3::Position::Reader pos = reader.getRoot<L3::Position>();

            Position newPos(pos.getHeading(), pos.getElevation(),
                            pos.getLatitude(), pos.getLongitude(),
                            pos.getHeightAboveEllipsoid(), pos.getRoll());
            if (m_testing)
                inspectPosition(newPos);
            m_positionCache.append(newPos);
            emit positionReceived(newPos);
        });
        m_nPerSecCounter++;
    }
}

void PositionReceiver::inspectPosition(const Position &pos)
{
    // Don't bother inspecting the first one.
    // We can't know which of the two we'll first receive.
    if (m_positionCache.isEmpty())
        return;

    Position expectedPos = m_positionCache.last();
    int nextPosAheadBehind = (qFuzzyCompare(expectedPos.heading, 44.0f)) ? 1 : -1;
    expectedPos.heading = expectedPos.heading + (nextPosAheadBehind * 11);
    expectedPos.elevation = expectedPos.elevation + (nextPosAheadBehind * 11);
    expectedPos.latitude = expectedPos.latitude + (nextPosAheadBehind * 11);
    expectedPos.longitude = expectedPos.longitude + (nextPosAheadBehind * 11);
    expectedPos.heightAboveEllipsoid = expectedPos.heightAboveEllipsoid + (nextPosAheadBehind * 11);
    expectedPos.roll = expectedPos.roll + (nextPosAheadBehind * 11);

    if (pos != expectedPos)
    {
        qWarning() << qPrintable("Received pos doesn't match expected. received:") << pos << qPrintable("expected:") << expectedPos;
    }
}
