#ifndef MULTITHREADSENKRONTCPCLIENT_H
#define MULTITHREADSENKRONTCPCLIENT_H

#include <QObject>
#include <QDebug>
#include <QRunnable>
#include <QThread>
#include <QTcpSocket>

class MultiThreadSenkronTcpClient : public QObject, public QRunnable
{
    Q_OBJECT
private:
    qintptr handle = 0;
public:
    explicit MultiThreadSenkronTcpClient(QObject *parent = nullptr, qintptr handle=0);

    // QRunnable interface
public:
    void run();

signals:
};

#endif // MULTITHREADSENKRONTCPCLIENT_H
