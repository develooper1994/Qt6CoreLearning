#include <QCoreApplication>
#include <QTimer>

#include "test.h"
#include "watcher.h"

namespace singleShot{
    void shot(){
        static int counter = 0;
        qInfo() << "Mustafa Selçuk Çağlar. Counter: " << QString::number(counter);
    }
    void main(){
        qInfo() << "starting soon...";
        QTimer::singleShot(2000, &shot);
    }
}

namespace clockThicking{
    void main(){
        Test t;
        t.doTheThing();
    }
}

namespace watcher{
/*
  What
  File system watcher

  Why
  We want to know when something changes

  How
  See example
  QFileSystemWatcher
 */
    void main(){
        Watcher w;
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //singleShot::main();
    //clockThicking::main();
    watcher::main();

    return a.exec();
}
