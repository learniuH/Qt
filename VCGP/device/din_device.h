/*******************************************************
 * File: din_device.h
 *******************************************************/

#ifndef DIN_DEVICE_H
#define DIN_DEVICE_H

#include "vgpio_device.h"

class DinDevice : public VgpioDevice
{
    Q_OBJECT

public:
    explicit DinDevice(QObject *parent = nullptr);

public:
    void setValue(int value) override;
};

#endif // DIN_DEVICE_H