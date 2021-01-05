/* dtkPlotViewPanner.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Fri Jun  8 12:55:56 2012 (+0200)
 * Version: $Id: 4f7b20e0de80f929dfdec90dbe7d6912f24799ac $
 * Last-Updated: Mon Jun 24 10:38:27 2013 (+0200)
 *           By: Selim Kraria
 *     Update #: 34
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkPlotViewPanner.h"
#include "dtkPlotView.h"

#include <qwt_plot.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_panner.h>

class dtkPlotViewPannerPrivate : public QwtPlotPanner
{
public:
    dtkPlotViewPannerPrivate(QwtPlotCanvas *canvas);
    ~dtkPlotViewPannerPrivate(void);
};

dtkPlotViewPannerPrivate::dtkPlotViewPannerPrivate(QwtPlotCanvas *canvas) : QwtPlotPanner(canvas) {}

dtkPlotViewPannerPrivate::~dtkPlotViewPannerPrivate(void) {}

// /////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////

dtkPlotViewPanner::dtkPlotViewPanner(dtkPlotView *parent) : QObject(parent)
{
    QwtPlot *plot = reinterpret_cast<QwtPlot *>(parent->plotWidget());
    d = new dtkPlotViewPannerPrivate(reinterpret_cast<QwtPlotCanvas *>(plot->canvas()));
}

dtkPlotViewPanner::~dtkPlotViewPanner(void)
{
    delete d;

    d = NULL;
}

void dtkPlotViewPanner::activate(void)
{
    d->setEnabled(true);
}

void dtkPlotViewPanner::deactivate(void)
{
    d->setEnabled(false);
}
