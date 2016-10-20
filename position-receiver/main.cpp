#if defined(QT_GUI_LIB)
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#else
#include <QCoreApplication>
#endif

#include "positionreceiver.h"
#include <QHostAddress>
#include <QTextStream>
#include <stdio.h>
#include <QStringBuilder>

quint32 nRatesPrinted = 0;
QChar printTicker()
{
    switch(nRatesPrinted % 4)
    {
    case 0:
        return QChar::fromLatin1('/'); break;
    case 1:
        return QChar::fromLatin1('-'); break;
    case 2:
        return QChar::fromLatin1('\\'); break;
    default:
        return QChar::fromLatin1('|'); break;
    }
}

void printRate(unsigned int rateHz)
{
    QTextStream out(stdout, QIODevice::WriteOnly);
    out << "\r" % printTicker() % " " % QString::number(rateHz) % " position updates/sec  \b\b";
    out.flush();
    nRatesPrinted++;
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#if defined(QT_GUI_LIB)
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
#else
    QCoreApplication app(argc, argv);
#endif

    PositionReceiver mySocket(&app);
    mySocket.bind(QHostAddress::AnyIPv4, 11212, QHostAddress("233.253.12.1"));
    app.connect(&mySocket, &PositionReceiver::rateChanged, printRate);

    return app.exec();
}