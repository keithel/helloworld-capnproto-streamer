#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QQuaternion>
#include <QTimer>
#include <QDateTime>
#include "quaternionsocket.h"
#include <memory>

using std::unique_ptr;

QuaternionSocket s_mySocket;

void receivePrintQuaternions()
{
    while(s_mySocket.hasPendingDatagrams())
    {
        qDebug() << s_mySocket.receiveQuaternion();
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    QTimer timer;
    app.connect(&timer, &QTimer::timeout, [=](){
        unique_ptr<QQuaternion> quaternion;
        while(s_mySocket.hasPendingDatagrams())
        {
            qDebug() << "Receiving quaternions at " << QDateTime::currentDateTime().toString();
            quaternion = unique_ptr<QQuaternion>(s_mySocket.receiveQuaternion());
            qDebug() << *quaternion;
        }
    });
    timer.start(500);

    return app.exec();
}
