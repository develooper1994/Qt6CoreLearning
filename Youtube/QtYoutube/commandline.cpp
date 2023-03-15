#include "commandline.h"

Commandline::Commandline(QObject *parent, FILE *handler)
    : QObject{parent}, stream{handler}
{

}

void Commandline::monitor()
{
    while (true) {
        QString value = stream.readLine();
        emit command(value);
    }
}
