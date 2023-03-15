#ifndef PRODUCER_H
#define PRODUCER_H

#include <QObject>
#include <QMutex>
#include <QWaitCondition>
#include <QThread>
#include <QDebug>
#include <QRandomGenerator>

class Producer : public QObject
{
    Q_OBJECT
private:
    QMutex* m_mutex;
    QWaitCondition* m_valueReady;
    int* m_value;

public:
    explicit Producer(QMutex* mutex, QWaitCondition* valueReady, int* value, QObject *parent = nullptr);

public slots:
    void run();

signals:

};

#endif // PRODUCER_H
