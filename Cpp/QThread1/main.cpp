#include <QCoreApplication>
#include <QDebug>
#include <QThread>
#include <QThreadPool>
#include <QTimer>

#include <QMutex>
#include <QSemaphore>
#include <QWaitCondition>

#include "test.h"
#include "worker.h"
#include "counter.h"
#include "manager.h"
#include "workersemaphores.h"
#include "consumer.h"
#include "producer.h"
#include "widget.h"


namespace threadingInfo{
    void test(){
        qInfo() << "Teeeeest" << QThread::currentThread();
    }
    void main(){
        QThread::currentThread()->setObjectName("MainThread");
        qInfo() << "Starting" << QThread::currentThread();
        test();
        qInfo() << "Finishing" << QThread::currentThread();
    }
}

namespace stillSingleThread{
    void test(){
            qInfo() << "Teeeeest" << QThread::currentThread();
    }
    void main(){
        QThread::currentThread()->setObjectName("MainThread");
        qInfo() << "Starting" << QThread::currentThread();
        QTimer timer;
        timer.singleShot(3000, &test);
        qInfo() << "Finishing" << QThread::currentThread();
    }
}

namespace timerInThread{
    void main(){
        // QEventLoop needed to keep QThread alive
        QScopedPointer<QEventLoop> loop;
        QThread::currentThread()->setObjectName("Main Thread");
        qInfo() << "Starting" << QThread::currentThread();

        QThread thread;
        thread.setObjectName("Worker Thread");

        Test test;
        test.moveToThread(&thread);
        QObject::connect(&thread,&QThread::started,&test,&Test::run);
        QObject::connect(&thread,&QThread::finished,loop.data(),&QEventLoop::quit, Qt::ConnectionType::QueuedConnection);
        thread.start();

        qInfo() << "Finished" << QThread::currentThread();
        loop->exec();
    }
}

namespace threadWithWorker{
    void main(){
        // You should use it between "QCoreApplication a" and "a.exec" otherwise it will not delete resources and crashes.
        // there is the threadpool object for
        QEventLoop loop;

        Manager manager;
        manager.start();
        QThread::currentThread()->sleep(20);

        loop.exec();
    }
}

namespace threadpool{
/*
  What
  Thread Pools

  Why
  We want multiple resuable threads

  How
  QTreadPool and QRunnable
 */
    void main(){
        QThread::currentThread()->setObjectName("Main Thread");

        QThreadPool* pool = QThreadPool::globalInstance();
        qInfo() << "Max Thread: " << pool->maxThreadCount();

        for(int i = 0; i < 100; i++){
            Counter* c = new Counter();
            c->setAutoDelete(true);
            pool->start(c);
        }
    }
}

namespace mutex{
/*
  What
  Mutex

  Why
  We want to protect an object from thread issues

  How
  QMutex

  Two threads updating the same thing at the same time
 */
    void main(){
        QEventLoop loop;

        // You should use these code inside eventloop otherwise use inside QCoreapplication loop
        QThread::currentThread()->setObjectName("Main");
        QThreadPool* pool = QThreadPool::globalInstance();

        QMutex mutex;
        int value = 0;
        qInfo() << pool->maxThreadCount() << " Threads";

        for(int i = 0; i < 100; i++)
        {
            Counter* c = new Counter(&mutex,&value);
            c->setAutoDelete(true);
            pool->start(c);
        }

        loop.exec();
    }
}

namespace Semaphores{
    // mutex with steoids :). we can lock(qcquire) and unlock(release) multiple value at the same instead of one by one
    // not a best option.
    void main(){
        QEventLoop loop;

        // You should use these code inside eventloop otherwise use inside QCoreapplication loop
        QList<int> values;
        values.resize(5);
        values.fill(0);

        QSemaphore sema(values.size());
        qInfo() << values;

        QThread thread1, thread2;
        thread1.setObjectName("Thread 1");
        thread2.setObjectName("Thread 2");

        WorkerSemaphores worker1(&sema,&values), worker2(&sema,&values);
        worker1.moveToThread(&thread1);
        worker2.moveToThread(&thread2);

        QObject::connect(&thread1,&QThread::started,&worker1,&WorkerSemaphores::run);
        QObject::connect(&thread2,&QThread::started,&worker2,&WorkerSemaphores::run);

        thread1.start();
        thread2.start();

        loop.exec();
    }
}

namespace WaitCondition{
// Producer/Consumer design pattern over simplified
// No body knows who is going to run first except operation system scheduler.
    void main(){
        QEventLoop loop;

        //code
        QThread consumerThread, producerThread;

        QMutex mutex;
        QWaitCondition valueReady;
        int value = 0;

        Consumer* consumer = new Consumer(&mutex,&valueReady,&value);
        Producer* producer = new Producer(&mutex,&valueReady,&value);

        producer->moveToThread(&producerThread);
        consumer->moveToThread(&consumerThread);

        QObject::connect(&producerThread,&QThread::started,producer,&Producer::run);
        QObject::connect(&consumerThread,&QThread::started,consumer,&Consumer::run);

        producerThread.start();
        consumerThread.start();

        loop.exec();
    }
}

namespace signalSlot{
/*
    What
    Signals and slots across threads

    Why
    Some minor complications

    How
    Qt Signal Types
*/
// "pulling mechanizm" or "a wild sleeper"
    void main(){
        QEventLoop loop;

        Widget* producer = new Widget();
        Widget* consumer = new Widget();

        producer->setObjectName("Producer");
        consumer->setObjectName("Consumer");
        producer->setIsSender(true);
        // I am paranoid and it is future proof :)
        producer->setAutoDelete(true);
        consumer->setAutoDelete(true);

        QObject::connect(producer,&Widget::started,
                         consumer,&Widget::workStarted,
                         Qt::ConnectionType::QueuedConnection);
        QObject::connect(producer,&Widget::update,
                         consumer,&Widget::workReady,
                         Qt::ConnectionType::QueuedConnection);
        QObject::connect(producer,&Widget::finished,
                         consumer,&Widget::workFinished,
                         Qt::ConnectionType::QueuedConnection);

        QThreadPool::globalInstance()->start(producer);
        QThreadPool::globalInstance()->start(consumer);

        loop.exec();
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //threadingInfo::main();
    //stillSingleThread::main();
    //timerInThread::main();
    //threadWithWorker::main();
    //threadpool::main();
    //mutex::main();
    //Semaphores::main();
    //WaitCondition::main();
    signalSlot::main();

    return a.exec();
}
