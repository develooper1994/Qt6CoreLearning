#include "multithreadtcpserver.h"

MultiThreadTcpServer::MultiThreadTcpServer(QObject *parent)
    : QTcpServer{parent}
{
    pool.setMaxThreadCount(200);
    qInfo() << "Threads: " << pool.maxThreadCount();
}

void MultiThreadTcpServer::start(quint16 port){
    qInfo() << this << " start " << QThread::currentThread();
    if(this->listen(QHostAddress::Any, port)){
        qInfo() << "Server started on " << port;
    }
    else{
        qCritical() << this->errorString();
    }
}

void MultiThreadTcpServer::quit()
{
    this->close();
    qInfo() << "Server Stopped!";
}

void MultiThreadTcpServer::incomingConnection(qintptr handle)
{
    //Not version friendly!!!
    qInfo() << "Incomming Connection " << handle << " on " << QThread::currentThread();
    MultiThreadSenkronTcpClient* client = new MultiThreadSenkronTcpClient(nullptr, handle);
    client->setAutoDelete(true);
    pool.start(client);
}
