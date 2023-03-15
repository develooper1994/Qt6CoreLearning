#include "udpclient.h"

UdpClient::UdpClient(QObject *parent)
    : QObject{parent}
{
    qInfo() << Q_FUNC_INFO << QThread::currentThread();

    timer = new QTimer(this);
    socket = new QUdpSocket(this);
    loop = new QEventLoop(this);

    connect(timer,&QTimer::timeout,this,&UdpClient::timeout);
    connect(socket,&QUdpSocket::readyRead,this,&UdpClient::readyRead);
    timer->setInterval(1000);
    counter=0;
}

UdpClient::~UdpClient()
{
    qInfo() << Q_FUNC_INFO << QThread::currentThread();
    qInfo() << "Deconstructed";
}

// thread start
void UdpClient::run()
{
    qInfo() << Q_FUNC_INFO << QThread::currentThread();

    start();
    loop->exec();
}

void UdpClient::start()
{
    qInfo() << Q_FUNC_INFO << QThread::currentThread();

    if (!socket->bind(port)) { // QAbstractSocket::ShareAddress
        qInfo() << socket->errorString(); // change port
        return;
    }

    qInfo() << "started udp on: " << socket->localAddress() << ":" << socket->localPort();

    broadcast();
}

void UdpClient::stop()
{
    qInfo() << Q_FUNC_INFO << QThread::currentThread();

    timer->stop();
    socket->close();
    if (loop) {
        qInfo() << "Stopping event loop";
        loop->quit();
        delete loop;
    }

    qInfo() << "Stopping";
    deleteLater();
}

void UdpClient::timeout()
{
    qInfo() << Q_FUNC_INFO << QThread::currentThread();

    QString date = QDateTime::currentDateTime().toString();
    QByteArray data = date.toUtf8();
    qInfo() << "send: " << data;

    QNetworkDatagram datagram(data,QHostAddress::Broadcast, port);
    socket->writeDatagram(datagram);

    ++counter;
    if(counter>5) stop();
}

void UdpClient::readyRead()
{
    qInfo() << Q_FUNC_INFO << QThread::currentThread();

    while (socket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = socket->receiveDatagram();
        qInfo() << "read: " << datagram.data() <<
        "from: " << datagram.senderAddress() << ":" << datagram.senderPort() <<
        "to: " << datagram.destinationAddress() << ":" << datagram.destinationPort();
    }
}

void UdpClient::broadcast()
{
    qInfo() << Q_FUNC_INFO << QThread::currentThread();

    qInfo() << "Broadcasting...";
    timer->start();
}

// Setter/Getter
qint16 UdpClient::getPort() const
{
    return port;
}

void UdpClient::setPort(qint16 newPort)
{
    port = newPort;
}
