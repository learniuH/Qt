#include "protoview.h"


ProtoView::ProtoView(QWidget *parent)
    : QTableView(parent)
{
}

void ProtoView::setModel(QAbstractItemModel *model)
{
    QTableView::setModel(model);

    protoModel = qobject_cast<ProtoModel *>(model);
    if (!protoModel)
        return;

    connect(protoModel, &QAbstractItemModel::modelReset,
            this, &ProtoView::rebuildSpans);

    connect(protoModel, &QAbstractItemModel::dataChanged,
            this, &ProtoView::rebuildSpans);

    // connect(protoModel, &QAbstractItemModel::layoutChanged,
            // this, &ProtoView::rebuildSpans);
}

void ProtoView::rebuildSpans()
{

}
