#include <QHash>

#include "node.h"
Node::Node()
{

}

Node::Node(int x, int y)
{
    setX(x);
    setY(y);
}

Node::~Node()
{

}

int Node::x() const
{
    return m_x;
}

void Node::setX(int x)
{
    m_x = x;
}

int Node::y() const
{
    return m_y;
}

void Node::setY(int y)
{
    m_y = y;
}
