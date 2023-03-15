#include "counter.h"

Counter::Counter(QObject *parent) : QObject(parent)
{
    qInfo() << this << "Constructed";
}

Counter::Counter(QMutex *mutex, int *value)
{
    m_mutex = mutex;
    m_value = value;
}

Counter::~Counter()
{
    qInfo() << this << "Deconstructed";
}

void Counter::start()
{
    for(int i = 0; i < 20; i++)
    {
        qInfo() << QThread::currentThread()->objectName() << " = " << i;
        unsigned long value = static_cast<unsigned long>(QRandomGenerator::global()->bounded(500));
        QThread::currentThread()->msleep(value);
        qInfo() << QThread::currentThread()->objectName() << " Complete";
    }

    qInfo() << this << "Completed!";
    deleteLater();

    QThread::currentThread()->quit();
}

/*
// threadpool without mutex
void Counter::run()
{
    qInfo() << "Starting" << QThread::currentThread();

    for (int i = 0; i < 20; ++i) {
        qInfo() << QThread::currentThread()->objectName() << " = " << i;
        unsigned long value = static_cast<unsigned long>(QRandomGenerator::global()->bounded(500));
        QThread::currentThread()->msleep(value);
        qInfo() << QThread::currentThread()->objectName() << " Complete";
    }

    qInfo() << "Finished" << QThread::currentThread();
}
*/

// Mutex
void Counter::run()
{
    //Wait until we can do stuff...

    //m_mutex->lock(); //We will wait until we can lock!
    QMutexLocker locker(m_mutex); // auto lock and unlock. (smart pointer for mutex)
    //Our code here
    QThread::currentThread()->msleep(5);
    *m_value = *m_value + 1;
    qInfo() << *m_value << QThread::currentThread();

    //m_mutex->unlock(); //Other threads can lock
}
