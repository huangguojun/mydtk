// Version: $Id: 80be09c7cc19c2fc912fa461752bfbc2a211d0d6 $
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

#include <dtkVisualizationWidgetsExport>

#include <QtCore/QObject>

#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QWidget>

// /////////////////////////////////////////////////////////////////
// dtkVisualizationWidgetsClutEditorVertex
// /////////////////////////////////////////////////////////////////

class DTKVISUALIZATIONWIDGETS_EXPORT dtkVisualizationWidgetsClutEditorVertex : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
     dtkVisualizationWidgetsClutEditorVertex(int x, int y, QColor color = Qt::yellow, QGraphicsItem *parent = nullptr);
     dtkVisualizationWidgetsClutEditorVertex(QPointF point, QColor color = Qt::yellow, QGraphicsItem *parent = nullptr);
    ~dtkVisualizationWidgetsClutEditorVertex(void);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);

    QRectF boundingRect(void) const;
    QPoint position(void) const;
    QColor color(void) const;

    void setColor(const QColor& color);

signals:
    void moved(const QPointF& position);

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:
    QColor fgColor;
    QColor bgColor;
};

// /////////////////////////////////////////////////////////////////
// dtkVisualizationWidgetsClutEditorScene
// /////////////////////////////////////////////////////////////////

class DTKVISUALIZATIONWIDGETS_EXPORT dtkVisualizationWidgetsClutEditorScene : public QGraphicsScene
{
    Q_OBJECT

public:
     dtkVisualizationWidgetsClutEditorScene(QObject *parent = nullptr);
    ~dtkVisualizationWidgetsClutEditorScene(void);

    void addItem(QGraphicsItem *item);

signals:
    void   moved(const QPointF& position);
    void removed(void);

protected:
    void keyPressEvent(QKeyEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:
    class dtkVisualizationWidgetsClutEditorScenePrivate *d;
};

// /////////////////////////////////////////////////////////////////
// dtkVisualizationWidgetsClutEditorView
// /////////////////////////////////////////////////////////////////

class DTKVISUALIZATIONWIDGETS_EXPORT dtkVisualizationWidgetsClutEditorView : public QGraphicsView
{
public:
     dtkVisualizationWidgetsClutEditorView(QWidget *parent = nullptr);
    ~dtkVisualizationWidgetsClutEditorView(void);

private:
    class dtkVisualizationWidgetsClutEditorViewPrivate *d;
};

// /////////////////////////////////////////////////////////////////
// dtkVisualizationWidgetsClutEditor
// /////////////////////////////////////////////////////////////////

class vtkColorTransferFunction;
class vtkPiecewiseFunction;

class DTKVISUALIZATIONWIDGETS_EXPORT dtkVisualizationWidgetsClutEditor : public QWidget
{
    Q_OBJECT

public:
    enum ColorSpace {
        HSV,
        RGB
    };

public:
    using Histogram = QList<int>;

public:
     dtkVisualizationWidgetsClutEditor(QWidget *parent = nullptr);
    ~dtkVisualizationWidgetsClutEditor(void);

    void   *colorTransferFunction(void);
    void *opacityTransferFunction(void);

    void setColorTransferFunction(vtkColorTransferFunction *color_transfer_function);
    void setOpacityTransferFunction(vtkPiecewiseFunction *opacity_transfer_function);

    void setRange(double min, double max);

    void setHistogram(const Histogram& histogram);

signals:
    void updated(void);

public slots:
    void importClut(void);
    void importClut(const QString&);
    void exportClut(void);

    void toggle(bool);

public:
    QSize sizeHint(void) const;

protected slots:
    void onApply(void);
    void onColorAuto(void);
    void onColorChoose(void);
    void onSelectionChanged(void);
    void onVertexMoved(const QPointF& point);

protected:
    void resizeEvent(QResizeEvent *event);

 private:
    void updateTable(void);

private:
    class dtkVisualizationWidgetsClutEditorPrivate *d;
};

//
// dtkVisualizationWidgetsClutEditor.h ends here
