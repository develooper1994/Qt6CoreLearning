// "pulling mechanizm" or "a wild sleeper"

#include "widget.h"

Widget::Widget(QObject *parent)
    : QObject{parent}
{
    m_isSender = false;
}

void Widget::workStarted()
{
    qInfo() << this << "Started" << sender();
}

void Widget::workFinished()
{
    qInfo() << this << "Finished" << sender();
    m_isActive = false;
}

void Widget::workReady(int i)
{
    qInfo() << this << "Ready" << i << sender();
}

void Widget::run()
{
    if (m_isSender) {
        // region for producer
        // produce work
        emit started();

        for (int i = 0; i < 10; ++i) {
            qInfo() << this << "Updating";
            emit update(i);
            QThread::currentThread()->msleep(500);
        }

        qInfo() << this << "finished";
        emit finished();
    } else {
        // region for consumer
        // don't produce work
        qInfo() << this << "waiting for signal";
        m_isActive = true;
        while (m_isActive) {
            QThread::currentThread()->msleep(100);
        }
    }

    qInfo() << this << "done";
}

bool Widget::isSender() const
{
    return m_isSender;
}

void Widget::setIsSender(bool newIsSender)
{
    m_isSender = newIsSender;
}
