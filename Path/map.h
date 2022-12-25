#ifndef MAP_H
#define MAP_H

#include "grid.h"

// Представляет область пространства, занятую ячейками.
// Каждая ячейка может быть заполненной или незаполненной.
// С помощью карты легко обнаруживать кратчайший путь между двумя ячейками.
class Map
{
public:
    Map();
    Map(const QSize& mapsize, const uint& cellsize);
    ~Map();

    QVector<Node> nodes() const;
    QVector<Node> SP(const QPoint& fromnode_pos, const QPoint& tonode_pos) const;

    int width() const;
    int height() const;
    int cellsize() const;
    int cellsInRow() const;
    int cellsInCol() const;

    // fill controls
    void fillCell(const QPoint& pos);
    void unfillCell(const QPoint& pos);
    bool isFilledCell(const QPoint& pos);

    void fillAtLoc(const QPoint& pos);
    void unfillAtLoc(const QPoint& pos);
    bool isFilledAtLoc(const QPoint& pos);

    void fillRow(int i);
    void fillCol(int i);
    void fillVec(const QVector<QVector<int> >& vec);

private:
    Grid m_pathgrid;
    QSize m_mapsize;
    uint m_cellsize;
};

#endif // MAP_H
