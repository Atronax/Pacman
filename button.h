#ifndef BUTTON_H
#define BUTTON_H

#include <QObject>
#include <QGraphicsRectItem>
class Button : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    Button(const QString& text, QGraphicsItem *parent = nullptr);
    ~Button();

    const QString& text() const;
    void setText(const QString &text);

    QRectF boundingRect() const;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *ev);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *ev);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *ev);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QString m_text;

signals:
    void clicked();
};

#endif // BUTTON_H
