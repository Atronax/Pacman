// logic
#include <QTimer>
#include <QPointF>

// events
#include <QKeyEvent>

// items
#include "food.h"
#include "enemy.h"
#include "game.h"

#include "player.h"
Player::Player(const int& stepsize, Game *manager, QGraphicsItem *parent)
    : QGraphicsRectItem (parent)
{
    // initialize variables
    setFlag(QGraphicsItem::ItemIsFocusable);
    m_manager = manager;
    m_direction = Direction::NOWHERE;
    m_stepsize = stepsize;

    // visuals
    setRect(0,0,stepsize,stepsize);

    // moving
    moving_timer = new QTimer (this);
    connect (moving_timer, SIGNAL(timeout()), this, SLOT(move()));
    moving_timer->start(1000/4);
}

Player::~Player()
{
    moving_timer->stop();

    moving_timer->deleteLater();
}

int Player::stepsize() const
{
    return m_stepsize;
}

void Player::setStepsize(int stepsize)
{
    m_stepsize = stepsize;
}

void Player::keyPressEvent(QKeyEvent *ev)
{
    if (ev->key() == Qt::Key_W)
        m_direction = Direction::UP;

    if (ev->key() == Qt::Key_D)
        m_direction = Direction::RIGHT;

    if (ev->key() == Qt::Key_S)
        m_direction = Direction::DOWN;

    if (ev->key() == Qt::Key_A)
        m_direction = Direction::LEFT;
}

void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->drawPixmap(rect().toAlignedRect(), QPixmap(":/images/pacman.png"));
}

void Player::moveUp()
{
    setPos(x(),y()-stepsize());
}

void Player::moveDown()
{
    setPos(x(),y()+stepsize());
}

void Player::moveLeft()
{
    setPos(x()-stepsize(),y());
}

void Player::moveRight()
{
    setPos(x()+stepsize(),y());
}

bool Player::isFrontBlocked()
{
    QPoint loc;

    Node node = m_manager->getNodeByLocation(pos());
    switch (m_direction)
    {
    case Direction::UP:
        loc.setX(node.x());
        loc.setY(node.y() - 1);
        break;

    case Direction::RIGHT:
        loc.setX(node.x() + 1);
        loc.setY(node.y());
        break;

    case Direction::DOWN:
        loc.setX(node.x());
        loc.setY(node.y() + 1);
        break;

    case Direction::LEFT:
        loc.setX(node.x() - 1);
        loc.setY(node.y());
        break;

    default:
        break;
    }

    return m_manager->isWallAt(loc);
}

// slots
void Player::move()
{
    if (isEnabled())
    {
        if (isFrontBlocked())
            return;

        switch (m_direction)
        {
        case Direction::UP:
            moveUp();
            break;

        case Direction::RIGHT:
            moveRight();
            break;

        case Direction::DOWN:
            moveDown();
            break;

        case Direction::LEFT:
            moveLeft();
            break;

        case Direction::NOWHERE:
            break;

        default:
            break;
       }

       m_manager->updateGamestate();
   }
}
