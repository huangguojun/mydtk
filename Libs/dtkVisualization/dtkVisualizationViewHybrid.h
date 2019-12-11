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

//#include <dtkWidgets/dtkWidgetsWidget>
#include <QWidget>

#include <dtkFonts/dtkFontAwesome>

class vtkRenderer;
class vtkRenderWindowInteractor;
class dtkVisualizationWidget;
class dtkVisualizationViewOverlay;
class dtkVisualizationWidgetsClutEditor;

class QSlider;

class DTKVISUALIZATION_EXPORT dtkVisualizationViewHybrid : public QWidget
{
    Q_OBJECT

public:
     dtkVisualizationViewHybrid(QWidget *parent = nullptr);
    ~dtkVisualizationViewHybrid(void);

public:
    QWidget *widget(void);

public:
    QWidget *bottomLeftWidget(void);
    void setBottomLeftWidget(fa::icon, QWidget *);

    QWidget *bottomRightWidget(void);
    void setBottomRightWidget(fa::icon, QWidget *);

    void setClutEditor(dtkVisualizationWidgetsClutEditor *);

public slots:
    void update(void);
    void render(void);

public slots:
    void importVTI(const QString& path);
    void importVTP(const QString& path);
    void importVTR(const QString& path);
    void importVTS(const QString& path);
    void importVTU(const QString& path);
    void importMHA(const QString& path);

public slots:
    void switchTo3D  (void);
    void switchTo2D  (void);
    void switchTo2DXY(void);
    void switchTo2DXZ(void);
    void switchTo2DYZ(void);

public slots:
    void switchCameraView(void);
    void setCameraViewNegativeX(void);
    void setCameraViewPositiveX(void);
    void setCameraViewNegativeY(void);
    void setCameraViewPositiveY(void);
    void setCameraViewNegativeZ(void);
    void setCameraViewPositiveZ(void);

public slots:
    void   link(dtkVisualizationViewHybrid *other);
    void unlink(dtkVisualizationViewHybrid *other);

signals:
    void   linking(void);
    void unlinking(void);

signals:
    void switchedTo3D  (void);
    void switchedTo2D  (void);
    void switchedTo2DXY(void);
    void switchedTo2DXZ(void);
    void switchedTo2DYZ(void);

public:
    vtkRenderer *renderer2D(void);
    vtkRenderer *renderer3D(void);
    vtkRenderWindowInteractor *interactor(void);

public slots:
    void sliceChange(int);

public slots:
    void onSliceChanged(int);

signals:
    void sliceOrientationChanged(int);
    void sliceChanged(int);

public:
    static QMap<double, QColor> grey_colormap;

protected:
    void dragEnterEvent(QDragEnterEvent *);
    void dragLeaveEvent(QDragLeaveEvent *);
    void dragMoveEvent(QDragMoveEvent *);
    void dropEvent(QDropEvent *);

protected:
    QSlider *sliceSlider(void);
    dtkVisualizationViewOverlay *renderer2DXY(void);
    dtkVisualizationViewOverlay *renderer2DXZ(void);
    dtkVisualizationViewOverlay *renderer2DYZ(void);
    dtkVisualizationViewOverlay *renderer2DButton(void);
    dtkVisualizationViewOverlay *renderer3DButton(void);

private:
    class dtkVisualizationViewHybridPrivate *d;
};

//
// dtkVisualizationViewHybrid.h ends here
