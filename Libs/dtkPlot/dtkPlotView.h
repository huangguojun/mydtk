/* dtkPlotView.h ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Wed Jun  1 17:02:08 2011 (+0200)
 * Version: $Id: a18a9ccbfb67373a5e869967ff835ac3f1eddd95 $
 * Last-Updated: mer. avril  9 13:21:16 2014 (+0200)
 *           By: Nicolas Niclausse
 *     Update #: 149
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKPLOTVIEW_H
#define DTKPLOTVIEW_H

#include <dtkPlotSupportExport.h>

#include "dtkPlotViewLegend.h"

#include <dtkCoreSupport/dtkAbstractView.h>

#include <QtCore>
#include <QtWidgets>

class dtkPlotCurve;
class dtkPlotViewPrivate;

class DTKPLOTSUPPORT_EXPORT dtkPlotView : public dtkAbstractView
{
    Q_OBJECT

public:
    enum Scale {
        Linear,
        Logarithmic
    };

    enum LegendPosition {
        LeftLegend,
        RightLegend,
        BottomLegend,
        TopLegend
    };

public:
    dtkPlotView(void);
    ~dtkPlotView(void);

public:
    void clear(void);

public:
    QList<dtkPlotCurve *> curves(void);

public:
    void   activatePanning(void);
    void deactivatePanning(void);

    void   activatePicking(void);
    void deactivatePicking(void);

    void   activateZooming(void);
    void deactivateZooming(void);

    void   activateGrid(void);
    void deactivateGrid(void);

    void   activateLegend(void);
    void deactivateLegend(void);

public:
    void setTitle(const QString&);
    void setTitleSize(const int&);

    void setAxisTitleX(const QString& title);
    void setAxisTitleY(const QString& title);

    void setAxisTitleSizeX(const int&);
    void setAxisTitleSizeY(const int&);

    void setAxisScaleX(double min, double max);
    void setAxisScaleY(double min, double max);

    void setAxisScaleX(Scale scale);
    void setAxisScaleY(Scale scale);

    void setLegendPosition(LegendPosition position);

    void fillCurveArea(int alpha);

    int alphaCurveArea(void) const;

    void setStyleSheet(const QString& sheet);

public:
    QString title(void) const;
    int titleSize(void) const;

    Scale axisScaleX(void) const;
    Scale axisScaleY(void) const;

    QString axisTitleX(void) const;
    QString axisTitleY(void) const;

    int axisTitleSizeX(void) const;
    int axisTitleSizeY(void) const;

    QColor gridColor(void) const;
    void setGridColor(const QColor& color);

    QColor pickingColor(void) const;
    void setPickingColor(const QColor& color);

    QColor zoomColor(void) const;
    void setZoomColor(const QColor& color);
    bool isZoomActivated(void) const;

    void updateAxes(void);

public:
    void setBackgroundColor(int red, int green, int blue);
    void setBackgroundColor(double red, double green, double blue);
    void setBackgroundColor(const QColor& color);
    void setForegroundColor(const QColor& color);
    void setDark(bool dark);

public:
    QColor backgroundColor(void) const;
    QColor foregroundColor(void) const;

public:
    void setRandomCurvesColor(int seed = 0);
    void updateColors(void);
    void updateCurveColor(dtkPlotCurve *curve);

public:
    QWidget *plotWidget(void);
    QWidget *widget(void);

public:
    dtkPlotView& operator<<(dtkPlotCurve *curve);

protected:
    void readSettings(void);
    void writeSettings(void);

public slots:
    void update(void);

signals:
    void updated(void);

private:
    friend class dtkPlotCurve;
    friend class dtkPlotRenderer;

private:
    dtkPlotViewPrivate *d;
};

// /////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////

DTKPLOTSUPPORT_EXPORT dtkAbstractView *createPlotView(void);

#endif
