/* dtkAbstractDataReader_p.h ---
 *
 * Author: tkloczko
 * Copyright (C) 2011 - Thibaud Kloczko, Inria.
 * Created: Tue Apr 24 10:34:11 2012 (+0200)
 * Version: $Id: 2ad4e5b9f21d8c6e428fd3d367cddd7172b02c7d $
 * Last-Updated: lun. févr.  3 13:40:09 2014 (+0100)
 *           By: Nicolas Niclausse
 *     Update #: 11
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKABSTRACTDATAREADER_P_H
#define DTKABSTRACTDATAREADER_P_H

#include "dtkAbstractObject_p.h"
#include "dtkSmartPointer.h"
#include <dtkCoreSupportExport.h>

class dtkAbstractDataReader;
class dtkAbstractData;

////////////////////////////////////////////////////
// dtkAbstractDataReaderPrivate interface
////////////////////////////////////////////////////

class DTKCORESUPPORT_EXPORT dtkAbstractDataReaderPrivate : public dtkAbstractObjectPrivate
{
public:
    dtkAbstractDataReaderPrivate(dtkAbstractDataReader *q = 0)
        : dtkAbstractObjectPrivate(q), data(NULL)
    {
    }
    dtkAbstractDataReaderPrivate(const dtkAbstractDataReaderPrivate &other)
        : dtkAbstractObjectPrivate(other),
          enabled(false),
          data(other.data),
          file(other.file),
          files(other.files)
    {
    }

public:
    virtual ~dtkAbstractDataReaderPrivate(void) {}

public:
    bool enabled;

public:
    dtkAbstractData *data;

public:
    QString file;
    QStringList files;
};

////////////////////////////////////////////////////
// dtkAbstractDataReader protected constructors
////////////////////////////////////////////////////

DTK_IMPLEMENT_PRIVATE(dtkAbstractDataReader, dtkAbstractObject);

#endif
