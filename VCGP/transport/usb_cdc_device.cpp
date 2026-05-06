/*******************************************************
 * File: usb_cdc_device.cpp
 *******************************************************/

#include "usb_cdc_device.h"

UsbCdcDevice::UsbCdcDevice(QObject *parent)
    : QObject(parent)
{
    m_serial = new QSerialPort(this);

    connect(m_serial,
            &QSerialPort::readyRead,
            this,
            &UsbCdcDevice::slotReadyRead);
}

UsbCdcDevice::~UsbCdcDevice()
{
    close();
}

bool UsbCdcDevice::open(const QString &portName,
                        int baudrate)
{
    if (m_serial->isOpen())
    {
        m_serial->close();
    }

    m_serial->setPortName(portName);

    m_serial->setBaudRate(baudrate);

    m_serial->setDataBits(QSerialPort::Data8);

    m_serial->setParity(QSerialPort::NoParity);

    m_serial->setStopBits(QSerialPort::OneStop);

    m_serial->setFlowControl(QSerialPort::NoFlowControl);

    if (!m_serial->open(QIODevice::ReadWrite))
    {
        emit sigLog(QString("Open Failed: %1")
                    .arg(portName));

        return false;
    }

    emit sigLog(QString("Open Success: %1")
                .arg(portName));

    emit sigConnected();

    return true;
}

void UsbCdcDevice::close()
{
    if (m_serial->isOpen())
    {
        m_serial->close();

        emit sigDisconnected();

        emit sigLog("Serial Closed");
    }
}

bool UsbCdcDevice::isOpened() const
{
    return m_serial->isOpen();
}

QString UsbCdcDevice::portName() const
{
    return m_serial->portName();
}

QStringList UsbCdcDevice::availablePorts() const
{
    QStringList list;

    const auto ports = QSerialPortInfo::availablePorts();

    for (const auto &port : ports)
    {
        list << port.portName();
    }

    return list;
}

bool UsbCdcDevice::sendFrame(const QByteArray &frame)
{
    if (!m_serial->isOpen())
    {
        return false;
    }

    qint64 ret = m_serial->write(frame);

    if (ret <= 0)
    {
        return false;
    }

    QString log;

    for (auto ch : frame)
    {
        log += QString("%1 ")
                .arg((quint8)ch,
                     2,
                     16,
                     QChar('0'))
                .toUpper();
    }

    emit sigLog(QString("TX: %1").arg(log));

    return true;
}

bool UsbCdcDevice::writeDIN(quint8 index,
                            bool value)
{
    QByteArray frame =
            VgpioProtocol::buildWriteFrame(
                VgpioProtocol::DEV_DIN,
                index,
                value);

    return sendFrame(frame);
}

bool UsbCdcDevice::writeAIN(quint8 index,
                            quint16 value)
{
    QByteArray frame =
            VgpioProtocol::buildWriteFrame(
                VgpioProtocol::DEV_AIN,
                index,
                value);

    return sendFrame(frame);
}

bool UsbCdcDevice::readDIN(quint8 index)
{
    QByteArray frame =
            VgpioProtocol::buildReadFrame(
                VgpioProtocol::DEV_DIN,
                index);

    return sendFrame(frame);
}

bool UsbCdcDevice::readAIN(quint8 index)
{
    QByteArray frame =
            VgpioProtocol::buildReadFrame(
                VgpioProtocol::DEV_AIN,
                index);

    return sendFrame(frame);
}

void UsbCdcDevice::slotReadyRead()
{
    QByteArray data = m_serial->readAll();

    if (data.isEmpty())
    {
        return;
    }

    QString log;

    for (auto ch : data)
    {
        log += QString("%1 ")
                .arg((quint8)ch,
                     2,
                     16,
                     QChar('0'))
                .toUpper();
    }

    emit sigLog(QString("RX: %1").arg(log));

    m_rxBuffer.append(data);

    parseBuffer();
}

void UsbCdcDevice::parseBuffer()
{
    while (1)
    {
        VgpioProtocol::Frame frame;

        bool ret =
                VgpioProtocol::parseFrame(
                    m_rxBuffer,
                    frame);

        if (!ret)
        {
            break;
        }

        emit sigFrameReceived(frame);
    }
}