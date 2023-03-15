// Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "senderthread.h"

#include <QSerialPort>
#include <QTime>

SenderThread::SenderThread(QObject *parent) :
    QThread(parent)
{
}

//! [0]
SenderThread::~SenderThread()
{
    m_mutex.lock();
    m_quit = true;
    m_cond.wakeOne();
    m_mutex.unlock();
    wait();
}
//! [0]

//! [1] //! [2]
void SenderThread::transaction(const QString &portName, int waitTimeout, const QString &request)
{
//! [1]
    const QMutexLocker locker(&m_mutex);
    m_portName = portName;
    m_waitTimeout = waitTimeout;
    m_request = request;
//! [3]
    if (!isRunning())
        start();
    else
        m_cond.wakeOne();
}
//! [2] //! [3]

QString SenderThread::byte2str(char& byte) {
    char const hex[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A',   'B','C','D','E','F'};

    std::string str;
    const char ch = byte;
    str.append(&hex[(ch  & 0xF0) >> 4], 1);
    str.append(&hex[ch & 0x0F], 1);
    return QString::fromStdString(str);
}

QString SenderThread::bytes2str(const QByteArray& bytes) {
    char const hex[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A',   'B','C','D','E','F'};

    std::string str;
    for (int index = 0; index < bytes.size(); ++index) {
        const char ch = bytes[index];
        str.append(&hex[(ch  & 0xF0) >> 4], 1);
        str.append(&hex[ch & 0x0F], 1);
    }
    return QString::fromStdString(str);
}

//! [4]
void SenderThread::run()
{
    bool currentPortNameChanged = false;

    m_mutex.lock();
//! [4] //! [5]
    QString currentPortName;
    if (currentPortName != m_portName) {
        currentPortName = m_portName;
        currentPortNameChanged = true;
    }

    int currentWaitTimeout = m_waitTimeout;
    QString currentRequest = m_request;
    m_mutex.unlock();
//! [5] //! [6]
    QSerialPort serial;

    if (currentPortName.isEmpty()) {
        emit error(tr("No port name specified"));
        return;
    }

    while (!m_quit) {
//![6] //! [7]
        if (currentPortNameChanged) {
            serial.close();
            serial.setPortName(currentPortName);
            serial.setBaudRate(QSerialPort::BaudRate::Baud115200);

            if (!serial.open(QIODevice::ReadWrite)) {
                emit error(tr("Can't open %1, error code %2")
                           .arg(m_portName).arg(serial.error()));
                return;
            }
        }
//! [7] //! [8]
        // write request
        // 420002E201A3
        //const QByteArray requestData = currentRequest.toUtf8();
        const QByteArray requestData = QByteArray::fromHex(currentRequest.toUtf8());

        int bytesWriten = serial.write(requestData);
        bool waitForBytesWrittenRET = serial.waitForBytesWritten(m_waitTimeout);
        if (!waitForBytesWrittenRET) {
//! [9] //! [11]
            emit timeout(tr("Wait write request timeout %1")
                             .arg(QTime::currentTime().toString()));
        }
        else if (bytesWriten == -1) {
            emit timeout(QObject::tr("Failed to write the data to port %1, error: %2")
                                 .arg(serial.portName()).arg(serial.errorString()));
        }
//! [8] //! [10]
        // read response
        QByteArray responseData = serial.readAll();
        //bool waitForReadyReadRET = serial.waitForReadyRead(currentWaitTimeout);
        bool waitForReadyReadRET = true; //serial.waitForReadyRead(currentWaitTimeout);
        //while (serial.waitForReadyRead(10))
        while(waitForReadyReadRET)
            waitForReadyReadRET = serial.waitForReadyRead(currentWaitTimeout);
            responseData.append(serial.readAll());
        if (!waitForReadyReadRET) {
//! [10] //! [11] //! [12]
            emit timeout(tr("Wait read response timeout %1")
                             .arg(QTime::currentTime().toString()));
        }
        else if (serial.error() == QSerialPort::ReadError) {
            emit tr("Failed to read from port %1, error: %2")
                .arg(serial.portName()).arg(serial.errorString());
        }
        else if (serial.error() == QSerialPort::TimeoutError && responseData.isEmpty()) {
            emit tr("No data was currently available for reading from port %1")
                .arg(serial.portName());
        }

        QString responseStr = ""; //= QString::fromUtf8(responseData);
        for (int index = 0; index < responseData.size(); ++index) {
            auto resp = responseData.at(index);
            auto resp2 = byte2str(resp);
            responseStr.append(resp2);
        }
//! [12]
        //responseStr = bytes2str(QByteArray::from);
        emit this->response(responseStr);

//! [9]  //! [13]
        m_mutex.lock();
        m_cond.wait(&m_mutex);
        if (currentPortName != m_portName) {
            currentPortName = m_portName;
            currentPortNameChanged = true;
        } else {
            currentPortNameChanged = false;
        }
        currentWaitTimeout = m_waitTimeout;
        currentRequest = m_request;
        m_mutex.unlock();
    }
//! [13]
}
