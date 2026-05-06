/*******************************************************
 * File: device_manager.h
 * Description:
 *      设备管理器
 *******************************************************/

 #ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H

#include <QObject>
#include <QList>

class DinDevice;
class AinDevice;

class DeviceManager : public QObject
{
    Q_OBJECT

public:
    explicit DeviceManager(QObject *parent = nullptr);

public:
    void initDIN(int count);

    void initAIN(int count);

public:
    QList<DinDevice*> dinList() const;

    QList<AinDevice*> ainList() const;

private:
    QList<DinDevice*> m_dinList;

    QList<AinDevice*> m_ainList;
};

#endif // DEVICE_MANAGER_H