#include "positionsender.h"
#include <QUdpSocket>
#include <QTimer>
#include <QDebug>
#include <capnp/message.h>
#include <capnp/serialize.h>
#include "position.capnp.h"

using ::capnp::word;
using ::kj::ArrayPtr;
typedef ::kj::Array<word> WordArray;

PositionSender::PositionSender(QObject* parent)
    : QObject(parent)
    , m_delayMsec(17) // this works out to ~58.82352941176 Hz
    , m_dest(QHostAddress::LocalHost)
    , m_destPort(31234)
    , m_socket(new QUdpSocket(this))
    , m_sendTimer(new QTimer(this))
{
    connect(m_sendTimer, &QTimer::timeout, this, &PositionSender::sendPosition);
    m_sendTimer->setTimerType(Qt::PreciseTimer);
}

void PositionSender::setRate(int rateHz)
{
    m_delayMsec = qRound(1000.0/rateHz);
}

void PositionSender::setDestination(QHostAddress dest, quint16 destPort)
{
    m_dest = dest;
    m_destPort = destPort;
}

void PositionSender::start()
{
    m_sendTimer->start(m_delayMsec);
}

void PositionSender::stop()
{
    m_sendTimer->stop();
}

void PositionSender::sendPosition()
{
    ::capnp::MallocMessageBuilder message;
    L3::Position::Builder position = message.initRoot<L3::Position>();
    position .setHeading(qrand() % 10); // booogus values
    position.setElevation(qrand() % 100);
    position.setLatitude(qrand() % 100);
    position.setLongitude(qrand() % 100);
    position.setHeightAboveEllipsoid(qrand() % 100);

    WordArray array = ::capnp::messageToFlatArray(message);
    ArrayPtr<unsigned char> messageBytes(array.asBytes());
    QByteArray messageBuffer((const char*)messageBytes.begin(), messageBytes.size());
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
