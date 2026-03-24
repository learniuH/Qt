#ifndef JOYSTICKITEM_H
#define JOYSTICKITEM_H

#include <QGraphicsObject>
#include <QPointF>
#include <QGraphicsSceneMouseEvent>

enum Direction {
    Default = 0x00,
    Up      = 0x01,
    Down    = 0x02,
    Left    = 0x04,
    Right   = 0x08
};

class JoyStickItem : public QGraphicsObject
{
    Q_OBJECT
public:
    // enum JoyStickMode {
        // Free,       // 万向
        // Cross,      // 十字
        // Horizontal, // 一字 (X轴)
        // Vertical    // 一字 (Y轴)
    // };
    explicit JoyStickItem(qreal size = 200, QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;

    void paint(QPainter *painter, 
               const QStyleOptionGraphicsItem * /* option */, 
               QWidget * /* widget */) override;

signals:
    // void directionChanged(QPointF dir); // (-1 ~ 1, -1 ~ 1)
    // void released();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    qreal w;
    QPointF knobPos;
    bool isMove = false;

    Direction direction;

    Direction calcDirection(qreal x, qreal y);
};

#endif // JOYSTICKITEM_H
