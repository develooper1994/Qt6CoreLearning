#include <QCoreApplication>
#include <QDebug>

#include "car.h"
#include "dog.h"
#include "racecar.h"
#include "feline.h"

/*
* the implicit cast is c++ doing it for us.
* the explicit cast is us telling c++ what to do.
* the static_cast operator performs a nonpolymorphic cast.
* the dynamic_cast operator ensures inheritence is good
* the reinterpret_cast operator types to convert it to different type
* the qobject_cast works with QObjects and takes away a lots of complexity.
    * similar to dynamic_cast but doesn't include RTTI :)
    * not works with non-qobjects :(
*/

// natural implicit conversion: char->int->short->int->...
void implicit_conversion(){
    double value = 45.75;
    qDebug() << "value: " << value;
    int val_int = value;
    qDebug() << "val_int: " << val_int;
}

void explicit_conversion(){
    // compiler trusts you. You promise.
    double value = 45.75;
    qDebug() << "value: " << value;
    int val_int = (int)value;
    qDebug() << "val_int: " << val_int;
}

namespace conversion
{
    void testDrive(car* obj)
    {
        obj->drive();
        obj->stop();
    }

    void race(racecar* obj)
    {
        obj->drive();
        obj->gofast();
        obj->stop();
    }

    void dynamic_conversion(QCoreApplication* app){
        //wrong way
        racecar* player1 = new racecar(app);
        testDrive(player1); // implicit conversion

        //correct way
        car* obj = dynamic_cast<car*>(player1); // explicit way
        if(obj) testDrive(obj);

        // black magic. convert cat->car
        feline* cat = new feline(app);
        car* catcar = dynamic_cast<car*>(cat);
        qInfo() << "catcar: " << catcar;
        if(catcar) testDrive(catcar); // it will not succeed
    }

    void static_conversion(QCoreApplication* app){
        car* mcar = new car(app);
        qInfo() << mcar;
        //race((racecar*)mcar); // :( !!WRONG!!

        // Correct :)
        racecar* racer = static_cast<racecar*>(mcar);
        qInfo() << racer;
        if(racer) race(racer);

        //testing non relevant class
        /*
        feline* catcar = static_cast<feline*>(mcar);
        qInfo() << racer;
        if(racer) race(racer);
        */
    }

    void reinterpreted_conversion(QCoreApplication* app){
        racecar* player1 = new racecar(app);

        // cast the racecar to its base
        car* obj = dynamic_cast<car*>(player1);
        if(obj) testDrive(obj);

        // cast it without changing it
        racecar* sppedstar = static_cast<racecar*>(obj);
        if(sppedstar) race(sppedstar);

        // reinterpret it as a different type
        int* pointer = reinterpret_cast<int*>(sppedstar);
        qInfo() << "pointer: " << pointer;
        qInfo() << "racecar: " << sppedstar;

        racecar* mycar = reinterpret_cast<racecar*>(pointer);
        qInfo() << "My Cars color is " << mycar->color;
        race(mycar);

        feline* cat = reinterpret_cast<feline*>(pointer);
        cat->hiss();
        cat->meow();
    }

}

namespace qobjectCast{
    void testDrive(car* obj)
    {
        obj->drive();
        obj->stop();
    }

    void race(racecar* obj)
    {
        obj->drive();
        obj->gofast();
        obj->stop();
    }

    void qobjectCast_main(QCoreApplication* a){
        racecar* mycar = new racecar(a);
        car* mcar = qobject_cast<car*>(mycar);
        mcar->drive();

        racecar* fastcar = qobject_cast<racecar*>(mcar);
        fastcar->gofast();

        dog* fifo = qobject_cast<dog*>(fastcar);
    }

}





int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //implicit_conversion();
    //explicit_conversion();
    //dynamic_static_conversion::dynamic_conversion(&a);
    //conversion::static_conversion(&a);
    //conversion::reinterpreted_conversion(&a);
    qobjectCast::qobjectCast_main(&a);

    return a.exec();
}
