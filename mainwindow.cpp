
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "protomodel.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ProtoModel *model1 = new ProtoModel(this);
    ui->tableView->setModel(model1);
    ComboItemDelegate *comboDelegate = new ComboItemDelegate(this);
    ui->tableView->setItemDelegateForColumn(1, comboDelegate);

    ProtoModel *model2 = new ProtoModel(this);
    ui->protoView->setModel(model2);
    ui->protoView->setItemDelegateForColumn(1, new ComboItemDelegate(this));

    connect(ui->protoSizeLine, &QLineEdit::editingFinished, [this, model1, model2](){
        int size = ui->protoSizeLine->text().toInt();
        model1->setProtoSize(size);
        model2->setProtoSize(size);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
