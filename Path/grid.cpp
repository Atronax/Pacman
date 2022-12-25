#include <QSize>
#include <QPoint>
#include <QString>

#include "grid.h"
Grid::Grid()
{

}

Grid::Grid(const QSize& size)
{
    m_nodes = QVector<QVector<Node> >();
    m_isFilled = QMap<Node,bool>();

    for (int i = 0; i < size.width(); ++i)
    {
        QVector<Node> node_column;
        for (int j = 0; j < size.height(); ++j)
            node_column.push_back(Node(i,j));

        m_nodes.push_back(node_column);
    }

    foreach (Node node, nodes())
        unfill(node);
}

Grid::~Grid()
{

}

// Возвращает граф, все узлы, кратчайший путь from-to сетки либо узлы конкретного ряда или столбца
Graph Grid::graph() const
{
    Graph result;

    // Добавляем в граф все узлы
    foreach (Node node, nodes())
        result.addNode(node);

    // Добавляем в граф ребра между незаполненными (unfilled) узлами
    foreach (Node node, result.nodes())
    {
        if (isFilled(node))
            continue;

        QVector<Node> neighbour_nodes = unfilledNeighbourNodesFor(node);
        foreach (Node neighbour, neighbour_nodes)
        {
            result.addEdge(node, neighbour, 1);
            result.addEdge(neighbour, node, 1);
        }
    }

    return result;
}

QVector<Node> Grid::nodes() const
{
    QVector<Node> result;

    foreach (QVector<Node> column, m_nodes)
    {
        foreach (Node node, column)
            result.push_back(node);
    }

    return result;
}

QVector<Node> Grid::SP(const Node &from, const Node &to) const
{
    return graph().SP(from,to);
}

QVector<Node> Grid::row(const int &i) const
{
    QVector<Node> result;

    foreach (QVector<Node> col_nodes, m_nodes)
        result.push_back(col_nodes[i]);

    return result;
}

QVector<Node> Grid::col(const int &i) const
{
    return m_nodes[i];
}

// Помечает вершины графа доступными или недоступными для SPT
void Grid::fill(const Node &node)
{
    m_isFilled[node] = true;
}

void Grid::fill(const QPoint &pos)
{
    fill(Node(pos.x(),pos.y()));
}

void Grid::unfill(const Node &node)
{
    m_isFilled[node] = false;
}

void Grid::unfill(const QPoint &pos)
{
    unfill(Node(pos.x(),pos.y()));
}

bool Grid::isFilled(const Node &node) const
{
    return m_isFilled[node];
}

bool Grid::isFilled(const QPoint &pos) const
{
    return isFilled(Node(pos.x(),pos.y()));
}

void Grid::fillRow(const int &i)
{
    foreach (Node node, row(i))
        fill(node);
}

void Grid::fillCol(const int &i)
{
    foreach (Node node, col(i))
        fill(node);
}

void Grid::fillVec(const QVector<QVector<int> > &vec)
{
    for (int y = 0; y < m_nodes[0].size(); ++y) // H    
        for (int x = 0; x < m_nodes.size(); ++x) // W        
            if (vec[y][x] == 1)
                fill(QPoint(x,y));
}

// Возвращает вектор всех соседних незаполненных узлов для узла node.
// Определяет все направления (!), в которых можно двигатся из определенного узла.
QVector<Node> Grid::unfilledNeighbourNodesFor(const Node &node) const
{
    QVector<Node> result;

    int rows_count = m_nodes[0].size();
    int cols_count = m_nodes.size();

    // вверх
    if (node.y() > 0)
    {
        Node top_node (node.x(),node.y() - 1);
        if (!isFilled(top_node))
            result.push_back(top_node);
    }

    // вниз
    if (node.y() < rows_count - 1)
    {
        Node bottom_node (node.x(), node.y() + 1);
        if (!isFilled(bottom_node))
            result.push_back(bottom_node);
    }

    // влево
    if (node.x() > 0)
    {
        Node left_node (node.x() - 1, node.y());
        if (!isFilled(left_node))
            result.push_back(left_node);
    }

    // вправо
    if (node.x() < cols_count - 1)
    {
        Node right_node (node.x() + 1, node.y());
        if (!isFilled(right_node))
            result.push_back(right_node);
    }


    /*
    // ПО ДИАГОНАЛИ:
    // вверх-влево
    if (node.y() > 0 && node.x() > 0)
    {
        Node topleft_node (node.x() - 1, node.y() - 1);
        if (!isFilled(topleft_node))
            result.push_back(topleft_node);
    }

    // вверх-вправо
    if (node.y() > 0 && node.x() < cols_count - 1)
    {
        Node topright_node (node.x() + 1, node.y() - 1);
        if (!isFilled(topright_node))
            result.push_back(topright_node);
    }

    // вниз-влево
    if (node.y() < rows_count - 1 && node.x() > 0)
    {
        Node bottomleft_node (node.x() - 1, node.y() + 1);
        if (!isFilled(bottomleft_node))
            result.push_back(bottomleft_node);
    }

    // вниз-вправо
    if (node.y() < rows_count - 1 && node.x() < cols_count - 1)
    {
        Node bottomright_node (node.x() + 1, node.y() + 1);
        if (!isFilled(bottomright_node))
            result.push_back(bottomright_node);
    }
    */

    return result;
}
