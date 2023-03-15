#ifndef WIDGET_H
#define WIDGET_H

#include <QObject>
#include <QRunnable>
#include <QThread>
#include <QDebug>

// "pulling mechanizm" or "a wild sleeper"

class Widget : public QObject, public QRunnable
{
    Q_OBJECT
private:
    bool m_isSender;
    bool m_isActive;

public:
    explicit Widget(QObject *parent = nullptr);

public slots:
    void workStarted();
    void workFinished();
    void workReady(int i);

public:
    void run(); // QRunnable interface
    bool isSender() const;
    void setIsSender(bool newIsSender);

signals:
    void started();
    void finished();
    void update(int i);
};

#endif // WIDGET_H
