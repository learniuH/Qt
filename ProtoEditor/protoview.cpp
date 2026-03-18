#include "protoview.h"
#include <QSize>

#define DEFAULT_RSPAN (1)
#define BITFIELD_RSPAN (8)

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
        // [this]() {
        // }
    // );

    // connect(protoModel, &QAbstractItemModel::layoutChanged,
            // this, &ProtoView::rebuildSpans);
    
    connect(protoModel, &QAbstractItemModel::dataChanged,
        [this](const QModelIndex &topLeft, const QModelIndex &bottomRight, const QList<int> &roles = QList<int>()){
            if (topLeft != bottomRight)
                return;

            int row = topLeft.row();
            if (topLeft.column() == TYPE_COL) {
                bool isBitField = protoModel->isBitField(topLeft);

                QSize s = protoModel->span(protoModel->index(row, TYPE_COL));
                this->setSpan(row, OFFSET_COL, isBitField ? BITFIELD_RSPAN : DEFAULT_RSPAN, s.width());
                this->setSpan(row, TYPE_COL, s.height(), s.width());
                this->setSpan(row, DESC_COL, isBitField ? DEFAULT_RSPAN : s.height(), s.width());
                this->setSpan(row, VAL_COL, isBitField ? DEFAULT_RSPAN : s.height(), s.width());
            }
        }
    );
}
