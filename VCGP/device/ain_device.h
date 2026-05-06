/*******************************************************
 * File: ain_device.h
 *******************************************************/

#ifndef AIN_DEVICE_H
#define AIN_DEVICE_H

#include "vgpio_device.h"

class AinDevice : public VgpioDevice
{
    Q_OBJECT

public:
    explicit AinDevice(QObject *parent = nullptr);

public:
    void setValue(int value) override;
};

#endif // AIN_DEVICE_H