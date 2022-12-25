#include <QPainter>

#include "food.h"
Food::Food(const Type& type, const QSize& cellsize, QGraphicsItem *parent)
    : QGraphicsRectItem (0,0,cellsize.width(),cellsize.height(),parent)
{
    setCacheMode(QGraphicsItem::ItemCoordinateCache);
    setCellsize(cellsize);
    setFoodType(type);

}

Food::~Food()
{

}

const Food::Type &Food::foodType() const
{
    return m_type;
}

void Food::setFoodType(const Food::Type &type)
{
    m_type = type;
    switch (type)
    {
    case Type::SEED:
        setPixmap(QPixmap(":/images/seed.png").scaled(m_cellsize.width(), m_cellsize.height()));
        break;

    case Type::CHERRY:
        setPixmap(QPixmap(":/images/cherry.png").scaled(m_cellsize));
        break;

    default:
        break;
    }
}

const QSize& Food::cellsize() const
{
    return m_cellsize;
}

void Food::setCellsize(const QSize &cellsize)
{
    m_cellsize = cellsize;
}

const QPixmap& Food::pixmap() const
{
    return m_pixmap;
}

void Food::setPixmap(const QPixmap &pixmap)
{
    m_pixmap = pixmap;
}

void Food::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->drawPixmap(rect().toAlignedRect(), pixmap());
}

