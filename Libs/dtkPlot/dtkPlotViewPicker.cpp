/* dtkPlotViewPicker.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Sun Jun 10 01:13:40 2012 (+0200)
 * Version: $Id: 32a87b94666c77dea74a1ad2d2e1295665061948 $
 * Last-Updated: Mon Jun 24 14:53:55 2013 (+0200)
 *           By: Selim Kraria
 *     Update #: 19
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkPlotViewPicker.h"
#include "dtkPlotView.h"

#include <qwt_plot.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_picker.h>

class dtkPlotViewPickerPrivate : public QwtPlotPicker
{
public:
    dtkPlotViewPickerPrivate(QwtPlotCanvas *canvas);
    ~dtkPlotViewPickerPrivate(void);
};

dtkPlotViewPickerPrivate::dtkPlotViewPickerPrivate(QwtPlotCanvas *canvas) : QwtPlotPicker(canvas)
{
    this->setTrackerMode(QwtPicker::AlwaysOn);
}

dtkPlotViewPickerPrivate::~dtkPlotViewPickerPrivate(void) {}

// /////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////

dtkPlotViewPicker::dtkPlotViewPicker(dtkPlotView *parent) : QObject(parent)
{
    QwtPlot *plot = reinterpret_cast<QwtPlot *>(parent->plotWidget());
    d = new dtkPlotViewPickerPrivate(reinterpret_cast<QwtPlotCanvas *>(plot->canvas()));
}

dtkPlotViewPicker::~dtkPlotViewPicker(void)
{
    delete d;

    d = NULL;
}

void dtkPlotViewPicker::activate(void)
{
    d->setEnabled(true);
}

void dtkPlotViewPicker::deactivate(void)
{
    d->setEnabled(false);
}

QColor dtkPlotViewPicker::color(void) const
{
    return d->trackerPen().color();
}

void dtkPlotViewPicker::setColor(const QColor &color)
{
    d->setTrackerPen(QPen(color));
}
