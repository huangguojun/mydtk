/* dtkAbstractViewInteractor.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Thu Jan 29 14:38:31 2009 (+0100)
 * Version: $Id: f0b247cea9d4b9f413fddf684bfd980e52cfc1f0 $
 * Last-Updated: lun. févr.  3 16:44:16 2014 (+0100)
 *           By: Thibaud Kloczko
 *     Update #: 68
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkAbstractViewInteractor.h"
#include "dtkAbstractViewInteractor_p.h"

// /////////////////////////////////////////////////////////////////
// dtkAbstractViewInteractor implementation
// /////////////////////////////////////////////////////////////////

dtkAbstractViewInteractor::dtkAbstractViewInteractor(void) : dtkAbstractObject(*new dtkAbstractViewInteractorPrivate(this), 0)
{
    DTK_D(dtkAbstractViewInteractor);

    d->enabled = false;
    d->view = NULL;
}

dtkAbstractViewInteractor::dtkAbstractViewInteractor(const dtkAbstractViewInteractor& other) : dtkAbstractObject(*new dtkAbstractViewInteractorPrivate(*other.d_func()), other)
{

}

dtkAbstractViewInteractor::~dtkAbstractViewInteractor(void)
{

}

bool dtkAbstractViewInteractor::enabled(void) const
{
    DTK_D(const dtkAbstractViewInteractor);

    return d->enabled;
}

void dtkAbstractViewInteractor::enable(void)
{
    DTK_D(dtkAbstractViewInteractor);

    d->enabled = true;
}

void dtkAbstractViewInteractor::disable(void)
{
    DTK_D(dtkAbstractViewInteractor);

    d->enabled = false;
}

void dtkAbstractViewInteractor::setData(dtkAbstractData *data)
{
    DTK_D(dtkAbstractViewInteractor);

    d->data = data;
}

void dtkAbstractViewInteractor::setData(dtkAbstractData *data, int channel)
{
    //DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(data);
    Q_UNUSED(channel);
}

void dtkAbstractViewInteractor::setData(dtkAbstractData *data, int channel, int frame)
{
    //DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(data);
    Q_UNUSED(channel);
    Q_UNUSED(frame);
}

void dtkAbstractViewInteractor::setView(dtkAbstractView *view)
{
    DTK_D(dtkAbstractViewInteractor);

    d->view = view;
}

dtkAbstractData *dtkAbstractViewInteractor::data(void)
{
    DTK_D(dtkAbstractViewInteractor);

    return d->data;
}

dtkAbstractData *dtkAbstractViewInteractor::output(void)
{
    //DTK_DEFAULT_IMPLEMENTATION;

    return NULL;
}

dtkAbstractData *dtkAbstractViewInteractor::output(int channel)
{
    //DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(channel);

    return NULL;
}

dtkAbstractData *dtkAbstractViewInteractor::output(int channel, int frame)
{
    //DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(channel);
    Q_UNUSED(frame);

    return NULL;
}

dtkAbstractView *dtkAbstractViewInteractor::view(void)
{
    DTK_D(dtkAbstractViewInteractor);

    return d->view;
}

void dtkAbstractViewInteractor::predraw(void)
{
    //DTK_DEFAULT_IMPLEMENTATION;
}

void dtkAbstractViewInteractor::draw(void)
{
    //DTK_DEFAULT_IMPLEMENTATION;
}

void dtkAbstractViewInteractor::postdraw(void)
{
    //DTK_DEFAULT_IMPLEMENTATION;
}
