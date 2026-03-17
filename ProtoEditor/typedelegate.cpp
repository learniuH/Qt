#include "typedelegate.h"
#include <QTimer>

extern const QVector<TypeInfo> dataType = {
    {.name=UINT8_NAME,      .size=1,    .rowSpan=1},
    {.name=BITFIELD_NAME,   .size=1,    .rowSpan=8},
    {.name=UINT16_NAME,     .size=2,    .rowSpan=2},
    {.name=UINT32_NAME,     .size=4,    .rowSpan=4},
    {.name=FLOAT_NAME,      .size=4,    .rowSpan=4},
    {.name=DOUBLE_NAME,     .size=8,    .rowSpan=8}
};

bool TypeInfo::operator==(const TypeInfo &other) const
{
    return name == other.name;
}

bool TypeInfo::operator!=(const TypeInfo &other) const
{
    return name != other.name;
}

ComboItemDelegate::ComboItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

ComboItemDelegate::~ComboItemDelegate()
{
    qDebug() << "ComboDelegate destructe.";
}

QWidget *ComboItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QComboBox *editor = new QComboBox(parent);
    for (const TypeInfo &t : dataType) {
        editor->addItem(t.name);
    }

    QTimer::singleShot(0, editor, [editor]() {
        editor->showPopup();
    });

    return editor;
}

void ComboItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();
    QComboBox *combo = static_cast<QComboBox *>(editor);
    quint8 idx = combo->findText(value);
    combo->setCurrentIndex(idx);
}

void ComboItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *combo = static_cast<QComboBox *>(editor);
    if (!combo) return;
    model->setData(index, combo->currentIndex(), Qt::EditRole);
}

void ComboItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
