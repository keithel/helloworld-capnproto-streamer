#include "quaternionsocket.h"
#include <QUdpSocket>
#include <QTimer>
#include <QDebug>
#include <capnp/message.h>
#include <capnp/serialize.h>
#include "quaternion.capnp.h"

using ::capnp::word;
typedef ::kj::Array<::capnp::word> WordArray;

QuaternionSocket::QuaternionSocket(QObject* parent)
    : QObject(parent)
    , m_delayMsec(17) // this works out to ~58.82352941176 Hz
    , m_dest(QHostAddress::LocalHost)
    , m_destPort(31234)
    , m_socket(new QUdpSocket(this))
    , m_sendTimer(new QTimer(this))
{
    connect(m_sendTimer, &QTimer::timeout, this, &QuaternionSocket::sendQuaternion);
    m_sendTimer->setTimerType(Qt::PreciseTimer);
}

void QuaternionSocket::setRate(int rateHz)
{
    m_delayMsec = qRound(1000.0/rateHz);
}

void QuaternionSocket::setDestination(QHostAddress dest, quint16 destPort)
{
    m_dest = dest;
    m_destPort = destPort;
    if (m_dest.isMulticast())
    {
        m_socket->joinMulticastGroup(m_dest);
    }
}

void QuaternionSocket::start()
{
    m_sendTimer->start(m_delayMsec);
}

void QuaternionSocket::stop()
{
    m_sendTimer->stop();
}

void QuaternionSocket::sendQuaternion()
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
    qint64 bytesSent = m_socket->writeDatagram(messageBuffer, m_dest, m_destPort);
    if (bytesSent < 0)
    {
        qWarning() << qPrintable("Error sending datagram:") << m_socket->errorString();
        return;
    }
    if (bytesSent != messageBuffer.size())
    {
        qWarning() << qPrintable("Partial send") << QString::number(bytesSent) + "/" + QString::number(messageBuffer.size()) << qPrintable("bytes sent");
        return;
    }
}
