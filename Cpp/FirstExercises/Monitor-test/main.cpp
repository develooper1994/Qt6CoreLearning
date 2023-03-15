#include <QCoreApplication>

#include "test.h"
#include "monitor.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Test obj;
    Monitor monitor;

    obj.connect(&obj,&Test::close,&monitor,&Monitor::closing);
    QObject::connect(&obj,&Test::close, &a,&QCoreApplication::quit, Qt::QueuedConnection);


    qInfo() << "Closing application...";
    obj.kill();

    int exit_code = a.exec();
    qInfo() << "exit code: " << exit_code;

    return exit_code;
}
