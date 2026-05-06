#ifndef SERIAL_H
#define SERIAL_H

#include <QDialog>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QList>

namespace Ui {
class Dialog;
} // namespace Ui

class Serial : public QDialog
{
    Q_OBJECT

public:
    Serial(QSerialPort *serial, QWidget *parent);
    ~Serial();

    void updateAvailablePorts();

private:
    Ui::Dialog *ui;
    QSerialPort *m_serial;
    QList<QSerialPortInfo> infos;   /* 可用的端口 */
};

#endif // SERIAL_H
