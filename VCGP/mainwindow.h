/*******************************************************
 * File: mainwindow.h
 *******************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class QTextEdit;

class UsbCdcDevice;

class DeviceManager;

class ProtocolManager;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void slotLog(const QString &log);

private:
    UsbCdcDevice *m_usb;

    DeviceManager *m_devMgr;

    ProtocolManager *m_protoMgr;

    QTextEdit *m_logEdit;
};

#endif // MAINWINDOW_H