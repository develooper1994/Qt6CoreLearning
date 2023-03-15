#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QDebug>
#include <QAbstractSocket>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>


class TcpServer : public QObject
{
    Q_OBJECT
private:
    QTcpServer* server;
    quint16 port;
public:
    explicit TcpServer(QObject *parent = nullptr);

    quint16 getPort() const;
    void setPort(quint16 newPort);

public slots:
    void start();
    void quit();
    void newConnection();
    void disconnect();
    void readyRead();
    void acceptError(QAbstractSocket::SocketError socketError);
    void pendingConnectionAvailable();


signals:
};

#endif // TCPSERVER_H
