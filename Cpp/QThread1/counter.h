#ifndef COUNTER_H
#define COUNTER_H

#include <QRunnable>
#include <QObject>
#include <QDebug>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QRandomGenerator>

class Counter : public QObject, public QRunnable
{
    Q_OBJECT
private:
    int* m_value;
    QMutex* m_mutex;
public:
    explicit Counter(QObject *parent = nullptr);
    Counter(QMutex* mutex, int* value);
    ~Counter();

    void run();

signals:

public slots:
    void start();
};

#endif // COUNTER_H
