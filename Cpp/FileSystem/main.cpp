#include <QCoreApplication>
#include <QDebug>

#include <QDateTime>

#include <QIODevice>
#include <QBuffer>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>

#include <QStorageInfo>

#include <QLockFile>

void QIODevice_Method(){
    // QBuffer is an interface to QIODevice
    QBuffer buffer;
    if (buffer.open(QIODevice::ReadWrite)) {
        qInfo() << "Writing to device";

        QByteArray data("What am i doing in here ?");
        for (int i = 0; i < 5; ++i) {
            buffer.write(data);
            buffer.write("\r\n");
        }
        //buffer.flush();

        buffer.seek(0);
        qInfo() << buffer.readAll();
        buffer.close();
    }
    else {
        qInfo() << "Could not open the device";
    }
    qInfo() << "Finished";
}

void File_Method(){
    QString filename = "test.txt";
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly)) {
        QString now = QDateTime::currentDateTime().toString();

        QByteArray data;
        data.append(now.toLocal8Bit());
        data.append(" - ");
        data.append("Qt c++ programing");

        file.write(data);

        file.close(); // automatically calls flush
    }
    else{
        qInfo() << file.errorString();
    }
}

namespace QTextStreamWriteRead{
void write(QFile& file){
    if (!file.isWritable()) {
        qInfo() << "cannot write to file!";
        return;
    }

    QTextStream stream(&file);
    stream.setEncoding(QStringConverter::LastEncoding);

    stream.seek(file.size()); // append
    for (int idx = 0; idx < 5; ++idx) {
        stream << QString::number(idx) << "-item\r\n";
    }
    stream.flush();
    qInfo() << "File written";
}
void read(QFile& file){
    if (!file.isReadable()) {
        qInfo() << "cannot read the file";
        return;
    }

    QTextStream stream(&file);
    stream.seek(0);
    while (!stream.atEnd()) {
        qInfo() << stream.readLine();
    }
}
void main(){
    QString filename = "test.txt";
    QFile file(filename);
    if (file.open(QIODevice::OpenModeFlag::ReadWrite)) {
        QTextStream stream(&file);
        qInfo() << "Encoding:" << stream.encoding();

        write(file);
        read(file);

        file.close();
    }
    else
        qInfo() << file.errorString();
}
}

namespace QDataStreamWriteRead{
bool write(QString filename)
{
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly))
    {
        qInfo() << file.errorString();
        return false;
    }

    QDataStream stream(&file);
    stream.setVersion(QDataStream::Qt_6_0);

    int age = 45;
    QString name = "Bryan";
    double weight = 185.06;

    qInfo() << "Writing file";

    stream << age << name << weight;
    if(!file.flush())
    {
        qInfo() << file.errorString();
        file.close();
        return false;
    }

    qInfo() << "Closing file";

    file.close();

    return true;
}

bool read(QString filename)
{
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly)){
        qInfo() << file.errorString();
        return false;
    }


    QDataStream stream(&file);
    if(stream.version() != QDataStream::Qt_6_0){
        qInfo() << "Wrong file version";
        file.close();
        return false;
    }

    int age;
    QString name;
    double weight;

    stream >> age >> name >> weight;

    qInfo() << "Name" << name;
    qInfo() << "Age" << age;
    qInfo() << "Weight" << weight;


    return true;
}
void main(){
QString filename = "test.dat";

if(write(filename)) read(filename);
}
}

namespace Directories{
    void list(QDir& root){
        qInfo() << "--Listing--";
        foreach (QFileInfo fi, root.entryInfoList(QDir::Filter::Dirs, QDir::Name))
            if (fi.isDir())
                qInfo() << fi.absoluteFilePath();
    }
    void listRecursive(QDir& root){
        qInfo() << "--Listing--";
        foreach (QFileInfo fi, root.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot, QDir::Name)) {
            if(fi.isFile()) qInfo() << "File: " << fi.filePath();
            if(fi.isDir()){
                qInfo() << "Dir: " << fi.filePath();
                QDir child(fi.filePath());
                qInfo() << "Inspecting: " << child.absolutePath();
                listRecursive(child);
            }
        }
    }
    void getFileInfo(QString path){
        qInfo() << "List:" << path;

        QDir dir(path);
        QFileInfoList dirs = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
        QFileInfoList files = dir.entryInfoList(QDir::Files);

        qInfo() << "Name"
                << "Size"
                << "Created"
                << "Modified"
                << "Absolute File Path";
        foreach (QFileInfo fi, dirs) {
            qInfo() << fi.fileName() << "||"
                    << fi.size() << "||"
                    << fi.birthTime() << "||"
                    << fi.lastModified() << "||"
                    << fi.absoluteFilePath();
        }
    }

    void main(){
        QDir dir(QCoreApplication::applicationDirPath());
        qInfo() << dir.dirName();
        qInfo() << dir.absolutePath();

        dir.cdUp(); // "cd .."
        qInfo() << dir.absolutePath(); // "pwd"
        list(dir); // "ls"

        dir.mkdir("test"); // "mkdir test"
        list(dir); // "ls"

        dir.rmdir("test");  // "rmdir test"
        list(dir); // "ls"

        qInfo() << "----recursive list----";
        dir.cdUp();
        listRecursive(dir);

        qInfo() << "----Get File Info----";
        getFileInfo(QCoreApplication::applicationDirPath());
    }
}

namespace Storage{
    void main(){
        QStorageInfo root = QStorageInfo::root();
        qInfo() << "Root: " << root.rootPath();
        QDir dir(root.rootPath());
        foreach (QFileInfo fi, dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot))
            qInfo() << fi.filePath();

        qInfo() << "------";

        qInfo() << "Name" << "Type" << "Total" << "Available" << "Device" << "Root" << "\n";
        foreach (QStorageInfo si, QStorageInfo::mountedVolumes()) {
            qInfo() << si.displayName() << "||"
                    << si.fileSystemType() << "||"
                    << si.bytesTotal() << "||"
                    << si.bytesAvailable() << "||"
                    << si.device() << "||"
                    << si.isRoot()
                    << "\n";
        }
    }
}

namespace LockFile{
    void main(){

        qInfo() << "Attempting to lock a file...";

        QString path = QDir::currentPath() + QDir::separator() + "test.txt";
        QFile file(path);
        QLockFile lock(file.fileName()+"l");
        lock.setStaleLockTime(10000);

        if (lock.tryLock()) {
            qInfo() << "Putting into file...";
            if (file.open(QIODevice::WriteOnly)) {
                QByteArray data;
                data.append("Bana da mı gazoz");
                file.write(data);
                file.flush();
                qInfo() << "Finished with file...";
                // took over N seconds here, auto unlock.
            }
            qInfo() << "Unloking file.";
            lock.unlock();
        }
        else {
            qInfo() << "Could not lock the file!";
            qint64 pid;
            QString host;
            QString application;
            if (lock.getLockInfo(&pid, &host, &application)) {
                qInfo() << "The file is locked by: ";
                qInfo() << "pid: " << pid;
                qInfo() << "host: " << host;
                qInfo() << "application: " << application;
            } else
                qInfo() << "File is locked, but we can't get the info!";
        }

    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //QIODevice_Method();
    //File_Method();
    //QTextStreamWriteRead::main();
    //QDataStreamWriteRead::main();
    //Directories::main();
    //Storage::main();
    LockFile::main();

    return a.exec();
}
