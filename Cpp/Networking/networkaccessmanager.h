#ifndef NETWORKACCESSMANAGER_H
#define NETWORKACCESSMANAGER_H

#include <QObject>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QAuthenticator>
#include <QNetworkProxy>
#include <QFile>


class NetworkAccessManager : public QObject
{
    Q_OBJECT
private:
    QNetworkAccessManager* manager;
    QFile* file;
    void wire(QNetworkReply* reply);
public:
    explicit NetworkAccessManager(QObject *parent = nullptr);

public slots:
    void get(QString hostname);
    void post(QString hostname, QByteArray data);

    // They will change FTP module. It is not working with qt6 without qt5compat module.
    void downlaod(QString hostname, QString path);
    void upload(QString hostname, QString path, QByteArray data);

private slots:
    void readyRead();
    void authenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator);
    void encrypted(QNetworkReply *reply);
    void finished(QNetworkReply *reply);
    void preSharedKeyAuthenticationRequired(QNetworkReply *reply, QSslPreSharedKeyAuthenticator *authenticator);
    void proxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *authenticator);
    void sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    // FTP
    void errorOccurred(QNetworkReply::NetworkError code);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void uploadProgress(qint64 bytesSent, qint64 bytesTotal);

signals:
};

#endif // NETWORKACCESSMANAGER_H
