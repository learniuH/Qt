/*******************************************************
 * File: vgpio_protocol.h
 * Description:
 *      VGPIO USB协议封装层
 *******************************************************/

#ifndef VGPIO_PROTOCOL_H
#define VGPIO_PROTOCOL_H

#include <QObject>
#include <QByteArray>
#include <QList>

class VgpioProtocol
{
public:
    enum CmdType
    {
        CMD_WRITE = 0x00,
        CMD_READ  = 0x01,
    };

    enum DeviceType
    {
        DEV_DIN  = 0x01,
        DEV_KEY  = 0x02,
        DEV_AIN  = 0x03,
    };

    struct Frame
    {
        quint8 cmd;
        quint8 devType;
        quint8 devIndex;
        quint16 value;
    };

public:
    static QByteArray buildWriteFrame(quint8 devType,
                                      quint8 devIndex,
                                      quint16 value);

    static QByteArray buildReadFrame(quint8 devType,
                                     quint8 devIndex);

    static bool parseFrame(QByteArray &buffer,
                           Frame &frame);

    static quint8 calcCRC(const QByteArray &data);

private:
    static constexpr quint8 FRAME_HEAD1 = 0xAA;
    static constexpr quint8 FRAME_HEAD2 = 0x55;
    static constexpr quint8 FRAME_LEN   = 0x09;
};

#endif // VGPIO_PROTOCOL_H