/*******************************************************
 * File: vgpio_protocol.cpp
 *******************************************************/

#include "vgpio_protocol.h"

QByteArray VgpioProtocol::buildWriteFrame(quint8 devType,
                                          quint8 devIndex,
                                          quint16 value)
{
    QByteArray frame;

    frame.append(FRAME_HEAD1);
    frame.append(FRAME_HEAD2);
    frame.append(FRAME_LEN);

    frame.append(CMD_WRITE);

    frame.append(devType);
    frame.append(devIndex);

    frame.append((value >> 8) & 0xFF);
    frame.append(value & 0xFF);

    quint8 crc = calcCRC(frame);

    frame.append(crc);

    return frame;
}

QByteArray VgpioProtocol::buildReadFrame(quint8 devType,
                                         quint8 devIndex)
{
    QByteArray frame;

    frame.append(FRAME_HEAD1);
    frame.append(FRAME_HEAD2);
    frame.append(FRAME_LEN);

    frame.append(CMD_READ);

    frame.append(devType);
    frame.append(devIndex);

    frame.append(0x00);
    frame.append(0x00);

    quint8 crc = calcCRC(frame);

    frame.append(crc);

    return frame;
}

bool VgpioProtocol::parseFrame(QByteArray &buffer,
                               Frame &frame)
{
    while (buffer.size() >= FRAME_LEN)
    {
        /*
            查找包头
        */
        if ((quint8)buffer[0] != FRAME_HEAD1 ||
            (quint8)buffer[1] != FRAME_HEAD2)
        {
            buffer.remove(0, 1);
            continue;
        }

        /*
            长度检查
        */
        quint8 len = (quint8)buffer[2];

        if (len != FRAME_LEN)
        {
            buffer.remove(0, 1);
            continue;
        }

        /*
            数据不足
        */
        if (buffer.size() < FRAME_LEN)
        {
            return false;
        }

        QByteArray oneFrame = buffer.left(FRAME_LEN);

        /*
            CRC校验
        */
        quint8 crc = calcCRC(oneFrame.left(8));

        if (crc != (quint8)oneFrame[8])
        {
            buffer.remove(0, 1);
            continue;
        }

        /*
            解析
        */
        frame.cmd      = (quint8)oneFrame[3];
        frame.devType  = (quint8)oneFrame[4];
        frame.devIndex = (quint8)oneFrame[5];

        frame.value =
                ((quint8)oneFrame[6] << 8) |
                ((quint8)oneFrame[7]);

        /*
            移除已解析数据
        */
        buffer.remove(0, FRAME_LEN);

        return true;
    }

    return false;
}

quint8 VgpioProtocol::calcCRC(const QByteArray &data)
{
    quint16 sum = 0;

    for (char ch : data)
    {
        sum += (quint8)ch;
    }

    return (quint8)(sum & 0xFF);
}