#include "quaternionsocket.h"
#include <QUdpSocket>
#include <QQuaternion>
#include <QTimer>
#include <QDebug>
#include <capnp/message.h>
#include <capnp/serialize.h>
#include "quaternion.capnp.h"

using ::capnp::word;
typedef ::kj::Array<::capnp::word> WordArray;

QuaternionSocket::QuaternionSocket(QObject* parent)
    : QObject(parent)
    , m_socket(new QUdpSocket(this))
    , m_sendTimer(new QTimer(this))
{
    connect(m_sendTimer, &QTimer::timeout, this, &QuaternionSocket::sendQuaternion);
    m_sendTimer->setTimerType(Qt::PreciseTimer);
    m_sendTimer->start(17); // this works out to ~58.82352941176 Hz
}

void QuaternionSocket::sendQuaternion(/*const QQuaternion* qquaternion*/)
{
    ::capnp::MallocMessageBuilder message;
    L3::Quaternion::Builder quaternion = message.initRoot<L3::Quaternion>();
    quaternion.setScalar(qrand() % 10); // booogus values
    quaternion.setXpos(qrand() % 100);
    quaternion.setYpos(qrand() % 100);
    quaternion.setZpos(qrand() % 100);

    L3::Quaternion::Reader qreader(quaternion.asReader());
    uint64_t size = qreader.totalSize().wordCount+1;

    WordArray array = ::capnp::messageToFlatArray(message);
    QByteArray messageBuffer((char*)array.begin(), size*sizeof(word));
    m_socket->writeDatagram(messageBuffer, QHostAddress::LocalHost, 31234);
}
