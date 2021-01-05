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

#include "dtkWidgetsParameterIntSlider.h"

#include <QtWidgets>

#include <dtkThemes/dtkThemesEngine>

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsParameterIntSliderPrivate declaration
// ///////////////////////////////////////////////////////////////////

class dtkWidgetsParameterIntSliderPrivate : public QSlider
{
    Q_OBJECT

public:
    QLabel *min = nullptr;
    QLabel *max = nullptr;

protected:
    void paintEvent(QPaintEvent *);
};

void dtkWidgetsParameterIntSliderPrivate::paintEvent(QPaintEvent *event)
{
    QSlider::paintEvent(event);

    QPainter painter(this);
    painter.setPen(dtkThemesEngine::instance()->color("@fg"));

    QFont font = painter.font();
    font.setBold(true);
    painter.setFont(font);

    double pos_rel = (value() - minimum()) / (double)(maximum() + tickInterval() - minimum());

    if (this->orientation() == Qt::Horizontal) {
        painter.drawText(QPoint((this->size().width()) * pos_rel, this->size().height()),
                         QString::number(value()));
    }
}

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsParameterIntSlider implementation
// ///////////////////////////////////////////////////////////////////

dtkWidgetsParameterIntSlider::dtkWidgetsParameterIntSlider(QWidget *parent)
    : dtkWidgetsParameterBase<dtk::d_int>(parent), d(new dtkWidgetsParameterIntSliderPrivate)
{
    d->setOrientation(Qt::Horizontal);
    d->setTickPosition(QSlider::TicksBelow);

    d->min = new QLabel("min");
    d->max = new QLabel("max");
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(d->min);
    layout->addWidget(d);
    layout->addWidget(d->max);
    QObject::connect(d, &QSlider::valueChanged, [=](int value) {
        QStyleOptionSlider opt;

        QRect sr = d->style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, d);
        QPoint bottomRightCorner = sr.bottomLeft();

        QToolTip::showText(mapToGlobal(QPoint(bottomRightCorner.x(), bottomRightCorner.y())),
                           QString::number(value), d);
    });

    this->setLayout(layout);
}

dtkWidgetsParameterIntSlider::~dtkWidgetsParameterIntSlider(void)
{
    delete d;
}

bool dtkWidgetsParameterIntSlider::connect(dtkCoreParameter *p)
{
    if (!p) {
        qWarning() << Q_FUNC_INFO << "The input parameter is null. Nothing is done.";
        return false;
    }

    m_parameter = dynamic_cast<dtk::d_int *>(p);

    if (!m_parameter) {
        qWarning() << Q_FUNC_INFO
                   << "The type of the parameter is not compatible with the "
                      "widget dtkWidgetsParameterIntSlider.";
        return false;
    }

    d->setMinimum(m_parameter->min());
    d->setMaximum(m_parameter->max());
    d->min->setText(QString::number(m_parameter->min()));
    d->max->setText(QString::number(m_parameter->max()));
    d->setToolTip(m_parameter->documentation());

    d->setValue(m_parameter->value());
    d->setTickInterval((m_parameter->max() - m_parameter->min()) / 10);

    m_parameter->connect([=](QVariant v) {
        int value = v.value<dtk::d_int>().value();
        d->blockSignals(true);
        d->setValue(value);
        d->blockSignals(false);
    });

    QObject::connect(d, QOverload<int>::of(&QSlider::valueChanged),
                     [=](int v) { m_parameter->shareValue(QVariant::fromValue(v)); });

    return true;
}

#include "dtkWidgetsParameterIntSlider.moc"
//
// dtkWidgetsParameterIntSliderWidget.cpp ends here
