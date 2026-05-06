/*******************************************************
 * File: mainwindow.cpp
 *******************************************************/

#include "mainwindow.h"

#include <QVBoxLayout>
#include <QGroupBox>
#include <QTextEdit>

#include "usb_cdc_device.h"

#include "device_manager.h"

#include "protocol_manager.h"

#include "din_widget.h"
#include "ain_widget.h"

#include "din_device.h"
#include "ain_device.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    resize(900, 600);

    /*
        通信层
    */
    m_usb = new UsbCdcDevice(this);

    /*
        设备层
    */
    m_devMgr = new DeviceManager(this);

    m_devMgr->initDIN(8);

    m_devMgr->initAIN(4);

    /*
        协议层
    */
    m_protoMgr =
            new ProtocolManager(this);

    m_protoMgr->setUsbDevice(m_usb);

    m_protoMgr->setDeviceManager(
                m_devMgr);

    m_protoMgr->init();

    /*
        UI
    */
    QVBoxLayout *mainLayout =
            new QVBoxLayout(this);

    /*
        DIN
    */
    QGroupBox *dinGroup =
            new QGroupBox("DIN");

    QVBoxLayout *dinLayout =
            new QVBoxLayout(dinGroup);

    for (auto dev : m_devMgr->dinList())
    {
        DinWidget *widget =
                new DinWidget(dev);

        dinLayout->addWidget(widget);
    }

    /*
        AIN
    */
    QGroupBox *ainGroup =
            new QGroupBox("AIN");

    QVBoxLayout *ainLayout =
            new QVBoxLayout(ainGroup);

    for (auto dev : m_devMgr->ainList())
    {
        AinWidget *widget =
                new AinWidget(dev);

        ainLayout->addWidget(widget);
    }

    /*
        日志
    */
    m_logEdit = new QTextEdit(this);

    m_logEdit->setReadOnly(true);

    mainLayout->addWidget(dinGroup);

    mainLayout->addWidget(ainGroup);

    mainLayout->addWidget(m_logEdit);

    /*
        日志
    */
    connect(m_usb,
            &UsbCdcDevice::sigLog,
            this,
            &MainWindow::slotLog);

    /*
        打开串口
    */
    m_usb->open("COM5");
}

void MainWindow::slotLog(const QString &log)
{
    m_logEdit->append(log);
}