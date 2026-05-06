/*******************************************************
 * File: din_device.cpp
 *******************************************************/

#include "din_device.h"

DinDevice::DinDevice(QObject *parent)
    : VgpioDevice(parent)
{
    m_type = DEV_DIN;
}

void DinDevice::setValue(int value)
{
    value = value ? 1 : 0;

    VgpioDevice::setValue(value);
}