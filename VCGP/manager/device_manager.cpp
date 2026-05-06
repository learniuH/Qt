/*******************************************************
 * File: device_manager.cpp
 *******************************************************/

#include "device_manager.h"

#include "din_device.h"
#include "ain_device.h"

DeviceManager::DeviceManager(QObject *parent)
    : QObject(parent)
{

}

void DeviceManager::initDIN(int count)
{
    for (int i = 0; i < count; i++)
    {
        DinDevice *dev =
                new DinDevice(this);

        dev->setIndex(i);

        m_dinList.append(dev);
    }
}

void DeviceManager::initAIN(int count)
{
    for (int i = 0; i < count; i++)
    {
        AinDevice *dev =
                new AinDevice(this);

        dev->setIndex(i);

        m_ainList.append(dev);
    }
}

QList<DinDevice*> DeviceManager::dinList() const
{
    return m_dinList;
}

QList<AinDevice*> DeviceManager::ainList() const
{
    return m_ainList;
}