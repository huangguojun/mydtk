/* dtkAbstractObject_p.h ---
 *
 * Author: tkloczko
 * Copyright (C) 2011 - Thibaud Kloczko, Inria.
 * Created: Mon Apr 23 10:50:52 2012 (+0200)
 * Version: $Id: 07ba45cf680692e9fb68a380f58638ff4844bfd1 $
 * Last-Updated: mar. avril  8 18:05:20 2014 (+0200)
 *           By: Nicolas Niclausse
 *     Update #: 13
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKABSTRACTOBJECT_P_H
#define DTKABSTRACTOBJECT_P_H

#include "dtkAbstractObject.h"
#include <dtkCoreSupportExport.h>

#include <QtCore>

////////////////////////////////////////////////////
// dtkAbstractObjectPrivate interface
////////////////////////////////////////////////////

class DTKCORESUPPORT_EXPORT dtkAbstractObjectPrivate
{
public:
    dtkAbstractObjectPrivate(dtkAbstractObject *q = 0) : q_ptr(q) {}
    dtkAbstractObjectPrivate(const dtkAbstractObjectPrivate &other, dtkAbstractObject *q = 0)
        : q_ptr(q), values(other.values), properties(other.properties), metadatas(other.metadatas)
    {
    }

public:
    virtual ~dtkAbstractObjectPrivate(void) {}

public:
    friend class dtkAbstractObject;

public:
    dtkAbstractObject *q_ptr;

public:
    QAtomicInt count;

public:
    QHash<QString, QStringList> values;
    QHash<QString, QString> properties;

public:
    QHash<QString, QStringList> metadatas;

public:
    bool isDeferredDeletionEnabled;
};

////////////////////////////////////////////////////
// dtkAbstractObject inline methods
////////////////////////////////////////////////////

inline dtkAbstractObject::dtkAbstractObject(dtkAbstractObjectPrivate &dd, QObject *parent)
    : QObject(parent), d_ptr(&dd)
{
}

inline dtkAbstractObject::dtkAbstractObject(dtkAbstractObjectPrivate &dd,
                                            const dtkAbstractObject &other)
    : QObject(other.parent()), d_ptr(&dd)
{
}

#endif // DTKABSTRACTOBJECT_P_H
