#ifndef GRID_H
#define GRID_H

#include "Graph/graph.h"

class QSize;
class QPoint;

// Представляет сетку из ячеек 1х1. Это своеобразная минималистическая карта путей.
// Каждая из ячеек имеет координаты (х,у) и может быть доступной либо недоступной.
class Grid
{
public:
    Grid();
    Grid(const QSize& size);
    ~Grid();

    // Возвращает граф, узлы либо кратчайший путь from-to сетки
    Graph graph() const;
    QVector<Node> nodes() const;
    QVector<Node> SP (const Node& from, const Node& to) const;
    QVector<Node> row (const int& i) const;
    QVector<Node> col (const int& i) const;

    // Помечает вершины графа доступными или недоступными для SPT
    void fill (const Node& node);
    void fill (const QPoint& pos);
    void unfill (const Node& node);
    void unfill (const QPoint& pos);
    bool isFilled (const Node& node) const;
    bool isFilled (const QPoint& pos) const;

    void fillRow (const int& i);
    void fillCol (const int& i);
    void fillVec (const QVector<QVector<int> >& vec);

private:
    QVector<Node> unfilledNeighbourNodesFor (const Node& node) const;

    QVector<QVector<Node> > m_nodes;
    QMap<Node, bool> m_isFilled;
};

#endif // GRID_H
