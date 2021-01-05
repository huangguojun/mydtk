/* dtkGraph.cpp ---
 *
 * Author: Nicolas Niclausse
 * Copyright (C) 2013 - Nicolas Niclausse, Inria.
 * Created: 2013/08/29 14:33:29
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include <dtkLog>

#include "dtkGraph.h"
#include "dtkGraphEdge.h"

class dtkGraphPrivate
{
public:
    QList<QObject *> nodes;
    QList<dtkGraphEdge> edges;

public:
    QMultiHash<QObject *, QObject *> predecessors;
    QMultiHash<QObject *, QObject *> successors;
};

dtkGraph::dtkGraph(void) : QObject(), d(new dtkGraphPrivate) {}

dtkGraph::dtkGraph(const dtkGraph &other) : QObject(), d(new dtkGraphPrivate)
{
    d->predecessors = other.d->predecessors;
    d->successors = other.d->successors;
    d->edges = other.d->edges;
    d->nodes = other.d->nodes;
}

dtkGraph &dtkGraph::operator=(const dtkGraph &other)
{
    return (*this);
}

dtkGraph::~dtkGraph(void)
{
    delete d;
}

void dtkGraph::addNode(QObject *o)
{
    if (!d->nodes.contains(o)) {
        d->nodes.append(o);
    }
}

void dtkGraph::deleteNode(QObject *o)
{
    if (d->nodes.contains(o)) {
        qlonglong count = d->edges.count();

        for (int i = count - 1; i >= 0; i--) {
            if ((d->edges.at(i).source() == o) || (d->edges.at(i).destination() == o)) {
                this->deleteEdge(d->edges.at(i));
            }
        }

        d->nodes.removeAll(o);
    } else {
        dtkWarn() << "removing unknown node from graph !" << o->objectName();
    }
}

void dtkGraph::addEdge(dtkGraphEdge e)
{

    if (!d->edges.contains(e)) {
        QObject *source = e.source();
        QObject *destination = e.destination();
        d->edges.append(e);
        d->predecessors.insert(destination, source);
        d->successors.insert(source, destination);
    }
}

void dtkGraph::deleteEdge(dtkGraphEdge e)
{
    if (d->edges.contains(e)) {
        QObject *source = e.source();
        QObject *destination = e.destination();
        d->edges.removeOne(e);
        d->predecessors.remove(destination, source);
        d->successors.remove(source, destination);
    }
}

QList<QObject *> dtkGraph::rootNodes(void)
{
    QList<QObject *> rootNodes;

    foreach (QObject *node, d->nodes)
        if (d->predecessors.values(node).count() == 0)
            rootNodes << node;

    return rootNodes;
}

QList<QObject *> dtkGraph::succcessors(QObject *node)
{
    return d->successors.values(node);
}

QList<QObject *> dtkGraph::predecessors(QObject *node)
{
    return d->predecessors.values(node);
}

QList<QObject *> dtkGraph::nodes(void)
{
    return d->nodes;
}

bool dtkGraph::contains(QObject *node)
{
    return d->nodes.contains(node);
}

QList<dtkGraphEdge> dtkGraph::edges(void)
{
    return d->edges;
}

void dtkGraph::clear()
{
    d->predecessors.clear();
    d->successors.clear();
    d->edges.clear();
    d->nodes.clear();
}

dtkGraph dtkGraph::subgraph(QObject *from, QObject *to)
{
    dtkGraph subGraph;
    subGraph.addNode(from);

    QList<QObject *> nodes = d->successors.values(from);

    while (!nodes.isEmpty()) {
        QObject *node = nodes.takeFirst();

        if (node == to) {
            break;
        }

        subGraph.addNode(node);

        foreach (QObject *n, d->successors.values(node)) {
            if ((n != to) && !(subGraph.contains(n))) {
                nodes << n;
            }
        }
    }

    subGraph.addNode(to);

    foreach (const dtkGraphEdge &e, d->edges) {
        if (subGraph.contains(e.source()) && subGraph.contains(e.destination())) {
            subGraph.addEdge(e);
        }
    }

    return subGraph;
}

QList<QObject *> dtkGraph::topologicalSort(void)
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

    QList<QObject *> result;
    QList<QObject *> rootNodes = this->rootNodes();
    // backup the edges list
    QList<dtkGraphEdge> tmp_edges = d->edges;

    while (!rootNodes.isEmpty()) {
        QObject *n = rootNodes.takeFirst();
        result << n;

        foreach (QObject *m, d->successors.values(n)) {
            int i = 0;
            bool found = false;

            while (i < d->edges.count() && !found) {
                dtkGraphEdge e = d->edges.at(i);

                if ((e.source() == n) && (e.destination() == m)) {
                    this->deleteEdge(e);

                    if (d->predecessors.values(m).isEmpty()) {
                        rootNodes << m;
                    }

                    found = true;
                }

                i++;
            }
        }
    }

    if (!d->edges.isEmpty()) {
        dtkError() << "Topological sorting has failed: the graph has at least "
                      "one cycle"
                   << d->edges.at(0).source()->objectName() << "->"
                   << d->edges.at(0).destination()->objectName();
        result.clear();
    }

    // rebuild edges
    foreach (dtkGraphEdge e, tmp_edges) {
        this->addEdge(e);
    }

    return result;
}

QString dtkGraph::description(void) const
{
    QString out;

    foreach (QObject *o, d->nodes) {
        out.append(o->objectName() + QString("\n"));
    }

    foreach (dtkGraphEdge e, d->edges) {
        out.append(e.source()->objectName() + QString(" -> ") + e.destination()->objectName()
                   + QString("\n"));
    }

    return out;
}
