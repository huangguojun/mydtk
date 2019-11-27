/* @(#)dtkGraph.h ---
 *
 * Author: Nicolas Niclausse
 * Copyright (C) 2013 - Nicolas Niclausse, Inria.
 * Created: 2013/08/29 14:24:15
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#pragma once

#include <dtkMathExport.h>

#include <QtCore>

class dtkGraphEdge;
class dtkGraphPrivate;

class DTKMATH_EXPORT dtkGraph : public QObject
{
    Q_OBJECT

public:
    dtkGraph(void);
    dtkGraph(const dtkGraph& g);
    virtual ~dtkGraph(void);

public:
    dtkGraph& operator =(const dtkGraph& other);

public:
    virtual void addNode(QObject *o);
    virtual void addEdge(dtkGraphEdge e);
    virtual void deleteNode(QObject *o);
    virtual void deleteEdge(dtkGraphEdge e);

public:
    QList<QObject *> rootNodes(void);
    QList<QObject *> succcessors(QObject *node);
    QList<QObject *> predecessors(QObject *node);

public:
    QList<QObject *>      nodes(void);
    QList< dtkGraphEdge > edges(void);

public:
    QList<QObject *> topologicalSort(void);

public:
    bool     contains(QObject *node);
    dtkGraph subgraph(QObject *from, QObject *to);

public:
    QString description(void) const;

public:
    void clear(void);

private:
    dtkGraphPrivate *d;
};

/* Q_DECLARE_METATYPE(dtkGraph) */
/* Q_DECLARE_METATYPE(dtkGraph *) */


