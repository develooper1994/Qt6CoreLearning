#include <QCoreApplication>
#include "owner.h"
#include "dog.h"

// Every time signal emitted, it's going to trigger the slot for every connection

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Owner owner;
    Dog dog;

    QObject::connect(&owner,&Owner::treats,&dog,&Dog::treats); //Auto
    QObject::connect(&owner,&Owner::treats,&dog,&Dog::treats); //Auto

    //Somewhere else
    for(int i = 0; i < 10; ++i){
        // if you already connected, you won't connect second time.
        QObject::connect(&owner,&Owner::treats,&dog,&Dog::treats,Qt::UniqueConnection);
    }

    owner.giveSnaks();

    return a.exec();
}
