#include "dog.h"
#include <QDebug>

Dog::Dog(QObject *parent)
    : QObject{parent}
{

}

void Dog::treats()
{
    qInfo() << "Bark!";
}
