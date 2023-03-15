#include <QCoreApplication>
#include <QVariant>

#include "test.h"
#include "watcher.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Test test;
    Watcher watch;

    QObject::connect(&test,&Test::messageChanged, &watch,&Watcher::messageChanged);

    test.setProperty("message", QVariant("DOOM"));
    test.setMessage("testing");

    return a.exec();
}
