/* dtkPlotViewLegend_p.h ---
 *
 * Author: Julien Wintz
 * Created: Tue Oct  1 06:17:40 2013 (+0200)
 */

/* Change Log:
 *
 */

#ifndef DTKPLOTVIEWLEGEND_P_H
#define DTKPLOTVIEWLEGEND_P_H

#include <dtkPlotSupportExport.h>

#include <QObject>

class dtkPlotView;

class QwtPlot;
class QwtPlotItem;
class QwtLegend;

class DTKPLOTSUPPORT_EXPORT dtkPlotViewLegendPrivate : public QObject
{
    Q_OBJECT

public:
    dtkPlotViewLegendPrivate(dtkPlotView *parent);
    ~dtkPlotViewLegendPrivate(void);

protected slots:
    void legendChecked(const QVariant& itemInfo, bool value);
    void showCurve(QwtPlotItem *item, bool value);

public:
    dtkPlotView *plotView;
    QwtPlot *plot;
    QwtLegend *legend;
};

#endif
