#include "protoview.h"
#include <QSize>

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

    // connect(protoModel, &QAbstractItemModel::modelReset,
    //         this, &ProtoView::rebuildSpans);

    // connect(protoModel, &QAbstractItemModel::layoutChanged,
            // this, &ProtoView::rebuildSpans);
    
    connect(protoModel, &QAbstractItemModel::dataChanged,
        [this](const QModelIndex &topLeft, const QModelIndex &bottomRight, const QList<int> &roles = QList<int>()){
            if (topLeft != bottomRight)
                return;

            int row = topLeft.row();
            if (topLeft.column() == TYPE_COL) {
                QSize s = protoModel->span(protoModel->index(row, TYPE_COL));
                this->setSpan(row, TYPE_COL, s.height(), s.width());
                int rowSpan = protoModel->isBitField(topLeft) ? 1 : s.height();
                int colSpan = protoModel->isBitField(topLeft) ? 1 : s.width();
                this->setSpan(row, DESC_COL, rowSpan, colSpan);
                this->setSpan(row, VAL_COL, rowSpan, colSpan);
            }
        }
    );
}
