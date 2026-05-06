/*******************************************************
 * File: usb_cdc_device.h
 * Description:
 *      USB CDC 通信层
 *******************************************************/

#ifndef USBCDCDEVICE_H
#define USBCDCDEVICE_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>

#include "vgpio_protocol.h"

class UsbCdcDevice : public QObject
{
    Q_OBJECT

public:
    explicit UsbCdcDevice(QObject *parent = nullptr);
    ~UsbCdcDevice();

public:
    bool open(const QString &portName,
              int baudrate = 115200);

    void close();

    bool isOpened() const;

    QString portName() const;

    QStringList availablePorts() const;

public:
    bool sendFrame(const QByteArray &frame);

    bool writeDIN(quint8 index,
                  bool value);

    bool writeAIN(quint8 index,
                  quint16 value);

    bool readDIN(quint8 index);

    bool readAIN(quint8 index);

signals:
    /*
        日志
    */
    void sigLog(const QString &log);

    /*
        协议解析结果
    */
    void sigFrameReceived(const VgpioProtocol::Frame &frame);

    /*
        串口状态
    */
    void sigConnected();

    void sigDisconnected();

private slots:
    void slotReadyRead();

private:
    void parseBuffer();

private:
    QSerialPort *m_serial;

    QByteArray m_rxBuffer;
};

#endif // USBCDCDEVICE_H