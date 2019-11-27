/* dtkAbstractDataWriter_p.h ---
 *
 * Author: tkloczko
 * Copyright (C) 2011 - Thibaud Kloczko, Inria.
 * Created: Tue Apr 24 10:48:55 2012 (+0200)
 * Version: $Id: 125d632980b131286bb43fefeafd4fdfa780987b $
 * Last-Updated: Tue Apr 24 11:38:01 2012 (+0200)
 *           By: tkloczko
 *     Update #: 3
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKABSTRACTDATAWRITER_P_H
#define DTKABSTRACTDATAWRITER_P_H

#include <dtkCoreSupportExport.h>
#include "dtkAbstractObject_p.h"
#include "dtkSmartPointer.h"

class dtkAbstractDataWriter;
class dtkAbstractData;

////////////////////////////////////////////////////
// dtkAbstractDataWriterPrivate interface
////////////////////////////////////////////////////

class DTKCORESUPPORT_EXPORT dtkAbstractDataWriterPrivate : public dtkAbstractObjectPrivate
{
public:
    dtkAbstractDataWriterPrivate(dtkAbstractDataWriter *q = 0) : dtkAbstractObjectPrivate(q), data(NULL) {}
    dtkAbstractDataWriterPrivate(const dtkAbstractDataWriterPrivate& other) : dtkAbstractObjectPrivate(other),
        enabled(false),
        data(other.data) {}

public:
    virtual ~dtkAbstractDataWriterPrivate(void) {}

public:
    bool enabled;

public:
    dtkAbstractData *data;
};

////////////////////////////////////////////////////
// dtkAbstractDataWriter protected constructors
////////////////////////////////////////////////////

DTK_IMPLEMENT_PRIVATE(dtkAbstractDataWriter, dtkAbstractObject);

#endif
