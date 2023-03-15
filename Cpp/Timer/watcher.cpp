#include "watcher.h"

Watcher::Watcher(QObject *parent)
    : QObject{parent}
{
    fwatcher.addPath(QDir::currentPath());
    fwatcher.addPath(QDir::currentPath() + QDir::separator() + "test.txt");

    connect(&fwatcher,&QFileSystemWatcher::fileChanged, this, &::Watcher::OnFileChanged);
    connect(&fwatcher,&QFileSystemWatcher::directoryChanged, this, &::Watcher::OnDirectoryChanged);
}

void Watcher::OnFileChanged(const QString &path){
    qInfo() << "File changed: " << path;
    if(fwatcher.files().length()){
        qInfo() << "Files that have changed:";
        foreach(QString file, fwatcher.files()){
            qInfo() << file;
        }
    }
}

void Watcher::OnDirectoryChanged(const QString &path){
    qInfo() << "Dir changed: " << path;
    if(fwatcher.directories().length()){
        qInfo() << "Dir that have changed:";
        foreach(QString dir, fwatcher.directories()){
            qInfo() << dir;
        }
    }
}
