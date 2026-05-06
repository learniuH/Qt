/*******************************************************
 * File: ain_device.cpp
 *******************************************************/

#include "ain_device.h"

AinDevice::AinDevice(QObject *parent)
    : VgpioDevice(parent)
{
    m_type = DEV_AIN;
}

void AinDevice::setValue(int value)
{
    /*
        12位ADC
    */
    if (value < 0)
    {
        value = 0;
    }

    if (value > 4095)
    {
        value = 4095;
    }

    VgpioDevice::setValue(value);
}