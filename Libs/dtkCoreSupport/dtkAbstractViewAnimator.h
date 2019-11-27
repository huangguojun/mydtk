/* dtkAbstractViewAnimator.h ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Fri Jan 30 16:13:08 2009 (+0100)
 * Version: $Id: e60b3294967847a1eb6b0c4faf133a02da6523aa $
 * Last-Updated: Tue Apr 24 14:21:55 2012 (+0200)
 *           By: tkloczko
 *     Update #: 31
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKABSTRACTVIEWANIMATOR_H
#define DTKABSTRACTVIEWANIMATOR_H

#include "dtkAbstractObject.h"

class dtkAbstractData;
class dtkAbstractView;
class dtkAbstractViewAnimatorPrivate;

// /////////////////////////////////////////////////////////////////
// dtkAbstractViewAnimator interface
// /////////////////////////////////////////////////////////////////

class DTKCORESUPPORT_EXPORT dtkAbstractViewAnimator : public dtkAbstractObject
{
    Q_OBJECT

public:
    dtkAbstractViewAnimator(void);
    dtkAbstractViewAnimator(const dtkAbstractViewAnimator& other);
    virtual ~dtkAbstractViewAnimator(void);

public:
    virtual QString description(void) const = 0;
    virtual QStringList handled(void) const = 0;

    dtkAbstractData *data(void) const;
    dtkAbstractView *view(void) const;

    virtual void setData(dtkAbstractData *data);
    virtual void setView(dtkAbstractView *view);

    bool enabled(void) const;
    void  enable(void);
    void disable(void);

public slots:
    virtual void start(void);
    virtual void  stop(void);

private:
    DTK_DECLARE_PRIVATE(dtkAbstractViewAnimator);
};

#endif
