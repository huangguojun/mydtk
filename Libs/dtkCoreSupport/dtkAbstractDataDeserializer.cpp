/* dtkAbstractDataSerializer.cpp ---
 *
 * Author: Nicolas Niclausse
 * Copyright (C) 2011 - Nicolas Niclausse, Inria.
 * Created: lun. oct. 17 13:20:01 2011 (+0200)
 * Version: $Id: b342e6c3b601136a7eac3b6e4b21846c8b600eb8 $
 * Last-Updated: mar. mai 29 09:10:40 2012 (+0200)
 *           By: Nicolas Niclausse
 *     Update #: 20
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkAbstractDataDeserializer.h"
#include "dtkAbstractData.h"
#include "dtkAbstractDataDeserializer_p.h"

// /////////////////////////////////////////////////////////////////
// dtkAbstractDataDeserializer implementation
// /////////////////////////////////////////////////////////////////

dtkAbstractDataDeserializer::dtkAbstractDataDeserializer(void)
    : dtkAbstractObject(*new dtkAbstractDataDeserializerPrivate(this), 0)
{
    DTK_D(dtkAbstractDataDeserializer);

    d->enabled = false;
}

dtkAbstractDataDeserializer::dtkAbstractDataDeserializer(const dtkAbstractDataDeserializer &other)
    : dtkAbstractObject(*new dtkAbstractDataDeserializerPrivate(*other.d_func()), other)
{
}

dtkAbstractDataDeserializer::~dtkAbstractDataDeserializer(void) {}

bool dtkAbstractDataDeserializer::enabled(void) const
{
    DTK_D(const dtkAbstractDataDeserializer);

    return d->enabled;
}

void dtkAbstractDataDeserializer::enable(void)
{
    DTK_D(dtkAbstractDataDeserializer);

    d->enabled = true;
}

void dtkAbstractDataDeserializer::disable(void)
{
    DTK_D(dtkAbstractDataDeserializer);

    d->enabled = false;
}

dtkAbstractData *dtkAbstractDataDeserializer::data(void) const
{
    DTK_D(const dtkAbstractDataDeserializer);

    return d->data;
}

void dtkAbstractDataDeserializer::setData(dtkAbstractData *data)
{
    DTK_D(dtkAbstractDataDeserializer);

    d->data = data;
}

dtkAbstractData *dtkAbstractDataDeserializer::deserialize(const QByteArray &array)
{
    DTK_UNUSED(array);
    return NULL;
}

void dtkAbstractDataDeserializer::setProgress(int value)
{
    emit progressed(value);
}
