/* dtkPlotViewToolBar.cpp ---
 *
 * Author: Selim Kraria
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Fri Jun  8 12:55:56 2012 (+0200)
 * Version: $Id: 8de93738c58c9fd07ddef14c10ee101ccba3a61f $
 * Last-Updated: Thu Sep 19 15:50:13 2013 (+0200)
 *           By: Julien Wintz
 *     Update #: 423
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkPlotViewToolBar.h"

#include "dtkPlotView.h"
#include "dtkPlotRenderer.h"

#include <dtkWidgets/dtkSpacer.h>

class dtkPlotViewToolBarPrivate
{
public:
    QAction *defaultViewAction;
    QAction *pickingAction;
    QAction *zoomAction;
    QAction *gridAction;
    QAction *legendAction;
    QAction *exportAction;

    dtkPlotView *view;
};

dtkPlotViewToolBar::dtkPlotViewToolBar(dtkPlotView *parent) : QFrame(parent->widget()), d(new dtkPlotViewToolBarPrivate())
{
    QToolBar *bar = new QToolBar(this);

    d->view = parent;

    d->defaultViewAction = new QAction("Default", this);
    d->defaultViewAction->setToolTip("Show default view");
    d->defaultViewAction->setIcon(QPixmap(":dtkPlot/pixmaps/dtkPlotAxis.png"));

    d->gridAction = new QAction("Grid", this);
    d->gridAction->setCheckable(true);
    d->gridAction->setChecked(false);
    d->gridAction->setToolTip("Draw grid coodinates");
    d->gridAction->setIcon(QPixmap(":dtkPlot/pixmaps/dtkPlotGrid.png"));

    d->pickingAction = new QAction("Picking", this);
    d->pickingAction->setCheckable(true);
    d->pickingAction->setChecked(false);
    d->pickingAction->setToolTip("Activate picking");
    d->pickingAction->setIcon(QPixmap(":dtkPlot/pixmaps/dtkPlotPicking.png"));

    d->zoomAction = new QAction("Zoom", this);
    d->zoomAction->setCheckable(true);
    d->zoomAction->setChecked(false);
    d->zoomAction->setToolTip("Activate zoom");
    d->zoomAction->setIcon(QPixmap(":dtkPlot/pixmaps/dtkPlotZoom.png"));

    d->legendAction = new QAction("Legend", this);
    d->legendAction->setCheckable(true);
    d->legendAction->setChecked(false);
    d->legendAction->setToolTip("Activate legend");
    d->legendAction->setIcon(QPixmap(":dtkPlot/pixmaps/dtkPlotLegend.png"));

    d->exportAction = new QAction("Export", this);
    d->exportAction->setToolTip("Export view");
    d->exportAction->setIcon(QPixmap(":dtkPlot/pixmaps/dtkPlotSupportExport.png"));

    bar->addAction(d->defaultViewAction);
    bar->addAction(d->zoomAction);
    bar->addAction(d->gridAction);
    bar->addAction(d->pickingAction);
    bar->addAction(d->legendAction);
    bar->addWidget(new dtkSpacer);
    bar->addAction(d->exportAction);
    bar->setFloatable(false);
    bar->setIconSize(QSize(18, 18));
    bar->setMovable(false);
    bar->setOrientation(Qt::Horizontal);
    bar->setFixedHeight(25);
    bar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(bar);

    connect(d->defaultViewAction, SIGNAL(triggered(bool)), this, SLOT(onDefaultView()));
    connect(d->zoomAction, SIGNAL(triggered(bool)), this, SLOT(onZoomActivated(bool)));
    connect(d->gridAction, SIGNAL(triggered(bool)), this, SLOT(onGridActivated(bool)));
    connect(d->pickingAction, SIGNAL(triggered(bool)), this, SLOT(onPickingActivated(bool)));
    connect(d->legendAction, SIGNAL(triggered(bool)), this, SLOT(onLegendActivated(bool)));
    connect(d->exportAction, SIGNAL(triggered()), this, SLOT(onExport()));
}

dtkPlotViewToolBar::~dtkPlotViewToolBar(void)
{
    delete d;

    d = NULL;
}

void dtkPlotViewToolBar::onDefaultView(void)
{
    d->view->updateAxes();
}

void dtkPlotViewToolBar::onPickingActivated(bool value)
{
    if (value) {
        d->view->activatePicking();
    } else {
        d->view->deactivatePicking();
    }
}

void dtkPlotViewToolBar::onZoomActivated(bool value)
{
    if (value) {
        d->view->activateZooming();
    } else {
        d->view->deactivateZooming();
    }
}

void dtkPlotViewToolBar::onGridActivated(bool value)
{
    if (value) {
        d->view->activateGrid();
    } else {
        d->view->deactivateGrid();
    }
}

void dtkPlotViewToolBar::onLegendActivated(bool value)
{
    if (value) {
        d->view->activateLegend();
    } else {
        d->view->deactivateLegend();
    }
}

void dtkPlotViewToolBar::onExport(void)
{
    const QList<QByteArray> imageFormats = QImageWriter::supportedImageFormats();

    QStringList filters;
    filters += "PDF Documents (*.pdf)";
#ifndef QWT_NO_SVG
    filters += "SVG Documents (*.svg)";
#endif
    filters += "Postscript Documents (*.ps)";

    for (int i = 0; i < imageFormats.size(); i++) {
        QString imageFilter;
        imageFilter += imageFormats[i].toUpper();
        imageFilter += " (*.";
        imageFilter += imageFormats[i];
        imageFilter += ")";
        filters += imageFilter;
    }

    QFileDialog *dialog = new QFileDialog(0, "Export plot", QDir::homePath(), filters.join(";;"));
    dialog->setStyleSheet("background-color: none ; color: none; background: none;");
    dialog->setAcceptMode(QFileDialog::AcceptSave);
    dialog->setConfirmOverwrite(true);
    dialog->setFileMode(QFileDialog::AnyFile);

    QRegExp filter_regex(QLatin1String("(?:^\\*\\.(?!.*\\()|\\(\\*\\.)(\\w+)"));

    if (dialog->exec() == QDialog::Accepted) {
        QString fileName = dialog->selectedFiles().first();
        QFileInfo info(fileName);

        if (info.suffix().isEmpty() && !dialog->selectedNameFilter().isEmpty()) {
            if (filter_regex.indexIn(dialog->selectedNameFilter()) != -1) {
                QString extension = filter_regex.cap(1);
                fileName += QLatin1String(".") + extension;
            }
        }

        this->onExport(fileName);
    }
}

void dtkPlotViewToolBar::setDark(bool dark)
{
    if (dark) {
        d->defaultViewAction->setIcon(QPixmap(":dtkPlot/pixmaps/dtkPlotAxis-light.png"));
        d->gridAction->setIcon(QPixmap(":dtkPlot/pixmaps/dtkPlotGrid-light.png"));
        d->pickingAction->setIcon(QPixmap(":dtkPlot/pixmaps/dtkPlotPicking-light.png"));
        d->zoomAction->setIcon(QPixmap(":dtkPlot/pixmaps/dtkPlotZoom-light.png"));
        d->legendAction->setIcon(QPixmap(":dtkPlot/pixmaps/dtkPlotLegend-light.png"));
        d->exportAction->setIcon(QPixmap(":dtkPlot/pixmaps/dtkPlotSupportExport-light.png"));
    } else {
        d->defaultViewAction->setIcon(QPixmap(":dtkPlot/pixmaps/dtkPlotAxis.png"));
        d->gridAction->setIcon(QPixmap(":dtkPlot/pixmaps/dtkPlotGrid.png"));
        d->pickingAction->setIcon(QPixmap(":dtkPlot/pixmaps/dtkPlotPicking.png"));
        d->zoomAction->setIcon(QPixmap(":dtkPlot/pixmaps/dtkPlotZoom.png"));
        d->legendAction->setIcon(QPixmap(":dtkPlot/pixmaps/dtkPlotLegend.png"));
        d->exportAction->setIcon(QPixmap(":dtkPlot/pixmaps/dtkPlotSupportExport.png"));
    }
}

void dtkPlotViewToolBar::onExport(const QString& file)
{
    dtkPlotRenderer renderer;
    renderer.setView(d->view);
    renderer.setPath(file);
    renderer.setSize(QSize(300, 200));
    renderer.render();
}
