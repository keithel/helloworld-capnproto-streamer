#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "quaternionsocket.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    QuaternionSocket mySocket(&app);

    return app.exec();
}
