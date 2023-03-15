#include <QCoreApplication>
#include <QDebug>
#include <QTextStream>

#include "radio.h"
#include "station.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    bool isRadioOn = false;
    radio radioBox;
    const int numberChannels = 3;
    station* channels[numberChannels]{
            new station(&radioBox, 94, "Jazz"),
            new station(&radioBox, 84, "News"),
            new station(&radioBox, 108, "Pop")
    };

    // close entire application when radio emits quit signal.
    //radioBox.connect(&radioBox, &radio::quit, &app, &QCoreApplication::quit); // Qt::AutoConnection by default. It doesn't close the application. because there is some other connections in other threads. // Qt::DirectConnection: The slot is executed in the signalling thread
    radioBox.connect(&radioBox, &radio::quit, &app, &QCoreApplication::quit, Qt::QueuedConnection); // The slot is executed in the receiver's thread

    while(true){
        qInfo() << "Menu: on | off | test | quit" << '\n';
        QTextStream qtin(stdin);
        QString line = qtin.readLine().trimmed().toUpper();
        if (line == "ON" && isRadioOn==false) // open the radio if radio is not already open
        {
            qDebug() << "Turning the radio on" << '\n';
            for (int index = 0; index < numberChannels; ++index) {
                station* channel = channels[index];
                radioBox.connect(channel, &station::send, &radioBox, &radio::listen);
            }
            qDebug() << "Radio is on" << '\n';
            isRadioOn = true;
        }
        else if (line == "OFF") // close the radio
        {
            qDebug() << "Turning the radio off" << '\n';
            for (int index = 0; index < numberChannels; ++index) {
                station* channel = channels[index];
                radioBox.disconnect(channel, &station::send, &radioBox, &radio::listen);
            }
            qDebug() << "Radio is off" << '\n';
            isRadioOn = false;
        }
        else if (line == "TEST") // sending test signal.
        {
            qDebug() << "Testing" << '\n';
            for (int index = 0; index < numberChannels; ++index) {
                station* channel = channels[index];
                channel->broadcast("bip bip bop bop TEST1 TEST2 TEST3 ALOOO");
            }
            qDebug() << "Test complete" << '\n';
            isRadioOn = false;
        }
        else if (line == "QUIT") // signal emitting to close the application. slot already connected up there.
        {
            qDebug() << "Quitting" << '\n';
            emit radioBox.quit();
            qDebug() << "Radio is off" << '\n';
            isRadioOn = false;
            break;
        }
    }

    return app.exec();
}
