#include "owner.h"
#include <QDebug>

Owner::Owner(QObject *parent)
    : QObject{parent}
{

}

void Owner::giveSnaks()
{
    qInfo() << "Sneaks are ready!";
    emit treats();
}
