/* dtkPlotRenderer.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Mon Jul  4 21:14:19 2011 (+0200)
 * Version: $Id: ddd6c01ab1941437d10648cef0cbedef264ba8f4 $
 * Last-Updated: Mon Jul  4 21:43:06 2011 (+0200)
 *           By: Julien Wintz
 *     Update #: 22
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkPlotView.h"
#include "dtkPlotRenderer.h"

#include <qwt_plot_renderer.h>


class dtkPlotRendererPrivate : public QwtPlotRenderer
{
public:
    dtkPlotView *view;

public:
    QString path;
    QSize size;
};

dtkPlotRenderer::dtkPlotRenderer(void) : d(new dtkPlotRendererPrivate)
{
    d->view = NULL;
}

dtkPlotRenderer::~dtkPlotRenderer(void)
{
    delete d;

    d = NULL;
}

void dtkPlotRenderer::render(void)
{
    if (d->view)
        d->renderDocument((QwtPlot *)(d->view->d), d->path, d->size);
}

void dtkPlotRenderer::setView(dtkPlotView *view)
{
    d->view = view;
}

void dtkPlotRenderer::setSize(const QSize& size)
{
    d->size = size;
}

void dtkPlotRenderer::setPath(const QString& path)
{
    d->path = path;
}
