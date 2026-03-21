#ifndef JOYSTICKITEM_H
#define JOYSTICKITEM_H

#include <QGraphicsObject>
#include <QPointF>
#include <QGraphicsSceneMouseEvent>

class JoyStickItem : public QGraphicsObject
{
    Q_OBJECT
public:
    enum JoyStickMode {
        Free,       // 万向
        Cross,      // 十字
        Horizontal, // 一字 (X轴)
        Vertical    // 一字 (Y轴)
    };
    explicit JoyStickItem(JoyStickMode mode = Free, qreal radius = 50, QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);

signals:
    void directionChanged(QPointF dir); // (-1 ~ 1, -1 ~ 1)
    void released();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    JoyStickMode m_mode;       /* 摇杆类型 */
    qreal m_radius;             /* 最大半径*/
    qreal m_handleRadius;       /* 小球半径 */
    QPointF m_offset;           /* 当前偏移 */
};

// struct TypeInfo {
    // QString name;
    // int size;
    // int rowSpan;
// 
    // bool operator==(const TypeInfo &other) const;
    // bool operator!=(const TypeInfo &other) const;
// };
// 
// class ComboItemDelegate : public QStyledItemDelegate
// {
    // Q_OBJECT
// public:
    // ComboItemDelegate(QObject *parent);
// 
    // ~ComboItemDelegate();
// 
    // /* item 被双击后进入编辑状态触发 */
    // QWidget *createEditor(QWidget *parent,
                        //   const QStyleOptionViewItem &option,
                        //   const QModelIndex &index) const override;
// 
    // void setEditorData(QWidget *editor, const QModelIndex &index) const override;
// 
    // void setModelData(QWidget *editor,
                    //   QAbstractItemModel *model,
                    //   const QModelIndex &index) const override;
// 
    // void updateEditorGeometry(QWidget *editor,
                            //   const QStyleOptionViewItem &option,
                            //   const QModelIndex &index) const override;
// protected:
// 
// };

#endif // JOYSTICKITEM_H
