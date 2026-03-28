#ifndef SWITCHITEM_H
#define SWITCHITEM_H

#include <QGraphicsObject>

class ButtonItem : public QGraphicsObject
{
public:
    explicit ButtonItem(qreal size = 150, QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;

    void paint(QPainter *painter, 
               const QStyleOptionGraphicsItem * /*option*/, 
               QWidget */*widget*/) override;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    qreal w;
    bool state;
};

#endif  // SWITCHITEM_H