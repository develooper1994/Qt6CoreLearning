#include "networkaccessmanager.h"

NetworkAccessManager::NetworkAccessManager(QObject *parent)
    : QObject{parent}
{
    manager = new QNetworkAccessManager(this);
    file = new QFile(this);

    connect(manager,&QNetworkAccessManager::authenticationRequired,this,&NetworkAccessManager::authenticationRequired);
    connect(manager,&QNetworkAccessManager::encrypted,this,&NetworkAccessManager::encrypted);
    connect(manager,&QNetworkAccessManager::preSharedKeyAuthenticationRequired,this,&NetworkAccessManager::preSharedKeyAuthenticationRequired);
    connect(manager,&QNetworkAccessManager::proxyAuthenticationRequired,this,&NetworkAccessManager::proxyAuthenticationRequired);
    connect(manager,&QNetworkAccessManager::sslErrors,this,&NetworkAccessManager::sslErrors);
}

// public slots:
void NetworkAccessManager::get(QString hostname)
{
    // I can sent get request as many as i want
    qInfo() << "Getting from server with Get Request";
    QUrl url(hostname);
    QNetworkRequest request = QNetworkRequest(url);
    QNetworkReply* reply = manager->get(request);
    connect(reply,&QNetworkReply::readyRead,this,&NetworkAccessManager::readyRead);
}

void NetworkAccessManager::post(QString hostname, QByteArray data)
{
    // I can sent post request as many as i want
    qInfo() << "Getting from server with Post request";
    QUrl url(hostname);
    QNetworkRequest request = QNetworkRequest(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "test/plain"); //mime type

    QNetworkReply* reply = manager->post(request,data);
    connect(reply,&QNetworkReply::readyRead,this,&NetworkAccessManager::readyRead);
}

void NetworkAccessManager::downlaod(QString hostname, QString path)
{
    qInfo() << "Downloading: " << path;
    file->setFileName(path);
    if (!file->open(QIODevice::WriteOnly)) {
        qInfo() << file->errorString();
        return;
    }

    QUrl url(hostname);
    //url.setUserName("user");
    //url.setPassword("pass");
    //url.setPort(21); // standart ftp port

    QNetworkRequest request = QNetworkRequest(url);
    QNetworkReply* reply = manager->get(request);
    wire(reply);
}

void NetworkAccessManager::upload(QString hostname, QString path, QByteArray data)
{
    qInfo() << "Uploading: " << path;
    file->setFileName(path);
    if (!file->open(QIODevice::ReadOnly)) {
        qInfo() << file->errorString();
        return;
    }

    QUrl url(hostname);
    //url.setUserName("user");
    //url.setPassword("pass");
    //url.setPort(21); // standart ftp port

    QNetworkRequest request = QNetworkRequest(url);
    QNetworkReply* reply = manager->post(request,data);
    wire(reply);
}

// private slots:
void NetworkAccessManager::readyRead()
{
    qInfo() << "ReadyRead";
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (reply) {
        QByteArray data = reply->readAll();
        qInfo() << data;
        file->write(data);
    }

}

void NetworkAccessManager::authenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator)
{
    Q_UNUSED(reply);
    Q_UNUSED(authenticator);
    qInfo() << Q_FUNC_INFO;
}

void NetworkAccessManager::encrypted(QNetworkReply *reply)
{
    Q_UNUSED(reply);
    qInfo() << Q_FUNC_INFO;
}

void NetworkAccessManager::finished(QNetworkReply *reply)
{
    Q_UNUSED(reply);
    qInfo() << Q_FUNC_INFO;
    reply->deleteLater();
}

void NetworkAccessManager::preSharedKeyAuthenticationRequired(QNetworkReply *reply, QSslPreSharedKeyAuthenticator *authenticator)
{
    Q_UNUSED(reply);
    Q_UNUSED(authenticator);
    qInfo() << Q_FUNC_INFO;
}

void NetworkAccessManager::proxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *authenticator)
{
    Q_UNUSED(proxy);
    Q_UNUSED(authenticator);
    qInfo() << Q_FUNC_INFO;
}

void NetworkAccessManager::sslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
    Q_UNUSED(reply);
    Q_UNUSED(errors);
    qInfo() << Q_FUNC_INFO;
}

// FTP
void NetworkAccessManager::errorOccurred(QNetworkReply::NetworkError code)
{
    Q_UNUSED(code);
    qInfo() << Q_FUNC_INFO << "\nError: " << code;
}

void NetworkAccessManager::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    if (bytesReceived<=0) return;
    qInfo() << "Downloading Process: " << bytesReceived << " of " << bytesTotal;
}

void NetworkAccessManager::uploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    if (bytesSent<=0) return;
    qInfo() << "Uploading Process: " << bytesSent << " of " << bytesTotal;
}

void NetworkAccessManager::wire(QNetworkReply *reply)
{
    connect(reply,&QNetworkReply::readyRead,this,&NetworkAccessManager::readyRead);
    connect(reply,&QNetworkReply::downloadProgress,this,&::NetworkAccessManager::downloadProgress);
    connect(reply,&QNetworkReply::uploadProgress,this,&NetworkAccessManager::uploadProgress);
    connect(reply,&QNetworkReply::errorOccurred,this,&::NetworkAccessManager::errorOccurred);
}
