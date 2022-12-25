// painting
#include <QPainter>
#include <QBrush>

// logic
#include <QTimer>
#include <QLineF>

// items
#include "game.h"

#include "enemy.h"
Enemy::Enemy(const QPoint& position, const uint& stepsize, Game *manager, QGraphicsItem* parent)
{
    // initialize variables    
    m_manager = manager;
    m_position = position;
    m_stepsize = stepsize;
    setParentItem(parent);
    setPath(QVector<QPointF>());
    setIsVulnerable(false);
    setPos(m_position);
    m_targetpt = 0;

    // visuals
    setRect(0,0,m_stepsize,m_stepsize);
    m_default_pixmap = QPixmap(":/images/ghost" + QString::number(1+rand()%4) + ".png");
    m_pixmap = m_default_pixmap;

    // state
    m_movingTimer = new QTimer (this);
    connect (m_movingTimer, SIGNAL(timeout()), this, SLOT(move()));
    m_movingTimer->start(1000/4);

    m_ressurectingTimer = new QTimer (this);
    m_ressurectingTimer->setInterval(5000);
    m_ressurectingTimer->setSingleShot(true);
    connect (m_ressurectingTimer, SIGNAL(timeout()), this, SLOT(ressurect()));

    m_vulnerabilityTimer = new QTimer (this);
    m_vulnerabilityTimer->setInterval(5000);
    m_vulnerabilityTimer->setSingleShot(true);
    connect (m_vulnerabilityTimer, SIGNAL(timeout()), this, SLOT(stopVulnerability()));
}

Enemy::~Enemy()
{
    m_movingTimer->stop();
    m_ressurectingTimer->stop();
    m_vulnerabilityTimer->stop();

    m_movingTimer->deleteLater();
    m_ressurectingTimer->deleteLater();
    m_vulnerabilityTimer->deleteLater();
}

const QVector<QPointF> Enemy::path() const
{
    return m_path;
}

void Enemy::setPath(const QVector<QPointF> &path)
{
    m_path = path;
    m_targetpt = 1;
}

// Функции для изменения состояния противников (ускорения, замедления, уязвимость и т.д.)
const QPixmap& Enemy::pixmap() const
{
    return m_pixmap;
}

void Enemy::setPixmap(const QPixmap &pixmap)
{
    m_pixmap = pixmap;
}

QTimer *Enemy::movingTimer() const
{
    return m_movingTimer;
}

QTimer *Enemy::ressurectingTimer() const
{
    return m_ressurectingTimer;
}

QTimer *Enemy::vulnerabilityTimer() const
{
    return m_vulnerabilityTimer;
}

bool Enemy::isVulnerable() const
{
    return m_isVulnerable;
}

void Enemy::setIsVulnerable(bool value)
{
    m_isVulnerable = value;
}

void Enemy::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->drawPixmap(rect().toAlignedRect(),pixmap());
}

// Эта переменная представляет собой индекс в списке точек, по который будет двигатся обьект.
// Изменяя индекс, мы можем изменять движение обьекта.
void Enemy::setNextTarget()
{
    ++m_targetpt;
}

// Перемещает обьект к целевой точке.
// 1. Создаем линию от позиции этого обьекта к позиции цели.
// 2. Устанавливаем длину линии в m_stepsize.
// 3. Перемещаем обьект на dx и dy этой линии.
void Enemy::moveTowardsTarget()
{
    // move
    QLineF line (pos(), m_path[m_targetpt]);    
    line.setLength(m_stepsize);
    moveBy(line.dx(), line.dy());

    // rotate
    // QLineF rot_line (boundingRect().center(), m_path[m_targetpt] + QPointF(m_stepsize,m_stepsize));
    // setRotation(-rot_line.angle());
}

// Возвращает TRUE, если обьект достиг своей текущей цели.
// 1. Создаем линию от позиции этого обьекта к позиции цели.
// 2. Определяем, насколька мала длина этой линии.
bool Enemy::isTargetReached()
{
    return QLineF(pos(),m_path[m_targetpt]).length() < m_stepsize;
}

// Возвращает TRUE, если обьект достиг своей последней точки
// В последней точке m_targetpt > m_path.size().
bool Enemy::isAtLastPoint()
{
    return (m_targetpt >= m_path.size());
}

// slots
void Enemy::move()
{
    if (isEnabled())
    {
        if (isAtLastPoint())
            return;

        moveTowardsTarget();
        if (isTargetReached())
            setNextTarget();

        m_manager->updateGamestate();
    }
}

void Enemy::ressurect()
{
    setPos(m_position);
    m_manager->addEnemyToTheList(this);
    m_manager->getScene()->addItem(this);
}

void Enemy::stopVulnerability()
{
    m_pixmap = m_default_pixmap;
    setIsVulnerable(false);
}
