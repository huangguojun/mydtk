#include "dtkVisualizationWidgetsColorMapEditor.h"

#include <cmath>

#include <QtGui>
#include <QtWidgets>
#include <QtXml>

// /////////////////////////////////////////////////////////////////
// dtkVisualizationWidgetsColorMapTable
// /////////////////////////////////////////////////////////////////

class dtkVisualizationWidgetsColorMapTable : public QWidget
{
public:
     dtkVisualizationWidgetsColorMapTable(QWidget *parent = nullptr);
    ~dtkVisualizationWidgetsColorMapTable(void) = default;

    void addColorPoint(double value, const QColor& color);
    void removeColorPoints(void);

public:
    void paintEvent(QPaintEvent *event);

public:
    QMap<double, QColor> color_points;
};

dtkVisualizationWidgetsColorMapTable::dtkVisualizationWidgetsColorMapTable(QWidget *parent) : QWidget(parent)
{
    color_points[0] = Qt::black;
    color_points[1] = Qt::white;
}

void dtkVisualizationWidgetsColorMapTable::addColorPoint(double value, const QColor& color)
{
    color_points[value] = color;
    this->update();
}

void dtkVisualizationWidgetsColorMapTable::removeColorPoints(void)
{
    color_points.clear();
}

void dtkVisualizationWidgetsColorMapTable::paintEvent(QPaintEvent* event)
// (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // Q_UNUSED(option);
    // Q_UNUSED(widget);

    QRect rectangle = event->rect();
    QPainter painter(this);

    // painter->save();

    int x = rectangle.bottomLeft().x();
    int y = rectangle.topRight().y();
    int lx = rectangle.width();
    int ly = rectangle.height();

    // QPen pen(Qt::black, 1);

    QLinearGradient linearGradient(x, y, x + lx, y);

    if (color_points.keys().first() > 0) {
        QColor color = color_points[color_points.keys().first()];
        color.setAlpha(255);
        linearGradient.setColorAt(0.0, color);
    }

    for (const auto& val : color_points.keys()) {
        QColor color = color_points[val];
        color.setAlpha(255);
        linearGradient.setColorAt(val, color);
    }

    if (color_points.keys().last()<1) {
        QColor color = color_points[color_points.keys().last()];
        color.setAlpha(255);
        linearGradient.setColorAt(1.0, color);
    }

    // painter->setPen(pen);
    // painter.setBrush(linearGradient);
    // painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.fillRect(x, y, lx, ly, linearGradient);

    // painter->restore();
}

// /////////////////////////////////////////////////////////////////
// StackedComboBox
// /////////////////////////////////////////////////////////////////

class StackedComboBox : public QComboBox
{
public:
    StackedComboBox(QWidget *parent);

public:
    void showPopup(void) override;
    void hidePopup(void) override;

public:
    bool busy = false;
};

StackedComboBox::StackedComboBox(QWidget *parent) : QComboBox(parent)
{

}

void StackedComboBox::showPopup(void)
{
    busy = true;
    QComboBox::showPopup();
}

void StackedComboBox::hidePopup(void)
{
    busy = false;
    QComboBox::hidePopup();
}

// /////////////////////////////////////////////////////////////////
// dtkVisualizationWidgetsColorMapEditor
// /////////////////////////////////////////////////////////////////

class dtkVisualizationWidgetsColorMapEditorPrivate
{
public:
    QMap<QString, QMap<double, QColor> > colormaps;
    QString current_map;

public:
    QStringList colormap_names;
    StackedComboBox *colormap_box;
    QStackedWidget *stacked_w;
    dtkVisualizationWidgetsColorMapTable *colormap_table;
};

dtkVisualizationWidgetsColorMapEditor::dtkVisualizationWidgetsColorMapEditor(QWidget *parent) : QWidget(parent), d(new dtkVisualizationWidgetsColorMapEditorPrivate)
{
    QPalette palette ;
    palette.setBrush(QPalette::Background, Qt::transparent);

    d->colormap_names.clear();

    d->colormap_names.append("0RGB_red");
    d->colormap_names.append("0RGB_green");
    d->colormap_names.append("0RGB_blue");
    d->colormap_names.append("0CMY_cyan");
    d->colormap_names.append("0CMY_magenta");
    d->colormap_names.append("0CMY_yellow");

    d->colormap_names.append("1Flashy_blue");
    d->colormap_names.append("1Flashy_green");
    d->colormap_names.append("1Flashy_orange");
    d->colormap_names.append("1Flashy_purple");
    d->colormap_names.append("1Flashy_red");
    d->colormap_names.append("1Flashy_turquoise");

    d->colormap_names.append("Blues");
    d->colormap_names.append("Greens");
    d->colormap_names.append("Greys");
    d->colormap_names.append("Oranges");
    d->colormap_names.append("Purples");
    d->colormap_names.append("Reds");

    d->colormap_names.append("BrBG");
    d->colormap_names.append("BuGn");
    d->colormap_names.append("BuGrRd");
    d->colormap_names.append("GnBu");
    d->colormap_names.append("PiYG");
    d->colormap_names.append("PuBu");
    d->colormap_names.append("PuBuGn");
    d->colormap_names.append("PuOr");
    d->colormap_names.append("PuRd");
    d->colormap_names.append("RdBu");
    d->colormap_names.append("RdYlBu");
    d->colormap_names.append("RdYlGn");
    d->colormap_names.append("YlGn");
    d->colormap_names.append("YlOrBr");

    d->colormap_names.append("acidity");
    d->colormap_names.append("atmosphere");
    d->colormap_names.append("bone");
    d->colormap_names.append("bronze");
    d->colormap_names.append("curvature");
    d->colormap_names.append("density");
    d->colormap_names.append("geo_jet");
    d->colormap_names.append("glasbey");
    d->colormap_names.append("grey");
    d->colormap_names.append("hot");
    d->colormap_names.append("inferno");
    d->colormap_names.append("jet");
    d->colormap_names.append("leaf");
    d->colormap_names.append("ocean");
    d->colormap_names.append("topography");
    d->colormap_names.append("viridis");

    d->colormap_box = new StackedComboBox(this);
    for(const auto& name : d->colormap_names) {
        d->colormap_box->addItem(name);
    }

    d->colormap_table = new dtkVisualizationWidgetsColorMapTable();
    d->colormap_table->setFixedHeight(40);
    d->colormap_table->setMinimumWidth(120);

    d->stacked_w = new QStackedWidget;
    d->stacked_w->addWidget(d->colormap_table);
    d->stacked_w->addWidget(d->colormap_box);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignTop);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(d->stacked_w);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    d->current_map = "inferno";
    d->colormap_box->setCurrentText(d->current_map);
    importColorMap(d->current_map);

    connect(d->colormap_box,
            SIGNAL(currentTextChanged(const QString&)),
            this,
            SLOT(importColorMap(const QString&)));
}

dtkVisualizationWidgetsColorMapEditor::~dtkVisualizationWidgetsColorMapEditor(void)
{
    delete d;

    d = nullptr;
}

void dtkVisualizationWidgetsColorMapEditor::enterEvent(QEvent *e)
{
    d->stacked_w->setCurrentIndex(1);
}

void dtkVisualizationWidgetsColorMapEditor::leaveEvent(QEvent *e)
{
    if(!d->colormap_box->busy)
        d->stacked_w->setCurrentIndex(0);
}


const QString& dtkVisualizationWidgetsColorMapEditor::name(void) const
{
    return d->current_map;
}

const QMap<double, QColor>& dtkVisualizationWidgetsColorMapEditor::value(void) const
{
    if(!d->colormaps.contains(d->current_map)) {
        qWarning() << Q_FUNC_INFO << "shouldn't happen";
    }

    return d->colormaps[d->current_map];
}

void dtkVisualizationWidgetsColorMapEditor::setValue(const QMap<double, QColor>& value)
{
    if(!d->colormaps.key(value, "").isEmpty() ) {
        d->current_map = d->colormaps.key(value);
    } else {
        qWarning() << Q_FUNC_INFO << "insert no_name color map";
        d->current_map = "no_name";
        d->colormaps.insert(d->current_map, value);
        d->colormap_box->addItem(d->current_map);
    }

    d->colormap_box->setCurrentText(d->current_map);
    d->colormap_table->removeColorPoints();

    for (const auto& v : value.keys())
        d->colormap_table->addColorPoint(v,value[v]);
    d->colormap_table->update();

    emit valueChanged(value);
}

void dtkVisualizationWidgetsColorMapEditor::setValue(const QString& value)
{
    if(d->colormaps.keys().contains(value)) {
        this->setValue(d->colormaps[value]);
    } else if (d->colormap_names.contains(value)) {
        this->importColorMap(value);
    }
}

void dtkVisualizationWidgetsColorMapEditor::importColorMap(const QString& clut)
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

    QMap<double, QColor> colormap;

    QDomElement root = doc.documentElement();
    double min = root.attribute("min").toDouble();
    double max = root.attribute("max").toDouble();
    int    log = root.attribute("log").toInt();

    QDomNode n = root.firstChild();
    while(!n.isNull()) {
        QDomElement e = n.toElement();
        if(!e.isNull()) {

            qreal v = e.attribute("v").toDouble();
            qreal a = e.attribute("a").toDouble();

            int r = e.attribute("r").toInt();
            int g = e.attribute("g").toInt();
            int b = e.attribute("b").toInt();

            double val = (v-min)/(max-min);
            colormap[val] = QColor(r,g,b);
        }
        n = n.nextSibling();
    }

    d->colormaps.insert(clut, colormap);
    this->setValue(colormap);
}
