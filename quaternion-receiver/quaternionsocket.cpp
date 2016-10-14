#include "quaternionsocket.h"
#include <QUdpSocket>
#include <capnp/message.h>
#include "quaternion.capnp.h"
#include <QtConcurrent/QtConcurrent>

using ::capnp::word;

QuaternionSocket::QuaternionSocket(QObject* parent)
    : QObject(parent)
    , m_nPerSecCounter(0)
    , m_lastNPerSec(0)
    , m_socket(new QUdpSocket(this))
    , m_perSecTimer(new QTimer(this))
    , m_qcache(500)
{
}

QuaternionSocket::~QuaternionSocket()
{
    close();
}

bool QuaternionSocket::bind(QHostAddress address, quint16 port, QHostAddress multicastGroup)
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
        connect(m_socket, &QUdpSocket::readyRead, this, &QuaternionSocket::receiveQuaternions);
        connect(m_perSecTimer, &QTimer::timeout, this, &QuaternionSocket::updateRate);
        m_perSecTimer->start(1000);
    }

    return true;
}

void QuaternionSocket::close()
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
        disconnect(m_perSecTimer, &QTimer::timeout, this, &QuaternionSocket::updateRate);
        disconnect(m_socket, &QUdpSocket::readyRead, this, &QuaternionSocket::receiveQuaternions);
    }
}

void QuaternionSocket::updateRate()
{
    if (m_nPerSecCounter != m_lastNPerSec)
    {
        m_lastNPerSec = m_nPerSecCounter;
        emit rateChanged(m_lastNPerSec);
    }
    m_nPerSecCounter = 0;
}

void QuaternionSocket::receiveQuaternions()
{
    while(m_socket->hasPendingDatagrams())
    {
        QByteArray buffer(m_socket->pendingDatagramSize(), (char)0);
        m_socket->readDatagram(buffer.data(), buffer.size());

        //::capnp::PackedFdMessageReader message(m_socket->socketDescriptor());
        //L3::Quaternion::Reader q = message.getRoot<L3::Quaternion>();

        QtConcurrent::run([=] {
            L3::Quaternion::Reader q = ::capnp::readMessageUnchecked<L3::Quaternion>((const word*)buffer.data());
            m_qcache.append(MyQuaternion(q.getScalar(), q.getXpos(), q.getYpos(), q.getZpos()));
        });
        m_nPerSecCounter++;
    }
}
