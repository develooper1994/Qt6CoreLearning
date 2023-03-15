#ifndef DOG_H
#define DOG_H

#include <QObject>

class Dog : public QObject
{
    Q_OBJECT
public:
    explicit Dog(QObject *parent = nullptr);

signals:

public slots:
    void treats();

};

#endif // DOG_H
