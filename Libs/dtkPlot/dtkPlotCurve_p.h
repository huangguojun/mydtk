/* dtkPlotCurve_p.h ---
 *
 * Author: Julien Wintz
 * Created: Tue Oct  1 06:17:40 2013 (+0200)
 * Version:
 * Last-Updated: mer. avril  9 14:06:10 2014 (+0200)
 *           By: Nicolas Niclausse
 *     Update #: 42
 */

/* Change Log:
 *
 */

#pragma once

#include <dtkPlotSupportExport.h>

#include <QtCore>
#include <QPointF>

#include <qwt_plot_curve.h>
#include <qwt_series_data.h>

class dtkPlotCurvePrivateData : public QwtPointSeriesData
{
public:
    dtkPlotCurvePrivateData(void) {}
    ~dtkPlotCurvePrivateData(void) {}

public:
    void append(const QPointF& point);

public:
    void setData(const QVector<QPointF>& data);

public:
    QRectF boundingRect(void) const;

public:
    void clear(void);
};

class dtkPlotCurvePrivate : public QwtPlotCurve
{
public:
    dtkPlotCurvePrivate(const QString& title);
    virtual ~dtkPlotCurvePrivate(void);

public:
    dtkPlotCurvePrivateData *data;
};
