/* dtkAbstractViewNavigator_p.h ---
 *
 * Author: tkloczko
 * Copyright (C) 2011 - Thibaud Kloczko, Inria.
 * Created: Tue Apr 24 14:34:26 2012 (+0200)
 * Version: $Id: c46b83e5d0f88bd808760e2addbca41e906ff9d4 $
 * Last-Updated: Tue Apr 24 14:34:50 2012 (+0200)
 *           By: tkloczko
 *     Update #: 2
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKABSTRACTVIEWNAVIGATOR_P_H
#define DTKABSTRACTVIEWNAVIGATOR_P_H

#include <dtkCoreSupportExport.h>

#include "dtkAbstractObject_p.h"

#include "dtkAbstractData.h"
#include "dtkAbstractView.h"
#include "dtkSmartPointer.h"

class dtkAbstractViewNavigator;

////////////////////////////////////////////////////
// dtkAbstractViewPrivate interface
////////////////////////////////////////////////////

class DTKCORESUPPORT_EXPORT dtkAbstractViewNavigatorPrivate : public dtkAbstractObjectPrivate
{
public:
    dtkAbstractViewNavigatorPrivate(dtkAbstractViewNavigator *q = 0) : dtkAbstractObjectPrivate(q)
    {
    }
    dtkAbstractViewNavigatorPrivate(const dtkAbstractViewNavigatorPrivate &other)
        : dtkAbstractObjectPrivate(other),
          enabled(other.enabled),
          data(other.data),
          view(other.view)
    {
    }

public:
    virtual ~dtkAbstractViewNavigatorPrivate(void) {}

public:
    bool enabled;

public:
    dtkSmartPointer<dtkAbstractData> data;

public:
    dtkAbstractView *view;
};

////////////////////////////////////////////////////
// dtkAbstractViewNavigator protected constructors
////////////////////////////////////////////////////

DTK_IMPLEMENT_PRIVATE(dtkAbstractViewNavigator, dtkAbstractObject);

#endif
