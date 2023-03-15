#include "test.h"

Test::Test(QObject *parent)
    : QObject{parent}
{
    connect(&m_timer,&QTimer::timeout,this,&Test::timeout);

    m_timer.setInterval(1000); // 1000 milisecond
    m_timer.start(); // start the timer
    count = 0;
}

void Test::timeout()
{
    qInfo() << "Test!" << count;
    count++;
    if (count>=3) {
        m_timer.stop(); // stop the timer
        qInfo() << "Done!";
    }
}
