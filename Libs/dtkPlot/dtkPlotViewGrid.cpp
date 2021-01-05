/* dtkPlotViewGrid.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Fri Jun  8 12:55:56 2012 (+0200)
 * Version: $Id: 079b97a4358a0f3760b8af7daae11dc74b0aaacd $
 * Last-Updated: Fri Jul  5 11:15:15 2013 (+0200)
 *           By: Selim Kraria
 *     Update #: 88
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkPlotViewGrid.h"
#include "dtkPlotView.h"

#include <qwt_plot.h>
#include <qwt_plot_grid.h>

class dtkPlotViewGridPrivate : public QwtPlotGrid
{
public:
    dtkPlotViewGridPrivate(dtkPlotView *parent);
    ~dtkPlotViewGridPrivate(void);
};

dtkPlotViewGridPrivate::dtkPlotViewGridPrivate(dtkPlotView *parent) : QwtPlotGrid()
{
    QwtPlot *plot = reinterpret_cast<QwtPlot *>(parent->plotWidget());

    attach(plot);
}

dtkPlotViewGridPrivate::~dtkPlotViewGridPrivate(void) {}

// /////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////

dtkPlotViewGrid::dtkPlotViewGrid(dtkPlotView *parent)
    : QObject(parent), d(new dtkPlotViewGridPrivate(parent))
{
    d->setPen(QPen(Qt::DotLine));
#if QWT_VERSION >= 0x060100
    d->setMajorPen(QPen(Qt::gray, 0, Qt::DotLine));
#else
    d->setMajPen(QPen(Qt::gray, 0, Qt::DotLine));
#endif
}

dtkPlotViewGrid::~dtkPlotViewGrid(void)
{
    delete d;

    d = NULL;
}

QColor dtkPlotViewGrid::color(void) const
{
#if QWT_VERSION >= 0x060100
    return d->majorPen().color();
#else
    return d->majPen().color();
#endif
}

void dtkPlotViewGrid::setColor(const QColor &color)
{
#if QWT_VERSION >= 0x060100
    d->setMajorPen(QPen(color, 0, Qt::DotLine));
#else
    d->setMajPen(QPen(color, 0, Qt::DotLine));
#endif
}
