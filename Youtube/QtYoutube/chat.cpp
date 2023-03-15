#include "chat.h"

Chat::Chat(QObject *parent)
    : QObject{parent}
{
    if (!socket.bind(port)) {
        qDebug() << socket.errorString();
    } else {
        qInfo() << "Started on: " << socket.localAddress() << ":" << socket.localPort();
        connect(&socket,&QUdpSocket::readyRead,this,&Chat::readyRead);
    }
}

void Chat::command(QString value)
{
    QString message = name + ": ";
    // yeni katılan
    if (name.isEmpty()) {
        name = value;
        message = name + ": Joined : ";
        send(message);
        return;
    }
    // katılmış ve mesaj giren
    message.append(value);
    send(message);
}

void Chat::send(QString value)
{
    QByteArray data = value.toUtf8();
    QNetworkDatagram datagram(data, QHostAddress::Broadcast, port);
    if (!socket.writeDatagram(datagram)) {
        qInfo() << socket.errorString();
    }
}

void Chat::readyRead()
{
    while (socket.hasPendingDatagrams()) {
        QNetworkDatagram datagram = socket.receiveDatagram();
        qInfo() << datagram.data();
    }
}
