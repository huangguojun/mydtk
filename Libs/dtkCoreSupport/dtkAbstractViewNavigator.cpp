/* dtkAbstractViewNavigator.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Thu Jan 29 14:38:31 2009 (+0100)
 * Version: $Id: facc50573db19215d48f58741123693a4b8f6221 $
 * Last-Updated: Tue Apr 24 14:35:55 2012 (+0200)
 *           By: tkloczko
 *     Update #: 60
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkAbstractViewNavigator.h"
#include "dtkAbstractViewNavigator_p.h"

// /////////////////////////////////////////////////////////////////
// dtkAbstractViewNavigator implementation
// /////////////////////////////////////////////////////////////////

dtkAbstractViewNavigator::dtkAbstractViewNavigator(void)
    : dtkAbstractObject(*new dtkAbstractViewNavigatorPrivate(this), 0)
{
    DTK_D(dtkAbstractViewNavigator);

    d->enabled = false;
    d->view = NULL;
}

dtkAbstractViewNavigator::dtkAbstractViewNavigator(const dtkAbstractViewNavigator &other)
    : dtkAbstractObject(*new dtkAbstractViewNavigatorPrivate(*other.d_func()), other)
{
}

dtkAbstractViewNavigator::~dtkAbstractViewNavigator(void) {}

bool dtkAbstractViewNavigator::enabled(void) const
{
    DTK_D(const dtkAbstractViewNavigator);

    return d->enabled;
}

void dtkAbstractViewNavigator::enable(void)
{
    DTK_D(dtkAbstractViewNavigator);

    d->enabled = true;
}

void dtkAbstractViewNavigator::disable(void)
{
    DTK_D(dtkAbstractViewNavigator);

    d->enabled = false;
}

void dtkAbstractViewNavigator::setData(dtkAbstractData *data)
{
    DTK_D(dtkAbstractViewNavigator);

    d->data = data;
}

void dtkAbstractViewNavigator::setView(dtkAbstractView *view)
{
    DTK_D(dtkAbstractViewNavigator);

    d->view = view;
}

dtkAbstractData *dtkAbstractViewNavigator::data(void)
{
    DTK_D(dtkAbstractViewNavigator);

    return d->data;
}

dtkAbstractData *dtkAbstractViewNavigator::output(void)
{
    return 0;
}

dtkAbstractData *dtkAbstractViewNavigator::output(int channel)
{
    DTK_UNUSED(channel);
    return 0;
}

dtkAbstractData *dtkAbstractViewNavigator::output(int channel, int frame)
{
    DTK_UNUSED(channel);
    DTK_UNUSED(frame);

    return 0;
}

dtkAbstractView *dtkAbstractViewNavigator::view(void)
{
    DTK_D(dtkAbstractViewNavigator);

    return d->view;
}
