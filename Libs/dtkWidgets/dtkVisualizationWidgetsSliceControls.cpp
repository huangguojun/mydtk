#include "dtkVisualizationWidgetsSliceControls.h"

#include <QtWidgets>

class dtkVisualizationWidgetsSliceControlsPrivate
{
public:
    QCheckBox *enable_slicing = nullptr;
    QCheckBox *reset_pos = nullptr;
    QDoubleSpinBox *slice_position;
};

dtkVisualizationWidgetsSliceControls::dtkVisualizationWidgetsSliceControls(QWidget *parent)
    : QWidget(parent), d(new dtkVisualizationWidgetsSliceControlsPrivate)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(10, 0, 10, 0);
    layout->setSpacing(5);

    d->enable_slicing = new QCheckBox(this);
    d->enable_slicing->setCheckable(true);
    d->enable_slicing->setChecked(true);
    d->enable_slicing->setToolTip("Enable/Disable slice");

    d->slice_position = new QDoubleSpinBox(this);
    d->slice_position->setRange(0, 99999999);
    d->slice_position->setAlignment(Qt::AlignRight);

    QLabel *label_reset = new QLabel(this);
    label_reset->setText("R");
    label_reset->setToolTip("Reset");

    d->reset_pos = new QCheckBox(this);
    d->reset_pos->setCheckable(false);

    layout->addWidget(d->enable_slicing);
    layout->addWidget(d->slice_position);
    layout->addWidget(label_reset);
    layout->addWidget(d->reset_pos);

    this->setLayout(layout);

    connect(d->slice_position, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            &dtkVisualizationWidgetsSliceControls::valueChanged);
    connect(d->enable_slicing, &QCheckBox::toggled, this,
            &dtkVisualizationWidgetsSliceControls::toggled);
    connect(d->reset_pos, &QCheckBox::clicked, [=](bool b) {
        d->slice_position->setValue(0.0);
        emit reset();
    });
}

dtkVisualizationWidgetsSliceControls::~dtkVisualizationWidgetsSliceControls(void)
{
    delete d;
}

bool dtkVisualizationWidgetsSliceControls::isChecked(void)
{
    return d->enable_slicing->isChecked();
}

void dtkVisualizationWidgetsSliceControls::setCheckState(Qt::CheckState state)
{
    d->enable_slicing->setCheckState(state);
}

void dtkVisualizationWidgetsSliceControls::setChecked(bool b)
{
    d->enable_slicing->setChecked(b);
}

void dtkVisualizationWidgetsSliceControls::setValue(double value)
{
    d->slice_position->setValue(value);
}

double dtkVisualizationWidgetsSliceControls::value(void)
{
    return d->slice_position->value();
}
