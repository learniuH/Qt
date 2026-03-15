#ifndef PROTOVIEW_H
#define PROTOVIEW_H

#include <QTableView>

#include "protomodel.h"

class ProtoView : public QTableView
{
    Q_OBJECT
public:
    explicit ProtoView(QWidget *parent = nullptr);

    void setModel(QAbstractItemModel *model) override;

private slots:

    void rebuildSpans();

private:
    ProtoModel *protoModel;

};

#endif // PROTOVIEW_H
