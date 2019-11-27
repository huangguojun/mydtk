/* dtkAbstractViewAnimator.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Fri Jan 30 16:14:03 2009 (+0100)
 * Version: $Id: 10d9dbacb9837d61ff95f2fc4f874aeab8accd81 $
 * Last-Updated: lun. févr.  3 16:42:52 2014 (+0100)
 *           By: Thibaud Kloczko
 *     Update #: 26
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkAbstractViewAnimator.h"
#include "dtkAbstractViewAnimator_p.h"

// /////////////////////////////////////////////////////////////////
// dtkAbstractViewAnimator implementation
// /////////////////////////////////////////////////////////////////

dtkAbstractViewAnimator::dtkAbstractViewAnimator(void) : dtkAbstractObject(*new dtkAbstractViewAnimatorPrivate(this), 0)
{
    DTK_D(dtkAbstractViewAnimator);

    d->enabled = false;
    d->view = NULL;
}

dtkAbstractViewAnimator::dtkAbstractViewAnimator(const dtkAbstractViewAnimator& other) : dtkAbstractObject(*new dtkAbstractViewAnimatorPrivate(*other.d_func()), other)
{

}

dtkAbstractViewAnimator::~dtkAbstractViewAnimator(void)
{

}

dtkAbstractData *dtkAbstractViewAnimator::data(void) const
{
    DTK_D(const dtkAbstractViewAnimator);

    return d->data;
}

dtkAbstractView *dtkAbstractViewAnimator::view(void) const
{
    DTK_D(const dtkAbstractViewAnimator);

    return d->view;
}

void dtkAbstractViewAnimator::setData(dtkAbstractData *data)
{
    DTK_D(dtkAbstractViewAnimator);

    d->data = data;
}

void dtkAbstractViewAnimator::setView(dtkAbstractView *view)
{
    DTK_D(dtkAbstractViewAnimator);

    d->view = view;
}

bool dtkAbstractViewAnimator::enabled(void) const
{
    DTK_D(const dtkAbstractViewAnimator);

    return d->enabled;
}

void dtkAbstractViewAnimator::enable(void)
{
    DTK_D(dtkAbstractViewAnimator);

    d->enabled = true;
}

void dtkAbstractViewAnimator::disable(void)
{
    DTK_D(dtkAbstractViewAnimator);

    d->enabled = false;
}

void dtkAbstractViewAnimator::start(void)
{
    //DTK_DEFAULT_IMPLEMENTATION;
}

void dtkAbstractViewAnimator::stop(void)
{
    //DTK_DEFAULT_IMPLEMENTATION;
}
