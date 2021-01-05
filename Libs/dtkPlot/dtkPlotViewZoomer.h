/* dtkPlotViewZoomer.h ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Fri Jun  8 12:52:22 2012 (+0200)
 * Version: $Id: d8db0f0390b1d5285c9c761b70ceba94ad41f142 $
 * Last-Updated: mer. avril  9 13:22:49 2014 (+0200)
 *           By: Nicolas Niclausse
 *     Update #: 57
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKPLOTVIEWZOOMER_H
#define DTKPLOTVIEWZOOMER_H

#include <dtkPlotExport.h>

#include <QColor>
#include <QObject>
#include <QRectF>

class dtkPlotView;
class dtkPlotViewZoomerPrivate;

class DTKPLOT_EXPORT dtkPlotViewZoomer : public QObject
{
    Q_OBJECT

public:
    dtkPlotViewZoomer(dtkPlotView *parent);
    ~dtkPlotViewZoomer(void);

public:
    void activate(void);
    void deactivate(void);

    bool isActivated(void) const;

public:
    QColor color(void) const;

    void setColor(const QColor &color);

public:
    void updateBase(const QRectF &rectf);

private:
    dtkPlotViewZoomerPrivate *d;
};

#endif
