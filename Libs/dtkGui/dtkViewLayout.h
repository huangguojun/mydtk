/* dtkViewLayout.h ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Wed May 16 09:38:08 2012 (+0200)
 * Version: $Id: 826f0fd0c4df387c2143b38e0780e45faf290345 $
 * Last-Updated: ven. oct. 11 14:56:46 2013 (+0200)
 *           By: Etienne
 *     Update #: 17
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKVIEWLAYOUT_H
#define DTKVIEWLAYOUT_H

#include <dtkGuiSupportExport.h>

#include <QtWidgets>

class dtkAbstractView;

class dtkViewLayoutItem;
class dtkViewLayoutPrivate;

class DTKGUISUPPORT_EXPORT dtkViewLayout : public QFrame
{
    Q_OBJECT

public:
    dtkViewLayout(QWidget *parent = 0);
    ~dtkViewLayout(void);

public:
    dtkViewLayoutItem *root(void);
    dtkViewLayoutItem *current(void);

public:
    void setCurrent(dtkViewLayoutItem *item);

signals:
    void focused(dtkAbstractView *view);
    void unfocused(dtkAbstractView *view);

public slots:
    void clear(void);

private:
    dtkViewLayoutPrivate *d;
};

#endif
