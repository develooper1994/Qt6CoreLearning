#include <QCoreApplication>
#include <QStringList>

#include "test.h"

void QList_Method(){
    // QVector is alias to QList
    QList<int> li;
    li << 1 << 2 << 3;

    for (int var = 0; var < 4; ++var) {
        li.append(var);
    }

    qInfo() << li;
    qInfo() << "Capacity: " << li.capacity();
    qInfo() << "Size: " << li.size();
    qInfo() << "Length: " << li.length();
    qInfo() << "Count: " << li.count();
    qInfo() << "Count(3): " << li.count(3);
    qInfo() << "Count(4): " << li.count(4);

    foreach (int element, li) {
        qInfo() << element;
    }
    li.removeAt(3);

    for (int idx = 0; idx < li.length(); ++idx) {
        qInfo() << idx << "=" << li.at(idx);
    }
    QList<int> slice = li.sliced(2,3);

    qInfo() << "Slice: " << slice;
    qInfo() << "List: " << li;

    qInfo() << "Contains: " << li.contains(3);
    qInfo() << "Indexof(3): " << li.indexOf(3);
    qInfo() << "Indexof(6): " << li.indexOf(6);

    li.clear();

    qInfo() << "Length: " << li.length();
    qInfo() << "Size: " << li.size();
    qInfo() << "Count: " << li.count();
}

void QSet_Method(){
    // fast lookup
    // QSet uses QHash under the hood
    QSet<QString> people;
    people << "Mustafa" << "Selçuk" << "Çağlar";
    people.insert("Prof");

    // Notice it is out of order
    foreach (QString person, people) {
        qInfo() << person;
    }

    //VERY VERY FAST!!
    qInfo() << "Prof in collection?" << people.contains("Prof");
}

void QMap_Method(){
    QMap<QString,int> person_age;
    person_age.insert("Mustafa", 21);
    person_age.insert("Selçuk", 22);
    person_age.insert("Çağlar", 23);
    person_age.insert("Rango", 24);

    qInfo() << "Keys: " << person_age.keys();
    qInfo() << "Values: " << person_age.values();
    qInfo() << "Rango is " << person_age["Rango"] << "age years old";

    person_age.remove("Rango");
    qInfo() << "Contains: " << person_age.contains("Rango");

    foreach (QString key, person_age.keys()) {
        qInfo() << key << "=" << person_age[key];
    }

    // if you cannot find then put a placeholder
    qInfo() << "Test: " << person_age.value("Test", -1);

    person_age["Ahmet"] = 55;
    qInfo() << "Ahmet: " << person_age.value("Ahmet", -1);

    person_age["Test"] = 852;
    qInfo() << "Test: " << person_age.value("Test", -1);

}

void QStringList_Method(){
    // QStringList inherist QList<QString>
    // adds a bunch of useful methods
    QStringList names{"Mustafa"};
    names << "Selçuk";
    names.append("Çağlar");

    names.replaceInStrings("a", "@");
    qInfo() << names;
    names.replaceInStrings("@", "a");
    names.sort();

    QString people = names.join(",");
    qInfo() << people;

    QStringList myList = names.filter("r");
    qInfo() << myList;

    QString me = "Selçuk";
    qInfo() << "Contains: " << names.contains(me);
    qInfo() << "IndexOf: " << names.indexOf(me);
    qInfo() << "Value: " << names.at(names.indexOf(me));
}

void qDeleteAll_List(){
    // qDeleteAll with QList
    // QObjects can not be copied
    QList<Test*> list;
    for (int idx = 0; idx < 5; ++idx) {
        list.append(new Test(nullptr, "Test"+QString::number(idx)));
    }

    foreach (Test* test, list) {
        qInfo() << test;
    }

    qDeleteAll(list);
    //qInfo() << list.at(0); // dangling pointer
    list.clear();
    qInfo() << "Count: " << list.count();
}

void QShared_List(){
    QList<QSharedPointer<Test>> list;
    for (int idx = 0; idx < 5; ++idx) {
        QSharedPointer<Test> item(new Test(nullptr, "Test"+QString::number(idx)));
        list.append(item);
    }

    foreach (QSharedPointer<Test> element, list) {
        qInfo() <<  element.data();
    }

    //QSharedPointer deletes automatically
    qInfo() << "Remove one item.";
    list.removeAt(0);
    qInfo() << "Clearing";
    list.clear();
}

void qDeleteAll_QMap(){
    QMap<QString, Test*> map;
    for (int idx = 0; idx < 5; ++idx) {
        QString id = "ID-" + QString::number(idx);
        QString name = "Test-" + QString::number(idx);
        Test* test = new Test(nullptr, name);
        map.insert(id, test);
    }

    foreach (QString key, map.keys()) {
        qInfo() << key << map[key];
    }
    qDeleteAll(map);
    qInfo() << "Count before clear" << map.count();
    map.clear();
    qInfo() << "Count after clear" << map.count();
}

void QShared_QMap(){
    QMap<QString, QSharedPointer<Test>> map;

    for (int idx = 0; idx < 5; ++idx) {
        QString id = "Id-"+QString::number(idx);
        QString name = "Name-"+QString::number(idx);
        QSharedPointer<Test> test(new Test(nullptr, name));
        map.insert(id, test);
    }

    foreach (QString key, map.keys()) {
        qInfo() << key << map[key];
    }
    //QSharedPointer deletes automatically
    qInfo() << "Remove one item.";
    map.remove("Id-0");
    qInfo() << "Count before clear" << map.count();
    map.clear();
    qInfo() << "Count after clear" << map.count();
}

namespace assignment{
    QList<int> makeList(){
        QList<int> list;
        for (int var = 0; var < 20; ++var) {
            list.append(var*3);
        }
        return list;
    }

    void find(QList<int>& list, int value){
        int pos = list.indexOf(value);
        if (pos < 0) {
            qInfo() << "Item not found";
            return;
        }
        qInfo() << "At: " << pos << " Value: " << list.at(pos);
    }

    void main(){
        QList<int> list = makeList();
        qInfo() << list;
        find(list, 9);
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //QList_Method();
    //QSet_Method();
    //QMap_Method();
    //QStringList_Method();

    //qDeleteAll_List();
    //QShared_List();

    //qDeleteAll_QMap();
    //QShared_QMap();

    assignment::main();

    return a.exec();
}
