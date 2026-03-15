#ifndef TYPEDELEGATE_H
#define TYPEDELEGATE_H

#include <QComboBox>
#include <QStyledItemDelegate>

#define UINT8_NAME          ("uint8")
#define BITFIELD_NAME       ("bitField")
#define UINT16_NAME		    ("uint16")
#define UINT32_NAME		    ("uint32")
#define FLOAT_NAME		    ("float")
#define DOUBLE_NAME 	    ("double")

struct TypeInfo {
    QString name;
    int size;
    int rowSpan;

    bool operator==(const TypeInfo &other) const;
};

class ComboItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    ComboItemDelegate(QObject *parent);

    ~ComboItemDelegate();

    /* item 被双击后进入编辑状态触发 */
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    void setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;
};

#endif // TYPEDELEGATE_H
