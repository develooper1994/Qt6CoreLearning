#include "consumer.h"

Consumer::Consumer(QMutex *mutex, QWaitCondition *valueReady, int *value, QObject *parent)
{
    m_mutex = mutex;
    m_valueReady = valueReady;
    m_value = value;
    Q_UNUSED(parent);
}

void Consumer::run()
{
    qInfo() << "Consumer running";
    //QThread::currentThread()->msleep(20);
    m_mutex->lock();
    qInfo() << "Consumer locked mutex";
    if (*m_value == 0) {
        qInfo() << "Consumer is waiting on value for producer to produce a value";
        m_valueReady->wait(m_mutex); // unlocks mutex if condition has met.
    }

    qInfo() << "consuming";
    m_mutex->unlock();
}
