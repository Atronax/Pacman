// logic
#include <QPointF>
#include <QTimer>
#include <QSize>

// events
#include <QMouseEvent>

// items
#include "button.h"
#include "food.h"
#include "game.h"


Game::Game(const QSize& mapsize, QWidget* parent)
{
    setParent(parent);
    m_mapsize = mapsize;

    m_scene = new QGraphicsScene (this);
    setScene(m_scene);
    setFixedSize(mapsize.width()*m_cellsize, mapsize.height()*m_cellsize);
    setSceneRect(0,0, mapsize.width()*m_cellsize, mapsize.height()*m_cellsize);

    // start game
    mainmenu();
}

Game::~Game()
{

}

void Game::initEditor()
{
     m_isEditor = false;

     for (int i = 0; i < 10; ++i)
         m_map.append(QVector<int>(10));

     for (int y = 0; y < 10; ++y)
         for (int x = 0; x < 10; ++x)
             m_map[y][x] = 1;
}

QGraphicsScene *Game::getScene() const
{
    return m_scene;
}

bool Game::isWallAt(const QPoint &pos)
{
    return m_pathmap.isFilledCell(pos);
}

void Game::fillWall(const QPoint &pos)
{
    // logic
    m_pathmap.fillCell(pos);

    // visuals
    if (m_pathmap.isFilledCell(pos))
    {
        QGraphicsRectItem* item = new QGraphicsRectItem (0,0,m_cellsize,m_cellsize);
        item->setPos(pos.x()*m_cellsize,pos.y()*m_cellsize);
        item->setBrush(QBrush(Qt::green));
        m_scene->addItem(item);
    }
}

void Game::fillPath(const QPoint &pos)
{
    // logic
    m_pathmap.unfillCell(pos);

    // visuals
    if (!m_pathmap.isFilledCell(pos))
    {
        // make background
        QGraphicsRectItem* item = new QGraphicsRectItem (0,0,m_cellsize,m_cellsize);
        item->setPos(pos.x()*m_cellsize,pos.y()*m_cellsize);
        item->setBrush(QBrush(Qt::lightGray));
        m_scene->addItem(item);

        // fill with food
        Food *food;
        if (1 + rand()%100 < 5)
            food = new Food (Food::CHERRY, QSize(m_cellsize, m_cellsize), item);
        else
        {
            food = new Food (Food::SEED, QSize(m_cellsize,m_cellsize), item);
            ++m_total_seeds;

        }
        food->setPos(item->boundingRect().width()/2.0 - food->boundingRect().width()/2.0,item->boundingRect().height()/2.0 - food->boundingRect().height()/2.0);
    }
}

void Game::fillMap(const QVector<QVector<int>> &vec)
{
    for (int y = 0; y < vec.size(); ++y) // H
        for (int x = 0; x < vec[0].size(); ++x) // W
        {
            if (vec[y][x] == 1)
                fillWall(QPoint(x,y));
            else
                fillPath(QPoint(x,y));
        }
}

QPointF Game::getLocationOfNode(const Node &node)
{
    return QPointF(node.x()*m_cellsize, node.y()*m_cellsize);
}

Node Game::getNodeByLocation(const QPointF &location)
{
    return Node(location.x()/m_cellsize, location.y()/m_cellsize);
}

void Game::removeEnemyFromTheList(Enemy *enemy)
{
    m_enemies.removeOne(enemy);
}

void Game::addEnemyToTheList(Enemy *enemy)
{
    m_enemies.append(enemy);
}

void Game::generateEnemyAt(const QPoint &pos)
{
    Enemy *enemy = new Enemy (QPoint(pos.x()*m_cellsize,pos.y()*m_cellsize), m_cellsize, this);
    addEnemyToTheList(enemy);
    m_scene->addItem(enemy);
}

void Game::drawPanel(const QPoint &position, const QSize &size, const QBrush &brush, const QString& caption)
{
    QGraphicsRectItem *panel = new QGraphicsRectItem (QRectF(position,size));
    panel->setBrush(brush);
    scene()->addItem(panel);

    QGraphicsSimpleTextItem *panel_text = new QGraphicsSimpleTextItem (caption);
    panel_text->setPen(QPen(Qt::yellow));
    panel_text->setFont(QFont("Stylo", 20));
    panel_text->setPos(position.x() + panel->boundingRect().width()/2.0 - panel_text->boundingRect().width()/2.0, 20 + position.y());
    scene()->addItem(panel_text);
}

void Game::gameoverWindow(const QString &message)
{
    foreach (QGraphicsItem* item, scene()->items())
        item->setEnabled(false);

    drawPanel(QPoint(0,0), rect().size(), QBrush(QColor(0,0,0,100)), tr("Конец"));
    drawPanel(QPoint(0, rect().height()/2.0 - 300.0/2.0), QSize(rect().width(), 300), QBrush(Qt::yellow), tr("Игра окончена"));

    QGraphicsSimpleTextItem *message_item = new QGraphicsSimpleTextItem(message);
    message_item->setPen(QPen(Qt::yellow));
    message_item->setFont(QFont("Stylo", 14));
    message_item->setPos(rect().width()/2.0 - message_item->boundingRect().width()/2.0, 150);
    scene()->addItem(message_item);

    Button* restart_button = new Button(tr("Повторить игру"));
    restart_button->setPos(rect().width()/2.0-restart_button->boundingRect().width()/2, 250);
    connect (restart_button, SIGNAL(clicked()), this, SLOT(restart()));
    scene()->addItem(restart_button);

    Button* quit_button = new Button(tr("Выйти с игры"));
    quit_button->setPos(rect().width()/2.0-quit_button->boundingRect().width()/2, 350);
    connect (quit_button, SIGNAL(clicked()), this, SLOT(close()));
    scene()->addItem(quit_button);
}

void Game::setEnemiesVulnerable()
{
    foreach (QGraphicsItem* item, scene()->items())
    {
        Enemy *enemy = dynamic_cast<Enemy*>(item);
        if (enemy)
        {
            enemy->setIsVulnerable(true);
            enemy->setPixmap(QPixmap(":/images/ghost_vulnerable.png"));
            enemy->vulnerabilityTimer()->start();
        }
    }
}

void Game::setEnemyRessurecting(Enemy *enemy)
{
    enemy->ressurectingTimer()->start();
    scene()->removeItem(enemy);
}

void Game::updateGamestate()
{
    bool playerEatenByGhosts = false;
    bool playerAteAllSeeds = false;
    foreach (QGraphicsItem* item, m_player->collidingItems(Qt::ContainsItemBoundingRect))
    {
        Food *food = dynamic_cast<Food*>(item);
        Enemy *enemy = dynamic_cast<Enemy*>(item);

        if (food)
        {
            if (food->foodType() == Food::SEED && ++m_ate_seeds >= m_total_seeds)
                playerAteAllSeeds = true;

            if (food->foodType() == Food::CHERRY)
                setEnemiesVulnerable();

            delete food;
        }

        if (enemy)
        {
            if (enemy->isVulnerable())
            {
                removeEnemyFromTheList(enemy);
                setEnemyRessurecting(enemy);
            }
            else
                playerEatenByGhosts = true;
        }
    }

    if (playerEatenByGhosts)
        gameoverWindow(tr("Враг тебя скушалъ! В следующий раз будь внимательнее."));
    else if (playerAteAllSeeds)
        gameoverWindow(tr("Ты успел сожрать все зерно. Молодец!"));
}

void Game::mousePressEvent(QMouseEvent *ev)
{
    if (m_isEditor)
    {
        if (ev->button() == Qt::LeftButton)
            fillWall(ev->pos());

        if (ev->button() == Qt::RightButton)
            fillPath(ev->pos());
    }

    QGraphicsView::mousePressEvent(ev);
}

void Game::deleteAllItemsFromScene()
{
    delete m_player;
    foreach (QGraphicsItem *item, scene()->items())
    {
        scene()->removeItem(item);
        delete item;
    }
}

// slots
void Game::start()
{
    // clear the scene
    scene()->clear();

    // init vars
    m_ate_seeds = 0;
    m_total_seeds = 0;

    // make pathmap
    m_pathmap = Map(m_mapsize, m_cellsize);

    // draw the map
    QVector<QVector<int>> vec =
    {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,1,1,1,1,1,0,1,1,1,1,1,1,0,1},
        {1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,1},
        {1,0,1,0,1,1,1,0,1,1,1,1,0,1,0,1},
        {1,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1},
        {1,0,1,0,1,0,0,0,0,0,0,1,0,1,0,1},
        {1,0,1,0,1,1,1,1,1,1,1,1,0,1,0,1},
        {1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,1},
        {1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };
    fillMap (vec);

    // set up the player
    m_player = new Player(m_cellsize, this);
    m_player->setPos(m_cellsize,2*m_cellsize);
    m_scene->addItem(m_player);
    m_player->setFocus();

    // create some enemies
    generateEnemyAt(QPoint(9,6));
    generateEnemyAt(QPoint(8,6));
    generateEnemyAt(QPoint(7,6));    

    QTimer *timer = new QTimer (this);
    connect (timer, SIGNAL(timeout()), this, SLOT(findPathToPlayer()));
    timer->start(1000);
}

void Game::restart()
{
    // clear enemies list
    m_enemies.clear();    

    // clear scene
    deleteAllItemsFromScene();

    // start
    start();
}

void Game::editor()
{
    // clear the scene
    scene()->clear();

    // init editor mode
    initEditor();

    // enter editor mode;
    m_isEditor = true;
    fillMap(m_map);
}

void Game::mainmenu()
{
    drawPanel(QPoint(0,0), rect().size(), QBrush(QColor(0,0,0,200)), "");
    drawPanel(QPoint(0,rect().height()/2.0 - 25), QSize(rect().width(), 150), QBrush(QColor(255,0,0), Qt::Dense4Pattern), "");

    Button* start_button = new Button(tr("Новая игра"));
    start_button->setPos(rect().width()/2.0-start_button->boundingRect().width()/2, rect().height()/2.0 - start_button->boundingRect().height()/2.0);
    connect (start_button, SIGNAL(clicked()), this, SLOT(start()));
    scene()->addItem(start_button);

    Button* editor_button = new Button(tr("Редактор"));
    editor_button->setPos(rect().width()/2.0-start_button->boundingRect().width()/2, rect().height()/2.0 - editor_button->boundingRect().height()/2.0+50);
    connect (editor_button, SIGNAL(clicked()), this, SLOT(editor()));
    scene()->addItem(editor_button);

    Button* quit_button = new Button(tr("Выйти с игры"));
    quit_button->setPos(rect().width()/2.0-quit_button->boundingRect().width()/2, rect().height()/2.0 - quit_button->boundingRect().height()/2.0+100);
    connect (quit_button, SIGNAL(clicked()), this, SLOT(close()));
    scene()->addItem(quit_button);
}

void Game::findPathToPlayer()
{
    if (!m_enemies.empty())
    {
        foreach (Enemy *enemy, m_enemies)
        {
            if (enemy->isEnabled())
            {
                QVector<Node> nodepath = m_pathmap.SP(enemy->pos().toPoint(), m_player->pos().toPoint());
                QVector<QPointF> pointpath;
                foreach (Node node, nodepath)
                    pointpath.push_back(QPointF(node.x(),node.y()));

                enemy->setPath(pointpath);
            }
        }
    }
}
