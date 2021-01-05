/* dtkPieChart.h ---
 *
 * Author: Nicolas Niclausse
 * Copyright (C) 2012 - Nicolas Niclausse, Inria.
 * Created: mar. avril 24 15:40:38 2012 (+0200)
 * Version: $Id: fa1cf81c993f3aca9a6e63757d923f518a9f6560 $
 * Last-Updated: Fri May 18 15:47:40 2012 (+0200)
 *           By: Julien Wintz
 *     Update #: 47
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKPIECHART_H
#define DTKPIECHART_H

#include <dtkGuiSupportExport.h>

#include <QtWidgets/QWidget>

class dtkPieChartPrivate;

class DTKGUISUPPORT_EXPORT dtkPieChart : public QWidget
{
    Q_OBJECT

public:
    dtkPieChart(QWidget *parent = 0, int width = 0, int height = 0);
    ~dtkPieChart(void);

public:
    QSize sizeHint(void) const;

public:
    void addPiece(const QString &key, int val, QColor color);

public:
    void paintEvent(QPaintEvent *ev);

public:
    void clear(void);

private:
    dtkPieChartPrivate *d;
};

#endif
