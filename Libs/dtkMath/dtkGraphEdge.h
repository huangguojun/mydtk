/* dtkGraphEdge.h ---
 *
 * Author: Nicolas Niclausse
 * Copyright (C) 2013 - Nicolas Niclausse, Inria.
 * Created: 2013/08/29 15:55:18
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

class dtkGraphEdgePrivate;

class DTKMATH_EXPORT dtkGraphEdge
{
public:
    dtkGraphEdge(void);
    dtkGraphEdge(QObject *source, QObject *destination);
    dtkGraphEdge(const dtkGraphEdge& other);
    virtual ~dtkGraphEdge(void);

public:
    dtkGraphEdge& operator=(const dtkGraphEdge& other);
    bool operator==(const dtkGraphEdge& other) const;

public:
    virtual QObject *source(void) const;
    virtual QObject *destination(void) const;

public:
    void setSource(QObject *source);
    void setDestination(QObject *destination);

private:
    dtkGraphEdgePrivate *d;
};
