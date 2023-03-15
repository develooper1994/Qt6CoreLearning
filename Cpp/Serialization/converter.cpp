#include "converter.h"

Converter::Converter(QObject *parent): QObject{parent}{}

void Converter::writeJson(Test *obj, QString path){
    QVariantMap items, map;
    foreach (QString key, obj->map().keys()) {
        items.insert(key, QVariant(obj->map().value(key)));
    }
    map["name"] = QVariant(obj->map()["name"]);
    map["items"] = QVariant(items);

    const QJsonDocument json = QJsonDocument::fromVariant(map);
    const QByteArray jsonBytes = json.toJson();
    QFile file(path);
    if (!file.open(QIODevice::OpenModeFlag::WriteOnly)) {
        qCritical() << "File cannot open to write json";
        qCritical() << file.errorString();
        return;
    }
    file.write(jsonBytes);
    file.close();
}

Test *Converter::readJson(QString path){
    QFile file(path);
    if (!file.open(QIODevice::OpenModeFlag::ReadOnly)) {
        qCritical() << "Cannot open file to read json";
        qCritical() << file.errorString();
        return nullptr;
    }
    QByteArray jsonBytes = file.readAll();
    file.close();

    Test *t = new Test();
    const QJsonDocument json = QJsonDocument::fromJson(jsonBytes);
    const QJsonValue name = json["name"];
    const QString nameStr = name.toString();
    const QJsonValue items = json["items"];
    const QVariantMap itemsVMap = qvariant_cast<QVariantMap>(items);
    QMap<QString,QString> itemsMap;
    foreach (QString key, itemsVMap.keys()) {
        itemsMap.insert(key, itemsVMap.value(key).toString());
    }
    t->setName(nameStr);
    t->setMap(itemsMap);

    return t;
}

void Converter::writeXml(Test *obj, QString path){
    QFile file(path);
    if (!file.open(QIODevice::OpenModeFlag::WriteOnly)) {
        qCritical() << "Cannot open file to write Xml";
        qCritical() << file.errorString();
        return;
    }

    QXmlStreamWriter stream(&file);
    stream.setAutoFormatting(true);

    stream.writeStartDocument();
        stream.writeStartElement("name");
        stream.writeAttribute("name", obj->name());

        stream.writeStartElement("items");
            foreach (QString key, obj->map().keys()) {
                stream.writeStartElement("item");
                    stream.writeAttribute("key", key);
                    stream.writeAttribute("value", obj->map().value(key));
                stream.writeEndElement();
            }
        stream.writeEndElement();

        stream.writeEndElement();
    stream.writeEndDocument();

    file.close();
}

Test *Converter::readXml(QString path){
    QFile file(path);
    if (!file.open(QIODevice::OpenModeFlag::ReadOnly)) {
        qCritical() << "Cannot open file to read Xml";
        qCritical() << file.errorString();
        return nullptr;
    }
    QByteArray XmlBytes = file.readAll();
    file.close();

    QXmlStreamReader stream(XmlBytes);
    Test* t = new Test();
    QMap<QString, QString> map;
    while (!stream.atEnd()) {
        QXmlStreamReader::TokenType token = stream.readNext();
        switch (token) {
            case QXmlStreamReader::StartElement:
                if (stream.name().toString() == "name") {
                    QString name = stream.attributes().value("name").toString();
                    t->setName(name);
                }
                if (stream.name().toString() == "item") {
                    QString key   = stream.attributes().value("key").toString();
                    QString items = stream.attributes().value("value").toString();
                    map.insert(key, items);
                }
                break;
            case QXmlStreamReader::EndDocument:
                t->setMap(map);
                break;
            case QXmlStreamReader::StartDocument:
            case QXmlStreamReader::EndElement:
            case QXmlStreamReader::NoToken:
            case QXmlStreamReader::Invalid:
            case QXmlStreamReader::Characters:
            case QXmlStreamReader::Comment:
            case QXmlStreamReader::DTD:
            case QXmlStreamReader::EntityReference:
            case QXmlStreamReader::ProcessingInstruction:
            break;
        default:
            break;
        }
    }


    return t;
}
