/* dtkAbstractDataConverter_p.h ---
 *
 * Author: tkloczko
 * Copyright (C) 2011 - Thibaud Kloczko, Inria.
 * Created: Tue Apr 24 10:57:11 2012 (+0200)
 * Version: $Id: a6b42946048d0e2d064f6a8c4e714b35eb5f0757 $
 * Last-Updated: Tue Apr 24 11:38:35 2012 (+0200)
 *           By: tkloczko
 *     Update #: 3
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKABSTRACTDATACONVERTER_P
#define DTKABSTRACTDATACONVERTER_P

#include <dtkCoreSupportExport.h>
#include "dtkAbstractObject_p.h"
#include "dtkSmartPointer.h"

class dtkAbstractDataConverter;
class dtkAbstractData;

////////////////////////////////////////////////////
// dtkAbstractDataConverterPrivate interface
////////////////////////////////////////////////////

class DTKCORESUPPORT_EXPORT dtkAbstractDataConverterPrivate : public dtkAbstractObjectPrivate
{
public:
    dtkAbstractDataConverterPrivate(dtkAbstractDataConverter *q = 0) : dtkAbstractObjectPrivate(q) {}
    dtkAbstractDataConverterPrivate(const dtkAbstractDataConverterPrivate& other) : dtkAbstractObjectPrivate(other),
        enabled(false),
        data(other.data) {}

public:
    virtual ~dtkAbstractDataConverterPrivate(void) {}

public:
    bool enabled;

public:
    dtkSmartPointer<dtkAbstractData> data;
};

////////////////////////////////////////////////////
// dtkAbstractDataConverter protected constructors
////////////////////////////////////////////////////

DTK_IMPLEMENT_PRIVATE(dtkAbstractDataConverter, dtkAbstractObject);

#endif
