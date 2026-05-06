/*******************************************************
 * File: protocol_manager.h
 * Description:
 *      协议管理器
 *
 * 职责：
 *  1. 管理协议收发
 *  2. 管理设备同步
 *  3. UI 与 USB 解耦
 *  4. Frame -> Device
 *  5. Device -> Frame
 *******************************************************/

#ifndef PROTOCOL_MANAGER_H
#define PROTOCOL_MANAGER_H

#include <QObject>

#include "vgpio_protocol.h"

class UsbCdcDevice;

class DeviceManager;

class DinDevice;
class AinDevice;

class ProtocolManager : public QObject
{
    Q_OBJECT

public:
    explicit ProtocolManager(QObject *parent = nullptr);

public:
    void setUsbDevice(UsbCdcDevice *usb);

    void setDeviceManager(DeviceManager *mgr);

public:
    /*
        初始化协议连接
    */
    void init();

private slots:
    /*
        USB收到协议帧
    */
    void slotFrameReceived(
            const VgpioProtocol::Frame &frame);

    /*
        DIN变化
    */
    void slotDinChanged(int index,
                        int value);

    /*
        AIN变化
    */
    void slotAinChanged(int index,
                        int value);

private:
    /*
        协议 -> Device
    */
    void processFrame(
            const VgpioProtocol::Frame &frame);

private:
    UsbCdcDevice *m_usb;

    DeviceManager *m_devMgr;
};

#endif // PROTOCOL_MANAGER_H