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
    mySocket.bind(QHostAddress::AnyIPv4, 11212, QHostAddress("233.253.12.1"));

    return app.exec();
}
