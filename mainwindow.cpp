#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "ProtoEditor/protomodel.h"

#include "GraphicsItems/joystickitem.h"
#include "GraphicsItems/switchItem.h"
#include <QGraphicsScene>

#include <QSerialPort>
#include <QSerialPortInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ProtoModel *model2 = new ProtoModel(this);
    ui->protoView->setModel(model2);
    ui->protoView->setItemDelegateForColumn(1, new ComboItemDelegate(this));
    // ui->protoView->setEditTriggers(
        // QAbstractItemView::CurrentChanged | /* 切换单元格进入编辑 */
        // QAbstractItemView::SelectedClicked  /* 单击已选中项进入编辑 */
    // );

    connect(ui->protoSizeLine, &QLineEdit::editingFinished, [this, model2](){
        int size = ui->protoSizeLine->text().toInt();
        model2->setProtoSize(size);
    });

    QGraphicsScene *scene = new QGraphicsScene(this);
    JoyStickItem *joyStick = new JoyStickItem();
    joyStick->setPos(QPointF(-200, 0));
    ButtonItem * sw = new ButtonItem();
    sw->setPos(QPointF(80, 0));
    scene->addItem(joyStick);
    scene->addItem(sw);
    
    ui->graphicsView->setScene(scene);



    /* Qt serial 操作 */
    QSerialPort *m_serial = new QSerialPort(this);

    /* 串口故障时，触发errorOccurred信号 */
    connect(m_serial, &QSerialPort::errorOccurred, [m_serial](QSerialPort::SerialPortError error){
        if (QSerialPort::ResourceError == error) {
            qDebug() << "Critical Error" << m_serial->errorString();
            m_serial->close();
        }
    });

    /* 串口收到数据时，触发readyRead信号 */
    connect(m_serial, &QSerialPort::readyRead, [m_serial](){
        const QByteArray data = m_serial->readAll();
    });

    /* 查看可用的串口 */
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        qDebug() << info.serialNumber();
        qDebug() << info.portName();
    }


    // QSerialPort::Baud115200
    // m_serial->setPortName();
}

MainWindow::~MainWindow()
{
    delete ui;
}
