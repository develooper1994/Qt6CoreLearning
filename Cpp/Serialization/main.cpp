#include <QCoreApplication>
#include "test.h"
#include "converter.h"

namespace Json{
    bool writeFile(QString path){
        Test t;
        t.fill();
        Converter::writeJson(&t, path);
        return EXIT_SUCCESS;
    }
    bool readFile(QString path){
        Test* t = Converter::readJson(path);
        if (!t) {
            qDebug() << "Object is not loadded successfully";
            return EXIT_FAILURE;
        }

        qInfo() << "name: " << t->name();
        qInfo() << "items";
        foreach (QString key, t->map().keys()) {
            qInfo() << key << " : " << t->map().value(key);
        }
        return EXIT_SUCCESS;
    }
    void main(){
        qInfo() << "-----Json-----";
        QString file = "converter.json";
        if (EXIT_FAILURE == writeFile(file)){
            qDebug() << "Write is not successful";
            return;
        }
        readFile(file);
    }
}

namespace Xml{
    bool writeFile(QString path){
        Test t;
        t.fill();
        Converter::writeXml(&t, path);
        return EXIT_SUCCESS;
    }
    bool readFile(QString path){
        Test* t = Converter::readXml(path);
        if (!t) {
            qDebug() << "Object is not loadded successfully";
            return EXIT_FAILURE;
        }

        qInfo() << "name: " << t->name();
        qInfo() << "items";
        foreach (QString key, t->map().keys()) {
            qInfo() << key << " : " << t->map().value(key);
        }
        return EXIT_SUCCESS;
    }
    void main(){
        qInfo() << "-----Xml-----";
        QString file = "converter.xml";
        if (EXIT_FAILURE == writeFile(file)){
            qDebug() << "Write is not successful";
            return;
        }
        readFile(file);
    }
}

namespace Assignment{
    void save(const QString& path, QList<int> data){
        QFile file(path);
        if (!file.open(QIODevice::OpenModeFlag::WriteOnly)) {
            qFatal("Cannot save the file");
        }

        QDataStream stream(&file);
        stream << data;

        file.close();
    }
    QList<int> load(const QString& path){
        QFile file(path);
        if (!file.open(QIODevice::OpenModeFlag::ReadOnly)) {
            qFatal("Cannot load the file");
        }

        QList<int> data;
        QDataStream stream(&file);
        stream >> data;

        file.close();
        return data;
    }
    void main(){
        QString filename = "Assignment.txt";

        QList<int> list;
        list << 1 << 2 << 3 << 4 << 5;

        save(filename, list);
        QList<int> data = load(filename);
        qInfo() << data;
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //Json::main();
    //Xml::main();
    Assignment::main();

    return a.exec();
}
