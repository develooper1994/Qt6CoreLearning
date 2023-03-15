#include "watcher.h"
#include <qdebug.h>

Watcher::Watcher(QObject *parent)
    : QObject{parent}
{

}

void Watcher::messageChanged(QString message)
{
    qInfo() << message;
}
