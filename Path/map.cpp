#include <QSize>
#include <QPoint>

#include "map.h"
Map::Map()
{

}

Map::Map(const QSize& mapsize, const uint& cellsize)
{
    m_mapsize = mapsize;
    m_pathgrid = Grid(m_mapsize);
    m_cellsize = cellsize;
}

Map::~Map()
{

}

QVector<Node> Map::nodes() const
{
    QVector<Node> result;

    // изменяем размер ячейки в модели для правильного построения графа пути
    foreach (Node node, m_pathgrid.nodes())
    {
        node.setX(node.x() * cellsize());
        node.setY(node.y() * cellsize());

        result.push_back(node);
    }

    return result;
}

QVector<Node> Map::SP (const QPoint &fromnode_pos, const QPoint &tonode_pos) const
{
    QVector<Node> result;

    Grid grid_copy = m_pathgrid;

    // Ищем кратчайший путь на модели и скейлим результат.      
    QVector<Node> path = grid_copy.SP(Node(fromnode_pos.x()/cellsize(), fromnode_pos.y()/cellsize()),
                                      Node(tonode_pos.x()/cellsize(), tonode_pos.y()/cellsize()));
    foreach (Node node, path)
    {        
        node.setX(node.x()*cellsize());
        node.setY(node.y()*cellsize());

        result.push_back(node);
    }

    return result;
}

int Map::width() const
{
    return cellsInRow()*cellsize();
}

int Map::height() const
{
    return cellsInCol()*cellsize();
}

int Map::cellsize() const
{
    return m_cellsize;
}

int Map::cellsInRow() const
{
    return m_mapsize.width();
}

int Map::cellsInCol() const
{
    return m_mapsize.height();
}

void Map::fillCell(const QPoint &pos)
{
    m_pathgrid.fill(pos);
}

void Map::fillAtLoc(const QPoint &pos)
{
    QPoint model_pos = QPoint(pos.x()/m_cellsize,pos.y()/m_cellsize);
    m_pathgrid.fill(model_pos);
}

void Map::unfillCell(const QPoint &pos)
{
    m_pathgrid.unfill(pos);
}

void Map::unfillAtLoc(const QPoint &pos)
{
    QPoint model_pos = QPoint(pos.x()/m_cellsize,pos.y()/m_cellsize);
    m_pathgrid.unfill(model_pos);
}

bool Map::isFilledCell(const QPoint &pos)
{
    return m_pathgrid.isFilled(pos);
}

bool Map::isFilledAtLoc(const QPoint &pos)
{
    QPoint model_pos = QPoint(pos.x()/m_cellsize,pos.y()/m_cellsize);
    return m_pathgrid.isFilled(model_pos);
}

void Map::fillRow(int i)
{
    m_pathgrid.fillRow(i);
}

void Map::fillCol(int i)
{
    m_pathgrid.fillCol(i);
}

void Map::fillVec(const QVector<QVector<int> > &vec)
{
    m_pathgrid.fillVec(vec);
}
