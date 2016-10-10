#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "quaternionsocket.h"
#include <QHostAddress>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    QuaternionSocket mySocket(&app);
    mySocket.bind(QHostAddress::LocalHost, 11212);

    return app.exec();
}
