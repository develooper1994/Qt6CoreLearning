#ifndef TEST_H
#define TEST_H

#include <QObject>

class Test : public QObject
{
    Q_OBJECT
public:
    explicit Test(QObject *parent = nullptr, QString name="");
    ~Test();
signals:

};

#endif // TEST_H
