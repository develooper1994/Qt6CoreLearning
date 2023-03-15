#include "racecar.h"
#include <QDebug>

racecar::racecar(QObject *parent)
    : car(parent)
{

}

void racecar::gofast()
{
    qInfo() << "zooooooom";
}
