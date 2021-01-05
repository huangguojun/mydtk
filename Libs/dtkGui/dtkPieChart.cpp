/* dtkPieChart.cpp ---
 *
 * Author: Nicolas Niclausse
 * Copyright (C) 2012 - Nicolas Niclausse, Inria.
 * Created: mar. avril 24 15:42:10 2012 (+0200)
 * Version: $Id: 10d2771e5a4d07aede4c1366352da3c76b67fbee $
 * Last-Updated: mer. avril  2 08:47:46 2014 (+0200)
 *           By: Nicolas Niclausse
 *     Update #: 81
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkPieChart.h"

#include <QtWidgets>

#include <dtkCoreSupport/dtkGlobal.h>

class dtkPieChartPrivate
{
public:
    QHash<QString, int> values;
    QHash<QString, QColor> colors;
};

dtkPieChart::dtkPieChart(QWidget *parent, int width, int height)
    : QWidget(parent), d(new dtkPieChartPrivate)
{
    this->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    this->setMinimumSize(width, height);

    if (width && !height)
        this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    else if (!width && height)
        this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    else
        this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}

dtkPieChart::~dtkPieChart(void)
{
    delete d;
}

QSize dtkPieChart::sizeHint(void) const
{
    QSize size = QSize(320, 80);

    return (size);
}

void dtkPieChart::addPiece(const QString &key, int val, QColor color)
{
    if (val < 1)
        return;

    d->values.insert(key, val);
    d->colors.insert(key, color);
}

void dtkPieChart::clear(void)
{
    d->values.clear();
    d->colors.clear();
}

void dtkPieChart::paintEvent(QPaintEvent *ev)
{

    QHash<QString, int>::const_iterator it;
    int total = 0;

    for (it = d->values.begin(); it != d->values.end(); ++it)
        total += it.value();

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    int height = rect().height();
    QRect pieRect(0, 0, height, height);

    QRect legendRect = rect();
    legendRect.setLeft(pieRect.width());
    legendRect.adjust(10, 10, -10, -10);
    int lastAngleOffset = 0;
    int currentPos = 0;

    for (it = d->values.begin(); it != d->values.end(); ++it) {
        int value = it.value();
        QString text = it.key();
        int angle = (int)(16 * 360 * (value / (double)total));

        QRadialGradient rg(pieRect.center(), pieRect.width() / 2, pieRect.topLeft());
        rg.setColorAt(0, Qt::white);
        rg.setColorAt(1, d->colors[text]);
        p.setBrush(rg);
        QPen pen = p.pen();
        p.setPen(Qt::NoPen);

        p.drawPie(pieRect, lastAngleOffset, angle);
        lastAngleOffset += angle;

        int fh = fontMetrics().height();
        QRect legendEntryRect(0, (fh * 1.5) * currentPos, fh, fh);
        currentPos++;
        legendEntryRect.translate(legendRect.topLeft());

        QLinearGradient lg(legendEntryRect.topLeft(), legendEntryRect.bottomRight());
        lg.setColorAt(0, d->colors[text]);
        lg.setColorAt(1, Qt::white);
        p.setBrush(QBrush(lg));
        p.drawRect(legendEntryRect);

        QPoint textStart = legendEntryRect.topRight();
        textStart = textStart + QPoint(fontMetrics().width('x'), 0);

        QPoint textEnd(legendRect.right(), legendEntryRect.bottom());
        QRect textEntryRect(textStart, textEnd);
        p.setPen(pen);
        p.drawText(textEntryRect, Qt::AlignVCenter, text);
    }
}
