#include "quaternionsocket.h"
#include <QUdpSocket>
#include <QQuaternion>
#include <capnp/message.h>
#include <capnp/serialize-packed.h>
#include "quaternion.capnp.h"
#include <QDebug>
#include <QtConcurrent/QtConcurrent>
#include <memory>

using ::capnp::word;
using std::unique_ptr;

QuaternionSocket::QuaternionSocket(QObject* parent)
    : QObject(parent)
    , m_nPerSecCounter(0)
    , m_lastNPerSec(0)
    , m_socket(new QUdpSocket(this))
    , m_perSecTimer(new QTimer(this))
    , m_qcache(500)
{
    m_socket->bind(QHostAddress::LocalHost, 31234);
    connect(m_socket, &QUdpSocket::readyRead, this, &QuaternionSocket::receiveQuaternions);
    connect(m_perSecTimer, &QTimer::timeout, [=] { qDebug() << (m_lastNPerSec = m_nPerSecCounter) << "quaternions/sec"; m_nPerSecCounter = 0; });
    m_perSecTimer->start(1000);
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
            m_qcache.append(QQuaternion(q.getScalar(), q.getXpos(), q.getYpos(), q.getZpos()));
        });
        m_nPerSecCounter++;
    }
}
