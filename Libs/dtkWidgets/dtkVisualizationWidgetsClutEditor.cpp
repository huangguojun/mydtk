// Version: $Id: b3634632a3e3eac8bcc4a3b15a61a881bb42aa64 $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "dtkVisualizationWidgetsClutEditor.h"

#include <dtkFonts/dtkFontAwesome>

#include <QtWidgets>
#include <QtXml>

#include <vtkColorTransferFunction.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkLookupTable.h>
#include <vtkMapper.h>
#include <vtkPiecewiseFunction.h>
#include <vtkScalarsToColors.h>
#include <vtkVolume.h>
#include <vtkVolumeProperty.h>

// /////////////////////////////////////////////////////////////////
// dtkVisualizationWidgetsClutEditor - Helper functions
// /////////////////////////////////////////////////////////////////

QPointF dtkVisualizationWidgetsClutEditorMap(QPointF vertex, qreal min, qreal max, int width,
                                             int height, bool logScale);
QPointF dtkVisualizationWidgetsClutEditorMapInv(QPointF vertex, qreal min, qreal max, int width,
                                                int height, bool logScale);

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationWidgetsClutEditorInterpolator
// ///////////////////////////////////////////////////////////////////

class dtkVisualizationWidgetsClutEditorInterpolator
{
public:
    virtual ~dtkVisualizationWidgetsClutEditorInterpolator(void) = default;

public:
    void setStart(dtkVisualizationWidgetsClutEditorVertex *vertex);
    void setStop(dtkVisualizationWidgetsClutEditorVertex *vertex);

    virtual QColor interpolate(dtkVisualizationWidgetsClutEditorVertex *vertex) const = 0;

protected:
    dtkVisualizationWidgetsClutEditorVertex *start;
    dtkVisualizationWidgetsClutEditorVertex *stop;
};

void dtkVisualizationWidgetsClutEditorInterpolator::setStart(
        dtkVisualizationWidgetsClutEditorVertex *vertex)
{
    this->start = vertex;
}

void dtkVisualizationWidgetsClutEditorInterpolator::setStop(
        dtkVisualizationWidgetsClutEditorVertex *vertex)
{
    this->stop = vertex;
}

// /////////////////////////////////////////////////////////////////
// dtkVisualizationWidgetsClutEditorInterpolatorHSV
// /////////////////////////////////////////////////////////////////

class dtkVisualizationWidgetsClutEditorInterpolatorHSV
    : public dtkVisualizationWidgetsClutEditorInterpolator
{
public:
    QColor interpolate(dtkVisualizationWidgetsClutEditorVertex *vertex) const;

private:
    static qreal computeHue(double x, double x0, qreal h0, double x1, qreal h1);
};

qreal dtkVisualizationWidgetsClutEditorInterpolatorHSV::computeHue(double x, double x0, qreal h0,
                                                                   double x1, qreal h1)
{
    if (x1 == x0)
        return (h0 + h1) / 2.0;

    double alpha = (x - x0) / (x1 - x0);

    return h1 * alpha + h0 * (1.0 - alpha);
}

QColor dtkVisualizationWidgetsClutEditorInterpolatorHSV::interpolate(
        dtkVisualizationWidgetsClutEditorVertex *vertex) const
{
    if (!this->start) {
        qWarning() << "No start value set for interpolation";
        return vertex->color();
    }

    if (!this->stop) {
        qWarning() << "No stop value set for interpolation";
        return vertex->color();
    }

    qreal x = vertex->scenePos().x();
    qreal xmin = start->scenePos().x();
    qreal xmax = stop->scenePos().x();

    qreal hmin = start->color().hueF();
    qreal hmax = stop->color().hueF();

    qreal h = this->computeHue(x, xmin, hmin, xmax, hmax);
    qreal s = vertex->color().saturationF();
    qreal v = vertex->color().valueF();

    return QColor::fromHsvF(h, s, v);
}

// /////////////////////////////////////////////////////////////////
// dtkVisualizationWidgetsClutEditorInterpolatorRGB
// /////////////////////////////////////////////////////////////////

class dtkVisualizationWidgetsClutEditorInterpolatorRGB
    : public dtkVisualizationWidgetsClutEditorInterpolator
{
public:
    QColor interpolate(dtkVisualizationWidgetsClutEditorVertex *vertex) const;
};

QColor dtkVisualizationWidgetsClutEditorInterpolatorRGB::interpolate(
        dtkVisualizationWidgetsClutEditorVertex *vertex) const
{
    if (!this->start) {
        qWarning() << Q_FUNC_INFO << "No start value set for interpolation";
        return vertex->color();
    }

    if (!this->stop) {
        qWarning() << Q_FUNC_INFO << "No stop value set for interpolation";
        return vertex->color();
    }

    QPropertyAnimation interpolator;
    interpolator.setEasingCurve(QEasingCurve::Linear);
    interpolator.setDuration(100);
    interpolator.setKeyValueAt(0.0, this->start->color());
    interpolator.setKeyValueAt(1.0, this->stop->color());
    interpolator.setCurrentTime(50);

    return interpolator.currentValue().value<QColor>();
}

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationWidgetsClutEditorHistogram
// ///////////////////////////////////////////////////////////////////

class dtkVisualizationWidgetsClutEditorHistogram : public QGraphicsItem
{
public:
    dtkVisualizationWidgetsClutEditorHistogram(QGraphicsItem *parent = Q_NULLPTR);
    ~dtkVisualizationWidgetsClutEditorHistogram();

public:
    void setup(const dtkVisualizationWidgetsClutEditor::Histogram &histogram);

public:
    QRectF boundingRect(void) const;

public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = Q_NULLPTR);

public:
    QList<double> normalizedHistogram;
};

dtkVisualizationWidgetsClutEditorHistogram::dtkVisualizationWidgetsClutEditorHistogram(
        QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
}

dtkVisualizationWidgetsClutEditorHistogram::~dtkVisualizationWidgetsClutEditorHistogram() {}

void dtkVisualizationWidgetsClutEditorHistogram::setup(
        const dtkVisualizationWidgetsClutEditor::Histogram &histogram)
{
    static const double epsilon = 1e-6;

    if (this->normalizedHistogram.count() != 0)
        this->normalizedHistogram.clear();

    double max = 0;

    for (int i = 0; i < histogram.count(); ++i) {
        if (histogram.at(i) < 0) {
            qWarning() << Q_FUNC_INFO << "bad value at:" << i;
            this->normalizedHistogram.append(0.0);
        } else {
            max = qMax(max, (double)histogram.at(i));
            this->normalizedHistogram.append((double)histogram.at(i));
        }
    }

    if (max < epsilon) {
        qWarning() << Q_FUNC_INFO << "histogram is null";
        return;
    }

    for (int i = 0; i < this->normalizedHistogram.count(); ++i) {
        this->normalizedHistogram.replace(i, this->normalizedHistogram.at(i) / max);
    }
}

QRectF dtkVisualizationWidgetsClutEditorHistogram::boundingRect(void) const
{
    return this->parentItem()->boundingRect();
}

void dtkVisualizationWidgetsClutEditorHistogram::paint(QPainter *painter,
                                                       const QStyleOptionGraphicsItem *option,
                                                       QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (this->normalizedHistogram.isEmpty())
        return;

    if (this->boundingRect().isEmpty())
        return;

    painter->setPen(Qt::darkGray);
    painter->setBrush(Qt::lightGray);

    qreal p_w = this->boundingRect().width();
    qreal p_h = this->boundingRect().height();
    qreal b_w = p_w / this->normalizedHistogram.count();

    for (int i = 0; i < this->normalizedHistogram.count(); ++i) {
        qreal v = this->normalizedHistogram.at(i) * 0.95;
        qreal x = i * b_w;
        qreal y = (1.0 - v) * p_h;
        qreal w = b_w;
        qreal h = v * p_h;

        painter->drawRect(x, y, w, h);
    }
}

// /////////////////////////////////////////////////////////////////
// dtkVisualizationWidgetsClutEditorVertex
// /////////////////////////////////////////////////////////////////

dtkVisualizationWidgetsClutEditorVertex::dtkVisualizationWidgetsClutEditorVertex(
        int x, int y, QColor color, QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
    this->fgColor = color;
    this->bgColor = QColor(0xc0, 0xc0, 0xc0);

    this->setPos(x, -y);
    this->setZValue(1);

    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
}

dtkVisualizationWidgetsClutEditorVertex::dtkVisualizationWidgetsClutEditorVertex(
        QPointF point, QColor color, QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
    this->fgColor = color;
    this->bgColor = QColor(0xc0, 0xc0, 0xc0);

    this->setPos(point.x(), -point.y());
    this->setZValue(2);

    this->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}

dtkVisualizationWidgetsClutEditorVertex::~dtkVisualizationWidgetsClutEditorVertex(void) {}

void dtkVisualizationWidgetsClutEditorVertex::paint(QPainter *painter,
                                                    const QStyleOptionGraphicsItem *option,
                                                    QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (this->isSelected())
        painter->setPen(Qt::magenta);
    else
        painter->setPen(Qt::black);

    painter->setBrush(bgColor);
    painter->drawEllipse(-10, -10, 20, 20);
    painter->setBrush(fgColor);
    painter->drawEllipse(-5, -5, 10, 10);
}

QRectF dtkVisualizationWidgetsClutEditorVertex::boundingRect(void) const
{
    return QRectF(-10, -10, 20, 20);
}

QPoint dtkVisualizationWidgetsClutEditorVertex::position(void) const
{
    return QPoint(this->x(), this->y());
}

QColor dtkVisualizationWidgetsClutEditorVertex::color(void) const
{
    return fgColor;
}

void dtkVisualizationWidgetsClutEditorVertex::setColor(const QColor &color)
{
    fgColor = color;

    this->update();
}

void dtkVisualizationWidgetsClutEditorVertex::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);

    emit moved(event->scenePos());
}

// /////////////////////////////////////////////////////////////////
// dtkVisualizationWidgetsClutEditorTable - helper functions
// /////////////////////////////////////////////////////////////////

static bool
dtkVisualizationWidgetsClutEditorVertexLessThan(const dtkVisualizationWidgetsClutEditorVertex *v1,
                                                const dtkVisualizationWidgetsClutEditorVertex *v2)
{
    return (v1->position().x() < v2->position().x());
}

// /////////////////////////////////////////////////////////////////
// dtkVisualizationWidgetsClutEditorTable
// /////////////////////////////////////////////////////////////////

class dtkVisualizationWidgetsClutEditorTable : public QGraphicsItem
{
public:
    dtkVisualizationWidgetsClutEditorTable(QGraphicsItem *parent = 0);
    ~dtkVisualizationWidgetsClutEditorTable(void);

    void addVertex(dtkVisualizationWidgetsClutEditorVertex *vertex);
    void removeVertex(dtkVisualizationWidgetsClutEditorVertex *vertex);

    void sort(void);

public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

public:
    QRectF boundingRect(void) const;

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

public:
    QList<dtkVisualizationWidgetsClutEditorVertex *> vertices;
};

dtkVisualizationWidgetsClutEditorTable::dtkVisualizationWidgetsClutEditorTable(
        QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setZValue(1);
}

dtkVisualizationWidgetsClutEditorTable::~dtkVisualizationWidgetsClutEditorTable(void) {}

void dtkVisualizationWidgetsClutEditorTable::addVertex(
        dtkVisualizationWidgetsClutEditorVertex *vertex)
{
    vertices << vertex;

    vertex->setParentItem(this);

    this->sort();
}

void dtkVisualizationWidgetsClutEditorTable::removeVertex(
        dtkVisualizationWidgetsClutEditorVertex *vertex)
{
    vertices.removeOne(vertex);

    this->sort();
}

void dtkVisualizationWidgetsClutEditorTable::sort(void)
{
    qSort(vertices.begin(), vertices.end(), dtkVisualizationWidgetsClutEditorVertexLessThan);
}

void dtkVisualizationWidgetsClutEditorTable::paint(QPainter *painter,
                                                   const QStyleOptionGraphicsItem *option,
                                                   QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    int n_points = vertices.size() + 4;

    QPoint *points = new QPoint[n_points];
    points[0] = this->mapFromScene(QPoint(0, 0)).toPoint();
    points[1] = this->mapFromScene(QPoint(0, vertices.first()->position().y())).toPoint();
    for (int i = 0; i < vertices.count(); i++)
        points[i + 2] = vertices.at(i)->position();
    points[vertices.count() + 2] = this->mapFromScene(QPoint(this->scene()->sceneRect().width(),
                                                             vertices.last()->position().y()))
                                           .toPoint();
    points[vertices.count() + 3] =
            this->mapFromScene(QPoint(this->scene()->sceneRect().width(), 0)).toPoint();

    QPen pen(Qt::black, 1);

    int xmin = vertices.first()->pos().x();
    int xmax = vertices.last()->pos().x();

    QLinearGradient linearGradient(xmin, 0, xmax, 0);
    {
        QColor color = vertices.first()->color();
        color.setAlpha(128);
        linearGradient.setColorAt(0.0, color);
    }

    for (dtkVisualizationWidgetsClutEditorVertex *vertex : vertices) {
        qreal position;
        position = (vertex->pos().x() - xmin) / (xmax - xmin);
        position = qMax(0.0, position);
        position = qMin(position, 1.0);

        QColor color = vertex->color();
        color.setAlpha(128);

        linearGradient.setColorAt(position, color);
    }

    {
        QColor color = vertices.last()->color();
        color.setAlpha(128);
        linearGradient.setColorAt(1.0, color);
    }

    painter->setPen(pen);
    painter->setBrush(linearGradient);
    painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter->drawPolygon(points, n_points);

    delete[] points;
}

QRectF dtkVisualizationWidgetsClutEditorTable::boundingRect(void) const
{
    int xmin, xmax, ymin, ymax;
    xmin = ymin = INT_MAX;
    xmax = ymax = INT_MIN;

    for (dtkVisualizationWidgetsClutEditorVertex *vertex : vertices) {
        if (vertex->x() < xmin)
            xmin = vertex->x();
        if (vertex->x() > xmax)
            xmax = vertex->x();
        if (vertex->y() < ymin)
            ymin = vertex->y();
        if (vertex->y() > ymax)
            ymax = vertex->y();
    }

    return QRectF(xmin, ymin, xmax - xmin, qAbs(ymin));
}

void dtkVisualizationWidgetsClutEditorTable::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QVector<QPointF> points;

    points << QPointF(vertices.first()->position().x(), 0);
    for (int i = 0; i < vertices.count(); i++)
        points << QPointF(vertices.at(i)->position());
    points << QPointF(vertices.last()->position().x(), 0);

    QPolygonF polygon = QPolygonF(points);

    if (!polygon.containsPoint(event->pos(), Qt::OddEvenFill)) {
        event->ignore();
        return;
    }

    QPointF delta = event->pos() - event->lastPos();

    if (mapRectToScene(this->boundingRect()).left() + delta.x() < 0) {
        event->ignore();
        return;
    }

    if (mapRectToScene(this->boundingRect()).right() + delta.x()
        > this->scene()->views().first()->width()) {
        event->ignore();
        return;
    }

    this->setPos(QPointF(this->pos().x() + delta.x(), this->pos().y()));

    event->accept();
}

void dtkVisualizationWidgetsClutEditorTable::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);

    this->scene()->clearSelection();
}

// /////////////////////////////////////////////////////////////////
// dtkVisualizationWidgetsClutEditorScene
// /////////////////////////////////////////////////////////////////

class dtkVisualizationWidgetsClutEditorScenePrivate
{
public:
    dtkVisualizationWidgetsClutEditorTable *table;
};

dtkVisualizationWidgetsClutEditorScene::dtkVisualizationWidgetsClutEditorScene(QObject *parent)
    : QGraphicsScene(parent), d(new dtkVisualizationWidgetsClutEditorScenePrivate)
{
}

dtkVisualizationWidgetsClutEditorScene::~dtkVisualizationWidgetsClutEditorScene(void)
{
    delete d;

    d = NULL;
}

void dtkVisualizationWidgetsClutEditorScene::addItem(QGraphicsItem *item)
{
    if (dtkVisualizationWidgetsClutEditorTable *table =
                dynamic_cast<dtkVisualizationWidgetsClutEditorTable *>(item))
        d->table = table;

    QGraphicsScene::addItem(item);
}

void dtkVisualizationWidgetsClutEditorScene::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Backspace || event->key() == Qt::Key_Delete) {
        for (QGraphicsItem *item : this->selectedItems()) {
            if (dtkVisualizationWidgetsClutEditorVertex *vertex =
                        dynamic_cast<dtkVisualizationWidgetsClutEditorVertex *>(item)) {
                d->table->removeVertex(vertex);
                d->table->update();
                this->removeItem(vertex);
                delete vertex;

                emit removed();
            }
        }
    }
}

void dtkVisualizationWidgetsClutEditorScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (!this->sceneRect().adjusted(0.0, 50.0, 0.0, 0.0).contains(event->scenePos()))
        return;

    if (QGraphicsItem *item = this->itemAt(event->scenePos(), QTransform()))
        if (dynamic_cast<dtkVisualizationWidgetsClutEditorVertex *>(item))
            return;

    if (d->table) {

        dtkVisualizationWidgetsClutEditorVertex *vertex =
                new dtkVisualizationWidgetsClutEditorVertex(d->table->mapFromScene(
                        QPointF(event->scenePos().x(), -event->scenePos().y())));
        d->table->addVertex(vertex);
        d->table->update();

        vertex->setSelected(true);

        connect(vertex, SIGNAL(moved(const QPointF &)), this, SIGNAL(moved(const QPointF &)));
    }
}

void dtkVisualizationWidgetsClutEditorScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF delta = event->scenePos() - event->lastScenePos();

    for (QGraphicsItem *item : this->selectedItems()) {

        if (dtkVisualizationWidgetsClutEditorVertex *vertex =
                    dynamic_cast<dtkVisualizationWidgetsClutEditorVertex *>(item)) {

            if ((vertex->scenePos().x() + delta.x() < 0) && (delta.x() <= 0)) {
                event->ignore();
                return;
            }

            if ((vertex->scenePos().x() + delta.x() > this->views().first()->width())
                && (delta.x() >= 0)) {
                event->ignore();
                return;
            }

            if ((vertex->scenePos().y() + delta.y() <= -100) && (delta.y() <= 0)) {
                event->ignore();
                return;
            }

            if ((vertex->scenePos().y() + delta.y() >= 0) && (delta.y() >= 0)) {
                event->ignore();
                return;
            }
        }
    }

    d->table->sort();
    d->table->update();

    QGraphicsScene::mouseMoveEvent(event);
}

// /////////////////////////////////////////////////////////////////
// dtkVisualizationWidgetsClutEditorView
// /////////////////////////////////////////////////////////////////

class dtkVisualizationWidgetsClutEditorViewPrivate
{
public:
};

dtkVisualizationWidgetsClutEditorView::dtkVisualizationWidgetsClutEditorView(QWidget *parent)
    : QGraphicsView(parent), d(new dtkVisualizationWidgetsClutEditorViewPrivate)
{
    this->setAttribute(Qt::WA_MacShowFocusRect, false);
    this->setDragMode(QGraphicsView::RubberBandDrag);
    this->setFrameShape(QFrame::NoFrame);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform
                         | QPainter::TextAntialiasing);
    this->setViewportUpdateMode(FullViewportUpdate);
}

dtkVisualizationWidgetsClutEditorView::~dtkVisualizationWidgetsClutEditorView(void)
{
    delete d;

    d = NULL;
}

// /////////////////////////////////////////////////////////////////
// dtkVisualizationWidgetsClutEditor
// /////////////////////////////////////////////////////////////////

class dtkVisualizationWidgetsClutEditorPrivate
{
public:
    dtkVisualizationWidgetsClutEditorScene *scene = nullptr;
    dtkVisualizationWidgetsClutEditorView *view = nullptr;

public:
    double min;
    double max;

public:
    dtkVisualizationWidgetsClutEditorTable *table = nullptr;

public:
    dtkVisualizationWidgetsClutEditorHistogram *histogram = nullptr;

public:
    QGraphicsRectItem *bg = nullptr;

public:
    vtkColorTransferFunction *colorTransferFunction = nullptr;
    vtkPiecewiseFunction *opacityTransferFunction = nullptr;

public:
    dtkVisualizationWidgetsClutEditor::ColorSpace color_space;

public:
    QCheckBox *button_log;
    QCheckBox *button_apply;

    QLineEdit *label_alpha;
    QLineEdit *label_value;

    QPushButton *button_auto;
    QPushButton *button_color;
    QPushButton *button_export;
    QPushButton *button_import;
    QComboBox *button_presets;
};

dtkVisualizationWidgetsClutEditor::dtkVisualizationWidgetsClutEditor(QWidget *parent)
    : QWidget(parent), d(new dtkVisualizationWidgetsClutEditorPrivate)
{
    d->min = 100.0;
    d->max = 200.0;

    d->color_space = dtkVisualizationWidgetsClutEditor::RGB;

    d->colorTransferFunction = Q_NULLPTR;
    d->opacityTransferFunction = Q_NULLPTR;

    d->histogram = Q_NULLPTR;

    d->bg = new QGraphicsRectItem;
    d->bg->setZValue(0);
    d->bg->setPen(QColor("#3b3b3b"));
    d->bg->setBrush(QColor(0, 0, 0, 128));

    d->table = new dtkVisualizationWidgetsClutEditorTable;

    d->scene = new dtkVisualizationWidgetsClutEditorScene(this);
    d->scene->addItem(d->bg);
    d->scene->addItem(d->table);

    d->view = new dtkVisualizationWidgetsClutEditorView(this);
    d->view->setScene(d->scene);

    // --

    QPalette palette;
    palette.setBrush(QPalette::Background, Qt::transparent);

    d->button_log = new QCheckBox;
    d->button_log->setIcon(dtkFontAwesome::instance()->icon(fa::areachart));
    d->button_log->setToolTip("Logarithmic scale");
    d->button_log->setPalette(palette);
    d->button_log->setAutoFillBackground(false);

    d->button_apply = new QCheckBox;
    d->button_apply->setIcon(dtkFontAwesome::instance()->icon(fa::check));
    d->button_apply->setToolTip("Applied");
    d->button_apply->setPalette(palette);
    d->button_apply->setAutoFillBackground(false);
    d->button_apply->setCheckState(Qt::Checked);

    d->label_value = new QLineEdit("v:");

    d->label_alpha = new QLineEdit("α:");

    d->button_auto = new QPushButton;
    d->button_auto->setIcon(dtkFontAwesome::instance()->icon(fa::linechart));
    d->button_auto->setToolTip("Interpolate");
    d->button_auto->setPalette(palette);
    d->button_auto->setAutoFillBackground(false);

    d->button_color = new QPushButton;
    d->button_color->setToolTip("Color");
    d->button_color->setIcon(dtkFontAwesome::instance()->icon(fa::code));
    d->button_color->setPalette(palette);
    d->button_color->setAutoFillBackground(false);

    d->button_export = new QPushButton;
    d->button_export->setToolTip("Export");
    d->button_export->setIcon(dtkFontAwesome::instance()->icon(fa::upload));
    d->button_export->setPalette(palette);
    d->button_export->setAutoFillBackground(false);

    d->button_import = new QPushButton;
    d->button_import->setToolTip("Import");
    d->button_import->setIcon(dtkFontAwesome::instance()->icon(fa::download));
    d->button_import->setPalette(palette);
    d->button_import->setAutoFillBackground(false);

    d->button_presets = new QComboBox;
    d->button_presets->setToolTip("Presets");
    d->button_presets->setPalette(palette);
    d->button_presets->setAutoFillBackground(false);

    d->button_presets->addItem("0RGB_red");
    d->button_presets->addItem("0RGB_green");
    d->button_presets->addItem("0RGB_blue");
    d->button_presets->addItem("0CMY_cyan");
    d->button_presets->addItem("0CMY_magenta");
    d->button_presets->addItem("0CMY_yellow");

    d->button_presets->addItem("1Flashy_blue");
    d->button_presets->addItem("1Flashy_green");
    d->button_presets->addItem("1Flashy_orange");
    d->button_presets->addItem("1Flashy_purple");
    d->button_presets->addItem("1Flashy_red");
    d->button_presets->addItem("1Flashy_turquoise");

    d->button_presets->addItem("Blues");
    d->button_presets->addItem("Greens");
    d->button_presets->addItem("Greys");
    d->button_presets->addItem("Oranges");
    d->button_presets->addItem("Purples");
    d->button_presets->addItem("Reds");

    d->button_presets->addItem("BrBG");
    d->button_presets->addItem("BuGn");
    d->button_presets->addItem("BuGrRd");
    d->button_presets->addItem("GnBu");
    d->button_presets->addItem("PiYG");
    d->button_presets->addItem("PuBu");
    d->button_presets->addItem("PuBuGn");
    d->button_presets->addItem("PuOr");
    d->button_presets->addItem("PuRd");
    d->button_presets->addItem("RdBu");
    d->button_presets->addItem("RdYlBu");
    d->button_presets->addItem("RdYlGn");
    d->button_presets->addItem("YlGn");
    d->button_presets->addItem("YlOrBr");

    d->button_presets->addItem("acidity");
    d->button_presets->addItem("atmosphere");
    d->button_presets->addItem("bone");
    d->button_presets->addItem("bronze");
    // d->button_presets->addItem("chestnut");
    // d->button_presets->addItem("cold");
    d->button_presets->addItem("curvature");
    d->button_presets->addItem("density");
    d->button_presets->addItem("geo_jet");
    d->button_presets->addItem("glasbey");
    d->button_presets->addItem("grey");
    d->button_presets->addItem("hot");
    d->button_presets->addItem("inferno");
    d->button_presets->addItem("jet");
    d->button_presets->addItem("leaf");
    // d->button_presets->addItem("morocco");
    d->button_presets->addItem("ocean");
    d->button_presets->addItem("topography");
    // d->button_presets->addItem("primordia_jet");
    // d->button_presets->addItem("quercus");
    d->button_presets->addItem("temperature");
    // d->button_presets->addItem("tourmaline");
    d->button_presets->addItem("viridis");
    // d->button_presets->addItem("wine");

    // --

    QHBoxLayout *t_layout = new QHBoxLayout;
    t_layout->setContentsMargins(0, 0, 0, 0);
    t_layout->addWidget(d->button_log);
    t_layout->addWidget(d->button_apply);
    t_layout->addWidget(d->label_value);
    t_layout->addWidget(d->label_alpha);

    QHBoxLayout *b_layout = new QHBoxLayout;
    b_layout->setContentsMargins(0, 0, 0, 0);
    b_layout->addWidget(d->button_auto);
    b_layout->addWidget(d->button_color);
    b_layout->addWidget(d->button_export);
    b_layout->addWidget(d->button_import);
    b_layout->addWidget(d->button_presets);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addLayout(t_layout);
    layout->addWidget(d->view);
    layout->addLayout(b_layout);

    this->setFixedHeight(100);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // --

    connect(d->scene, SIGNAL(moved(const QPointF &)), this, SLOT(onVertexMoved(const QPointF &)));
    connect(d->scene, SIGNAL(removed()), this, SLOT(onApply()));
    connect(d->scene, SIGNAL(selectionChanged()), this, SLOT(onSelectionChanged()));

    connect(d->button_auto, SIGNAL(clicked()), this, SLOT(onColorAuto()));
    connect(d->button_color, SIGNAL(clicked()), this, SLOT(onColorChoose()));
    connect(d->button_import, SIGNAL(clicked()), this, SLOT(importClut()));
    connect(d->button_export, SIGNAL(clicked()), this, SLOT(exportClut()));
    // connect(d->button_apply, SIGNAL(clicked()), this, SLOT(onApply()));
    connect(d->button_apply, SIGNAL(toggled(bool)), this, SLOT(onApply()));
    connect(d->button_log, SIGNAL(toggled(bool)), this, SLOT(onApply()));
    connect(d->button_presets, SIGNAL(currentTextChanged(const QString &)), this,
            SLOT(importClut(const QString &)));
}

dtkVisualizationWidgetsClutEditor::~dtkVisualizationWidgetsClutEditor(void)
{
    delete d;

    d = NULL;
}

void dtkVisualizationWidgetsClutEditor::importClut(const QString &clut)
{
    QString fileName = QString(":dtk-visualization/cluts/%1.clut").arg(clut);
    QDomDocument doc("clut");

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly))
        return;

    if (!doc.setContent(&file)) {
        file.close();
        return;
    }

    file.close();

    for (dtkVisualizationWidgetsClutEditorVertex *vertex : d->table->vertices) {
        d->table->removeVertex(vertex);
        d->scene->removeItem(vertex);
        delete vertex;
    }

    QDomElement root = doc.documentElement();
    double min = root.attribute("min").toDouble();
    double max = root.attribute("max").toDouble();
    int log = root.attribute("log").toInt();

    QDomNode n = root.firstChild();
    while (!n.isNull()) {
        QDomElement e = n.toElement();
        if (!e.isNull()) {

            qreal v =
                    d->min + (e.attribute("v").toDouble() - min) / (max - min) * (d->max - d->min);
            qreal a = e.attribute("a").toDouble();
            int r = e.attribute("r").toInt();
            int g = e.attribute("g").toInt();
            int b = e.attribute("b").toInt();
            dtkVisualizationWidgetsClutEditorVertex *vertex =
                    new dtkVisualizationWidgetsClutEditorVertex(
                            dtkVisualizationWidgetsClutEditorMap(QPointF(v, a), d->min, d->max,
                                                                 this->width(), 100, log),
                            QColor(r, g, b));

            d->table->addVertex(vertex);

            connect(vertex, SIGNAL(moved(const QPointF &)), d->scene,
                    SIGNAL(moved(const QPointF &)));
        }
        n = n.nextSibling();
    }

    d->table->update();

    d->button_log->setChecked(log);

    if (d->button_apply->isChecked())
        this->onApply();

    d->view->fitInView(d->view->sceneRect());
}

void *dtkVisualizationWidgetsClutEditor::colorTransferFunction(void)
{
    return d->colorTransferFunction;
}

void dtkVisualizationWidgetsClutEditor::setColorTransferFunction(
        vtkColorTransferFunction *color_transfer_function)
{
    if (!d->colorTransferFunction)
        d->colorTransferFunction = vtkColorTransferFunction::New();

    d->colorTransferFunction->DeepCopy(color_transfer_function);

    this->updateTable();

    this->onApply();
}

void *dtkVisualizationWidgetsClutEditor::opacityTransferFunction(void)
{
    return d->opacityTransferFunction;
}

void dtkVisualizationWidgetsClutEditor::setOpacityTransferFunction(
        vtkPiecewiseFunction *opacity_transfer_function)
{
    if (!d->opacityTransferFunction)
        d->opacityTransferFunction = vtkPiecewiseFunction::New();

    d->opacityTransferFunction->DeepCopy(opacity_transfer_function);

    this->updateTable();

    this->onApply();
}

void dtkVisualizationWidgetsClutEditor::updateTable(void)
{
    if (!d->colorTransferFunction)
        return;

    for (dtkVisualizationWidgetsClutEditorVertex *vertex : d->table->vertices) {
        d->table->removeVertex(vertex);
        d->scene->removeItem(vertex);
        delete vertex;
    }

    int size = d->colorTransferFunction->GetSize();

    double vtk_node[6];
    double vtk_range[2];

    double alpha = 0;

    d->colorTransferFunction->GetRange(vtk_range);
    for (int i = 0; i < size; ++i) {
        d->colorTransferFunction->GetNodeValue(i, vtk_node);

        double value = d->min
                + (vtk_node[0] - vtk_range[0]) / (vtk_range[1] - vtk_range[0]) * (d->max - d->min);

        if (d->opacityTransferFunction)
            alpha = d->opacityTransferFunction->GetValue(vtk_node[0]);
        else
            alpha = 1.;

        dtkVisualizationWidgetsClutEditorVertex *new_vertex =
                new dtkVisualizationWidgetsClutEditorVertex(
                        dtkVisualizationWidgetsClutEditorMap(QPointF(value, alpha), d->min, d->max,
                                                             this->width(), 100,
                                                             d->button_log->isChecked()),
                        QColor(vtk_node[1] * 255, vtk_node[2] * 255, vtk_node[3] * 255));
        d->table->addVertex(new_vertex);
        connect(new_vertex, SIGNAL(moved(const QPointF &)), d->scene,
                SIGNAL(moved(const QPointF &)));
    }
}

void dtkVisualizationWidgetsClutEditor::setRange(double min, double max)
{
    d->min = min;
    d->max = max;

    this->updateTable();

    this->onApply();
}

void dtkVisualizationWidgetsClutEditor::setHistogram(const Histogram &histogram)
{
    if (!d->histogram)
        d->histogram = new dtkVisualizationWidgetsClutEditorHistogram(d->bg);

    d->histogram->setup(histogram);
}

void dtkVisualizationWidgetsClutEditor::importClut(void)
{
    QString fileName = QFileDialog::getOpenFileName(this, "Import color lookup table");

    QDomDocument doc("clut");

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly))
        return;

    if (!doc.setContent(&file)) {
        file.close();
        return;
    }

    file.close();

    for (dtkVisualizationWidgetsClutEditorVertex *vertex : d->table->vertices) {
        d->table->removeVertex(vertex);
        d->scene->removeItem(vertex);
        delete vertex;
    }

    QDomElement root = doc.documentElement();
    double min = root.attribute("min").toDouble();
    double max = root.attribute("max").toDouble();
    int log = root.attribute("log").toInt();

    QDomNode n = root.firstChild();
    while (!n.isNull()) {
        QDomElement e = n.toElement();
        if (!e.isNull()) {

            qreal v =
                    d->min + (e.attribute("v").toDouble() - min) / (max - min) * (d->max - d->min);
            qreal a = e.attribute("a").toDouble();
            int r = e.attribute("r").toInt();
            int g = e.attribute("g").toInt();
            int b = e.attribute("b").toInt();

            dtkVisualizationWidgetsClutEditorVertex *vertex =
                    new dtkVisualizationWidgetsClutEditorVertex(
                            dtkVisualizationWidgetsClutEditorMap(QPointF(v, a), d->min, d->max,
                                                                 this->width(), 100, log),
                            QColor(r, g, b));

            d->table->addVertex(vertex);

            connect(vertex, SIGNAL(moved(const QPointF &)), d->scene,
                    SIGNAL(moved(const QPointF &)));
        }
        n = n.nextSibling();
    }

    d->table->update();
    d->view->fitInView(d->view->sceneRect());

    d->button_log->setChecked(log);
}

void dtkVisualizationWidgetsClutEditor::exportClut(void)
{
    QDomDocument document("clut");

    QDomElement root = document.createElement("clut");
    root.setAttribute("min", d->min);
    root.setAttribute("max", d->max);
    root.setAttribute("log", d->button_log->isChecked() ? "1" : "0");
    document.appendChild(root);

    for (dtkVisualizationWidgetsClutEditorVertex *vertex : d->table->vertices) {
        QPointF mapped = dtkVisualizationWidgetsClutEditorMapInv(
                vertex->scenePos(), d->min, d->max, this->width(), 100, d->button_log->isChecked());

        QDomElement element = document.createElement("vertex");
        element.setAttribute("v", mapped.x());
        element.setAttribute("a", mapped.y());
        element.setAttribute("r", vertex->color().red());
        element.setAttribute("g", vertex->color().green());
        element.setAttribute("b", vertex->color().blue());
        root.appendChild(element);
    }

    QString fileName = QFileDialog::getSaveFileName(this, "Export color lookup table");

    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly))
        return;

    QTextStream out(&file);
    out << document.toString();

    file.close();
}

void dtkVisualizationWidgetsClutEditor::toggle(bool toggle)
{
    QPropertyAnimation *animation = new QPropertyAnimation(this, "height");
    animation->setDuration(500);
    animation->setStartValue(0);
    animation->setEndValue(150);
    animation->setDirection(toggle ? QAbstractAnimation::Forward : QAbstractAnimation::Backward);
    animation->setEasingCurve(toggle ? QEasingCurve::OutBounce : QEasingCurve::InQuart);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

QSize dtkVisualizationWidgetsClutEditor::sizeHint(void) const
{
    return QSize(300, this->height());
}

void dtkVisualizationWidgetsClutEditor::onApply(void)
{
    if (!d->table->vertices.count())
        return;

    if (!d->colorTransferFunction)
        d->colorTransferFunction = vtkColorTransferFunction::New();

    d->colorTransferFunction->RemoveAllPoints();

    d->colorTransferFunction->AddRGBPoint(
            dtkVisualizationWidgetsClutEditorMapInv(QPointF(d->min, 0.0), d->min, d->max,
                                                    this->width(), 100, d->button_log->isChecked())
                    .x(),
            d->table->vertices.first()->color().redF(),
            d->table->vertices.first()->color().greenF(),
            d->table->vertices.first()->color().blueF());

    for (int i = 0; i < d->table->vertices.count(); i++) {

        dtkVisualizationWidgetsClutEditorVertex *vertex = d->table->vertices.at(i);

        double v = dtkVisualizationWidgetsClutEditorMapInv(vertex->scenePos(), d->min, d->max,
                                                           this->width(), 100,
                                                           d->button_log->isChecked())
                           .x();
        double r = vertex->color().redF();
        double g = vertex->color().greenF();
        double b = vertex->color().blueF();

        d->colorTransferFunction->AddRGBPoint(v, r, g, b);
    }

    d->colorTransferFunction->AddRGBPoint(
            dtkVisualizationWidgetsClutEditorMapInv(QPointF(d->max, 0.0), d->min, d->max,
                                                    this->width(), 100, d->button_log->isChecked())
                    .x(),
            d->table->vertices.first()->color().redF(),
            d->table->vertices.first()->color().greenF(),
            d->table->vertices.first()->color().blueF());

    if (!d->opacityTransferFunction)
        d->opacityTransferFunction = vtkPiecewiseFunction::New();

    d->opacityTransferFunction->RemoveAllPoints();

    d->opacityTransferFunction->AddPoint(
            d->min,
            dtkVisualizationWidgetsClutEditorMapInv(d->table->vertices.first()->scenePos(), d->min,
                                                    d->max, this->width(), 100,
                                                    d->button_log->isChecked())
                    .y());

    for (dtkVisualizationWidgetsClutEditorVertex *vertex : d->table->vertices) {
        d->opacityTransferFunction->AddPoint(dtkVisualizationWidgetsClutEditorMapInv(
                                                     vertex->scenePos(), d->min, d->max,
                                                     this->width(), 100, d->button_log->isChecked())
                                                     .x(),
                                             dtkVisualizationWidgetsClutEditorMapInv(
                                                     vertex->scenePos(), d->min, d->max,
                                                     this->width(), 100, d->button_log->isChecked())
                                                     .y());

        // ///////////////////////////////////////////////////////////////////
        // Oversampling to tend to logarithmic
        // ///////////////////////////////////////////////////////////////////

        // if(!d->button_log->isChecked())
        //     continue;

        // if(!d->table->vertices.indexOf(vertex))
        //     continue;

        // dtkVisualizationWidgetsClutEditorVertex *start =
        // d->table->vertices.at(d->table->vertices.indexOf(vertex) - 1);
        // dtkVisualizationWidgetsClutEditorVertex *stop = vertex;

        // qreal sampling = 100;

        // for(int i = 0; i < (int)sampling; i++) {

        //     qreal a = (stop->pos().y() - start->pos().y()) / (stop->pos().x()
        //     - start->pos().x()); qreal b = start->pos().y() - a *
        //     start->pos().x(); qreal v = start->pos().x() + (((stop->pos().x()
        //     - start->pos().x()) / sampling) * i); qreal l = a*v + b;

        //     d->opacityTransferFunction->AddPoint(
        //         dtkVisualizationWidgetsClutEditorMapInv(QPointF(v, l),
        //         d->min, d->max, this->width(), 100,
        //         d->button_log->isChecked()).x(),
        //         dtkVisualizationWidgetsClutEditorMapInv(QPointF(v, l),
        //         d->min, d->max, this->width(), 100,
        //         d->button_log->isChecked()).y());
        // }

        // ///////////////////////////////////////////////////////////////////
    }

    d->opacityTransferFunction->AddPoint(
            d->max,
            dtkVisualizationWidgetsClutEditorMapInv(d->table->vertices.last()->scenePos(), d->min,
                                                    d->max, this->width(), 100,
                                                    d->button_log->isChecked())
                    .y());

    emit updated();
}

void dtkVisualizationWidgetsClutEditor::onColorAuto(void)
{
    QList<QGraphicsItem *> selection = d->scene->selectedItems();

    if (selection.count() != 1) {
        qWarning() << Q_FUNC_INFO << "Choose only one vertex for automatic color determination";
        return;
    }

    dtkVisualizationWidgetsClutEditorVertex *vertex =
            dynamic_cast<dtkVisualizationWidgetsClutEditorVertex *>(selection.first());

    if (!vertex) {
        qWarning() << Q_FUNC_INFO << "Choose only one vertex for automatic color determination";
        return;
    }

    QList<dtkVisualizationWidgetsClutEditorVertex *> vertices = d->table->vertices;

    if (vertices.indexOf(vertex) == 0 || vertices.indexOf(vertex) == vertices.count() - 1) {
        qWarning() << Q_FUNC_INFO << "Selected vertex must be surrounded by two other vertices";
        return;
    }

    dtkVisualizationWidgetsClutEditorVertex *prev = vertices.at(vertices.indexOf(vertex) - 1);
    dtkVisualizationWidgetsClutEditorVertex *next = vertices.at(vertices.indexOf(vertex) + 1);

    dtkVisualizationWidgetsClutEditorInterpolator *interpolator = Q_NULLPTR;

    if (d->color_space == dtkVisualizationWidgetsClutEditor::HSV)
        interpolator = new dtkVisualizationWidgetsClutEditorInterpolatorHSV;

    if (d->color_space == dtkVisualizationWidgetsClutEditor::RGB)
        interpolator = new dtkVisualizationWidgetsClutEditorInterpolatorRGB;

    if (!interpolator)
        return;

    interpolator->setStart(prev);
    interpolator->setStop(next);

    vertex->setColor(interpolator->interpolate(vertex));

    if (d->button_apply->isChecked())
        this->onApply();

    delete interpolator;
}

void dtkVisualizationWidgetsClutEditor::onColorChoose(void)
{
    QList<dtkVisualizationWidgetsClutEditorVertex *> selection;

    for (QGraphicsItem *item : d->scene->selectedItems())
        if (dtkVisualizationWidgetsClutEditorVertex *vertex =
                    dynamic_cast<dtkVisualizationWidgetsClutEditorVertex *>(item))
            selection << vertex;

    if (selection.count() == 0)
        return;

    QColorDialog dialog(this);
    dialog.setCurrentColor(selection.first()->color());

    if (dialog.exec())
        for (dtkVisualizationWidgetsClutEditorVertex *vertex : selection)
            vertex->setColor(dialog.selectedColor());

    if (d->button_apply->isChecked())
        this->onApply();
}

void dtkVisualizationWidgetsClutEditor::onSelectionChanged(void)
{
    QList<QGraphicsItem *> selection = d->scene->selectedItems();

    if (selection.count() != 1) {
        d->label_value->setText("--");
        d->label_alpha->setText("--");
    } else {
        if (dtkVisualizationWidgetsClutEditorVertex *vertex =
                    dynamic_cast<dtkVisualizationWidgetsClutEditorVertex *>(selection.first()))
            this->onVertexMoved(vertex->scenePos());
    }
}

void dtkVisualizationWidgetsClutEditor::onVertexMoved(const QPointF &point)
{
    QPointF mapped = dtkVisualizationWidgetsClutEditorMapInv(point, d->min, d->max, this->width(),
                                                             100, d->button_log->isChecked());

    d->label_value->setText(QString::number(mapped.x(), 'g', 4));
    d->label_alpha->setText(QString::number(mapped.y(), 'g', 4));

    if (d->button_apply->isChecked())
        this->onApply();
}

void dtkVisualizationWidgetsClutEditor::resizeEvent(QResizeEvent *event)
{
    d->scene->setSceneRect(0, -event->size().height(), event->size().width(),
                           event->size().height());

    d->bg->setPos(0, -100);
    d->bg->setRect(0, 0, event->size().width(), 100);

    if (event->size().height() < 105)
        return;

    for (dtkVisualizationWidgetsClutEditorVertex *vertex : d->table->vertices)
        vertex->setPos(dtkVisualizationWidgetsClutEditorMap(
                dtkVisualizationWidgetsClutEditorMapInv(
                        vertex->scenePos(), d->min, d->max, event->oldSize().width(),
                        event->oldSize().height(), d->button_log->isChecked()),
                d->min, d->max, event->size().width(), event->size().height(),
                d->button_log->isChecked()));
}

// /////////////////////////////////////////////////////////////////
// dtkVisualizationWidgetsClutEditor - helper functions
// /////////////////////////////////////////////////////////////////

#include <cmath>

const double opacityCoefficient = 3;

QPointF dtkVisualizationWidgetsClutEditorMap(QPointF vertex, qreal min, qreal max, int width,
                                             int height, bool logScale)
{
    qreal x = vertex.x();
    qreal y = vertex.y();

    if (logScale)
        y = std::pow(y, 1.0 / opacityCoefficient);

    x = (x - min) / (max - min) * width;
    y = y * height;

    y = qMax(0.0, y);
    y = qMin(y, (double)height);

    return QPointF(x, y);
}

QPointF dtkVisualizationWidgetsClutEditorMapInv(QPointF vertex, qreal min, qreal max, int width,
                                                int height, bool logScale)
{
    qreal x = vertex.x() / double(width) * (max - min) + min;
    qreal y = vertex.y() / double(height) * -1.0;

    if (logScale)
        y = std::pow(y, opacityCoefficient);

    y = qMax(0.0, y);
    y = qMin(y, 1.0);

    return QPointF(x, y);
}

//
// dtkVisualizationWidgetsClutEditor.cpp ends here
