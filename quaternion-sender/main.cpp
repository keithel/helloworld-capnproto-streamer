#include <QCoreApplication>

#include <QTimer>
#include <QByteArray>
#include <QDebug>
#include <capnp/message.h>
#include <capnp/serialize.h>
#include "quaternion.capnp.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using ::capnp::word;
typedef ::kj::Array<word> WordArray;

int s_delayMsec = 0;

ssize_t sendData(const QByteArray& data)
{
    static int socketFd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketFd < 0)
    {
        qWarning() << qPrintable("Failure allocating socket!");
        return socketFd;
    }
    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(11212);
    inet_aton("192.168.1.70", &servaddr.sin_addr);

    ssize_t bytesSent = sendto(socketFd, data.constData(), data.size(), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if(bytesSent < 0)
    {
        qWarning() << qPrintable("Couldn't send data");
    }
    return bytesSent;
}

void generateAndSendData()
{
    static int msecsSinceLastSec = 0;
    static quint64 totalNSent = 0;
    static quint64 lastnBytesSent = 0;
    if (msecsSinceLastSec >= 1000)
    {
        qDebug() << "Sent" << totalNSent << "quaternions, lastnBytesSent" << lastnBytesSent;
        msecsSinceLastSec = 0;
    }

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
    ssize_t bytesSentOrError = sendData(messageBuffer);
    if (bytesSentOrError < 0)
        return;
    lastnBytesSent = (quint64)bytesSentOrError;

    msecsSinceLastSec += s_delayMsec;
    totalNSent++;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTimer* timer = new QTimer(&a);
    a.connect(timer, &QTimer::timeout, &generateAndSendData);
    int frequency = 60;
    timer->start(qRound(1000.0/frequency));

    return a.exec();
}
