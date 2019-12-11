/* dtkPlotCurve.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Tue Jun  7 16:09:17 2011 (+0200)
 * Version: $Id: 0d47e96ca01e7ab1b9af29502c40c7d541228ce6 $
 * Last-Updated: Tue Oct  1 09:14:19 2013 (+0200)
 *           By: Julien Wintz
 *     Update #: 239
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkPlotCurve.h"
#include "dtkPlotCurve_p.h"

// /////////////////////////////////////////////////////////////////
// dtkPlotCurvePrivateData
// /////////////////////////////////////////////////////////////////

void dtkPlotCurvePrivateData::append(const QPointF& data)
{
    this->d_samples << data;
}

QRectF dtkPlotCurvePrivateData::boundingRect(void) const
{
    QPolygonF poly(this->samples());

    return poly.boundingRect();
}

void dtkPlotCurvePrivateData::setData(const QVector<QPointF>& data)
{
    foreach (const QPointF& point, data) {
        this->append(point);
    }
}

void dtkPlotCurvePrivateData::clear(void)
{
    this->d_samples.clear();
}

// /////////////////////////////////////////////////////////////////
// dtkPlotCurvePrivate
// /////////////////////////////////////////////////////////////////

dtkPlotCurvePrivate::dtkPlotCurvePrivate(const QString& title) : QwtPlotCurve(title)
{
    this->data = new dtkPlotCurvePrivateData;
    this->setData(this->data);
}

dtkPlotCurvePrivate::~dtkPlotCurvePrivate(void)
{
    delete this->data;
}

// /////////////////////////////////////////////////////////////////
// dtkPlotCurve
// /////////////////////////////////////////////////////////////////

dtkPlotCurve::dtkPlotCurve(const QString& title) : QObject(), d(new dtkPlotCurvePrivate(title))
{
    d->setLegendAttribute(QwtPlotCurve::LegendShowLine);
    this->setName("curve");
    this->setAntialiased(true);
    this->setColor(Qt::black);
}

dtkPlotCurve::dtkPlotCurve(const dtkPlotCurve& other) : QObject(), d(new dtkPlotCurvePrivate(other.d->title().text()))
{
    d->data->setSamples(other.d->data->samples());
}

dtkPlotCurve::~dtkPlotCurve(void)
{
    delete d;

    d = NULL;
}

dtkPlotCurve& dtkPlotCurve::operator=(const dtkPlotCurve& other)
{
    d->setTitle(other.d->title().text());

    d->data->setSamples(other.d->data->samples());

    return *this;
}

void dtkPlotCurve::clear(void)
{
    d->data->clear();
}

void dtkPlotCurve::append(const QPointF& data)
{
    d->data->append(data);
    d->itemChanged();
}

void dtkPlotCurve::setName(const QString& name)
{
    d->setTitle(name);
}

void dtkPlotCurve::setAntialiased(bool antiliased)
{
    d->setRenderHint(QwtPlotItem::RenderAntialiased, antiliased);
}

void dtkPlotCurve::setColor(const QColor& color)
{
    d->setPen(color);
}

void dtkPlotCurve::setColorArea(const QColor& color)
{
    d->setBrush(color);
}

QString dtkPlotCurve::name(void) const
{
    return d->title().text();
}

QColor dtkPlotCurve::color(void) const
{
    return d->pen().color();
}

QRectF dtkPlotCurve::boundingRect(void) const
{
    return d->boundingRect();
}

void dtkPlotCurve::setData(const QVector<QPointF>& data)
{
    d->data->setData(data);

    // emit updated();
}

const QVector<QPointF> dtkPlotCurve::data(void) const
{
    return d->data->samples();
}

QString  dtkPlotCurve::description(void)
{
    QString string;

    string = "[ " ;

    for (int i = 0; i < d->data->samples().count(); i++) {
        if (i > 0)
            string.append("; ");

        QString stringx = QString("%1").arg(d->data->samples().at(i).x());
        QString stringy = QString("%1").arg(d->data->samples().at(i).y());

        string += "(" + stringx + ", " + stringy + ")";
    }

    string.append(" ]");

    return string;
}
