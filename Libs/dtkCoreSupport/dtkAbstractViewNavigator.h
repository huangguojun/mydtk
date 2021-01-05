/* dtkAbstractViewNavigator.h ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Thu Jan 29 14:36:41 2009 (+0100)
 * Version: $Id: 418961cc8464f08ba17c9a5399c13a05d57f1154 $
 * Last-Updated: Tue Apr 24 14:35:43 2012 (+0200)
 *           By: tkloczko
 *     Update #: 84
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKABSTRACTVIEWNAVIGATOR_H
#define DTKABSTRACTVIEWNAVIGATOR_H

#include "dtkAbstractObject.h"

class dtkAbstractViewNavigatorPrivate;

class dtkAbstractData;
class dtkAbstractView;

// /////////////////////////////////////////////////////////////////
// dtkAbstractViewNavigator interface
// /////////////////////////////////////////////////////////////////

class DTKCORESUPPORT_EXPORT dtkAbstractViewNavigator : public dtkAbstractObject
{
    Q_OBJECT

public:
    dtkAbstractViewNavigator(void);
    dtkAbstractViewNavigator(const dtkAbstractViewNavigator &other);
    virtual ~dtkAbstractViewNavigator(void);

public:
    virtual QString description(void) const = 0;
    virtual QStringList handled(void) const = 0;

    bool enabled(void) const;
    virtual void enable(void);
    virtual void disable(void);

    virtual void setData(dtkAbstractData *data);
    virtual void setView(dtkAbstractView *view);

    virtual dtkAbstractData *data(void);
    virtual dtkAbstractView *view(void);

    virtual dtkAbstractData *output(void);
    virtual dtkAbstractData *output(int channel);
    virtual dtkAbstractData *output(int channel, int frame);

    virtual void predraw(void) {}
    virtual void draw(void) {}
    virtual void postdraw(void) {}

signals:
    void updated(void);

private:
    DTK_DECLARE_PRIVATE(dtkAbstractViewNavigator);
};

#endif
