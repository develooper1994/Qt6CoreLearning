#include <QCoreApplication>

#include "worker.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Worker producer;
    Worker consumer;

    QThread pThread;
    QThread cThread;

    producer.moveToThread(&pThread);
    consumer.moveToThread(&cThread);

    producer.setProducer(true);

    pThread.setObjectName("Producer Thread");
    cThread.setObjectName("Consumer Thread");
    producer.setObjectName("Producer");
    consumer.setObjectName("Consumer");

    //connect threads
    QObject::connect(&pThread,&QThread::started,
                     &producer,&Worker::start);
    QObject::connect(&cThread,&QThread::started,
                     &consumer,&Worker::start);
    //connect producer produced->consumer consume
    QObject::connect(&producer,&Worker::produced,
                     &consumer,&Worker::consume);
    //connect producer finished->consumer quit
    QObject::connect(&producer,&Worker::finished,
                     &consumer,&Worker::quit);

    cThread.start();
    pThread.start();


    return a.exec();
}
