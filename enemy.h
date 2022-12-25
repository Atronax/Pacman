#ifndef ENEMY_H
#define ENEMY_H

class Game;
#include <QVector>
#include <QPointF>

#include <QObject>
#include <QGraphicsRectItem>
class Enemy : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:
    Enemy(const QPoint& position, const uint& stepsize, Game* manager, QGraphicsItem* parent = nullptr);
    ~Enemy();

    // path control
    const QVector<QPointF> path() const;
    void setPath(const QVector<QPointF>& path);

    // state control
    bool isVulnerable() const;
    void setIsVulnerable(bool value);

    const QPixmap& pixmap() const;
    void setPixmap(const QPixmap &pixmap);

    QTimer *movingTimer() const;
    QTimer *ressurectingTimer() const;
    QTimer *vulnerabilityTimer() const;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    // core
    Game *m_manager;
    QPoint m_position;
    uint m_stepsize;

    // moving
    void moveTowardsTarget();
    bool isTargetReached();
    bool isAtLastPoint();
    void setNextTarget();
    QVector<QPointF> m_path;
    int m_targetpt;


    // state
    QPixmap m_default_pixmap, m_pixmap;
    QTimer *m_vulnerabilityTimer;
    QTimer *m_ressurectingTimer;
    QTimer *m_movingTimer;
    bool m_isVulnerable;

public slots:
    void move();
    void ressurect();
    void stopVulnerability();
};

#endif // ENEMY_H
