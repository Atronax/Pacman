// logic
#include <QTimer>

// items
#include "game.h"

#include "animgraphicsitem.h"
AnimGraphicsItem::AnimGraphicsItem(const QString &anim_path, const QSize &anim_framesize, QGraphicsItem *parent, Game *view)
    : QGraphicsRectItem(parent)
{
    m_parent = view;        

    m_animation = QPixmap(anim_path);
    m_animation_position = QPoint(0,0);
    m_animation_framesize = anim_framesize;

    m_timer = new QTimer (this);
    m_timer->setInterval(1000/30);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateAnimation()));
    m_timer->start();
}

AnimGraphicsItem::~AnimGraphicsItem()
{

}

QRectF AnimGraphicsItem::boundingRect() const
{
    return QRectF(0,0,m_animation_framesize.width(),m_animation_framesize.height());
}

void AnimGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->drawPixmap(boundingRect().toAlignedRect(), m_current_frame);
}

void AnimGraphicsItem::nextFrame()
{
    m_animation_position.setX(m_animation_position.x() + m_animation_framesize.width());
    if (m_animation_position.x() >= m_animation.width())
    {
        m_animation_position.setX(0);
        m_animation_position.setY(m_animation_position.y() + m_animation_framesize.height());
        if (m_animation_position.y() >= m_animation.height())
            m_animation_position.setY(0);
    }

    m_current_frame = m_animation.copy(m_animation_position.x(), m_animation_position.y(),
                                       m_animation_framesize.width(), m_animation_framesize.height());
}


void AnimGraphicsItem::updateAnimation()
{  
    nextFrame();
    update();
}

