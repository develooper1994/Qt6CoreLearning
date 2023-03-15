#include <QCoreApplication>
#include <QDebug>
#include <QSettings>
#include <QRandomGenerator>

//global settings
void info(QSettings& settings){
    qInfo() << "File: " << settings.fileName();
    qInfo() << "Key: " << settings.allKeys();
}

void save(QSettings& settings){
    settings.setValue("table/color", "white");
    settings.setValue("table/size", 300);

    qInfo() << settings.status();
}

void load(QSettings& settings){
    info(settings);

    qInfo() << settings.value("table/color", QVariant("black")).toString();
    bool ok;
    qInfo() << settings.value("table/size").toInt(&ok);
    if (!ok) {
        qInfo() << "cannot convert to int";
    }
}

// group settings
void saveAge(QSettings* settings, QString group, QString name, int age){
    settings->beginGroup(group);
    settings->setValue(name, age);
    settings->endGroup();
}

int getAge(QSettings* settings, QString group, QString name){
    settings->beginGroup(group);
    if (!settings->contains(name)) {
        qWarning() << "Does not contain" << name << " in " <<group;
        settings->endGroup();
        return EXIT_FAILURE;
    }
    bool ok;
    int value = settings->value(name).toInt(&ok);
    settings->endGroup();
    if (!ok) {
        qWarning() << "Could not convert to int";
        return EXIT_FAILURE;
    }
    settings->endGroup();
    return value;
}

int main(int argc, char *argv[])
{
    // global settings
    QCoreApplication a(argc, argv);
    QCoreApplication::setOrganizationName("Aloha");
    QCoreApplication::setOrganizationDomain("Home");
    QCoreApplication::setApplicationName("Awesomeness");
    QCoreApplication::setApplicationVersion("1.0.0");

    // key-value pairs
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    if (settings.allKeys().length() == 0){
        qInfo() << "No settings...savings...";
        save(settings);
    }
    else {
        load(settings);
    }

    // group settings
    saveAge(&settings,"people","Bryan",44);
    saveAge(&settings,"bear","Baby",1);
    saveAge(&settings,"bear","twohearts",44);

    qInfo()<<"Baby"<<getAge(&settings,"bear","Baby");
    qInfo()<<"twoHeart"<<getAge(&settings,"bear","twoHeart");
    qInfo()<<"Bryan"<<getAge(&settings,"people","Bryan");

    // sync
    QStringList people;
    people << "Mustafa" << "Selçuk" << "Çağlar" << "Mazda";
    foreach (QString person, people) {
        int value = QRandomGenerator::global()->bounded(100);
        saveAge(&settings, "test", person, value);
    }

    settings.sync(); // writes any unsaved changes to permanent storage
    qInfo() << "File : " << settings.fileName();
    foreach (QString group, settings.childGroups()) {
        settings.beginGroup(group);
        qInfo() << "Group : " << group;

        foreach (QString key, settings.childKeys()) {
            qInfo() << "... key: " << key << " = " << settings.value(key).toString();
        }

        settings.endGroup();
    }

    // filename
    QString filename = a.applicationName();
    qInfo() << "filename" << filename;

    QSettings settings2(filename, QSettings::Format::IniFormat);
    settings2.beginGroup("people");
    settings2.setValue("Bryan", 46);
    settings2.endGroup();

    settings2.sync();

    settings2.beginGroup("people");
    qInfo() << "Tammy: " << settings2.value("Tammy", QVariant("Person not found"));
    qInfo() << "Bryan: " << settings2.value("Bryan", QVariant("Person not found"));
    settings2.endGroup();

    return a.exec();
}
