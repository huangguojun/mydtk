/* dtkPlotViewSettings.h ---
 *
 * Author: Selim Kraria
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Fri Jun  8 12:52:22 2012 (+0200)
 * Version: $Id: 639ea3f731c4e19b66cb7edf7ac0cf1d878f9a1a $
 * Last-Updated: mar. févr.  4 15:22:29 2014 (+0100)
 *           By: Thibaud Kloczko
 *     Update #: 89
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKPLOTVIEWSETTINGS_H
#define DTKPLOTVIEWSETTINGS_H

#include <dtkPlotSupportExport.h>

#include <dtkWidgets/dtkToolBox.h>

class dtkPlotView;
class dtkPlotViewSettingsPrivate;

class DTKPLOTSUPPORT_EXPORT dtkPlotViewSettings : public dtkToolBox
{
    Q_OBJECT

public:
    dtkPlotViewSettings(QWidget *parent);
    ~dtkPlotViewSettings(void);

public:
    QColor gridColor(void) const;
    QColor pickingColor(void) const;
    QColor zoomColor(void) const;

public:
    void setView(dtkPlotView *view);

    void update(void);

protected:
    void updateCurves(void);

protected slots:
    void onMainTitleChanged(void);
    void onAxisTitleXChanged(void);
    void onAxisTitleYChanged(void);
    void onTitleSizeChanged(const int& value);
    void onAxesTitleSizeChanged(const int& value);
    void onAxisScaleXChanged(const int& index);
    void onAxisScaleYChanged(const int& index);
    void onLegendPositionChanged(const int& index);
    void onGridColorChanged(const QColor& color);
    void onPickingColorChanged(const QColor& color);
    void onZoomColorChanged(const QColor& color);
    void onBackgroundColorChanged(const QColor& color);
    void onForegroundColorChanged(const QColor& color);
    void onRandomColorsClicked(void);
    void onColorAreaChanged(const int& value);

    void updateCurveName(int);
    void updateCurveColor(int);

private:
    dtkPlotViewSettingsPrivate *d;
};

#endif
