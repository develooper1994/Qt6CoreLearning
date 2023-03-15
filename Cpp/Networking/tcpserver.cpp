#include "tcpserver.h"

TcpServer::TcpServer(QObject *parent)
    : QObject{parent}
{
    server = new QTcpServer(this);

    connect(server,&QTcpServer::newConnection,this,&TcpServer::newConnection); // important
    connect(server,&QTcpServer::acceptError,this,&TcpServer::acceptError);
    connect(server,&QTcpServer::pendingConnectionAvailable,this,&TcpServer::pendingConnectionAvailable);
}

void TcpServer::start()
{
    // server can have many addresses.
    // listen on both IPv4 and IPv6 interfaces.
    if (!server->listen(QHostAddress::Any,getPort())) {
        qWarning() << server->errorString();
        return;
    }
    qInfo() << "Listening...";
}

void TcpServer::quit()
{
    server->close();
}

void TcpServer::newConnection()
{
    QTcpSocket* socket = server->nextPendingConnection();
    connect(socket,&QTcpSocket::disconnected,this,&TcpServer::disconnect);
    connect(socket,&QTcpSocket::readyRead,this,&TcpServer::readyRead);

    qInfo() << "Connected!";
}

void TcpServer::disconnect()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    qDebug() << "Parent" << socket->parent(); // which client?
    qInfo() << "Disconnected!" << socket;
    socket->deleteLater();
}

void TcpServer::readyRead()
{
    QTcpSocket* socket =qobject_cast<QTcpSocket*>(sender());
    qInfo() << socket->readAll();
    qInfo() << "readyRead!" << socket;
}

void TcpServer::acceptError(QAbstractSocket::SocketError socketError)
{
    qInfo() << "acceptError!";
}

void TcpServer::pendingConnectionAvailable()
{
    qInfo() << "pendingConnectionAvailable!";
}

// Getter/Setter
quint16 TcpServer::getPort() const
{
    return port;
}

void TcpServer::setPort(quint16 newPort)
{
    port = newPort;
}
