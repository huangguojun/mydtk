/* dtkPlotViewLegend.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Fri Jun  8 12:55:56 2012 (+0200)
 */

/* Commentary:
 *
 */

#include "dtkPlotViewLegend.h"
#include "dtkPlotViewLegend_p.h"
#include "dtkPlotView.h"

#include <qwt_plot_layout.h>
#include <qwt_plot_item.h>
#include <qwt_legend.h>

#if QWT_VERSION >= 0x060100
#include <qwt_legend_label.h>
#else
#include <qwt_legend_item.h>
#endif

// /////////////////////////////////////////////////////////////////
// dtkPlotViewLegendPrivate
// /////////////////////////////////////////////////////////////////

dtkPlotViewLegendPrivate::dtkPlotViewLegendPrivate(dtkPlotView *parent)
{
    legend = new QwtLegend;

#if QWT_VERSION >= 0x060100
    legend->setDefaultItemMode(QwtLegendData::Checkable);
#else
    legend->setItemMode(QwtLegend::CheckableItem);
#endif
    plot = reinterpret_cast<QwtPlot *>(parent->plotWidget());
    plot->insertLegend(legend, QwtPlot::RightLegend);

#if QWT_VERSION >= 0x060100
    connect(legend, SIGNAL(checked(const QVariant&, bool, int)), this, SLOT(legendChecked(const QVariant&, bool)));
#else
    connect(plot, SIGNAL(legendChecked(QwtPlotItem *, bool)), this, SLOT(showCurve(QwtPlotItem *, bool)));
#endif
}

dtkPlotViewLegendPrivate::~dtkPlotViewLegendPrivate(void)
{

}

void dtkPlotViewLegendPrivate::legendChecked(const QVariant& itemInfo, bool value)
{
#if QWT_VERSION >= 0x060100
    QwtPlotItem *item = plot->infoToItem(itemInfo);

    if (item)
        showCurve(item, value);

#endif
}

void dtkPlotViewLegendPrivate::showCurve(QwtPlotItem *item, bool value)
{
    item->setVisible(value);
    plotView->updateAxes();
}

// /////////////////////////////////////////////////////////////////
// dtkPlotViewLegend
// /////////////////////////////////////////////////////////////////

dtkPlotViewLegend::dtkPlotViewLegend(dtkPlotView *parent) : d(new dtkPlotViewLegendPrivate(parent))
{
    d->plotView = parent;

    QwtPlotItemList items = d->plot->itemList();

    for ( int i = 0; i < items.size(); i++ ) {
#if QWT_VERSION >= 0x060100
        QList<QWidget *> legendWidgets = d->legend->legendWidgets(d->plot->itemToInfo(items[i]));

        if (legendWidgets.size() == 1 ) {
            QwtLegendLabel *legendLabel = qobject_cast<QwtLegendLabel *>(legendWidgets[0]);

            if (legendLabel) {
                legendLabel->setChecked(true);
            }
        }

#else
        QwtLegendItem *legendItem = (QwtLegendItem *)d->legend->find(items[i]);

        if (legendItem) {
            if (items[i]->isVisible()) {
                legendItem->setChecked(true);
            }
        }

#endif
    }
}

dtkPlotViewLegend::~dtkPlotViewLegend(void)
{
    delete d;

    d = NULL;
}

void dtkPlotViewLegend::activate(void)
{
    d->legend->setVisible(true);
}

void dtkPlotViewLegend::deactivate(void)
{
    d->legend->setVisible(false);
}

void dtkPlotViewLegend::setPosition(int position)
{
    QwtPlot::LegendPosition pos;

    switch (position) {
    case 0:
        pos = QwtPlot::LeftLegend;
        break;

    case 1:
        pos = QwtPlot::RightLegend;
        break;

    case 2:
        pos = QwtPlot::BottomLegend;
        break;

    default:
        pos = QwtPlot::TopLegend;
        break;
    }

    d->plot->plotLayout()->setLegendPosition(pos);

    d->plotView->update();
}
