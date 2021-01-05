/* dtkPlotViewPanner.h ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Fri Jun  8 12:52:22 2012 (+0200)
 * Version: $Id: ae890a72675e31d206d3b9ccbca835a37f5252d9 $
 * Last-Updated: mer. avril  9 13:25:52 2014 (+0200)
 *           By: Nicolas Niclausse
 *     Update #: 22
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKPLOTVIEWPANNER_H
#define DTKPLOTVIEWPANNER_H

#include <dtkPlotExport.h>

#include <QObject>

class dtkPlotView;
class dtkPlotViewPannerPrivate;

class DTKPLOT_EXPORT dtkPlotViewPanner : public QObject
{
    Q_OBJECT

public:
    dtkPlotViewPanner(dtkPlotView *parent);
    ~dtkPlotViewPanner(void);

public:
    void activate(void);
    void deactivate(void);

private:
    dtkPlotViewPannerPrivate *d;
};

#endif
