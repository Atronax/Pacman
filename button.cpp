// events
#include <QPainter>

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>

#include "button.h"
Button::Button(const QString &text, QGraphicsItem *parent)
    : QGraphicsRectItem (parent)
{
    setText(text);
    setRect(boundingRect());
    setAcceptHoverEvents(true);
    setBrush(QBrush(QPixmap(":/images/button_unhovered.jpg")));
}

Button::~Button()
{

}

void Button::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setFont(QFont("Deutsch Gothic", 14, 14));
    painter->setPen(QPen(Qt::black, 1));
    painter->setBrush(brush());

    painter->drawRoundedRect(boundingRect(), 10, 10);
    painter->drawText(boundingRect(), m_text, QTextOption(Qt::AlignCenter));
}

const QString& Button::text() const
{
    return m_text;
}

void Button::setText(const QString &text)
{
    m_text = text;
}

QRectF Button::boundingRect() const
{
    return QRectF(0,0,200,40);
}

void Button::mousePressEvent(QGraphicsSceneMouseEvent *ev)
{
    Q_UNUSED(ev);
    emit clicked();
}

void Button::hoverEnterEvent(QGraphicsSceneHoverEvent *ev)
{
    Q_UNUSED(ev);
    setBrush(QBrush(QPixmap(":/images/button_hovered.jpg")));
}

void Button::hoverLeaveEvent(QGraphicsSceneHoverEvent *ev)
{
    Q_UNUSED(ev);
    setBrush(QBrush(QPixmap(":/images/button_unhovered.jpg")));
}
