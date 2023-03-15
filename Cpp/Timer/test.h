#ifndef TEST_H
#define TEST_H

#include <QObject>
#include <QTimer>

class Test : public QObject
{
    Q_OBJECT
private:
    QTimer timer;
    int number;
public:
    explicit Test(QObject *parent = nullptr);

signals:

public slots:
    void timeout();
    void doTheThing();
};

#endif // TEST_H
