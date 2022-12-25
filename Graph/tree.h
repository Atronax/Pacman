#ifndef TREE_H
#define TREE_H

#include "node.h"
#include "graph.h"
#include <QVector>

// Дерево, как и граф, - тоже множество вершин и ребер.
// Отличительная особенность дерева - отсутствие циклов.
class Tree
{
public:
    Tree(const Node& root);
    Tree(const Node& root, const Graph& graph);
    ~Tree();

    void addChildNodeTo (const Node& to, const Node& child, int weight);
    QSet<Node> nodes() const;
    QSet<Edge> edges() const;

    QVector<Node> DFS_RootTo (const Node& to) const;
    QVector<Node> DFS_FromTo (const Node& node, const Node& to, QVector<Node> path);

private:
    Node m_root;
    Graph m_graph;

    // ПОИСК В ГЛУБИНУ (Depth First Search - DFS)
    Node unvisitedChildFor (const Node& node);
    bool hasUnvisitedChild (const Node& node);
    bool isVisited (const Node& node);
    void visit (const Node& node);

    QSet<Node> m_visited_nodes;

};

#endif // TREE_H
