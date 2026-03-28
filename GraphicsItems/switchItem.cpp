#include <QPainter>
#include "switchItem.h"

#define BUTTON_DOWN (0)
#define BUTTON_UP   (!BUTTON_DOWN)


ButtonItem::ButtonItem(qreal size, QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , w(size)
    , state(BUTTON_UP)
{
    setAcceptedMouseButtons(Qt::LeftButton);
}

QRectF ButtonItem::boundingRect() const
{
    // return QRectF(-w/2, -w, w, 2*w);
    return QRectF(-w/2, -w/2, w, w + 25);
}

void ButtonItem::paint(QPainter *painter, 
                       const QStyleOptionGraphicsItem * /* option */, 
                       QWidget *)
{
    painter->setRenderHint(QPainter::Antialiasing); // 开启抗锯齿
    painter->setPen(Qt::NoPen);

    /* 绘制最外层银色边框（带金属渐变）*/
    QRectF outerRingRect(-w/2, -w/2, w, w);
    QConicalGradient outerGrad(outerRingRect.center(), 135);
    outerGrad.setColorAt(0, QColor(255, 255, 255));
    outerGrad.setColorAt(0.5, QColor(150, 150, 150));
    outerGrad.setColorAt(0.6, QColor(210, 210, 210));
    outerGrad.setColorAt(1.0, QColor(255, 255, 255));
    painter->setBrush(outerGrad);
    painter->drawEllipse(outerRingRect);

    /* 绿色环渐变（模拟LED发光）*/
    QRectF greenRingOuter(-w/2 + w/9, -w/2 + w/9, 7*w/9, 7*w/9);
    QRadialGradient greenGrad(greenRingOuter.center(), 7*w/18);
    greenGrad.setColorAt(0.8, (state == BUTTON_DOWN) ? 
                               QColor(0, 255, 100, 200) : 
                               QColor(220, 220, 220, 220));
    greenGrad.setColorAt(1, (state == BUTTON_DOWN) ? 
                             QColor(0, 200, 80, 100) : 
                             QColor(180, 180, 180, 180));
    painter->setBrush(greenGrad);
    painter->drawEllipse(greenRingOuter);

    /* 绘制中间金属按钮面（带拉丝金属质感）*/
    QRectF centerBtnRect(-w/2 + w/5, -w/2 + w/5, 11*w/18, 11*w/18);
    QRadialGradient centerGrad(centerBtnRect.center(), 
                               11*w/36, 
                               QPointF(-5*w/36, -5*w/36));
    centerGrad.setColorAt(0, (state == BUTTON_DOWN) ? 
                              QColor(240, 240, 240) : 
                              QColor(255, 255, 255));
    centerGrad.setColorAt(0.4, (state == BUTTON_DOWN) ? 
                                QColor(220, 220, 220) : 
                                QColor(240, 240, 240));
    centerGrad.setColorAt(1, (state == BUTTON_DOWN) ? 
                              QColor(180, 180, 180) : 
                              QColor(200, 200, 200));
    painter->setBrush(centerGrad);
    painter->drawEllipse(centerBtnRect);

    /* 绘制按钮边缘的细微阴影（增强立体感）*/
    painter->setBrush(QColor(255, 255, 255, 20));
    painter->drawEllipse(centerBtnRect.adjusted(2, 2, -2, -2));

    /* 文本 */
    QString text("前灯");
    QRectF textRect(-w/2, w/2 + 5, w, 20);
    painter->setPen((state == BUTTON_DOWN) ? QColor(0, 200, 80) : Qt::black);
    painter->drawText(textRect, Qt::AlignCenter, text);
    

    // painter->setRenderHint(QPainter::Antialiasing);
    // painter->setPen(Qt::NoPen);
// 
    // /* 面板 */
    // painter->setClipRect(-3*w/8, -w, 6*w/8, 2*w);
    // QLinearGradient g1(0, -w/2, 0, w/2);
    // g1.setColorAt(0, QColor(169, 169, 169));
    // g1.setColorAt(1, QColor(158, 156, 156));
    // painter->setBrush(g1);
    // painter->drawEllipse(QPointF(0, 0), w, w);
    // painter->setClipping(false);
// 
    // painter->setClipRect(-3*w/8 + 5, -w + 5, 6*w/8 - 10, 2*w - 10);
    // QRadialGradient g2(0, 0, w, -w/2, -w);
    // g2.setColorAt(0.0, Qt::white);
    // g2.setColorAt(0.7, QColor(245, 245, 245));
    // g2.setColorAt(1.0, QColor(211, 211, 211));
    // painter->setBrush(g2);
    // painter->drawEllipse(QPointF(0, 0), w - 5, w - 5);
    // painter->setClipping(false);
// 
    // /* 底座 */
    /*1. 创建径向渐变：圆心(100, 100)，半径80，焦点(100, 100)*/
    // QRadialGradient radialGrad(0, 0, w/4, 0, 0);
    // 
    /*2. 设置渐变颜色 (0.0是中心，1.0是边缘)
    radialGrad.setColorAt(0.0, QColor(245, 245, 245));  // 中心为白色
    radialGrad.setColorAt(0.5, QColor(211, 211, 211)); // 中间为黄色
    radialGrad.setColorAt(1.0, QColor(105, 105, 105));   // 边缘为蓝色*/
    // radialGrad.setColorAt(0.0, Qt::white);  // 中心为白色
    // radialGrad.setColorAt(0.5, Qt::yellow); // 中间为黄色
    // radialGrad.setColorAt(1.0, Qt::blue);   // 边缘为蓝色
    // radialGrad.setSpread(QGradient::RepeatSpread);
    // painter->setBrush(radialGrad);
    // painter->drawEllipse(QPointF(0, 0), w/2, w/2);
}

void ButtonItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    state = !state;
    update();
}

void ButtonItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    /* 自恢复开关 */
    state = BUTTON_UP;
    update();
}