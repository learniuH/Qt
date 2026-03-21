#include "joystickitem.h"
#include <QPainter>

JoyStickItem::JoyStickItem(JoyStickMode mode, qreal radius, QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , m_mode(mode)
    , m_radius(radius)
    , m_handleRadius(radius / 3)
    , m_offset(0, 0)
{
    setAcceptedMouseButtons(Qt::LeftButton);
}

QRectF JoyStickItem::boundingRect() const
{
    return QRectF(-m_radius, -m_radius, m_radius * 2, m_radius * 2);
}

void JoyStickItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing);
    
    /* 背景圆 */
    painter->setBrush(QColor(200, 200, 200, 100));
    painter->setPen(Qt::NoPen);
    painter->drawEllipse(QPointF(0, 0), m_radius, m_radius);

    /* 摇杆 */
    painter->setBrush(QColor(100, 100, 255, 100));
    painter->drawEllipse(QPointF(0, 0), m_handleRadius, m_handleRadius);
}

void JoyStickItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_offset = event->pos();
    update();
}

void JoyStickItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF delta = event->pos();
    
    switch (m_mode) {
    case Free:
        break;
    
    case Cross:
        if (fabs(delta.x() > fabs(delta.y()))) {
            delta.setY(0);
        }
        else {
            delta.setX(0);
        }
        break;

    case Horizontal:
        delta.setY(0);
        break;

    case Vertical:
        delta.setX(0);
        break;
    }

    /* 限制在圆内 */
    qreal dist = std::sqrt(delta.x()*delta.x() + delta.y()*delta.y());
    if (dist > m_radius) {
        delta = delta / dist * m_radius;
    }

    m_offset = delta;

    /* 归一化方向(-1, 1) */
    QPointF dir = delta / m_radius;

    emit directionChanged(dir);
    update();
}

void JoyStickItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    m_offset = event->pos();

    emit directionChanged(QPointF(0, 0));
    emit released();
    update();
}
