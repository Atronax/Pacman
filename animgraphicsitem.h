#ifndef ANIMGRAPHICSITEM_H
#define ANIMGRAPHICSITEM_H

class Game;

#include <QObject>
#include <QGraphicsRectItem>
class AnimGraphicsItem : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:
    explicit AnimGraphicsItem(const QString& anim_path,
                              const QSize& anim_framesize,
                              QGraphicsItem* parent, Game* view);
    ~AnimGraphicsItem();

    QRectF boundingRect() const;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    void nextFrame();

    QTimer *m_timer;
    Game *m_parent;

    // animation
    QPixmap m_animation;
    QPixmap m_current_frame;
    QPoint m_animation_position;
    QSize m_animation_framesize;

public slots:
    void updateAnimation();
};

#endif // ANIMGRAPHICSITEM_H
