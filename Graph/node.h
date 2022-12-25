#ifndef NODE_H
#define NODE_H

#include <QVector>
#include <QHash>

// Представляет вершину графа.
// Вершина определена ее значениями позиции (x;y).
// Две вершини эквивалентны, если они имеют одинаковые значения позиции.
class Node
{
public:
    Node();
    Node(int x, int y);
    ~Node();

    int x() const;
    void setX(int x);

    int y() const;
    void setY(int y);

private:
    int m_x;
    int m_y;
};

// функции, необходимые для правильного взаимодействия обьектов класса со структурами данных:
// - операторы равенства/неравенства
inline bool operator==(const Node &lhs, const Node &rhs)
{
    if (lhs.x() == rhs.x() && lhs.y() == rhs.y())
        return true;
    else
        return false;
}

inline bool operator!=(const Node &lhs, const Node &rhs)
{
    if (lhs.x() != rhs.x() || lhs.y() != rhs.y())
        return true;
    else
        return false;
}

inline bool operator <(const Node &lhs, const Node &rhs) // ВАЖНА ШТУКА ДЛЯ ДАТАСРАКЧОВ
{
    if (lhs.y() < rhs.y())
        return true;
    else if (lhs.y() == rhs.y())
    {
        if (lhs.x() < rhs.x())
            return true;
        else
            return false;
    }
    else
        return false;

}

// - хеширование
inline uint qHash(const Node& node, uint seed)
{
    return qHash(node.x() + node.y(),seed);
}

#endif // NODE_H
