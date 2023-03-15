#include "worker.h"

Worker::Worker(QObject *parent)
    : QObject{parent}
{
    qInfo() << this << "Constructed" << QThread::currentThread();
}

Worker::~Worker()
{
    qInfo() << this << "Deconstructed" << QThread::currentThread();
}

void Worker::run()
{
    for (int var = 0; var < 10; ++var) {
        qInfo() << this << "Working" << QString::number(var) << QThread::currentThread();
        QThread::currentThread()->msleep(500); // I want to track
    }

    this->deleteLater();
}
