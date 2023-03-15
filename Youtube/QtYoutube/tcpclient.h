#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QDebug>
#include <QEventLoop>
#include <QAbstractSocket>
#include <QTcpSocket>
#include <QMetaEnum>
#include <QNetworkProxy>
#include <QSslSocket>
#include <QSslPreSharedKeyAuthenticator>
#include <QThread>

class TcpClient : public QObject
{
    Q_OBJECT
private:
    //QTcpSocket socket; // unencrypted
    QSslSocket* socket; // encrypted
    QEventLoop* loop;
    QString m_hostname;
    quint16 m_port;
public:
    explicit TcpClient(QObject *parent = nullptr);
    ~TcpClient();

    QString getHostname() const;
    void setHostname(const QString &newHostname);

    quint16 getPort() const;
    void setPort(quint16 newPort);

public slots:
    // thread start
    void run();

    void connectToHost(QString hostname, quint16 port);
    void disconnect();

private slots:
    void connecting();
    void disconnecting();
    void error(QAbstractSocket::SocketError socketError);
    void stateChanging(QAbstractSocket::SocketState socketState);
    void readyReady();
    //SSL
    void encrypted(); // establish encrypted tunnel
    void encryptedBytesWritten(qint64 written); // write bytes to encrypted tunnel
    void modeChanged(QSslSocket::SslMode mode); //
    void peerVerifyError(const QSslError& error); // verify who are talking with you. Is this really person that you are talking to?
    void preSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator* authenticator); //
    void sslErrors(const QList<QSslError>& errors); // there are 38 errors.

signals:
};

#endif // TCPCLIENT_H
