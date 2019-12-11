/* dtkPlotViewSettings.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Fri Jun  8 12:55:56 2012 (+0200)
 * Version: $Id: 7d065bb4b2b30da2510abb1cea842dd463cd6f4a $
 * Last-Updated: mar. févr.  4 15:43:37 2014 (+0100)
 *           By: Thibaud Kloczko
 *     Update #: 767
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkPlotViewSettings.h"

#include "dtkPlotView.h"
#include "dtkPlotCurve.h"

#include <dtkGuiSupport/dtkColorButton.h>

class dtkPlotViewSettingsPrivate
{
public:
    QLineEdit *mainTitle;
    QLineEdit *axisTitleX;
    QLineEdit *axisTitleY;

    QSpinBox *titleSize;
    QSpinBox *axesTitleSize;

    QComboBox *axisScaleX;
    QComboBox *axisScaleY;

    QGroupBox *legendGroup;
    QComboBox *legendPosition;

    dtkColorButton *gridColor;
    dtkColorButton *pickingColor;
    dtkColorButton *zoomColor;

    dtkColorButton *backgroundColor;
    dtkColorButton *foregroundColor;

    QFormLayout *curvesNameLayout;
    QFormLayout *curvesColorLayout;

    QFrame *curvesNameGroup;
    QFrame *curvesColorGroup;

public:
    int c_count;
    int p_count;

public:
    dtkPlotView *view;

public:
    QSignalMapper *mapperCurvesName;
    QSignalMapper *mapperCurvesColor;
};

dtkPlotViewSettings::dtkPlotViewSettings(QWidget *parent) : dtkToolBox(parent), d(new dtkPlotViewSettingsPrivate)
{
    d->view = NULL;

    d->c_count = 0;
    d->p_count = 0;

    dtkToolBoxItem *item = NULL;

    // ///////////////////////////////////////////////////////////////////
    // Titles
    // ///////////////////////////////////////////////////////////////////

    {

        d->mainTitle = new QLineEdit(this);
        d->mainTitle->setAlignment(Qt::AlignLeft);
        d->axisTitleX = new QLineEdit(this);
        d->axisTitleX->setAlignment(Qt::AlignLeft);
        d->axisTitleY = new QLineEdit(this);
        d->axisTitleY->setAlignment(Qt::AlignLeft);

        QFormLayout *layout = new QFormLayout;
        layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        layout->addRow("Main", d->mainTitle);
        layout->addRow("X-axis", d->axisTitleX);
        layout->addRow("Y-axis", d->axisTitleY);

        QFrame *page = new QFrame(this);
        page->setLayout(layout);

        item = new dtkToolBoxItem(this);
        item->setWidget(page, "Plot titles");

        this->addItem(item);

    }

    // ///////////////////////////////////////////////////////////////////
    // Sizes
    // ///////////////////////////////////////////////////////////////////

    {

        d->titleSize = new QSpinBox(this);
        d->titleSize->setRange(8, 25);
        d->titleSize->setValue(20);
        d->titleSize->setSuffix("px");
        d->titleSize->setFocusPolicy(Qt::StrongFocus);
        d->titleSize->setAlignment(Qt::AlignRight);
        d->titleSize->setAttribute(Qt::WA_MacShowFocusRect, false);

        d->axesTitleSize = new QSpinBox(this);
        d->axesTitleSize->setRange(8, 25);
        d->axesTitleSize->setValue(15);
        d->axesTitleSize->setSuffix("px");
        d->axesTitleSize->setFocusPolicy(Qt::StrongFocus);
        d->axesTitleSize->setAlignment(Qt::AlignRight);
        d->axesTitleSize->setAttribute(Qt::WA_MacShowFocusRect, false);

        QFormLayout *layout = new QFormLayout;
        layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        layout->addRow("Title", d->titleSize);
        layout->addRow("Axes title", d->axesTitleSize);

        QFrame *page = new QFrame(this);
        page->setLayout(layout);

        item = new dtkToolBoxItem(this);
        item->setWidget(page, "Plot sizes");

        this->addItem(item);

    }

    // ///////////////////////////////////////////////////////////////////
    // Scale
    // ///////////////////////////////////////////////////////////////////

    {

        QStringList scaleList = QStringList() << "Linear" << "Logarithmic";

        d->axisScaleX = new QComboBox(this);
        d->axisScaleX->addItems(scaleList);
        d->axisScaleX->setCurrentIndex(0);
        d->axisScaleX->setFocusPolicy(Qt::NoFocus);

        d->axisScaleY = new QComboBox(this);
        d->axisScaleY->addItems(scaleList);
        d->axisScaleY->setCurrentIndex(0);
        d->axisScaleY->setFocusPolicy(Qt::NoFocus);

        QFormLayout *layout = new QFormLayout;
        layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        layout->addRow("X-axis", d->axisScaleX);
        layout->addRow("Y-axis", d->axisScaleY);

        QFrame *page = new QFrame(this);
        page->setLayout(layout);

        item = new dtkToolBoxItem(this);
        item->setWidget(page, "Plot scales");

        this->addItem(item);

    }

    // ///////////////////////////////////////////////////////////////////
    // Legend
    // ///////////////////////////////////////////////////////////////////

    {

        QStringList legendPositionList = QStringList() << "Left" << "Right" << "Bottom" << "Top";

        d->legendPosition = new QComboBox(this);
        d->legendPosition->addItems(legendPositionList);
        d->legendPosition->setCurrentIndex(1);
        d->legendPosition->setFocusPolicy(Qt::NoFocus);

        QFormLayout *layout = new QFormLayout;
        layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        layout->addRow("Position", d->legendPosition);

        QFrame *page = new QFrame(this);
        page->setLayout(layout);

        item = new dtkToolBoxItem(this);
        item->setWidget(page, "Plot legend");

        this->addItem(item);

    }

    // ///////////////////////////////////////////////////////////////////
    // Colors
    // ///////////////////////////////////////////////////////////////////

    {

        d->gridColor = new dtkColorButton(this);
        d->pickingColor = new dtkColorButton(this);
        d->zoomColor = new dtkColorButton(this);

        d->backgroundColor = new dtkColorButton(this);
        d->foregroundColor = new dtkColorButton(this);

        QFormLayout *layout = new QFormLayout;
        layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        layout->addRow("Grid", d->gridColor);
        layout->addRow("Picking", d->pickingColor);
        layout->addRow("Zoom", d->zoomColor);
        layout->addRow("Background", d->backgroundColor);
        layout->addRow("Foreground", d->foregroundColor);

        QFrame *page = new QFrame(this);
        page->setLayout(layout);

        item = new dtkToolBoxItem(this);
        item->setWidget(page, "Plot colors");

        this->addItem(item);

    }

    // ///////////////////////////////////////////////////////////////////
    //
    // ///////////////////////////////////////////////////////////////////

    d->curvesNameGroup = NULL;
    d->curvesNameLayout = NULL;

    d->curvesColorGroup = NULL;
    d->curvesColorLayout = NULL;

    d->mapperCurvesName = new QSignalMapper;
    d->mapperCurvesColor = new QSignalMapper;

    // ///////////////////////////////////////////////////////////////////

    connect(d->mainTitle, SIGNAL(returnPressed()), this, SLOT(onMainTitleChanged()));
    connect(d->axisTitleX, SIGNAL(returnPressed()), this, SLOT(onAxisTitleXChanged()));
    connect(d->axisTitleY, SIGNAL(returnPressed()), this, SLOT(onAxisTitleYChanged()));

    connect(d->titleSize, SIGNAL(valueChanged(const int&)), this, SLOT(onTitleSizeChanged(const int&)));
    connect(d->axesTitleSize, SIGNAL(valueChanged(const int&)), this, SLOT(onAxesTitleSizeChanged(const int&)));

    connect(d->axisScaleX, SIGNAL(currentIndexChanged(const int&)), this, SLOT(onAxisScaleXChanged(const int&)));
    connect(d->axisScaleY, SIGNAL(currentIndexChanged(const int&)), this, SLOT(onAxisScaleYChanged(const int&)));

    connect(d->legendPosition, SIGNAL(currentIndexChanged(const int&)), this, SLOT(onLegendPositionChanged(const int&)));

    connect(d->mapperCurvesName, SIGNAL(mapped(int)), this, SLOT(updateCurveName(int)));
    connect(d->mapperCurvesColor, SIGNAL(mapped(int)), this, SLOT(updateCurveColor(int)));

    connect(d->gridColor, SIGNAL(colorChanged(const QColor&)), this, SLOT(onGridColorChanged(const QColor&)));
    connect(d->pickingColor, SIGNAL(colorChanged(const QColor&)), this, SLOT(onPickingColorChanged(const QColor&)));
    connect(d->zoomColor, SIGNAL(colorChanged(const QColor&)), this, SLOT(onZoomColorChanged(const QColor&)));

    connect(d->backgroundColor, SIGNAL(colorChanged(const QColor&)), this, SLOT(onBackgroundColorChanged(const QColor&)));
    connect(d->foregroundColor, SIGNAL(colorChanged(const QColor&)), this, SLOT(onForegroundColorChanged(const QColor&)));

    // ///////////////////////////////////////////////////////////////

    this->setStyleSheet(dtkReadFile(":dtkPlot/dtkPlotView.qss"));
}

dtkPlotViewSettings::~dtkPlotViewSettings(void)
{
    delete d;

    d = NULL;
}

void dtkPlotViewSettings::setView(dtkPlotView *view)
{
    d->view = view;

    connect(d->view, SIGNAL(updated()), this, SLOT(update()));

    this->update();
}

void dtkPlotViewSettings::onMainTitleChanged(void)
{
    d->view->setTitle(d->mainTitle->text());
}

void dtkPlotViewSettings::onAxisTitleXChanged(void)
{
    d->view->setAxisTitleX(d->axisTitleX->text());
}

void dtkPlotViewSettings::onAxisTitleYChanged(void)
{
    d->view->setAxisTitleY(d->axisTitleY->text());
}

void dtkPlotViewSettings::onTitleSizeChanged(const int& value)
{
    d->view->setTitleSize(value);
}

void dtkPlotViewSettings::onAxesTitleSizeChanged(const int& value)
{
    d->view->setAxisTitleSizeX(value);
    d->view->setAxisTitleSizeY(value);
}

void dtkPlotViewSettings::onAxisScaleXChanged(const int& index)
{
    d->view->setAxisScaleX((dtkPlotView::Scale)index);
}

void dtkPlotViewSettings::onAxisScaleYChanged(const int& index)
{
    d->view->setAxisScaleY((dtkPlotView::Scale)index);
}

void dtkPlotViewSettings::onLegendPositionChanged(const int& index)
{
    d->view->setLegendPosition((dtkPlotView::LegendPosition)index);
}

void dtkPlotViewSettings::onGridColorChanged(const QColor& color)
{
    d->view->setGridColor(d->gridColor->color());
    d->view->update();
}

void dtkPlotViewSettings::onPickingColorChanged(const QColor& color)
{
    d->view->setPickingColor(d->pickingColor->color());
    d->view->update();
}

void dtkPlotViewSettings::onZoomColorChanged(const QColor& color)
{
    d->view->setZoomColor(d->zoomColor->color());
    d->view->update();
}

void dtkPlotViewSettings::onBackgroundColorChanged(const QColor& color)
{
    d->view->setBackgroundColor(d->backgroundColor->color());
}

void dtkPlotViewSettings::onForegroundColorChanged(const QColor& color)
{
    d->view->setForegroundColor(d->foregroundColor->color());
}

void dtkPlotViewSettings::onRandomColorsClicked(void)
{
    int index = 0;

    QTime time = QTime::currentTime();

    d->view->setRandomCurvesColor((uint)time.msec());

    foreach (dtkPlotCurve *curve, d->view->curves()) {

        QColor color = curve->color();

        QLayoutItem *item = d->curvesColorLayout->itemAt(index + 2, QFormLayout::FieldRole);

        dtkColorButton *button = dynamic_cast<dtkColorButton *>(item->widget());
        button->setColor(color);

        index++;
    }

    this->onColorAreaChanged(d->view->alphaCurveArea());

    d->view->update();
}

void dtkPlotViewSettings::onColorAreaChanged(const int& value)
{
    d->view->fillCurveArea(value);
}

QColor dtkPlotViewSettings::gridColor(void) const
{
    return d->gridColor->color();
}

QColor dtkPlotViewSettings::pickingColor(void) const
{
    return d->pickingColor->color();
}

QColor dtkPlotViewSettings::zoomColor(void) const
{
    return d->zoomColor->color();
}

void dtkPlotViewSettings::update(void)
{
    if (!d->view)
        return;

    d->mainTitle->setText(d->view->title());
    d->axisTitleX->setText(d->view->axisTitleX());
    d->axisTitleY->setText(d->view->axisTitleY());

    d->axisScaleX->setCurrentIndex(d->view->axisScaleX());
    d->axisScaleY->setCurrentIndex(d->view->axisScaleY());

    d->titleSize->setValue(d->view->titleSize());
    d->axesTitleSize->setValue(d->view->axisTitleSizeX());

    d->gridColor->setColor(d->view->gridColor());
    d->pickingColor->setColor(d->view->pickingColor());
    d->zoomColor->setColor(d->view->zoomColor());
    d->backgroundColor->setColor(d->view->backgroundColor());
    d->foregroundColor->setColor(d->view->foregroundColor());

    this->updateCurves();
}

void dtkPlotViewSettings::updateCurves(void)
{
    QList<dtkPlotCurve *> curves = d->view->curves();

    d->c_count = curves.count();

    if (d->c_count && !d->p_count) {

        if (!d->curvesNameLayout) {
            d->curvesNameLayout = new QFormLayout;
            d->curvesNameLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        }

        if (!d->curvesColorLayout) {

            QPushButton *random = new QPushButton("Randomize colors", this);
            random->setFocusPolicy(Qt::NoFocus);

            QSpinBox *alphaCurveArea = new QSpinBox(this);
            alphaCurveArea->setRange(0, 255);
            alphaCurveArea->setValue(d->view->alphaCurveArea());

            d->curvesColorLayout = new QFormLayout;
            d->curvesColorLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
            d->curvesColorLayout->addWidget(random);
            d->curvesColorLayout->addRow("Alpha", alphaCurveArea);

            connect(random, SIGNAL(clicked()), this, SLOT(onRandomColorsClicked()));
            connect(alphaCurveArea, SIGNAL(valueChanged(const int&)), this, SLOT(onColorAreaChanged(const int&)));

            if (d->c_count > 1)
                d->view->setRandomCurvesColor();
        }
    }

    for (int i = 0; i < d->c_count; i++) {

        if (i < d->p_count) {

            QLayoutItem *item;

            item = d->curvesNameLayout->itemAt(i, QFormLayout::FieldRole);

            reinterpret_cast<QLineEdit *>(item->widget())->setText(curves[i]->name());

            item = d->curvesColorLayout->itemAt(i + 2, QFormLayout::FieldRole);

            reinterpret_cast<dtkColorButton *>(item->widget())->setColor(curves[i]->color());

        } else {

            QString name = "Curve " + QString::number(i + 1);

            dtkPlotCurve *curve = curves[i];

            QString title = curve->name();

            QLineEdit *entry = new QLineEdit(title, this);

            d->curvesNameLayout->addRow(name, entry);

            connect(entry, SIGNAL(editingFinished()), d->mapperCurvesName, SLOT(map()));

            d->mapperCurvesName->setMapping(entry, i);

            QColor color = curve->color();

            dtkColorButton *button = new dtkColorButton(this);
            button->setColor(color);

            d->curvesColorLayout->addRow(name, button);

            connect(button, SIGNAL(colorChanged(const QColor&)), d->mapperCurvesColor, SLOT(map()));

            d->mapperCurvesColor->setMapping(button, i);
        }
    }

    if (d->c_count && !d->p_count) {

        d->curvesNameGroup = new QFrame(this);
        d->curvesNameGroup->setLayout(d->curvesNameLayout);

        dtkToolBoxItem *item = new dtkToolBoxItem(this);
        item->setWidget(d->curvesNameGroup, "Plot curves");
        this->addItem(item);

        d->curvesColorGroup = new QFrame(this);
        d->curvesColorGroup->setLayout(d->curvesColorLayout);

        item = new dtkToolBoxItem(this);
        item->setWidget(d->curvesColorGroup, "Plot curves attributes");
        this->addItem(item);
    }

    d->p_count = d->c_count;
}

void dtkPlotViewSettings::updateCurveName(int index)
{
    QList<dtkPlotCurve *> curves = d->view->curves();

    QString name = "Curve " + QString::number(index + 1);

    QLayoutItem *item = d->curvesNameLayout->itemAt(index, QFormLayout::FieldRole);

    QLineEdit *lineedit = dynamic_cast<QLineEdit *>(item->widget());

    curves[index]->setName(lineedit->text());

    d->view->update();
}

void dtkPlotViewSettings::updateCurveColor(int index)
{
    QList<dtkPlotCurve *> curves = d->view->curves();

    QLayoutItem *item = d->curvesColorLayout->itemAt(index + 2, QFormLayout::FieldRole);

    dtkColorButton *button = dynamic_cast<dtkColorButton *>(item->widget());

    curves[index]->setColor(button->color());

    this->onColorAreaChanged(d->view->alphaCurveArea());

    d->view->update();
}
