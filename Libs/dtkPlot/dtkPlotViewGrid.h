/* dtkPlotViewGrid.h ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Fri Jun  8 12:52:22 2012 (+0200)
 * Version: $Id: 9a9d75826d9f3bb1c5fa82c9f7792919b17268cd $
 * Last-Updated: mer. avril  9 13:21:05 2014 (+0200)
 *           By: Nicolas Niclausse
 *     Update #: 43
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKPLOTVIEWGRID_H
#define DTKPLOTVIEWGRID_H

#include <dtkPlotExport.h>

#include <QtCore/QObject>

#include <QColor>

class dtkPlotView;
class dtkPlotViewGridPrivate;

class DTKPLOT_EXPORT dtkPlotViewGrid : public QObject
{
    Q_OBJECT

public:
    dtkPlotViewGrid(dtkPlotView *parent);
    ~dtkPlotViewGrid(void);

public:
    QColor color(void) const;

    void setColor(const QColor& color);

private:
    dtkPlotViewGridPrivate *d;
};

#endif
