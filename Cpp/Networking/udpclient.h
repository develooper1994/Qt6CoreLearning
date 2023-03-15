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
    QTimer* timer;
    QEventLoop* loop;
    QUdpSocket* socket;
    qint16 port = 1234; //Note < 1024 those admin ports / special services. HTTP, FTP, POP3, SMTP
    int counter;
public:
    explicit UdpClient(QObject *parent = nullptr);
    ~UdpClient();

    qint16 getPort() const;
    void setPort(qint16 newPort);

public slots:
    void run(); // thread start
    void start();
    void stop();
    void timeout();
    void readyRead();
    void broadcast();

signals:
};

#endif // UDPCLIENT_H
