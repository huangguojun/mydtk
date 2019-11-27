/* dtkAbstractDataReader.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Tue Feb 24 22:03:03 2009 (+0100)
 * Version: $Id: 5c74cff23582616b2e6d460dd610fe45ba62a5f8 $
 * Last-Updated: Tue Apr 24 11:36:27 2012 (+0200)
 *           By: tkloczko
 *     Update #: 59
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkAbstractDataReader.h"
#include "dtkAbstractDataReader_p.h"
#include "dtkAbstractData.h"

// /////////////////////////////////////////////////////////////////
// dtkAbstractDataReader implementation
// /////////////////////////////////////////////////////////////////

dtkAbstractDataReader::dtkAbstractDataReader(void) : dtkAbstractObject(*new dtkAbstractDataReaderPrivate(this), 0)
{
    DTK_D(dtkAbstractDataReader);

    d->enabled = false;
}

dtkAbstractDataReader::dtkAbstractDataReader(const dtkAbstractDataReader& other) : dtkAbstractObject(*new dtkAbstractDataReaderPrivate(*other.d_func()), other)
{

}

dtkAbstractDataReader::~dtkAbstractDataReader(void)
{

}

bool dtkAbstractDataReader::enabled(void) const
{
    DTK_D(const dtkAbstractDataReader);

    return d->enabled;
}

void dtkAbstractDataReader::enable(void)
{
    DTK_D(dtkAbstractDataReader);

    d->enabled = true;
}

void dtkAbstractDataReader::disable(void)
{
    DTK_D(dtkAbstractDataReader);

    d->enabled = false;
}

dtkAbstractData *dtkAbstractDataReader::data(void) const
{
    DTK_D(const dtkAbstractDataReader);

    return d->data;
}

/**
 * Set the data that has been read.
 */
void dtkAbstractDataReader::setData(dtkAbstractData *data)
{
    DTK_D(dtkAbstractDataReader);

    d->data = data;
}

//  Verify that the reader can handle this/these file(s).

bool dtkAbstractDataReader::canRead(const QString& file)
{
    DTK_UNUSED(file);

    return false;
}

bool dtkAbstractDataReader::canRead(const QStringList& files)
{
    if (files.size() == 1)
        return canRead(files[0]);

    return false;
}

bool dtkAbstractDataReader::read(const QString& file)
{
    DTK_D(dtkAbstractDataReader);

    d->file = file;

    return false;
}

bool dtkAbstractDataReader::read(const QStringList& files)
{
    DTK_D(dtkAbstractDataReader);

    d->files = files;

    //  Provide a sensible default for the case the list contains only one file.

    if (files.size() == 1)
        return read(files[0]);

    return false;
}

bool dtkAbstractDataReader::readInformation(const QString& path)
{
    DTK_UNUSED(path);
    return false;
}

bool dtkAbstractDataReader::readInformation(const QStringList& paths)
{
    if (paths.size() == 1)
        return readInformation(paths[0]);

    return false;
}

void dtkAbstractDataReader::setProgress(int value)
{
    emit progressed (value);
}

const QString& dtkAbstractDataReader::file(void) const
{
    DTK_D(const dtkAbstractDataReader);

    return d->file;
}

const QStringList& dtkAbstractDataReader::files(void) const
{
    DTK_D(const dtkAbstractDataReader);

    return d->files;
}
