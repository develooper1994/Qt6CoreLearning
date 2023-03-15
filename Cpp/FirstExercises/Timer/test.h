#ifndef TEST_H
#define TEST_H

#include <QObject>
#include <QDebug>
#include <QTimer>

class Test : public QObject
{
    Q_OBJECT
private:
    QTimer m_timer;
    int count;
public:
    explicit Test(QObject *parent = nullptr);

signals:

public slots:
    void timeout();
};

#endif // TEST_H
