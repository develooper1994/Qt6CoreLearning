#include "worker.h"

Worker::Worker(QObject *parent)
    : QObject{parent}
{
    qInfo() << this << "Constructed";
}

Worker::~Worker()
{
    qInfo() << this << "Deconstructed";
}

void Worker::work()
{
    for (int i = 0; i < 10; ++i) {
        qInfo() << QThread::currentThread() << " = " << i;
        QThread::currentThread()->msleep(100);
        qInfo() << QThread::currentThread()->objectName() << " Complete";
    }
    qInfo() << this << "Complete!";
    deleteLater();

    QThread::currentThread()->quit();
}
