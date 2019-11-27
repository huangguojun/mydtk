/* dtkAbstractViewAnimator_p.h ---
 *
 * Author: tkloczko
 * Copyright (C) 2011 - Thibaud Kloczko, Inria.
 * Created: Tue Apr 24 14:25:52 2012 (+0200)
 * Version: $Id: b5230d0450feca2c0bbb3fa866302fe2133bd16e $
 * Last-Updated: Tue Apr 24 14:27:59 2012 (+0200)
 *           By: tkloczko
 *     Update #: 4
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKABSTRACTVIEWANIMATOR_P_H
#define DTKABSTRACTVIEWANIMATOR_P_H

#include <dtkCoreSupportExport.h>

#include "dtkAbstractObject_p.h"

#include "dtkAbstractData.h"
#include "dtkAbstractView.h"
#include "dtkSmartPointer.h"

class dtkAbstractViewAnimator;

////////////////////////////////////////////////////
// dtkAbstractViewPrivate interface
////////////////////////////////////////////////////

class DTKCORESUPPORT_EXPORT dtkAbstractViewAnimatorPrivate : public dtkAbstractObjectPrivate
{
public:
    dtkAbstractViewAnimatorPrivate(dtkAbstractViewAnimator *q = 0) : dtkAbstractObjectPrivate(q) {}
    dtkAbstractViewAnimatorPrivate(const dtkAbstractViewAnimatorPrivate& other) : dtkAbstractObjectPrivate(other),
        enabled(other.enabled),
        data(other.data),
        view(other.view) {}

public:
    virtual ~dtkAbstractViewAnimatorPrivate(void) {}

public:
    bool enabled;

public:
    dtkSmartPointer<dtkAbstractData> data;

public:
    dtkAbstractView *view;
};

////////////////////////////////////////////////////
// dtkAbstractViewAnimator protected constructors
////////////////////////////////////////////////////

DTK_IMPLEMENT_PRIVATE(dtkAbstractViewAnimator, dtkAbstractObject);

#endif
