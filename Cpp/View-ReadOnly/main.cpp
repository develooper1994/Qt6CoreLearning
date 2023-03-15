#include <QCoreApplication>
#include <QByteArray>
#include <QByteArrayView> // readonly, const, cannot modify
#include <QStringView>

#include "person.h"

void displayQByteArrayView(QByteArrayView& view){
    qInfo() << view;
    qInfo() << view.size();
    qInfo() << view.constData();
    qInfo() << view.sliced(3.5);
    qInfo() << view.contains("are");
    qInfo() << view.trimmed();
    qInfo() << view.at(10);
    qInfo() << view[10];
    qInfo() << view.indexOf('Q');

    for(auto c: view){
        qInfo() << c;
    }
}

void displayQStringView(QStringView& view){
/*
Issues with QStringView
If you are having issues with QStringView, for example, it is displaying what looks like garbage:
Output: "\u0001\u0000\u0000................."

Your strings encoding and the console does not know how to display it correctly.
String views are just classes that display a string from a pointer.

try:
.toString()
.toLatin1()
.toUtf8()

Additionally if you are creating a QString in the constructor you might have to separate the QString from the QStringView:

QStringView Person::display(){
    QString name = QString(m_firstname + " " + m_lastname);
    QStringView view(name);
    return view;
}

https://doc.qt.io/qt-6/qstringview.html
*/
    qInfo() << view;
    qInfo() << view.size();
    qInfo() << view.constData();
    qInfo() << view.sliced(3.5);
    qInfo() << view.contains(QString("are"));
    qInfo() << view.trimmed();
    qInfo() << view.at(10);
    qInfo() << view[10];
    qInfo() << view.indexOf('E');

    for(auto c: view){
        qInfo() << c;
    }
}

namespace assignment{
    void main(){
        Person p(nullptr, "Mustafa", "Selçuk");
        qInfo() << p.display();
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    {
        qInfo() << "QByteArrayView";
        QByteArray bytes("hello Qt How are you today?");
        QByteArrayView view(bytes); // doesn't copy anything. It is just a like const reference
        displayQByteArrayView(view);
    }

    {
        qInfo() << "QStringView";
        QString str = "Hello datalamangalaştıramadıklarımızdansanız mısınız? Eveeet..";
        QStringView view(str);
        displayQStringView(view);
    }

    assignment::main();

    return a.exec();
}
