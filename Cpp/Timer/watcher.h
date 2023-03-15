#ifndef WATCHER_H
#define WATCHER_H

#include <QObject>
#include <QFileSystemWatcher>
#include <QDir>

class Watcher : public QObject
{
    Q_OBJECT
private:
    QFileSystemWatcher fwatcher;
public:
    explicit Watcher(QObject *parent = nullptr);

signals:

public slots:
    void OnFileChanged(const QString& path);
    void OnDirectoryChanged(const QString& path);
};

#endif // WATCHER_H
