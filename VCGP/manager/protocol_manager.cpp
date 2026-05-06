/*******************************************************
 * File: protocol_manager.cpp
 *******************************************************/

#include "protocol_manager.h"

#include "usb_cdc_device.h"

#include "device_manager.h"

#include "din_device.h"
#include "ain_device.h"

ProtocolManager::ProtocolManager(QObject *parent)
    : QObject(parent)
{
    m_usb = nullptr;

    m_devMgr = nullptr;
}

void ProtocolManager::setUsbDevice(
        UsbCdcDevice *usb)
{
    m_usb = usb;
}

void ProtocolManager::setDeviceManager(
        DeviceManager *mgr)
{
    m_devMgr = mgr;
}

void ProtocolManager::init()
{
    if (!m_usb || !m_devMgr)
    {
        return;
    }

    /*
        USB -> Protocol
    */
    connect(m_usb,
            &UsbCdcDevice::sigFrameReceived,
            this,
            &ProtocolManager::slotFrameReceived);

    /*
        Device -> Protocol
    */
    for (auto dev : m_devMgr->dinList())
    {
        connect(dev,
                &DinDevice::sigValueChanged,
                this,
                [=](int value)
        {
            slotDinChanged(dev->index(),
                           value);
        });
    }

    for (auto dev : m_devMgr->ainList())
    {
        connect(dev,
                &AinDevice::sigValueChanged,
                this,
                [=](int value)
        {
            slotAinChanged(dev->index(),
                           value);
        });
    }
}

void ProtocolManager::slotFrameReceived(
        const VgpioProtocol::Frame &frame)
{
    processFrame(frame);
}

void ProtocolManager::slotDinChanged(
        int index,
        int value)
{
    if (!m_usb)
    {
        return;
    }

    m_usb->writeDIN(index,
                    value);
}

void ProtocolManager::slotAinChanged(
        int index,
        int value)
{
    if (!m_usb)
    {
        return;
    }

    m_usb->writeAIN(index,
                    value);
}

void ProtocolManager::processFrame(
        const VgpioProtocol::Frame &frame)
{
    if (!m_devMgr)
    {
        return;
    }

    /*
        STM32 -> UI同步
    */

    switch (frame.devType)
    {
        case VgpioProtocol::DEV_DIN:
        {
            if (frame.devIndex >=
                    m_devMgr->dinList().size())
            {
                return;
            }

            auto dev =
                    m_devMgr->dinList()
                    [frame.devIndex];

            /*
                防止重复触发发送
            */
            dev->blockSignals(true);

            dev->setValue(frame.value);

            dev->blockSignals(false);

            break;
        }

        case VgpioProtocol::DEV_AIN:
        {
            if (frame.devIndex >=
                    m_devMgr->ainList().size())
            {
                return;
            }

            auto dev =
                    m_devMgr->ainList()
                    [frame.devIndex];

            dev->blockSignals(true);

            dev->setValue(frame.value);

            dev->blockSignals(false);

            break;
        }

        default:
            break;
    }
}