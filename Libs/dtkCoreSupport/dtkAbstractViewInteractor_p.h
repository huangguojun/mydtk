/* dtkAbstractViewInteractor_p.h ---
 *
 * Author: tkloczko
 * Copyright (C) 2011 - Thibaud Kloczko, Inria.
 * Created: Tue Apr 24 12:55:27 2012 (+0200)
 * Version: $Id: b9dca96302fa6d74fb196e6287e86f1aeb689fb1 $
 * Last-Updated: Tue Apr 24 14:27:38 2012 (+0200)
 *           By: tkloczko
 *     Update #: 9
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKABSTRACTVIEWINTERACTOR_P_H
#define DTKABSTRACTVIEWINTERACTOR_P_H

#include <dtkCoreSupportExport.h>

#include "dtkAbstractObject_p.h"

#include "dtkAbstractData.h"
#include "dtkAbstractView.h"
#include "dtkSmartPointer.h"

class dtkAbstractViewInteractor;

////////////////////////////////////////////////////
// dtkAbstractViewPrivate interface
////////////////////////////////////////////////////

class DTKCORESUPPORT_EXPORT dtkAbstractViewInteractorPrivate : public dtkAbstractObjectPrivate
{
public:
    dtkAbstractViewInteractorPrivate(dtkAbstractViewInteractor *q = 0) : dtkAbstractObjectPrivate(q)
    {
    }
    dtkAbstractViewInteractorPrivate(const dtkAbstractViewInteractorPrivate &other)
        : dtkAbstractObjectPrivate(other),
          enabled(other.enabled),
          data(other.data),
          view(other.view)
    {
    }

public:
    virtual ~dtkAbstractViewInteractorPrivate(void) {}

public:
    bool enabled;

public:
    dtkSmartPointer<dtkAbstractData> data;

public:
    dtkAbstractView *view;
};

////////////////////////////////////////////////////
// dtkAbstractViewInteractor protected constructors
////////////////////////////////////////////////////

DTK_IMPLEMENT_PRIVATE(dtkAbstractViewInteractor, dtkAbstractObject);

#endif
