#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <QObject>
#include <QDebug>
#include <QEventLoop>
#include <QTimer>
#include <QDateTime>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QThread>

class UdpClient : public QObject
{
    Q_OBJECT
private:
    QUdpSocket* socket;
    QTimer* timer;
    QEventLoop* loop;
    qint16 port = 1234; // port<1024 => admin ports, special ports. http, ftp, smtp
    int counter;
public:
    explicit UdpClient(QObject *parent = nullptr);
    ~UdpClient();

    qint16 getPort() const;
    void setPort(qint16 newPort);

public slots:
    //thread start
    void run();

    void start();
    void stop();
    void timeout();
    void readyRead();
    void broadcast();

signals:

};

#endif // UDPCLIENT_H
