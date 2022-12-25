#ifndef GRAPH_H
#define GRAPH_H

// used data structures
#include <QSet>
#include <QMap>
#include <QVector>

// used elements
#include "node.h"
#include "edge.h"
class Tree;

// Представляет граф.
// Граф определяется набором вершин и узлов между ними.
class Graph
{
public:
    Graph(const QSet<Node>& nodes = QSet<Node>(), const QSet<Edge>& edges = QSet<Edge>());
    ~Graph();

    // keep track of all nodes and edges
    const QSet<Node>& nodes() const;
    const QSet<Edge>& edges() const;
    bool contains(const Node& node);
    bool contains(const Edge& edge);
    void setNodes(const QSet<Node> &value);
    void setEdges(const QSet<Edge> &value);
    void addNode (const Node& node);
    void addEdge (const Node& from, const Node& to, int weight);

    QVector<Node> SP (const Node& from, const Node& to) const;
    Tree SPT (const Node& root) const;

    // return a vector of outgoing/incoming edges or nodes, that are to()/from() of these edges
    QVector<Edge> outgoingEdgesFor (const Node& node) const;
    QVector<Node> outgoingNodesFor (const Node& node) const;
    QVector<Edge> incomingEdgesFor (const Node& node) const;
    QVector<Node> incomingNodesFor (const Node& node) const;

private:
    // keep track of all nodes and edges
    QSet<Node> m_nodes;
    QSet<Edge> m_edges;

    // ПОИСК КРАТЧАЙШЕГО ДРЕВА ПУТИ
    void unpickAll ();
    void pick (const Node& node);
    void pick (const Edge& edge);    
    bool isPicked (const Node& node) const;
    bool isPicked (const Edge& edge) const;
    QSet<Node> m_pickedNodes;
    QSet<Edge> m_pickedEdges;
    QSet<Node> m_unpickedNodes;
    QSet<Edge> m_unpickedEdges;

    Edge connectingEdgeFor (const Node& node) const;
    void setConnectingEdgeFor (const Node& node, const Edge& edge);
    Edge connectingEdgeOf (const Node& from, const Node& to) const;
    void pickConnectingEdgeFor (const Node& node);
    QMap<Node,Edge> m_connectingEdge;

    int weightOf (const Node& node) const;
    void setWeightOf (const Node& node, int weight);
    QMap<Node,int> m_nodeWeight;

    bool hasUnpickedNode() const;
    Node lightestUnpickedNode() const;
    QVector<Node> unpickedNeighbourNodesFor (const Node& node) const;

    void initNodeWeights (const Node& startnode);
    void updateNeighbourNodeWeights (const Node& node);
};

#endif // GRAPH_H
