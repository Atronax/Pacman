#ifndef FOOD_H
#define FOOD_H

#include <QSize>
#include <QGraphicsRectItem>
class Food : public QGraphicsRectItem
{
public:
    enum Type {NOTHING, SEED, CHERRY};

    Food(const Type& type, const QSize& cellsize, QGraphicsItem *parent = nullptr);
    ~Food();

    const Type& foodType() const;
    void setFoodType(const Type& type);

    const QSize& cellsize() const;
    void setCellsize(const QSize &cellsize);

    const QPixmap& pixmap() const;
    void setPixmap(const QPixmap &pixmap);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QPixmap m_pixmap;
    QSize m_cellsize;
    Type m_type;

};

#endif // FOOD_H
