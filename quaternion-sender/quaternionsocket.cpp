#include "quaternionsocket.h"
#include <QUdpSocket>
#include <QQuaternion>
#include <QTimer>
#include <QDebug>
#include <capnp/message.h>
#include <capnp/serialize-packed.h>
#include "quaternion.capnp.h"

using ::capnp::word;
typedef ::kj::Array<::capnp::word> WordArray;

QuaternionSocket::QuaternionSocket(QObject* parent)
    : QObject(parent)
    , m_socket(new QUdpSocket(this))
    , m_sendTimer(new QTimer(this))
{
    //m_socket->bind(QHostAddress::LocalHost, 31234);
    //m_socket->connectToHost(QHostAddress::LocalHost, 31234, QAbstractSocket::WriteOnly);
    connect(m_sendTimer, &QTimer::timeout, this, &QuaternionSocket::sendQuaternion);
    m_sendTimer->start(1000);
}

void QuaternionSocket::sendQuaternion(/*const QQuaternion* qquaternion*/)
{
    ::capnp::MallocMessageBuilder message;
    L3::Quaternion::Builder quaternion = message.initRoot<L3::Quaternion>();
//    if (qquaternion != nullptr)
//    {
//        quaternion.setScalar(qquaternion->scalar());
//        quaternion.setXpos(qquaternion->x());
//        quaternion.setYpos(qquaternion->y());
//        quaternion.setZpos(qquaternion->z());
//    }
//    else
    {
        quaternion.setScalar(qrand() % 10); // booogus values
        quaternion.setXpos(qrand() % 100);
        quaternion.setYpos(qrand() % 100);
        quaternion.setZpos(qrand() % 100);
    }

    L3::Quaternion::Reader qreader(quaternion.asReader());
    uint64_t size = qreader.totalSize().wordCount+1;

    WordArray array = ::kj::heapArray<word>(size);
    memset(array.begin(), 0, size*sizeof(word));

    ::capnp::copyToUnchecked(quaternion.asReader(), array.asPtr());
    QByteArray messageBuffer((char*)array.begin(), size*sizeof(word));

    m_socket->writeDatagram(messageBuffer, QHostAddress::LocalHost, 31234);
    //::capnp::writePackedMessageToFd(m_socket->socketDescriptor(), message);
    qDebug() << "Sent quaternion";
}
