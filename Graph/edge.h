#ifndef EDGE_H
#define EDGE_H

#include "node.h"

// Представляет ребро графа.
// Ребро определено двумя вершинами (from() и to()) и весом.
// Два ребра эквивалентны, если они начинаются и заканчиваются в тех же самых вершинах, и имеют одинаковый вес.
class Edge
{
public:
    Edge();
    Edge(const Node& from, const Node& to, uint weight);
    ~Edge();

    const Node& from() const;
    const Node& to() const;
    uint weight() const;

private:
    Node m_from;
    Node m_to;
    uint m_weight;
};

// функции, необходимые для правильного взаимодействия обьектов класса со структурами данных:
// - операторы равенства/неравенства
inline bool operator==(const Edge &lhs, const Edge &rhs)
{
    if (lhs.from() == rhs.from() && lhs.to() == rhs.to() && lhs.weight() == rhs.weight())
        return true;
    else
        return false;
}

inline bool operator!=(const Edge &lhs, const Edge &rhs)
{
    if (lhs.from() != rhs.from() || lhs.to() != rhs.to() || lhs.weight() != rhs.weight())
        return true;
    else
        return false;
}

// - хеширование
inline uint qHash(const Edge& edge, uint seed)
{
    return qHash(edge.from(), seed) + qHash(edge.to(), seed) + qHash(edge.weight(), seed);
}

#endif // EDGE_H
