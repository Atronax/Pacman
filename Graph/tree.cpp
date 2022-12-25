#include "tree.h"

Tree::Tree(const Node& root)
{
    m_root = root;
    m_graph.addNode(root);
}

Tree::Tree(const Node &root, const Graph &graph)
{
    m_root = root;
    m_graph = graph;
}

Tree::~Tree()
{

}

void Tree::addChildNodeTo(const Node &to, const Node &child, int weight)
{
    m_graph.addNode(child);
    m_graph.addEdge(to,child,weight);
}

QSet<Node> Tree::nodes() const
{
    return m_graph.nodes();
}

QSet<Edge> Tree::edges() const
{
    return m_graph.edges();
}

// Рекурсивный ПОИСК В ГЛУБИНУ
QVector<Node> Tree::DFS_RootTo(const Node &to) const
{
    Tree clone = *this;
    QVector<Node> path;
    return clone.DFS_FromTo(m_root, to, path);
}

QVector<Node> Tree::DFS_FromTo(const Node &node, const Node &to, QVector<Node> path) // ПЕРЕДИВИТИСЬ!!!
{
    visit (node); // помечаем node посещенной вершиной

    if (node == to) // если node - цель, добавляет ее в вектор пути и возвращаем
    {
        path.push_back(node);
        return path;
    }

    if (!hasUnvisitedChild(node)) // если нет непосещенных потомков, запоминаем предыдущую вершину, удаляем текущую из вектора пути и продолжаем DFS на предыдущей
    {
        Node last_node = path.back();
        path.pop_back();
        return DFS_FromTo(last_node,to,path);
    }
    else // if (hasUnvisitedChild(node)) // если есть непосещенные потомки, выбрать одну и продолжить DFS на ней
    {
        path.push_back(node);
        return DFS_FromTo(unvisitedChildFor(node),to,path);
    }
}

// Возвращает непосещенную вершину (первую в векторе) для данной вершины.
Node Tree::unvisitedChildFor(const Node &node)
{
    QVector<Node> children = m_graph.outgoingNodesFor(node);
    foreach (Node child, children)
        if (!isVisited(child))
            return child;
    return Node();
}

// Возвращает TRUE, если данная вершина имеет хотя бы одного непосещенного потомка.
bool Tree::hasUnvisitedChild(const Node &node)
{
    QVector<Node> children = m_graph.outgoingNodesFor(node);

    int children_unvisited = 0;
    foreach (Node child, children)
        if (!isVisited(child))
            ++children_unvisited;

    return children_unvisited > 0;
}

// Возвращает TRUE, если данная вершина посещена.
bool Tree::isVisited(const Node &node)
{
    return m_visited_nodes.contains(node);
}

// Помечает данную вершину посещенной.
void Tree::visit(const Node &node)
{
    m_visited_nodes.insert(node);
}

