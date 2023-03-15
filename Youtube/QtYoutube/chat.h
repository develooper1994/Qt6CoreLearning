#ifndef CHAT_H
#define CHAT_H

#include <QObject>
#include <QDebug>
#include <QUdpSocket>
#include <QNetworkDatagram>

class Chat : public QObject
{
    Q_OBJECT
private:
    QString name; // username
    QUdpSocket socket;
    qint16 port = 1234;

public:
    explicit Chat(QObject *parent = nullptr);

public slots:
    void command(QString value);
    void send(QString value);
    void readyRead();

signals:
};

#endif // CHAT_H
