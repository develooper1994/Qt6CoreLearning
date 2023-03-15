#include "monitor.h"
#include <QDebug>

Monitor::Monitor(QObject *parent)
    : QObject{parent}
{

}

void Monitor::closing()
{
    qInfo() << "Application is closing";
}
