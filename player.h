#ifndef PLAYER_H
#define PLAYER_H

class QGraphicsItem;
class QKeyEvent;
class Game;

#include <QObject>
#include <QGraphicsRectItem>
class Player : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:
    Player(const int& stepsize, Game *manager, QGraphicsItem *parent = nullptr);
    ~Player();

    int stepsize() const;
    void setStepsize(int stepsize);

protected:
    void keyPressEvent(QKeyEvent *ev);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:   
    // manager
    Game *m_manager;

    // moving
    enum class Direction {NOWHERE, UP, RIGHT, DOWN, LEFT};
    Direction m_direction;
    bool isFrontBlocked();
    void moveRight();
    void moveDown();
    void moveLeft();
    void moveUp();

    QTimer *moving_timer;
    int m_stepsize;

public slots:
    void move();
};

#endif // PLAYER_H
