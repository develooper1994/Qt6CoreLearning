#include <QCoreApplication>
#include <QRandomGenerator>
#include "test.h"

namespace DeleteAll{
    void main(){
        QList<Test*> list;
        // memory leak
        for (int i = 0; i < 10; ++i) {
            Test* t = new Test(); // no parent, no smart pointer
            t->setObjectName(QString::number(i));
            list.append(t);
        }
        qInfo() << list.at(0);
        qDeleteAll(list); // deconstruct all of them, deletes underlying memory
        qInfo() << "Count: " << list.count();

        //qInfo() << list.at(0); // It will crash
        qInfo() << "Clearing...";
        list.clear(); // doesn't deletes underlying memory. It just deletes pointers which they pointing to my data.

        qInfo() << "Count: " << list.count();

    }
}

namespace Algorithms{
    void fill(){
        QList<int> list;
        list <<1<<2<<3;

        //qFill(); // don't use
        list.fill(5);
        qInfo() << list;

        list.resize(12);
        std::fill(list.begin(), list.end(), 0);
        qInfo() << list;
    }
    void randomGenerate(QList<int>& list, int max){
        list.reserve(max);
        for (int i = 0; i < max; ++i) {
            int value = QRandomGenerator::global()->bounded(100);
            list.append(value);
        }
    }
    void sort(){
        QList<int> list;
        randomGenerate(list, 20);
        qInfo() << "unsorted: " << list;
        std::sort(list.begin(), list.end());
        qInfo() << "sorted: " << list;
        std::reverse(list.begin(), list.end());
        qInfo() << "reverse sorted: " << list;
    }
    void equal(){
        QList<int> list1;
        randomGenerate(list1,10);
        QList<int> list2;
        randomGenerate(list2,10);

        qInfo() << list1;
        qInfo() << list2;

        qInfo() << "Equal: " << std::equal(list1.begin(), list1.end(), list2.begin());

        list1.fill(9);
        list2.fill(9);

        qInfo() << list1;
        qInfo() << list2;
        qInfo() << "Equal: " << std::equal(list1.begin(), list1.end(), list2.begin());
    }
    void copy(){
        QList<int> list1;
        randomGenerate(list1, 10);
        QList<int> list2(list1.size());

        qInfo() << list1;
        qInfo() << list2;

        qInfo() << "Copying...";
        std::copy(list1.begin(), list1.end(), list2.begin());
        qInfo() << list1;
        qInfo() << list2;
        qInfo() << std::equal(list1.begin(), list1.end(), list2.begin());
    }
    void macro(){
    #define add(x,y) x+y
        //Easy...but dangerous.
        // There is no type checking
        qInfo() << add(1,2);

        //Lets break it
        //    qInfo() << add("Test",22);
        //    qInfo() << add(true,true);
        //    qInfo() << add(false, true);
        //    qInfo() << add(false, false);
        //    qInfo() << add("Test",'\n');
        //    qInfo() << add("Test",'/n');
    }
    void TypeDef(){
        // rename long type names
        typedef QMap<QString,int> People;
        People p;
        p.insert("Adam", 10);
        p.insert("Dam", 20);
        p.insert("Madam", 30);

        foreach (QString key, p.keys()) {
            qInfo() << key << " : " << p.value(key);
        }
    }
}

namespace Assignment{
#define RANDOM_GENERATE(list, max)\
{\
        list.reserve(max);\
        for (int i = 0; i < max; ++i) {\
            int value = QRandomGenerator::global()->bounded(100);\
            list.append(value);\
        }\
}
    void main(){
        QList<int> list(10);
        RANDOM_GENERATE(list, 10);
        qInfo() << list;
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //DeleteAll::main();
    //Algorithms::fill();
    //Algorithms::sort();
    //Algorithms::equal();
    //Algorithms::copy();
    Algorithms::macro();
    Algorithms::TypeDef();
    Assignment::main();
    return a.exec();
}
