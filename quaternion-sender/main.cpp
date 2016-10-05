#include <QCoreApplication>
#include <quaternionsocket.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QuaternionSocket mySocket;

    return a.exec();
}
