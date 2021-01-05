// Version: $Id: 5554340de26fe2cb59c85f99f3426930fe56a9b9 $
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

#include "dtkGraphTemplateEdge.h"

#include <QtCore>

// /////////////////////////////////////////////////////////////////
// dtkGraphTemplate interface
// /////////////////////////////////////////////////////////////////

template<typename Node, typename Edge>
class dtkGraphTemplate
{
};

template<typename Node, typename Edge>
class dtkGraphTemplate<Node *, Edge>
{
public:
    dtkGraphTemplate(void) = default;
    dtkGraphTemplate(const dtkGraphTemplate &o);
    virtual ~dtkGraphTemplate(void);

public:
    dtkGraphTemplate &operator=(const dtkGraphTemplate &o);

public:
    QString description(void) const;

public:
    void clear(void);

public:
    bool contains(Node *n) const;
    bool contains(const Edge &e) const;

public:
    virtual void addNode(Node *n);
    virtual void addEdge(const Edge &e);
    virtual void removeNode(Node *n);
    virtual void removeEdge(const Edge &e);

public:
    QList<Node *> rootNodes(void) const;
    QList<Node *> succcessors(Node *n) const;
    QList<Node *> predecessors(Node *n) const;

public:
    const QList<Node *> &nodes(void) const;
    const QList<Edge> &edges(void) const;

public:
    QList<Node *> topologicalSort(void) const;

public:
    dtkGraphTemplate subgraph(Node *from, Node *to) const;

protected:
    QList<Node *> m_nodes;
    QList<Edge> m_edges;

    QHash<Node *, QList<Node *>> m_predecessors;
    QHash<Node *, QList<Node *>> m_successors;
};

// /////////////////////////////////////////////////////////////////

#include "dtkGraphTemplate.tpp"

//
// dtkGraphTemplate.h ends here
