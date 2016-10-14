#include <QCoreApplication>
#include <quaternionsocket.h>
#include <QHostAddress>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QuaternionSocket mySocket;
    mySocket.setRate(60);
    mySocket.setDestination(QHostAddress("192.168.1.203"), 11212);
    mySocket.start();

    return a.exec();
}
