#include "tcpclient.h"

TcpClient::TcpClient(QObject *parent)
    : QObject{parent}
{
    qInfo() << Q_FUNC_INFO << QThread::currentThread();

    loop = new QEventLoop(this);
    socket = new QSslSocket(this);

    connect(socket,&QTcpSocket::connected,this,&TcpClient::connecting);
    connect(socket,&QTcpSocket::disconnected,this,&TcpClient::disconnecting);

    connect(socket,&QTcpSocket::stateChanged,this,&TcpClient::stateChanging);
    connect(socket,&QTcpSocket::readyRead,this,&TcpClient::readyReady);

    //Qt 5
    //connect(&socket,QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),this,&TcpClient::error); //no longer valid;
    connect(socket,&QTcpSocket::errorOccurred,this,&TcpClient::error);

    // SSL
    connect(socket,&QSslSocket::encrypted,this,&TcpClient::encrypted);
    connect(socket,&QSslSocket::encryptedBytesWritten,this,&TcpClient::encryptedBytesWritten);
    connect(socket,&QSslSocket::modeChanged,this,&TcpClient::modeChanged);
    connect(socket,&QSslSocket::peerVerifyError,this,&TcpClient::peerVerifyError);
    connect(socket,&QSslSocket::preSharedKeyAuthenticationRequired,this,&TcpClient::preSharedKeyAuthenticationRequired);
    connect(socket,QOverload<const QList<QSslError> &>::of(&QSslSocket::sslErrors),this,&TcpClient::sslErrors);

    /*
    // Proxy connection
    QString ProxyIP = "190.7.141.66"; // "180.250.65.83";
    quint16 ProxyPort = 47576; //8080
    QNetworkProxy proxy(QNetworkProxy::HttpProxy, ProxyIP, ProxyPort);

    // set auth for Proxy. No need for free proxy servers !!!Just be extremly carreful with free proxy servers!!!
    //proxy.setUser("user");
    //proxy.setPassword("pass");


    // Apply one of them
    // Application proxy. Global proxy. Applies for all sockets
    QNetworkProxy::setApplicationProxy(proxy);
    // Per socket. Applies only for one socket.
    socket->setProxy(proxy);
*/

}

TcpClient::~TcpClient()
{
    qInfo() << Q_FUNC_INFO << QThread::currentThread();
    qInfo() << "Deconstructed";
}

// thread start
void TcpClient::run()
{
    qInfo() << Q_FUNC_INFO << QThread::currentThread();

    connectToHost(m_hostname, m_port);
    loop->exec();
}

// public slots:
void TcpClient::connectToHost(QString hostname, quint16 port)
{
    qInfo() << Q_FUNC_INFO << QThread::currentThread();

    if(socket->isOpen()) disconnect();
    qInfo() << "Connecting to: " << hostname << " on port " << port;

    //socket->connectToHost(hostname,port); // normal tcp, unencrypted

    socket->ignoreSslErrors(); // NOT SECURE! Don't use in production. just use it for testing.
    socket->setProtocol(QSsl::SecureProtocols); //QSsl::AnyProtocol
    socket->connectToHostEncrypted(hostname,port); // makes 3way handshake and creates encrypted tunnel Highly complex topic.

}

void TcpClient::disconnect()
{
    qInfo() << Q_FUNC_INFO << QThread::currentThread();

    socket->close();
    socket->waitForDisconnected();
    if (loop) {
        qInfo() << "Stopping event loop";
        loop->quit();
        delete loop;
    }

    qInfo() << "Stopped";
    deleteLater();
}

// private slots:
void TcpClient::connecting()
{
    qInfo() << Q_FUNC_INFO << QThread::currentThread();

    qInfo() << "Connected!";

    qInfo() << "Sending";
    QByteArray data;

    // valid http request
    data.append("GET /get HTTP/1.1\r\n");
    data.append("User-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.0; Trident/4.0)\r\n");
    data.append("Host: local\r\n");
    data.append("Connection: Close\r\n");
    data.append("\r\n");

    // simple and not valid handshake
    //data.append("HELLO\r\n");
    socket->write(data); // junk data
    socket->waitForBytesWritten(); // not mandetory
}

void TcpClient::disconnecting()
{
    qInfo() << Q_FUNC_INFO << QThread::currentThread();

    qInfo() << "Disconnecting";
}

void TcpClient::error(QAbstractSocket::SocketError socketError)
{
    qInfo() << Q_FUNC_INFO << QThread::currentThread();

    qInfo() << "Error: " << socketError << " " << socket->errorString();
}

void TcpClient::stateChanging(QAbstractSocket::SocketState socketState)
{
    qInfo() << Q_FUNC_INFO << QThread::currentThread();

    QMetaEnum metaEnum = QMetaEnum::fromType<QAbstractSocket::SocketState>();
    qInfo() << "State:" << metaEnum.valueToKey(socketState);
}

void TcpClient::readyReady()
{
    qInfo() << Q_FUNC_INFO << QThread::currentThread();

    qInfo() << "Data from: " << sender() << " bytes: " << socket->bytesAvailable();
    qInfo() << "Data: " << socket->readAll();
}

// SSL
void TcpClient::encrypted()
{
    qInfo() << Q_FUNC_INFO << QThread::currentThread();

    qInfo() << "Encrypted";
}

void TcpClient::encryptedBytesWritten(qint64 written)
{
    qInfo() << Q_FUNC_INFO << QThread::currentThread();

    qInfo() << "Encrypted Written: " << written;
}

void TcpClient::modeChanged(QSslSocket::SslMode mode)
{
    qInfo() << Q_FUNC_INFO << QThread::currentThread();

    //Qt does not support this!!!
    //QMetaEnum metaEnum = QMetaEnum::fromType<QSslSocket::SslMode>();
    //qDebug() << "SSL Mode: " << metaEnum.valueToKey(mode);
    qInfo() << "SslMode: " << mode;
}

void TcpClient::peerVerifyError(const QSslError &error)
{
    qInfo() << Q_FUNC_INFO << QThread::currentThread();

    qInfo() << "Peer Verify Error: " << error.errorString();
}

void TcpClient::preSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator *authenticator)
{
    qInfo() << Q_FUNC_INFO << QThread::currentThread();

    qInfo() << "PreShared key  required!";
    //QSslPreSharedKeyAuthenticator lookup if needed
}

void TcpClient::sslErrors(const QList<QSslError> &errors)
{
    qInfo() << Q_FUNC_INFO << QThread::currentThread();

    //socket.ignoreSslErrors(); //NOT SECURE!!!
    qInfo() << "SSL Errors";
    foreach (QSslError error, errors) {
        qInfo() << error.errorString();
    }
}

// Setter/Getter
QString TcpClient::getHostname() const{
    return m_hostname;
}

void TcpClient::setHostname(const QString &newHostname){
    m_hostname = newHostname;
}

quint16 TcpClient::getPort() const{
    return m_port;
}

void TcpClient::setPort(quint16 newPort){
    m_port = newPort;
}
