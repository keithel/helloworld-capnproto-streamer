#include <QCoreApplication>
#include <positionsender.h>
#include <QHostAddress>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    PositionSender sender;
    sender.setRate(60);
    sender.setDestination(QHostAddress("233.253.12.1"), 11212);
    sender.start();

    return a.exec();
}
