#include "ui_serialsettings.h"
#include "serial.h"

Serial::Serial(QSerialPort *serial, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
    , m_serial(serial)
{
    ui->setupUi(this);

    connect(ui->serialPortCombo, &QComboBox::currentIndexChanged, [this](int index){
        const QSerialPortInfo &info = infos[index];
        QString description = info.description();
        QString manufacturer = info.manufacturer();
        QString serialNumber = info.serialNumber();
        QString serialInfo = 
        "Description:"        + (!description.isEmpty()   ? description  : "N/A") + '\n' +
        "Manufacturer:"       + (!manufacturer.isEmpty()  ? manufacturer : "N/A") + '\n' +
        "SerialNumber:"       + (!serialNumber.isEmpty()  ? serialNumber : "N/A") + '\n' +
        "Localtion:"          + (info.systemLocation())                           + '\n' +
        "Vendor Identifier:"  + (info.vendorIdentifier()  ? QString::number(info.vendorIdentifier(),  16) : "N/A") + '\n' +
        "Product Identifier:" + (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : "N/A");

        ui->serialPortInfo->setText(serialInfo);
    });
}

Serial::~Serial()
{
    delete ui;
}
    
void Serial::updateAvailablePorts()
{
    infos = QSerialPortInfo::availablePorts();

    for (const QSerialPortInfo &info : infos)
    {
        ui->serialPortCombo->addItem(info.portName());
    }
}