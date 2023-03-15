#ifndef CONSUMER_H
#define CONSUMER_H

#include <QObject>
#include <QWaitCondition>
#include <QMutex>
#include <QThread>
#include <QDebug>

class Consumer : public QObject
{
    Q_OBJECT
private:
    QMutex* m_mutex;
    QWaitCondition* m_valueReady;
    int* m_value;
public:
    explicit Consumer(QMutex *mutex, QWaitCondition *valueReady, int *value, QObject *parent = nullptr);

public slots:
    void run();

signals:

};

#endif // CONSUMER_H
