/* dtkAbstractDataSerializer_p.h ---
 *
 * Author: tkloczko
 * Copyright (C) 2011 - Thibaud Kloczko, Inria.
 * Created: Tue Apr 24 11:04:31 2012 (+0200)
 * Version: $Id: b616f8bcdb9b0b5974d3c416ecf34f8dbef4e61c $
 * Last-Updated: lun. févr.  3 13:40:16 2014 (+0100)
 *           By: Nicolas Niclausse
 *     Update #: 8
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKABSTRACTDATASERIALIZER_P
#define DTKABSTRACTDATASERIALIZER_P

#include <dtkCoreSupportExport.h>
#include "dtkAbstractObject_p.h"
#include "dtkAbstractDataSerializer.h"

class QByteArray;

////////////////////////////////////////////////////
// dtkAbstractDataSerializerPrivate interface
////////////////////////////////////////////////////

class DTKCORESUPPORT_EXPORT dtkAbstractDataSerializerPrivate : public dtkAbstractObjectPrivate
{
public:
    dtkAbstractDataSerializerPrivate(dtkAbstractDataSerializer *q = 0) : dtkAbstractObjectPrivate(q) {}
    dtkAbstractDataSerializerPrivate(const dtkAbstractDataSerializerPrivate& other) : dtkAbstractObjectPrivate(other),
        enabled(false),
        array(other.array) {}

public:
    virtual ~dtkAbstractDataSerializerPrivate(void) {}

public:
    bool enabled;

public:
    QByteArray *array;
};

////////////////////////////////////////////////////
// dtkAbstractDataSerializer protected constructors
////////////////////////////////////////////////////

DTK_IMPLEMENT_PRIVATE(dtkAbstractDataSerializer, dtkAbstractObject);

#endif
