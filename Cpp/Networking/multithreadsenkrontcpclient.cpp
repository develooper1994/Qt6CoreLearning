#include "multithreadsenkrontcpclient.h"

MultiThreadSenkronTcpClient::MultiThreadSenkronTcpClient(QObject *parent, qintptr handle)
    : QObject{parent}
{
    this->handle = handle;
}

void MultiThreadSenkronTcpClient::run()
{
    // senkron tcp client
    QTcpSocket* socket = new QTcpSocket(nullptr);
    if (!socket->setSocketDescriptor(handle)) {
        qCritical() << socket->errorString();
        delete socket;
        return;
    }

    socket->waitForReadyRead();
    QByteArray request = socket->readAll();
    qInfo() << "Request Len: " << request.length();

    QByteArray content("Mazide kaldı hep Mazide kaldı hep Mazide kaldı hep Mazide kaldı hep Mazide kaldı hep Mazide kaldı hep");
    QByteArray response;
    response.append("HTTP/1.1 200 OK \r\n");
    response.append("Content Type: text/plain \r\n");
    response.append("Content Lenght: " + QString::number(content.length()).toUtf8() + "\r\n");
    response.append("Connection: Close\r\n");
    response.append(content);
    response.append("\r\n");
    socket->write(response);
    socket->waitForBytesWritten();

    socket->close();
    socket->deleteLater();

    qInfo() << this << " complete " << QThread::currentThread();
}
