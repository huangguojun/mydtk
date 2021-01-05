/* dtkAbstractView_p.h ---
 *
 * Author: tkloczko
 * Copyright (C) 2011 - Thibaud Kloczko, Inria.
 * Created: Mon Apr 23 16:46:48 2012 (+0200)
 * Version: $Id: 10b6395358f70ea1d7705898ff3836822c242aa3 $
 * Last-Updated: Tue Jan 21 16:28:37 2014 (+0100)
 *           By: Selim Kraria
 *     Update #: 11
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKABSTRACTVIEW_P_H
#define DTKABSTRACTVIEW_P_H

#include "dtkAbstractObject_p.h"
#include "dtkAbstractViewAnimator.h"
#include "dtkAbstractViewInteractor.h"
#include "dtkAbstractViewNavigator.h"
#include "dtkSmartPointer.h"
#include <dtkCoreSupportExport.h>

class dtkAbstractView;

////////////////////////////////////////////////////
// dtkAbstractViewPrivate interface
////////////////////////////////////////////////////

class DTKCORESUPPORT_EXPORT dtkAbstractViewPrivate : public dtkAbstractObjectPrivate
{
public:
    dtkAbstractViewPrivate(dtkAbstractView *q = 0) : dtkAbstractObjectPrivate(q) {}
    dtkAbstractViewPrivate(const dtkAbstractViewPrivate &other)
        : dtkAbstractObjectPrivate(other),
          stereo(other.stereo),
          animators(other.animators),
          navigators(other.navigators),
          interactors(other.interactors)
    {
    }

public:
    virtual ~dtkAbstractViewPrivate(void) {}

public:
    bool stereo;

public:
    QString name;

public:
    QMap<QString, dtkSmartPointer<dtkAbstractViewAnimator>> animators;
    QMap<QString, dtkSmartPointer<dtkAbstractViewNavigator>> navigators;
    QMap<QString, dtkSmartPointer<dtkAbstractViewInteractor>> interactors;
};

////////////////////////////////////////////////////
// dtkAbstractView protected constructors
////////////////////////////////////////////////////

DTK_IMPLEMENT_PRIVATE(dtkAbstractView, dtkAbstractObject);

#endif
