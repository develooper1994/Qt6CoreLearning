/*
  What
  Intercepting QDebug messages

  Why
  We want to catch ANY Qdebug message

  How
  QtMessageHandler
 */

#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include <QTextStream>

#include "logger.h"
#include "test.h"

namespace QDebugMessageHandle1{
    const QtMessageHandler QT_DEFAULT_MESSAGEHANDLER = qInstallMessageHandler(nullptr);


    void lhandler(QtMsgType type, const QMessageLogContext& context, const QString& msg){
    QString txt;
    switch (type) {
    case QtInfoMsg:
        txt = QString("Info : %1").arg(msg);
        break;
    case QtDebugMsg:
        txt = QString("Debug: %1").arg(msg);
        break;
    case QtWarningMsg:
        txt = QString("Warning: %1").arg(msg);
        break;
    case QtCriticalMsg:
        txt = QString("Critical: %1").arg(msg);
        break;
    case QtFatalMsg:
        txt = QString("Fatal: %1").arg(msg);
        break;
    default:
        txt = QString("Unknown: %1").arg(msg);
        break;
        QFile file("log.txt");
        if (file.open(QIODevice::Append)) {
            QTextStream stream(&file);
            stream << QDateTime::currentDateTime().toString() << " - " << txt << "file: " << context.file << "line: " << context.line << "\r\n";
            stream.flush();
            file.close();
        }
        (*QT_DEFAULT_MESSAGEHANDLER)(type, context, msg);
    }
    }

    void main(){
        qInstallMessageHandler(lhandler);
        qInfo() << "This is a info message";
        qDebug() << "This is a debug message";
        qWarning() << "This is a warning message";
        qCritical() << "This is a critical message";
        qFatal("THIS IS SPARTA!!!");
    }
}

namespace MyLogger{
    void main(){
        qInfo() << "File: " << Logger::filename;
        Logger::attach();

        qInfo() << "test";

        Logger::logging = false;
        qInfo() << "This will get logged";
        Logger::logging = true;

        Test t;
        t.testing();

        qInfo() << "Finished";
    }
}

namspace EasyLogging{


}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //QDebugMessageHandle1::main();
    MyLogger::main();

    return a.exec();
}
