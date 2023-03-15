#include "logger.h"

QString Logger::filename = QDir::currentPath() + QDir::separator() + "log.txt";
bool Logger::logging = false;
static const QtMessageHandler QT_DEFAULT_MESSAGE_HANDLER = qInstallMessageHandler(nullptr);

Logger::Logger(QObject *parent)
    : QObject{parent}
{

}

void Logger::attach()
{
    logging = true;
    qInstallMessageHandler(Logger::handler);
}

void Logger::handler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString txt;
    if (Logger::logging) {
        switch (type) {
        case QtInfoMsg:
            txt = QString("Info: %1").arg(msg);
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
            break;
        }
    }

    QFile file(Logger::filename);
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << QDateTime::currentDateTime().toString() << " - " << txt << context.file << "line: " << context.line << "\r\n";
        file.flush(); // not neccesary. close do the same thing.
        file.close();
    }

    (*QT_DEFAULT_MESSAGE_HANDLER)(type, context, msg);
}
