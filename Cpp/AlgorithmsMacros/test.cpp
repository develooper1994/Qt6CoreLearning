#include "test.h"
#include "qdebug.h"

Test::Test(QObject *parent)
    : QObject{parent}
{
    qInfo() << "Constructed";
}

Test::~Test(){
    qInfo() << "Deconstructed";
}
