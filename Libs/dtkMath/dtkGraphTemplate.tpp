// Version: $Id: eee1612c96fcdb0f89ddca85d27e550273b93a1b $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#pragma once

#include <dtkLog>

// /////////////////////////////////////////////////////////////////
// dtkGraphTemplate implementation
// /////////////////////////////////////////////////////////////////

template < typename Node, typename Edge > inline dtkGraphTemplate<Node *, Edge>::dtkGraphTemplate(const dtkGraphTemplate& o) : m_nodes(o.m_nodes), m_edges(o.m_edges), m_predecessors(o.m_predecessors), m_successors(o.m_successors)
{
}

template < typename Node, typename Edge > inline dtkGraphTemplate<Node *, Edge>::~dtkGraphTemplate(void)
{
    this->clear();
}

template < typename Node, typename Edge > inline dtkGraphTemplate<Node *, Edge>& dtkGraphTemplate<Node *, Edge>::operator = (const dtkGraphTemplate& o)
{
    m_nodes = o.m_nodes;
    m_edges = o.m_edges;
    m_successors = o.m_successors;
    m_predecessors = o.m_predecessors;

    return *this;
}

template < typename Node, typename Edge > inline QString dtkGraphTemplate<Node *, Edge>::description(void) const
{
    return QString("dtkGraphTemplate contains: %1 nodes and %2 edges.").arg(m_nodes.size()).arg(m_edges.size());
}

template < typename Node, typename Edge > inline void dtkGraphTemplate<Node *, Edge>::clear(void)
{
    m_nodes.clear();
    m_edges.clear();
    m_predecessors.clear();
    m_successors.clear();
}

template < typename Node, typename Edge > inline bool dtkGraphTemplate<Node *, Edge>::contains(Node *n) const
{
    return m_nodes.contains(n);
}

template < typename Node, typename Edge > inline bool dtkGraphTemplate<Node *, Edge>::contains(const Edge& e) const
{
    return m_edges.contains(e);
}

template < typename Node, typename Edge > inline void dtkGraphTemplate<Node *, Edge>::addNode(Node *n)
{
    if (!m_nodes.contains(n)) {
        m_nodes << n;
    }
}

template < typename Node, typename Edge > inline void dtkGraphTemplate<Node *, Edge>::addEdge(const Edge& e)
{
    if (!m_edges.contains(e)) {
        m_edges << e;
        m_predecessors[e.destination()] << e.source();
        m_successors[e.source()] << e.destination();
    }
}

template < typename Node, typename Edge > inline void dtkGraphTemplate<Node *, Edge>::removeNode(Node *n)
{
    int id = m_nodes.indexOf(n);
    if (id >= 0) {
        int count = m_edges.size();
        for ( int i = count-1 ; i >= 0 ; i--) {
            if ((m_edges.at(i).source() == n) || (m_edges.at(i).destination() == n)) {
                this->removeEdge(m_edges.at(i));
            }
        }
        m_nodes.removeAt(id);

    } else {
        dtkWarn() << Q_FUNC_INFO << "removing unknown node from graph !";
    }
}

template < typename Node, typename Edge > inline void dtkGraphTemplate<Node *, Edge>::removeEdge(const Edge& e)
{
    if (m_edges.contains(e)) {
        Node *source = e.source();
        Node *destination = e.destination();
        m_edges.removeOne(e);
        m_predecessors[destination].removeOne(source);
        m_successors[source].removeOne(destination);
    }
}

template < typename Node, typename Edge > inline QList<Node *> dtkGraphTemplate<Node *, Edge>::rootNodes(void) const
{
    QList<Node *> root_nodes;
    auto cit = m_nodes.cbegin();
    auto cend = m_nodes.cend();
    for (; cit != cend; ++cit) {
        if (m_predecessors[(*cit)].isEmpty()) {
            root_nodes << *cit;
        }
    }

    return root_nodes;
}

template < typename Node, typename Edge > inline QList<Node *> dtkGraphTemplate<Node *, Edge>::succcessors(Node *n) const
{
    return m_successors[n];
}

template < typename Node, typename Edge > inline QList<Node *> dtkGraphTemplate<Node *, Edge>::predecessors(Node *n) const
{
    return m_predecessors[n];
}

template < typename Node, typename Edge > inline const QList<Node *>& dtkGraphTemplate<Node *, Edge>::nodes(void) const
{
    return m_nodes;
}

template < typename Node, typename Edge > inline const QList<Edge>& dtkGraphTemplate<Node *, Edge>::edges(void) const
{
    return m_edges;
}

template < typename Node, typename Edge > inline dtkGraphTemplate<Node *, Edge> dtkGraphTemplate<Node *, Edge>::subgraph(Node *from, Node *to) const
{
    dtkGraphTemplate subGraph;
    subGraph.addNode(from);

    QList<Node *> nodes = m_successors[from];

    while (!nodes.empty()) {
        Node *node = nodes.takeFirst();
        if (node == to) {
            break;
        }
        subGraph.addNode(node);
        for (Node *n : m_successors[node]) {
            if ((n != to) && !(subGraph.contains(n))) {
                nodes << n;
            }
        }
    }
    subGraph.addNode(to);
    for (const Edge& e : m_edges) {
        if (subGraph.contains(e.source()) && subGraph.contains(e.destination())) {
            subGraph.addEdge(e);
        }
    }
    return subGraph;
}

template < typename Node, typename Edge > inline QList<Node *> dtkGraphTemplate<Node *, Edge>::topologicalSort(void) const
{
// from wikipedia:
// L ← Empty list that will contain the sorted elements
// S ← Set of all nodes with no incoming edges
// while S is non-empty do
//     remove a node n from S
//     insert n into L
//     for each node m with an edge e from n to m do
//         remove edge e from the graph
//         if m has no other incoming edges then
//             insert m into S
// if graph has edges then
//     return error (graph has at least one cycle)
// else
//     return L (a topologically sorted order)

    QList<Node *> result;
    QList<Node *> rootNodes = this->rootNodes();
    // backup the edges list
    QList<Edge> tmp_edges = m_edges;

    dtkGraphTemplate<Node *, Edge> *self = const_cast<dtkGraphTemplate<Node *, Edge> *>(this);

    while (!rootNodes.empty()) {
        Node *n = rootNodes.takeFirst();
        result << n;
        for(Node *m : m_successors[n]) {
            int i = 0;
            bool found = false;
            while (i < m_edges.size() && !found) {
                Edge e = m_edges.at(i);
                if ((e.source() == n) && (e.destination() == m)) {
                    self->removeEdge(e);
                    if (m_predecessors[m].empty()) {
                        rootNodes << m;
                    }
                    found = true;
                }
                ++i;
            }
        }
    }

    if (!m_edges.empty()) {
        dtkError() << "Topological sorting has failed: the graph has at least one cycle";
        result.clear();
    }

    // rebuild edges
    for (const Edge& e : tmp_edges) {
        self->addEdge(e);
    }
    return result;
}

//
// dtkGraphTemplate.tpp ends here
