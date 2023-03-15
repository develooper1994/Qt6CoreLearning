#include <QCoreApplication>
#include <QTextStream>
#include <QDebug>

#include <QSerialPortInfo>
#include <QSerialPort>

void printAllSerialInfo(const QList<QSerialPortInfo>& ports)
{
    for (size_t index=0; index < ports.count(); ++index) {
        auto port = ports.at(index);
        qInfo() << "-*-*-*-*-* " << index << " *-*-*-*-*-\n";


        qInfo() << "portName: " << port.portName() << '\n';
        qInfo() << "description: " << port.description() << '\n';
        qInfo() << "manufacturer: " << port.manufacturer() << '\n';
        qInfo() << "standardBaudRates: " << port.standardBaudRates() << '\n';

        quint16 productIdentifier = port.hasProductIdentifier() ? port.productIdentifier() : 0;
        qInfo() << "productIdentifier: " << productIdentifier << '\n';
        quint16 vendorIdentifier = port.hasVendorIdentifier() ? port.vendorIdentifier() : 0;
        qInfo() << "productIdentifier: " << vendorIdentifier << '\n';

        qInfo() << "serialNumber: " << port.serialNumber() << '\n';
        qInfo() << "systemLocation: " << port.systemLocation() << '\n';
        qInfo() << "systemLocation: " << port.hasProductIdentifier() << '\n';


        qInfo() << "-*-*-*-*-* " << index << " *-*-*-*-*-\n";
    }
}

QByteArray writeToSerial(QTextStream& stdOut, QSerialPort& serial, uint maxWait=1000){
    QByteArray geminiCode = QByteArray::fromHex("420002E201A3");
    const int ByteCount = serial.write(geminiCode);
    if (ByteCount == -1) {
        stdOut << QObject::tr("Failed to write the data to port %1, error: %2")
                      .arg(serial.portName()).arg(serial.errorString()) << Qt::endl;
        return QByteArray();
    }  else if (!serial.waitForBytesWritten(maxWait)) {
        stdOut << QObject::tr("Operation timed out or an error "
                              "occurred for port %1, error: %2")
                      .arg(serial.portName()).arg(serial.errorString()) << Qt::endl;
        return QByteArray();
    }
    return geminiCode;
}

QByteArray readFromSerial(QTextStream& stdOut, QSerialPort& serial, uint maxWait=1000){
    //read from serial
    QByteArray readData = serial.readAll();
    bool waitForReadyReadRET = true;
    while (waitForReadyReadRET) {
        waitForReadyReadRET = serial.waitForReadyRead(maxWait);
        readData.append(serial.readAll());
    }
    if (serial.error() == QSerialPort::ReadError) {
        stdOut << QObject::tr("Failed to read from port %1, error: %2").arg(serial.portName()).arg(serial.errorString())  << '\n';
        return QByteArray();
    }
    else if (serial.error() == QSerialPort::TimeoutError && readData.isEmpty()) {
        stdOut << QObject::tr("No data was currently available"
                              " for reading from port %1")
                      .arg(serial.portName()) << '\n';
    }

    return readData;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTextStream stdOut(stdout);
    QTextStream stdErr(stderr);

    uint maxWait = 5000;
    uint portNumber = 0;

    QSerialPortInfo serialInfo;
    QList<QSerialPortInfo> ports = serialInfo.availablePorts();
    QSerialPortInfo port = ports.at(portNumber);
    //printAllSerialInfo(ports);

    QSerialPort serial;
    serial.setPortName(port.portName());
    serial.setBaudRate(QSerialPort::BaudRate::Baud115200);

    bool openError = serial.open(QIODevice::ReadWrite);
    if (!openError) {
        stdErr << QObject::tr("Failed to open port: %1, error: %2").arg(port.portName()).arg(serial.error())<<'\n';
        return EXIT_FAILURE;
    }

    //write to serial
    QByteArray geminiCode = writeToSerial(stdOut, serial, maxWait);
    if (geminiCode.isEmpty()) {
        return EXIT_FAILURE;
    }
    stdOut << QObject::tr("Data successfully sent to port %1, error %2")
                  .arg(serial.portName()) << '\n';
    //read from serial
    QByteArray readData = readFromSerial(stdOut, serial, maxWait);
    if (readData.isEmpty()) {
        return EXIT_FAILURE;
    }
    stdOut << QObject::tr("Data successfully received from port %1")
                  .arg(serial.portName()) << '\n';

    qInfo() << "geminiCode: " << geminiCode
            << "readData: "   << readData
            << '\n';


    return a.exec();
}
