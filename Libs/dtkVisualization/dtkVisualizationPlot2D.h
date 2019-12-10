// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#pragma once

#include <dtkVisualizationExport>

#include "dtkVisualizationCanvas.h"

#include <vtkChartXY.h>
#include <vtkPlotPoints.h>

template <typename T> class vtkSmartPointer;
class vtkAbstractArray;

class DTKVISUALIZATION_EXPORT dtkVisualizationPlot2D : public dtkVisualizationCanvas
{
    Q_OBJECT

public:
     dtkVisualizationPlot2D(QWidget *parent = nullptr);
    ~dtkVisualizationPlot2D(void);

public:
    void setGridVisible(bool visible);
    QStringList fields(void);

public:
    void addFieldX(vtkSmartPointer<vtkAbstractArray>);
    void addFieldY(vtkSmartPointer<vtkAbstractArray>,
                   bool checked = false,
                   int type = vtkChart::POINTS,
                   int marker_style = vtkPlotPoints::PLUS,
                   QColor color = Qt::transparent,
                   int width = 3);

    void clearFields(void);
    void removeField(vtkSmartPointer<vtkAbstractArray>);
    void removeField(const QString&);

public:
    QStringList selectedFields(void);
    void setAddPlotSuffix(const QString& s);
    void setShowLegend(bool);

private slots:
    void exportAsCSV(const QString &);
    void exportAsPNG(const QString &);

protected:
    void render(void);
    //void keyPressEvent(QKeyEvent *e);

private:
    class dtkVisualizationPlot2DPrivate *d;
};

//
// dtkVisualizationPlot2D.h ends here
