/* dtkAbstractViewInteractor.h ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Thu Jan 29 14:36:41 2009 (+0100)
 * Version: $Id: 6c8937b8317509caceeebea07226754bdd2b82d7 $
 * Last-Updated: Tue Apr 24 12:50:05 2012 (+0200)
 *           By: tkloczko
 *     Update #: 89
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKABSTRACTVIEWINTERACTOR_H
#define DTKABSTRACTVIEWINTERACTOR_H

#include "dtkAbstractObject.h"

class dtkAbstractViewInteractorPrivate;

class dtkAbstractData;
class dtkAbstractView;

// /////////////////////////////////////////////////////////////////
// dtkAbstractViewInteractor interface
// /////////////////////////////////////////////////////////////////

class DTKCORESUPPORT_EXPORT dtkAbstractViewInteractor : public dtkAbstractObject
{
    Q_OBJECT

public:
    dtkAbstractViewInteractor(void);
    dtkAbstractViewInteractor(const dtkAbstractViewInteractor &other);
    virtual ~dtkAbstractViewInteractor(void);

public:
    virtual QString description(void) const = 0;
    virtual QStringList handled(void) const = 0;

    bool enabled(void) const;
    virtual void enable(void);
    virtual void disable(void);

    virtual void setData(dtkAbstractData *data);
    virtual void setData(dtkAbstractData *data, int channel);
    virtual void setData(dtkAbstractData *data, int channel, int frame);
    virtual void setView(dtkAbstractView *view);

    virtual dtkAbstractData *data(void);
    virtual dtkAbstractView *view(void);

    virtual dtkAbstractData *output(void);
    virtual dtkAbstractData *output(int channel);
    virtual dtkAbstractData *output(int channel, int frame);

    virtual void predraw(void);
    virtual void draw(void);
    virtual void postdraw(void);

signals:
    void updated(void);

private:
    DTK_DECLARE_PRIVATE(dtkAbstractViewInteractor);
};

#endif
