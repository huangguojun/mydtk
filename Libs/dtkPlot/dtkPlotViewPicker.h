/* dtkPlotViewPicker.h ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Sun Jun 10 01:13:02 2012 (+0200)
 * Version: $Id: 537cc62d387096545f0d0f9c31f8b7e7fa61cc91 $
 * Last-Updated: mer. avril  9 13:21:24 2014 (+0200)
 *           By: Nicolas Niclausse
 *     Update #: 7
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKPLOTVIEWPICKER_H
#define DTKPLOTVIEWPICKER_H

#include <dtkPlotExport.h>

#include <QtCore/QObject>
#include <QColor>

class dtkPlotView;
class dtkPlotViewPickerPrivate;

class DTKPLOT_EXPORT dtkPlotViewPicker : public QObject
{
    Q_OBJECT

public:
    dtkPlotViewPicker(dtkPlotView *parent);
    ~dtkPlotViewPicker(void);

public:
    void   activate(void);
    void deactivate(void);

public:
    QColor color(void) const;

    void setColor(const QColor& color);

private:
    dtkPlotViewPickerPrivate *d;
};

#endif
