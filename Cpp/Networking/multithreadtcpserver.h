#ifndef MULTITHREADTCPSERVER_H
#define MULTITHREADTCPSERVER_H

#include <QObject>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThreadPool>
#include <QThread>

#include "multithreadsenkrontcpclient.h"

class MultiThreadTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit MultiThreadTcpServer(QObject *parent = nullptr);

public slots:
    void start(quint16 port);
    void quit();

private:
    QThreadPool pool;

    // QTcpServer interface
protected:
    void incomingConnection(qintptr handle) Q_DECL_OVERRIDE;

signals:
};

#endif // MULTITHREADTCPSERVER_H
