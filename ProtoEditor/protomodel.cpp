#include "protomodel.h"
#include <algorithm>
#include <QSize>

#define DEFAULT_FIELD   (Field{"", 0})
#define DEFAULT_TYPE	(dataType[0])

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
    return protoSize();
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

// quint16 ProtoModel::offsetOfRow(int row) const
// {
    // if (fields.empty())
        // return 0;

    // quint16 i = 0;  /* 遍历行索引 */
    // quint16 offset = fields[i].offset;
    // while (i < row) {
        // if (i + fields[offset].tkkkkkkkpe.rowSpan > row) {
            // return offset;
        // }
        // i += fields[offset].type.rowSpan;
        // offset++;
    // }
    // return offset;
// }

void ProtoModel::procFieldRow(const QModelIndex &index, ProtoField &f, quint8 type_idx)
{
    /* 当前是 bitField */
    if (f.type.name == BITFIELD_NAME) {
        auto it = std::find(fields.begin(), fields.end(), f);
        if (it != fields.end() && std::distance(it, fields.end()) > f.type.rowSpan) {
            /* 移除 bitField 的7行字段 */
            int first = std::distance(fields.begin(), it);
            beginRemoveRows(index, first, first + 7);
            fields.erase(it + 1, it + f.type.rowSpan);
            endRemoveRows();
            /* 后续字段行号 -7 */
            for (; it != fields.end(); ++it) {
                it->row -= 7;
            }
        }
    /* 变成 bitField */
    } else if (dataType[type_idx].name == BITFIELD_NAME) {
        auto it = std::find(fields.begin(), fields.end(), f);
        if (it != fields.end()) {
            /* 这个函数需要重新写，这么一直找 f 太啰嗦了 */
        }
        /* 添加 bitField 的7行字段 */
        for (int r = f.row + 1; r < f.row + f.type.rowSpan; ++r) {
            ProtoField bitField;
            bitField.offset = f.offset;
            bitField.type = dataType[type_idx];
            bitField.field = DEFAULT_FIELD;
            bitField.row = r;
            bitField.continuation = false;
            bitField.ownerRow = bitField.row;
            fields.insert(it + 1 + (r - f.row), bitField);
        }
        /* 后续字段行号 +7 */
        for (it += dataType[type_idx].rowSpan; it != fields.end(); ++it) {
            it->row += 7;
        }
    }
    f.field = DEFAULT_FIELD;
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
        int type_span = dataType[type_idx].rowSpan;
        int type_size = dataType[type_idx].size;

        int rowCount = fields.size();
        int lastOwnerRow = ownerOfRow(rowCount - 1);
        int size = fields[lastOwnerRow].offset + fields[lastOwnerRow].type.size;

        if (index.row() + type_size > size)
            return false;

        /* 处理 bitField 造成的 row 改变 */

        /* 减少行合并 */
        if (f.type.size > type_size) {
            for (int i = f.row + type_size; i < f.row + f.type.size; i++) {
                fields[i].continuation = false;
                fields[i].ownerRow = i;
            }
            procFieldRow(f, type_idx);

        /* 增加行合并 */
        } else if (f.type.size < type_size) {
            for (int i = f.row + f.type.size; i < (f.row + f.type.size) + (type_size - f.type.size); i++) {
                if (fields[i].type.name != UINT8_NAME) {
                    if (fields[i].row + fields[i].type.size > f.row + type_size) {
                        for (int j = f.row + type_size; j < fields[i].row + fields[i].type.size; ++j) {
                            fields[j].continuation = false;
                            fields[j].ownerRow = fields[j].row;
                        }
                    }
                    fields[i].type = DEFAULT_TYPE;
                }
                fields[i].field = DEFAULT_FIELD;
                fields[i].continuation = true;
                fields[i].ownerRow = f.row;
            }
            procFieldRow(f, type_idx);
        } else {
            /* 行跨相同 */
            procFieldRow(index, f, type_idx);
        }
        f.type = dataType[type_idx];
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

    qDebug() << "model::span(): r" << index.row() << "c" << index.column() << "span" << f.type.rowSpan;
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
    return (f.type.name == BITFIELD_NAME);
}