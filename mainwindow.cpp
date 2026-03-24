
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "ProtoEditor/protomodel.h"

#include "GraphicsItems/joystickitem.h"
#include <QGraphicsScene>

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
    scene->addItem(joyStick);
    // joyStick->setPos(100, 100);
    
    ui->graphicsView->setScene(scene);
}

MainWindow::~MainWindow()
{
    delete ui;
}
