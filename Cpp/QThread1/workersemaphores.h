#ifndef WORKERSEMAPHORES_H
#define WORKERSEMAPHORES_H

#include <QObject>
#include <QSemaphore>
#include <QList>
#include <QDebug>
#include <QThread>
#include <QRandomGenerator>

class WorkerSemaphores : public QObject
{
    Q_OBJECT
private:
    QSemaphore* m_sema;
    QList<int>* m_values;
public:
    explicit WorkerSemaphores(QSemaphore* sema, QList<int>* values, QObject *parent = nullptr);

public slots:
    void run();

signals:

};

#endif // WORKERSEMAPHORES_H
