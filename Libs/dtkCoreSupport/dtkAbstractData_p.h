/* dtkAbstractData_p.h ---
 *
 * Author: tkloczko
 * Copyright (C) 2011 - Thibaud Kloczko, Inria.
 * Created: Mon Apr 23 12:12:08 2012 (+0200)
 * Version: $Id: 82e5530d5e07fe22b38613f01871a9ed85e38c9b $
 * Last-Updated: mar. avril  8 18:03:40 2014 (+0200)
 *           By: Nicolas Niclausse
 *     Update #: 22
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKABSTRACTDATA_P_H
#define DTKABSTRACTDATA_P_H

#include <dtkCoreSupportExport.h>
#include "dtkAbstractObject_p.h"

class dtkAbstractData;

////////////////////////////////////////////////////
// dtkAbstractDataPrivate interface
////////////////////////////////////////////////////

class DTKCORESUPPORT_EXPORT dtkAbstractDataPrivate : public dtkAbstractObjectPrivate
{
public:
    dtkAbstractDataPrivate(dtkAbstractData *q = 0) : dtkAbstractObjectPrivate(q) {}
    dtkAbstractDataPrivate(const dtkAbstractDataPrivate& other, dtkAbstractData *q = 0) : dtkAbstractObjectPrivate(other, q),
        readers(other.readers),
        writers(other.writers),
        converters(other.converters),
        serializers(other.serializers),
        deserializers(other.deserializers),
        path(other.path),
        paths(other.paths),
        numberOfChannels(other.numberOfChannels),
        thumbnails(other.thumbnails) {}

public:
    virtual ~dtkAbstractDataPrivate(void) {}

public:
    QMap<QString, bool> readers;
    QMap<QString, bool> writers;
    QMap<QString, bool> converters;
    QMap<QString, bool> serializers;
    QMap<QString, bool> deserializers;

public:
    QString     path;
    QStringList paths;

public:
    int numberOfChannels;

public:
    QList<QImage> thumbnails;
};

////////////////////////////////////////////////////
// dtkAbstractData protected constructors
////////////////////////////////////////////////////

DTK_IMPLEMENT_PRIVATE(dtkAbstractData, dtkAbstractObject);

#endif
