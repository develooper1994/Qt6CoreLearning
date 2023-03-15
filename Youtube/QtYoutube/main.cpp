#include <QCoreApplication>
#include <QEventLoop>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QAbstractSocket>

#include <QFuture>
#include <QtConcurrent>

#include "udpclient.h"


int main(int argc, char *argv[])
{
    QThread::currentThread()->setObjectName("Main Thread");
    QCoreApplication a(argc, argv);

    quint16 port = 1234;

    UdpClient* client  = new UdpClient();
    QThread thread;
    thread.setObjectName("Thread 1");
    client->moveToThread(&thread);
    QObject::connect(&thread,&QThread::started,client,&UdpClient::run);
    client->setPort(port);

    thread.start();

    return a.exec();
}
