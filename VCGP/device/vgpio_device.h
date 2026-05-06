/*******************************************************
 * File: vgpio_device.h
 * Description:
 *      虚拟GPIO设备基类
 *******************************************************/

#ifndef VGPIO_DEVICE_H
#define VGPIO_DEVICE_H

#include <QObject>

class VgpioDevice : public QObject
{
    Q_OBJECT

public:
    enum DeviceType
    {
        DEV_DIN = 1,
        DEV_KEY = 2,
        DEV_AIN = 3,
    };

public:
    explicit VgpioDevice(QObject *parent = nullptr);

    virtual ~VgpioDevice();

public:
    int index() const;

    void setIndex(int index);

    int value() const;

    virtual void setValue(int value);

    DeviceType type() const;

signals:
    void sigValueChanged(int value);

protected:
    int m_index;

    int m_value;

    DeviceType m_type;
};

#endif // VGPIO_DEVICE_H