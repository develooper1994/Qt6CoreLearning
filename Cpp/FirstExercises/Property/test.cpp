#include "test.h"

Test::Test(QObject *parent)
    : QObject{parent}
{

}

void Test::setMessage(const QString &message)
{
    m_message = message;
    emit messageChanged(message);
}

QString Test::message() const
{
    return m_message;
}
