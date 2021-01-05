/* dtkPlotViewZoomer.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Fri Jun  8 12:55:56 2012 (+0200)
 * Version: $Id: 0dcfdbc63347d4030663d402fad4b57dfb7015a7 $
 * Last-Updated: Fri Sep 20 17:20:55 2013 (+0200)
 *           By: Selim Kraria
 *     Update #: 102
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkPlotViewZoomer.h"
#include "dtkPlotView.h"

#include <qwt_plot.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_zoomer.h>

class dtkPlotViewZoomerPrivate : public QwtPlotZoomer
{
public:
    dtkPlotViewZoomerPrivate(QwtPlotCanvas *canvas);
    ~dtkPlotViewZoomerPrivate(void);
};

dtkPlotViewZoomerPrivate::dtkPlotViewZoomerPrivate(QwtPlotCanvas *canvas) : QwtPlotZoomer(canvas)
{
    setMousePattern(QwtEventPattern::MouseSelect2, Qt::RightButton, Qt::ControlModifier);
    setMousePattern(QwtEventPattern::MouseSelect3, Qt::RightButton);
}

dtkPlotViewZoomerPrivate::~dtkPlotViewZoomerPrivate(void) {}

// /////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////

dtkPlotViewZoomer::dtkPlotViewZoomer(dtkPlotView *parent) : QObject(parent)
{
    QwtPlot *plot = reinterpret_cast<QwtPlot *>(parent->plotWidget());
    d = new dtkPlotViewZoomerPrivate(reinterpret_cast<QwtPlotCanvas *>(plot->canvas()));
}

dtkPlotViewZoomer::~dtkPlotViewZoomer(void)
{
    delete d;

    d = NULL;
}

void dtkPlotViewZoomer::activate(void)
{
    d->setEnabled(true);
}

void dtkPlotViewZoomer::deactivate(void)
{
    d->setEnabled(false);
}

bool dtkPlotViewZoomer::isActivated(void) const
{
    return d->isEnabled();
}

QColor dtkPlotViewZoomer::color(void) const
{
    return d->rubberBandPen().color();
}

void dtkPlotViewZoomer::setColor(const QColor &color)
{
    d->setRubberBandPen(QPen(color));
}

void dtkPlotViewZoomer::updateBase(const QRectF &rectf)
{
    d->setZoomBase(rectf);
}
