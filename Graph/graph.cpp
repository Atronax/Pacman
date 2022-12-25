#include "tree.h"

#include "graph.h"
Graph::Graph(const QSet<Node>& nodes, const QSet<Edge>& edges)
{
    m_nodes = nodes;
    m_edges = edges;
}

Graph::~Graph()
{

}

const QSet<Node> &Graph::nodes() const
{
    return m_nodes;
}

const QSet<Edge> &Graph::edges() const
{
    return m_edges;
}

bool Graph::contains(const Node &node)
{
    return m_nodes.contains(node);
}

bool Graph::contains(const Edge &edge)
{
    return m_edges.contains(edge);
}

void Graph::setNodes(const QSet<Node> &value)
{
    m_nodes = value;
}

void Graph::setEdges(const QSet<Edge> &value)
{
    m_edges = value;
}

void Graph::addNode(const Node &node)
{
    m_nodes.insert(node);
}

void Graph::addEdge(const Node &from, const Node &to, int weight)
{
    m_edges.insert(Edge(from,to,weight));
}

// Возвращает вектор всех ребер, которые выходят из данной вершины.
QVector<Edge> Graph::outgoingEdgesFor(const Node &node) const
{
    QVector<Edge> result;

    foreach (Edge edge, m_edges)
        if (node == edge.from())
            result.push_back(edge);

    return result;
}

// Возвращает вектор всех вершин, в которые есть путь из данной вершины.
QVector<Node> Graph::outgoingNodesFor(const Node &node) const
{
    QVector<Node> result;

    QVector<Edge> outgoing_edges = outgoingEdgesFor(node);
    foreach (Edge edge, outgoing_edges)
        result.push_back(edge.to());

    return result;
}

// Возвращает вектор всех ребер, которые входят в данную вершину.
QVector<Edge> Graph::incomingEdgesFor(const Node &node) const
{
    QVector<Edge> result;

    foreach (Edge edge, m_edges)
        if (node == edge.to())
            result.push_back(edge);

    return result;
}

// Возвращает вектор вершин, из которых есть путь в данную вершину
QVector<Node> Graph::incomingNodesFor(const Node &node) const
{
    QVector<Node> result;

    QVector<Edge> incoming_edges = incomingEdgesFor(node);
    foreach (Edge edge, incoming_edges)
        result.push_back(edge.from());

    return result;
}

// ============================ ПОИСК ДРЕВА КРАТЧАЙШЕГО ПУТИ (Shortest Path Tree - SPT)

// Возвращает кратчайшее древо пути с корнем в вершине root.
// Оперирует на копии графа с целью исключить изменения данного обьекта.
// Использует нижележащие вспомогательные функции.
QVector<Node> Graph::SP(const Node &from, const Node &to) const
{
    return SPT(from).DFS_RootTo(to);
}

Tree Graph::SPT(const Node &root) const
{
    Graph graph_copy = *this;
    graph_copy.unpickAll(); // помечаем все вершины невыбранными
    graph_copy.initNodeWeights(root); // инициализирум веса вершин


    Node rootSPT = Node(0,0);
    bool isFirstNode = true;
    while (graph_copy.hasUnpickedNode())
    {
        Node lightest_node = graph_copy.lightestUnpickedNode(); // выбираем вершину с минимальным весом
        graph_copy.pick(lightest_node);

        graph_copy.updateNeighbourNodeWeights(lightest_node); // обновляем веса ее соседей
        if (isFirstNode) // первая выбранная вершина - корень нового дерева, для него ребро не выбираем
        {
            rootSPT = lightest_node;
            isFirstNode = false;
            continue;
        }
        graph_copy.pickConnectingEdgeFor(lightest_node); // выбираем соответствующее ей ребро
    }

    Graph graph (graph_copy.m_pickedNodes, graph_copy.m_pickedEdges); // создаем граф с выбраными элементами
    return Tree(rootSPT, graph);

}

// Убирает конкретную вершину из списка невыбранных и добавляет ее в список выбранных.
void Graph::pick(const Node &node)
{
    m_unpickedNodes.remove(node);
    m_pickedNodes.insert(node);
}

// Убирает конкретное ребро из списка невыбранных и добавляет его в список выбранных.
void Graph::pick(const Edge &edge)
{
    m_unpickedEdges.remove(edge);
    m_pickedEdges.insert(edge);
}

// Помечает все вершины и ребра невыбранными.
void Graph::unpickAll()
{
    foreach (Node node, m_nodes)
        m_unpickedNodes.insert(node);

    foreach (Edge edge, m_edges)
        m_unpickedEdges.insert(edge);
}

// Проверяем, есть ли среди выбранных вершин данная вершина.
bool Graph::isPicked(const Node &node) const
{
    return m_pickedNodes.contains(node);
}

// Проверяем, есть ли среди выбранных ребер данное ребро.
bool Graph::isPicked(const Edge &edge) const
{
    return m_pickedEdges.contains(edge);
}

// Выбирает подходяшее ребро для конкретной вершины.
// Когда выбрана вершина, должно быть выбрано и подходящее для нее ребро.
Edge Graph::connectingEdgeFor(const Node& node) const
{
    return m_connectingEdge[node];
}

void Graph::setConnectingEdgeFor(const Node &node, const Edge &edge)
{
    m_connectingEdge[node] = edge;
}

Edge Graph::connectingEdgeOf(const Node &from, const Node &to) const
{
    foreach (Edge edge, m_edges)
        if (edge.from() == from && edge.to() == to)
            return edge;

    return Edge();
}


void Graph::pickConnectingEdgeFor(const Node &node)
{
    pick(connectingEdgeFor(node));
}

// Узнаем вес вершины либо устанавливаем его.
int Graph::weightOf(const Node &node) const
{
    return m_nodeWeight[node];
}

void Graph::setWeightOf(const Node &node, int weight)
{
    m_nodeWeight[node] = weight;
}

// Возвращает TRUE, если в графе еще есть невыбранные вершины.
bool Graph::hasUnpickedNode() const
{
    return !m_unpickedNodes.empty();
}

// Возвращает вершину с минимальным весом среди невыбранных.
Node Graph::lightestUnpickedNode() const
{
    Node result = m_unpickedNodes.values().first();

    foreach (Node node, m_unpickedNodes)
        if (weightOf(node) < weightOf(result))
            result = node;

    return result;
}

// Возвращает вектор вершин, являющихся соседними для данной вершины.
QVector<Node> Graph::unpickedNeighbourNodesFor(const Node &node) const
{
    QVector<Node> result;

    foreach (Node neighbour_node, outgoingNodesFor(node))
        if (!isPicked(neighbour_node))
            result.push_back(neighbour_node);

    return result;
}

// Инициализирует вес стартовой вершины нулем, а всех остальных - тысячей.
void Graph::initNodeWeights(const Node &startnode)
{
    setWeightOf(startnode, 0);
    foreach (Node node, m_nodes)
        if (node != startnode)
            setWeightOf(node, 1000);
}

// Обновляет веса соседних вершин, если их новый вес меньше.
// Устанавливает соответствующее ребро между данной вершиной и соседней связывающим их (кратчайшим).
void Graph::updateNeighbourNodeWeights(const Node &node)
{
    foreach (Node neighbour, unpickedNeighbourNodesFor(node))
    {
        Edge node_to_neighbour = connectingEdgeOf(node,neighbour);

        int old_weight = weightOf(neighbour);
        int new_weight = weightOf(node) + node_to_neighbour.weight();

        if (new_weight < old_weight)
        {
            setWeightOf(neighbour, new_weight);
            setConnectingEdgeFor(neighbour, node_to_neighbour);
        }
    }
}



