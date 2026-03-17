#include "protomodel.h"
#include <QSize>

#define DEFAULT_FIELD   (Field{"", 0})

#define DEFAULT_TYPE	(dataType[0])
#define BITFIELD_TYPE   (dataType[1])

extern const QVector<TypeInfo> dataType;

bool ProtoField::operator==(const ProtoField &other) const
{
    return (offset == other.offset);
}

QDebug operator<<(QDebug dbg, const ProtoField &f)
{
    dbg.nospace() << "ProtoField("
                  << "offset=" << f.offset
                  << ", type=" << f.type.name
                  << ", row=" << f.row
                  << ", continuation=" << f.continuation
                  << ", ownerOfRow=" << f.ownerRow
                  << ", field=[desc=" << f.field.desc
                  << ", val=" << f.field.val;
    dbg << "])";
    return dbg;
}

ProtoModel::ProtoModel(QObject *parent)
    : QAbstractTableModel{parent}
    , fields({})
{
    // setProtoSize(2);
}

ProtoModel::~ProtoModel()
{
    qDebug() << "ProtoModel destructe.";
}

int ProtoModel::rowCount(const QModelIndex &/*parent*/) const
{
    return fields.size();
}

int ProtoModel::columnCount(const QModelIndex &/*parent*/) const
{
    return COL_COUNT; // Offset, Type, Desc, Val, Prop
}

QVariant ProtoModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return {};

    if (orientation == Qt::Horizontal) {
        switch (section)
        {
        case OFFSET_COL:
            return "Byte";
        case TYPE_COL:
            return "Type";
        case DESC_COL:
            return "Desc";
        case VAL_COL:
            return "Val";
        case PROP_COL:
            return "Prop";
        }

    } else if (orientation == Qt::Vertical) {
        return (section + 1);
    }
    return {};
}

int ProtoModel::ownerOfRow(int row) const
{
    if (row < 0 || row >= fields.size())
        return -1;

    const ProtoField &f = fields[row];
    return f.continuation ? f.ownerRow : row;
}

QVariant ProtoModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return {};

    const ProtoField &f = fields[index.row()];

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column())
        {
        case OFFSET_COL: {
            return QString("Byte%1").arg(f.offset + 1);
        }
        case TYPE_COL: {
            if (f.continuation)
                return QString();
            return f.type.name;
        }
        case DESC_COL: {
            return f.field.desc;
        }
        case VAL_COL: {
            return f.field.val;
        }
        }
    }
    return {};
}

/* 数据类型切换到 bitField 时调用次函数 */
bool ProtoModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (row < 1 || count < 1)
        return false;

    beginInsertRows(parent, row, row + count - 1);

    const ProtoField &f = fields[row - 1];
    /* 添加 bitField 的后7行字段 */
    for (int r = row; r < row + count; ++r) {
        ProtoField bitField;
        bitField.offset = f.offset;
        bitField.type = BITFIELD_TYPE;
        bitField.field = DEFAULT_FIELD;
        bitField.row = r;
        bitField.continuation = false;
        bitField.ownerRow = bitField.row;
        fields.insert(fields.begin() + r, bitField);
    }
    /* 重置后续字段行号 */
    for (int r = row + count; r < rowCount(); ++r) {
        fields[r].row = r;
        fields[r].ownerRow = fields[r].continuation ? ownerOfRow(r) + count : ownerOfRow(r);
    }
    endInsertRows();
    return true;
}

/* bitField 切换成其他时调用 */
bool ProtoModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (row < 0 || count < 1 || row + count > rowCount())
        return false;

    beginRemoveRows(parent, row, row + count - 1);

    /* 移除 bitField 的后7行字段, 并重置后续字段行号*/
    fields.erase(fields.begin() + row, fields.begin() + row + count);
    for (int r = row; r < rowCount(); ++r) {
        fields[r].row = r;
        fields[r].ownerRow = fields[r].continuation ? ownerOfRow(r) - count : ownerOfRow(r);
    }
    endRemoveRows();
    return true;
}

bool ProtoModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole || !index.isValid())
        return false;

    ProtoField &f = fields[index.row()];

    switch (index.column())
    {
    case OFFSET_COL:
        return false;   /* 不可编辑 */

    case TYPE_COL: {
        int type_idx = value.toInt();
        const TypeInfo &type = dataType[type_idx];

        int rowCount = fields.size();
        int lastOwnerRow = ownerOfRow(rowCount - 1);
        int size = fields[lastOwnerRow].offset + fields[lastOwnerRow].type.size;

        if (index.row() + type.size > size)
            return false;

        /* 减少行合并 */
        if (f.type.size > type.size) {
            for (int r = f.row + type.size; r < f.row + f.type.rowSpan; r++) {
                fields[r].continuation = false;
                fields[r].ownerRow = r;
            }
            if (type == BITFIELD_TYPE) {
                /* insert() 会导致vector内存地址发生改变 */
                insertRows(f.row + 1, type.rowSpan -1);
            }
        }
        /* 增加行合并 */
        else if (f.type.size < type.size) {
            if (f.type == BITFIELD_TYPE) {
                removeRows(f.row + 1, f.type.rowSpan - 1);
            }
            for (int r = f.row + f.type.size; r < f.row +  type.rowSpan; r++) {
                if (fields[r].type == BITFIELD_TYPE) {
                    removeRows(fields[r].row + 1, fields[r].type.rowSpan - 1);
                }
                else if (fields[r].type.size > 1) 
                {
                    if (fields[r].row + fields[r].type.rowSpan > f.row + type.rowSpan) {
                        for (int i = f.row + type.rowSpan; i < fields[r].row + fields[r].type.rowSpan; ++i) {
                            fields[i].type = DEFAULT_TYPE;
                            fields[i].continuation = false;
                            fields[i].ownerRow = fields[i].row;
                        }
                    }
                }
                else
                {
                    /* fields[r].type == DEFAULT_TYPE */
                }
                fields[r].type = DEFAULT_TYPE;
                fields[r].field = DEFAULT_FIELD;
                fields[r].continuation = true;
                fields[r].ownerRow = f.row;
            }
        }
        /* 行跨相同 */
        else {
            if (f.type != type) {
                if (f.type == BITFIELD_TYPE) {
                    removeRows(f.row + 1, f.type.rowSpan - 1);
                } else if (type == BITFIELD_TYPE) {
                    insertRows(f.row + 1, type.rowSpan - 1);
                }
            }
        }
        /* f.type = type;	insert() 导致vector内存改变后, 通过 &f 修改数据失效 */
        fields[index.row()].type = type;
        break;
    }
    case DESC_COL: {
        f.field.desc = value.toString();
        break;
    }
    case VAL_COL: {
        f.field.val = value.toInt();
        break;
    }
    }
    /* ----------打印 ProtoField 内容---------- */
    for (const ProtoField &f : fields) {
        qDebug() << f;
    }
    qDebug() << "==============================";
    emit dataChanged(index, index);
    return true;
}

Qt::ItemFlags ProtoModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    switch (index.column())
    {
    case OFFSET_COL:
        return Qt::NoItemFlags;
    case TYPE_COL:
        return Qt::ItemIsEnabled | Qt::ItemIsEditable;
    case DESC_COL:
    case VAL_COL:
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    }

    return Qt::NoItemFlags;
}

QSize ProtoModel::span(const QModelIndex &index) const
{
    if (!index.isValid())
        return QSize(1, 1);

    const ProtoField &f = fields[index.row()];

    qDebug() << "model::span(): r" << index.row()
             << "c" << index.column() << "span" << f.type.rowSpan;
    return QSize(1, f.type.rowSpan);
}

int ProtoModel::protoSize() const
{
    int rowCount = fields.size();
    if (rowCount == 0)
        return 0;

    int lastOwnerRow = ownerOfRow(rowCount - 1);
    return fields[lastOwnerRow].offset + fields[lastOwnerRow].type.size;
}

void ProtoModel::setProtoSize(quint16 size)
{
    beginResetModel();

    int oldSize = protoSize();

    if (size == oldSize)
        return;

    int oldRowCount = fields.size();
    /* 减小 */
    if (size < oldSize) {
        if (size != 0) {
            int row = ownerOfRow(oldRowCount - 1);
            /* 将位于size大小的字段变为默认字段 */
            while (fields[row].offset > size) {
                row = ownerOfRow(--row);
            }
            if (fields[row].offset == size) {
                if (fields[row].type.size > 1) {
                    fields[row].type = DEFAULT_TYPE;
                    fields[row].field = DEFAULT_FIELD;
                }

            } else { /* fields[row].offset < size */
                for (int i = row; i < row + (size - fields[row].offset); ++i) {
                    fields[i].type = DEFAULT_TYPE;
                    fields[i].field = DEFAULT_FIELD;
                    fields[i].continuation = false;
                    fields[i].ownerRow = fields[i].row;
                }
            }
        }
        fields.resize(size);
    /* 变大 */
    } else {
        fields.resize(oldRowCount + (size - oldSize));

        for (int i = oldRowCount; i < fields.size(); ++i) {
            ProtoField &f = fields[i];
            f.offset = oldSize + (i - oldRowCount);
            f.type = DEFAULT_TYPE;
            f.field = DEFAULT_FIELD;
            f.row = i;
            f.continuation = false;
            f.ownerRow = f.row;
        }
    }
    endResetModel();
}

bool ProtoModel::isBitField(const QModelIndex &index) const
{
    if (!index.isValid())
        return false;

    const ProtoField &f = fields[index.row()];
    return (f.type == BITFIELD_TYPE);
}
