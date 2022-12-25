#ifndef GAME_H
#define GAME_H

// items
#include "enemy.h"
#include "player.h"
#include "Path/map.h"

#include <QGraphicsScene>
#include <QGraphicsView>
class Game : public QGraphicsView
{
    Q_OBJECT

public:
    Game(const QSize& mapsize, QWidget* parent = nullptr);
    ~Game();    

    QGraphicsScene* getScene() const;

    // fills the cell on the specified position and makes visual representation of it
    bool isWallAt (const QPoint& pos);
    void fillPath (const QPoint& pos);
    void fillWall (const QPoint& pos);

    // returns position of specified node or vise versa
    QPointF getLocationOfNode (const Node& node);
    Node getNodeByLocation (const QPointF& location);

    // generates an enemy at specified position
    void generateEnemyAt (const QPoint& pos);    
    void removeEnemyFromTheList (Enemy *enemy);
    void addEnemyToTheList (Enemy *enemy);

    // updates game state
    void updateGamestate();

protected:
    void mousePressEvent(QMouseEvent *ev);

private:    
    void initMapsize();
    void initEditor();
    void initSceneview();

    void deleteAllItemsFromScene();

    void drawPanel(const QPoint& position, const QSize& size, const QBrush &brush, const QString &caption);
    void gameoverWindow(const QString& message);

    void fillMap(const QVector<QVector<int> >& vec);
    void setEnemyRessurecting(Enemy *enemy);
    void setEnemiesVulnerable();

    // core
    QGraphicsScene *m_scene;

    // map
    const int m_cellsize = 75;
    Map m_pathmap;
    QSize m_mapsize;


    // gameplay
    Player *m_player;
    QList<Enemy*> m_enemies;
    int m_ate_seeds,
        m_total_seeds;

    // editor
    bool m_isEditor;
    QVector<QVector<int>> m_map;


public slots:
    void start();
    void restart();
    void editor();
    void mainmenu();
    void findPathToPlayer();
};

#endif // GAME_H
