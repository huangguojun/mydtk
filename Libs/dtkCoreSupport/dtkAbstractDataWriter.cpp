/* dtkAbstractDataWriter.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Tue Feb 24 22:03:03 2009 (+0100)
 * Version: $Id: 76e3f72240f41e22b1293557d0e6e7deb69f553e $
 * Last-Updated: Tue Apr 24 11:38:11 2012 (+0200)
 *           By: tkloczko
 *     Update #: 38
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkAbstractDataWriter.h"
#include "dtkAbstractData.h"
#include "dtkAbstractDataWriter_p.h"

// /////////////////////////////////////////////////////////////////
// dtkAbstractDataWriter implementation
// /////////////////////////////////////////////////////////////////

dtkAbstractDataWriter::dtkAbstractDataWriter(void)
    : dtkAbstractObject(*new dtkAbstractDataWriterPrivate(this), 0)
{
    DTK_D(dtkAbstractDataWriter);

    d->enabled = false;
}

dtkAbstractDataWriter::dtkAbstractDataWriter(const dtkAbstractDataWriter &other)
    : dtkAbstractObject(*new dtkAbstractDataWriterPrivate(*other.d_func()), other)
{
}

dtkAbstractDataWriter::~dtkAbstractDataWriter(void) {}

bool dtkAbstractDataWriter::enabled(void) const
{
    DTK_D(const dtkAbstractDataWriter);

    return d->enabled;
}

void dtkAbstractDataWriter::enable(void)
{
    DTK_D(dtkAbstractDataWriter);

    d->enabled = true;
}

void dtkAbstractDataWriter::disable(void)
{
    DTK_D(dtkAbstractDataWriter);

    d->enabled = false;
}

dtkAbstractData *dtkAbstractDataWriter::data(void) const
{
    DTK_D(const dtkAbstractDataWriter);

    return d->data;
}

/**
 * Set the data that will be written.
 * The writer will increase the reference count of the data.
 */
void dtkAbstractDataWriter::setData(dtkAbstractData *data)
{
    DTK_D(dtkAbstractDataWriter);

    d->data = data;
}

bool dtkAbstractDataWriter::canWrite(const QString &file)
{
    DTK_UNUSED(file);

    return false;
}

bool dtkAbstractDataWriter::canWrite(const QStringList &files)
{
    DTK_UNUSED(files);

    return false;
}

bool dtkAbstractDataWriter::write(const QString &file)
{
    DTK_UNUSED(file);

    return false;
}

bool dtkAbstractDataWriter::write(const QStringList &files)
{
    DTK_UNUSED(files);

    return false;
}

void dtkAbstractDataWriter::setProgress(int value)
{
    emit progressed(value);
}

//!
/*!
 * Get the extension this writer prefers.  The list may depend on the
 * data set by setData.  The default implementation returns an
 * empty StringList, indicating no preferred extension.
 *
 * \return The list of supported extensions.
 */

QStringList dtkAbstractDataWriter::supportedFileExtensions(void) const
{
    return QStringList();
}
