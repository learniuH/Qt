/*******************************************************
 * File: vgpio_device.cpp
 *******************************************************/

#include "vgpio_device.h"

VgpioDevice::VgpioDevice(QObject *parent)
    : QObject(parent)
{
    m_index = 0;

    m_value = 0;
}

VgpioDevice::~VgpioDevice()
{

}

int VgpioDevice::index() const
{
    return m_index;
}

void VgpioDevice::setIndex(int index)
{
    m_index = index;
}

int VgpioDevice::value() const
{
    return m_value;
}

void VgpioDevice::setValue(int value)
{
    if (m_value == value)
    {
        return;
    }

    m_value = value;

    emit sigValueChanged(m_value);
}

VgpioDevice::DeviceType VgpioDevice::type() const
{
    return m_type;
}