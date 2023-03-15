#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include <QObject>
#include <QDebug>
#include <QTextStream>

class Commandline : public QObject
{
    Q_OBJECT
private:
    QTextStream stream;
public:
    explicit Commandline(QObject *parent = nullptr, FILE* handler=nullptr);

public slots:
    void monitor();

signals:
    void command(QString value);
};

#endif // COMMANDLINE_H
