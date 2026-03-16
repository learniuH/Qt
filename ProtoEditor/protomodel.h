#ifndef PROTOMODEL_H
#define PROTOMODEL_H

#include <QAbstractTableModel>
#include <QVector>

#include "typedelegate.h"


#define OFFSET_COL      (0)
#define TYPE_COL        (1)
#define DESC_COL        (2)
#define VAL_COL         (3)
#define PROP_COL        (4)    
#define COL_COUNT       (5)

struct Field {
    QString desc;
    int val;
};

struct ProtoField {
    int offset;        // 字节偏移（可以用于显示 ByteN）
    TypeInfo type;         // 当前字段类型（只有 owner 有效）
    Field field;
    int row;               // 字段在 model 中的行号
    bool continuation; // 如果 true，表示这是 owner 之后的被合并行, bitField 为 true(通过offset判断字节号)
    int ownerRow;           // 如果 continuation==true，指向 owner 字段的行号



    /* Deprecated */
    int ownerOffset = 0;      // 如果 continuation==true，指向 owner 字段的offset
    /* Deprecate */
    QVector<Field> fields; // owner 用的每个子行的数据，size == rowSpan

    bool operator==(const ProtoField &other) const;
    friend QDebug operator<<(QDebug dbg, const ProtoField &f);
};

class ProtoModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ProtoModel(QObject *parent = nullptr);

    ~ProtoModel();

    // QModelIndex index(int row, int column,
                              // const QModelIndex &parent = QModelIndex()) const override;

    // QModelIndex parent(const QModelIndex &child) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    /**
     * @param section: 表头的索引，水平表头代表列号，垂直表头代表行号
     */
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QSize span(const QModelIndex &index) const override;



    void setProtoSize(quint16 size);

    bool isBitField(const QModelIndex &index) const;

private:
    QVector<ProtoField> fields;

    int ownerOfRow(int row) const;

    /* Deprecated */
    // quint16 offsetOfRow(int row) const;

    void procFieldRow(const QModelIndex &index, ProtoField &f, quint8 type_idx);

    int protoSize() const;
};

#endif // PROTOMODEL_H
