#include <QCoreApplication>
#include <QThread>
#include <QDebug>

#include "worker.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QThread::currentThread()->setObjectName("Main Thread");

    qInfo() << "Starting" << QThread::currentThread();
    qInfo() << "Doing Stuff" << QThread::currentThread();
    //Worker worker;
    //worker->run();

    //worker thread
    Worker* worker = new Worker(); // !!!No Parent!!!
    QThread thread;
    thread.setObjectName("Worker Thread");
    worker->moveToThread(&thread);
    // we need to tell the worker when the worker start
    QObject::connect(&thread,&QThread::started,
                     worker,&Worker::run);
    thread.start();

    //main thread
    qInfo() << "Doing stuff" << QThread::currentThread();
    for (int var = 0; var < 10; ++var) {
        qInfo() << "Working" << QString::number(var) << QThread::currentThread();
        QThread::currentThread()->msleep(500); // I want to track
    }

    qInfo() << "Finished" << QThread::currentThread();

    return a.exec();
}
