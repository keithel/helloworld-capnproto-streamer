#if defined(QT_GUI_LIB)
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#else
#include <QCoreApplication>
#endif

#include "positionreceiver.h"
#include "options.h"
#include <QHostAddress>
#include <QTextStream>
#include <stdio.h>
#include <QStringBuilder>

static Options s_opts;

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

MyPosition s_pos(0.0f,0.0f,0.0f,0.0f,0.0f,0.0f);

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#if defined(QT_GUI_LIB)
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    engine.rootContext()->setContextProperty("positionn", &s_pos);
#else
    QCoreApplication app(argc, argv);
#endif

    s_opts.parseArgs(argc, argv);

    PositionReceiver mySocket(&app, s_opts.isTesting());
    mySocket.bind(QHostAddress::AnyIPv4, 11212, QHostAddress("233.253.12.1"));
    app.connect(&mySocket, &PositionReceiver::rateChanged, printRate);
    app.connect(&mySocket, &PositionReceiver::positionReceived, [](MyPosition* newPos){s_pos.copyPosition(*newPos); delete newPos;});

    return app.exec();
}
