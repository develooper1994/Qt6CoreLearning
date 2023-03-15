#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include <QObject>
#include <QDebug>
#include <QTextStream>>

class CommandLine : public QObject
{
    Q_OBJECT
private:
    QTextStream cin;
public:
    explicit CommandLine(QObject *parent = nullptr, FILE* fileHandler = nullptr);

    [[noreturn]] void monitor(); // [[noreturn]] is optional. modern c++ thing.

signals:
    void command(QString value);

};

#endif // COMMANDLINE_H
