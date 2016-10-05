#include "quaternionsocket.h"
#include <QUdpSocket>
#include <QQuaternion>
#include <QTimer>
#include <capnp/message.h>
#include <capnp/serialize-packed.h>
#include "quaternion.capnp.h"
#include <QDebug>

using ::capnp::word;

QuaternionSocket::QuaternionSocket(QObject* parent)
    : QObject(parent)
    , m_socket(new QUdpSocket(this))
{
    m_socket->bind(QHostAddress::LocalHost, 31234);
}

bool QuaternionSocket::hasPendingDatagrams() const
{
    if (m_socket->state() == QAbstractSocket::BoundState
        || m_socket->state() == QAbstractSocket::ConnectedState)
    {
        return m_socket->hasPendingDatagrams();
    }
    return false;
}

QQuaternion* QuaternionSocket::receiveQuaternion()
{
    if (!m_socket->hasPendingDatagrams())
        return nullptr;

    QByteArray buffer(m_socket->pendingDatagramSize(), (char)0);
    m_socket->readDatagram(buffer.data(), buffer.size());

    L3::Quaternion::Reader q = ::capnp::readMessageUnchecked<L3::Quaternion>((const word*)buffer.data());

//    ::capnp::PackedFdMessageReader message(m_socket->socketDescriptor());
//    L3::Quaternion::Reader q = message.getRoot<L3::Quaternion>();

    return new QQuaternion(q.getScalar(), q.getXpos(), q.getYpos(), q.getZpos());
}
