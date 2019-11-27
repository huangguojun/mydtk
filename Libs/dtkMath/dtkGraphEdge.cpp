/* @(#)dtkGraphEdge.cpp ---
 *
 * Author: Nicolas Niclausse
 * Copyright (C) 2013 - Nicolas Niclausse, Inria.
 * Created: 2013/08/29 15:57:36
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkGraphEdge.h"

class dtkGraphEdgePrivate
{
public:
    QObject *source;
    QObject *destination;

};

dtkGraphEdge::dtkGraphEdge(void) : d(new dtkGraphEdgePrivate)
{
    d->source      = NULL;
    d->destination = NULL;
}

dtkGraphEdge::dtkGraphEdge(QObject *source, QObject *destination) : d(new dtkGraphEdgePrivate)
{
    d->source      = source;
    d->destination = destination;
}

dtkGraphEdge::dtkGraphEdge(const dtkGraphEdge& other) : d(new dtkGraphEdgePrivate)
{
    d->source      = other.d->source;
    d->destination = other.d->destination;
}

dtkGraphEdge& dtkGraphEdge::operator=(const dtkGraphEdge& other)
{
    d->source      = other.d->source;
    d->destination = other.d->destination;

    return (*this);
}

bool dtkGraphEdge::operator==(const dtkGraphEdge& other) const
{
    return (d->source == other.d->source) && (d->destination == other.d->destination);
}

dtkGraphEdge::~dtkGraphEdge(void)
{
    delete d;

    d = NULL;
}

QObject *dtkGraphEdge::source(void) const
{
    return d->source;
}

QObject *dtkGraphEdge::destination(void) const
{
    return d->destination;
}

void dtkGraphEdge::setSource(QObject *source)
{
    d->source = source;
}

void dtkGraphEdge::setDestination(QObject *destination)
{
    d->destination = destination;
}

