#include "quaternionsocket.h"
#include <QUdpSocket>
#include <QQuaternion>
#include <QTimer>
#include <capnp/message.h>
#include <capnp/serialize-packed.h>
#include "quaternion.capnp.h"
#include <QDebug>
#include <QDateTime>
#include <memory>

using ::capnp::word;
using std::unique_ptr;

QuaternionSocket::QuaternionSocket(QObject* parent)
    : QObject(parent)
    , m_socket(new QUdpSocket(this))
{
    m_socket->bind(QHostAddress::LocalHost, 31234);
    connect(m_socket, &QUdpSocket::readyRead, this, &QuaternionSocket::receivePrintQuaternions);
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

void QuaternionSocket::receivePrintQuaternions()
{
    unique_ptr<QQuaternion> quaternion;
    while(m_socket->hasPendingDatagrams())
    {
        qDebug() << "Receiving quaternions at " << QDateTime::currentDateTime().toString();
        quaternion = unique_ptr<QQuaternion>(receiveQuaternion());
        if(quaternion)
            qDebug() << *quaternion;
    }
}
