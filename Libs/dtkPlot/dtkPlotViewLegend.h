/* dtkPlotViewLegend.h ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Fri Jun  8 12:52:22 2012 (+0200)
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKPLOTVIEWLEGEND_H
#define DTKPLOTVIEWLEGEND_H

#include <dtkPlotSupportExport.h>

class dtkPlotView;
class dtkPlotViewLegendPrivate;

class DTKPLOTSUPPORT_EXPORT dtkPlotViewLegend
{
public:
    dtkPlotViewLegend(dtkPlotView *parent);
    ~dtkPlotViewLegend(void);

public:
    void   activate(void);
    void deactivate(void);

public:
    void setPosition(int position);

private:
    dtkPlotViewLegendPrivate *d;
};

#endif
