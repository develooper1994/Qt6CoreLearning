#ifndef OWNER_H
#define OWNER_H

#include <QObject>


class Owner : public QObject
{
    Q_OBJECT
public:
    explicit Owner(QObject *parent = nullptr);
    void giveSnaks();

signals:
    void treats();

};

#endif // OWNER_H
