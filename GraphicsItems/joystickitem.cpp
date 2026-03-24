#include "joystickitem.h"
#include <QPainter>

#define HIGH_LIGHT(x) ((x) ? QColor(52, 160, 220) : Qt::white)


void drawArcText(QPainter *painter, 
                 const QString &text, 
                 qreal midAngle,
                 qreal angleStep, 
                 qreal radius, 
                 Direction position)
{
    if (!painter || text.isEmpty()) 
        return;

    int textCount = text.length();
    double startAngle = 0.0;

    if (textCount % 2 == 0) {
        startAngle = (textCount <= 2) ? midAngle - angleStep/2
                                      : midAngle + angleStep/2 - textCount/2 * angleStep;
    }
    else {
        startAngle = (textCount < 2) ? midAngle
                                     : midAngle - textCount/2 * angleStep;
    }

    switch (position) 
    {
    case Up:
    case Right: {
        for (int i = 0; i < textCount; ++i) {
            qreal angle = startAngle + i * angleStep;

            qreal x = radius * cos(angle);
            qreal y = radius * sin(angle);

            painter->save();
            painter->translate(x, y);
            painter->rotate(angle * 180 / M_PI + 90);

            painter->drawText(0, 0, text[i]);
            painter->restore();
        }
        break;
    }
    case Left:
    case Down:
        for (int i = textCount - 1; i >= 0; --i) {
            qreal angle = startAngle + i * angleStep;

            qreal x = radius * cos(angle);
            qreal y = radius * sin(angle);

            painter->save();
            painter->translate(x, y);
            painter->rotate(angle * 180 / M_PI + 90);

            painter->drawText(0, 0, text[textCount - 1 - i]);
            painter->restore();
        }
        break;
    }
}

JoyStickItem::JoyStickItem(qreal size, QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , w(size)
    , knobPos(0, 0)
    , direction(Default)
{
    setAcceptedMouseButtons(Qt::LeftButton);
}

QRectF JoyStickItem::boundingRect() const
{
    return QRectF(-w/2, -w/2, w, w);
}

void JoyStickItem::paint(QPainter *painter, 
                         const QStyleOptionGraphicsItem * /* option */, 
                         QWidget * /* widget */)
{
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(Qt::NoPen);
    
    /* 背景圆 */
    QLinearGradient g1(0, -w/2, 0, w/2);
    g1.setColorAt(0, QColor(74, 68, 68));
    g1.setColorAt(1, QColor(158, 156, 156));
    painter->setBrush(g1);
    painter->drawEllipse(QPointF(0, 0), w/2, w/2);

    /* 外圈 */
    QRadialGradient g2(0, 0, w/2 - 5, 0, 0);
    g2.setColorAt(0, QColor(110, 110, 110));
    g2.setColorAt(0.8, QColor(35, 35, 35));
    g2.setColorAt(1, QColor(14, 14, 14));
    painter->setBrush(g2);
    painter->drawEllipse(QPointF(0, 0), w/2 - 5, w/2 - 5);

    /* 轨道 */
    QRadialGradient g3(0, 0, w/2 - 5, 0, 0);
    g3.setColorAt(0, QColor(95, 95, 95));
    g3.setColorAt(0.4, QColor(35, 35, 35));
    g3.setColorAt(1, QColor(0, 0, 0));
    painter->setBrush(g3);
    painter->drawRoundedRect(-w/2 + 5, -w/9, w - 10, 2*w/9, w/9, w/9);  // Horizontal
    painter->drawRoundedRect(-w/9, -w/2 + 5, 2*w/9, w - 10, w/9, w/9);   // Vertical

    /* 摇杆背景 */
    QRadialGradient g4(0, 0, w/6 + 5, 0, 0);
    g4.setColorAt(0, QColor(95, 95, 95));
    g4.setColorAt(1, QColor(45, 45, 45));
    painter->setBrush(g4);
    painter->drawEllipse(QPointF(0, 0), w/6 + 5, w/6 + 5);

    /* 上下左右 */
    qreal x = w/4;
    qreal x_ = x + w/15;
    qreal y = w/30;

    painter->setBrush(HIGH_LIGHT(direction & Right));
    painter->drawPolygon(QPolygonF({QPointF(x, -y), QPointF(x, y), QPointF(x_, 0)}));

    painter->setBrush(HIGH_LIGHT(direction & Left));
    painter->drawPolygon(QPolygonF({QPointF(-x, -y), QPointF(-x, y), QPointF(-x_, 0)}));

    painter->setBrush(HIGH_LIGHT(direction & Up));
    painter->drawPolygon(QPolygonF({QPointF(-y, -x), QPointF(y, -x), QPointF(0, -x_)}));

    painter->setBrush(HIGH_LIGHT(direction & Down));
    painter->drawPolygon(QPolygonF({QPointF(-y, x), QPointF(y, x), QPointF(0, x_)}));

    /* 可拖拽摇杆 */
    QRadialGradient g5(0, 0, w/6, 0, 0);
    g5.setColorAt(0, QColor(80, 80, 80));
    g5.setColorAt(0.5, QColor(40, 40, 40));
    g5.setColorAt(1, QColor(15, 15, 15));
    painter->setBrush(g5);
    painter->drawEllipse(knobPos, w/6, w/6);

    /* 文字描述 */
    QString textRight("大臂举升");
    QString textLeft("大臂降下");
    QString textUp("铲斗卸料");
    QString textDown("铲斗装料");

    qreal angleStep = 0.35;
    painter->setPen(HIGH_LIGHT(direction & Right));
    drawArcText(painter, textRight,        - 0.1,  angleStep, 3*w/8, Right);
    painter->setPen(HIGH_LIGHT(direction & Down));
    drawArcText(painter, textDown,  M_PI/2 - 0.1,  angleStep, 3*w/8, Down);
    painter->setPen(HIGH_LIGHT(direction & Left));
    drawArcText(painter, textLeft,  M_PI   - 0.1,  angleStep, 3*w/8, Left);
    painter->setPen(HIGH_LIGHT(direction & Up));
    drawArcText(painter, textUp,   -M_PI/2 - 0.1,  angleStep, 3*w/8, Up);

}

Direction JoyStickItem::calcDirection(qreal x, qreal y)
{
    qreal min_ = w/10;
    
    if (abs(x) > abs(y)) {
        if (x > min_) return Right;
        if (x < -min_) return Left;
    }
    else {
        if (y > min_) return Down;
        if (y < -min_) return Up;
    }

    return Default;
}

void JoyStickItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * /* event */)
{
    isMove = false;
    knobPos = QPointF(0, 0);
    direction = Default;

    update();
}

void JoyStickItem::mousePressEvent(QGraphicsSceneMouseEvent * /* event */)
{
    isMove = true;
}

void JoyStickItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (!isMove) return;

    QPointF p = event->pos();
    qreal x = p.x();
    qreal y = p.y();
    
    qreal max_ = w/3 + w/6/2;
    
    if (abs(x) > abs(y)) {
        y = 0;
    }
    else {
        x = 0;
    }
        
    if (abs(x) > max_) 
        x = max_ * x / abs(x);
    if (abs(y) > max_)
        y = max_ * y / abs(y);

    knobPos = QPointF(x, y);

    Direction dir = calcDirection(x, y);
    if (dir != direction) {
        direction = dir;
    }

    update();
}
