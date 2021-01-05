/* dtkAbstractDataConverter.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Tue Feb 24 22:03:03 2009 (+0100)
 * Version: $Id: 51990e1277d19bb8f1dab2af73e008a03f5c561f $
 * Last-Updated: Tue Apr 24 11:38:50 2012 (+0200)
 *           By: tkloczko
 *     Update #: 47
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkAbstractDataConverter.h"
#include "dtkAbstractData.h"
#include "dtkAbstractDataConverter_p.h"

// /////////////////////////////////////////////////////////////////
// dtkAbstractDataConverter implementation
// /////////////////////////////////////////////////////////////////

dtkAbstractDataConverter::dtkAbstractDataConverter(void)
    : dtkAbstractObject(*new dtkAbstractDataConverterPrivate(this), 0)
{
    DTK_D(dtkAbstractDataConverter);

    d->enabled = false;
}

dtkAbstractDataConverter::dtkAbstractDataConverter(const dtkAbstractDataConverter &other)
    : dtkAbstractObject(*new dtkAbstractDataConverterPrivate(*other.d_func()), other)
{
}

dtkAbstractDataConverter::~dtkAbstractDataConverter(void) {}

bool dtkAbstractDataConverter::enabled(void) const
{
    DTK_D(const dtkAbstractDataConverter);

    return d->enabled;
}

void dtkAbstractDataConverter::enable(void)
{
    DTK_D(dtkAbstractDataConverter);

    d->enabled = true;
}

void dtkAbstractDataConverter::disable(void)
{
    DTK_D(dtkAbstractDataConverter);

    d->enabled = false;
}

dtkAbstractData *dtkAbstractDataConverter::data(void) const
{
    DTK_D(const dtkAbstractDataConverter);

    return d->data;
}

void dtkAbstractDataConverter::setData(dtkAbstractData *data)
{
    DTK_D(dtkAbstractDataConverter);

    d->data = data;
}

bool dtkAbstractDataConverter::canConvert(const QString &toType)
{
    return (this->toType() == toType);
}

dtkAbstractData *dtkAbstractDataConverter::convert(void)
{
    return NULL;
}

void dtkAbstractDataConverter::setProgress(int value)
{
    emit progressed(value);
}
