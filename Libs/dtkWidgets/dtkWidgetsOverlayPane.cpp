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

#include "dtkWidgetsOverlayPane.h"

#include <dtkThemes/dtkThemesEngine>

#include <QtWidgets>

// ///////////////////////////////////////////////////////////////////

static const int duration = 250;

// /////////////////////////////////////////////////////////////////////////////
// dtkWidgetsOverlayPanePrivate
// /////////////////////////////////////////////////////////////////////////////

class dtkWidgetsOverlayPanePrivate
{
public:
    int width;

public:
    int bound = 300;

public:
    bool on = false;
    bool deactivate = false;

public:
    QVBoxLayout *layout;
};

// /////////////////////////////////////////////////////////////////////////////
// dtkWidgetsOverlayPane
// /////////////////////////////////////////////////////////////////////////////

dtkWidgetsOverlayPane::dtkWidgetsOverlayPane(QWidget *parent) : QFrame(parent)
{
    d = new dtkWidgetsOverlayPanePrivate;
    d->width = 0;

    d->layout = new QVBoxLayout;
    d->layout->setAlignment(Qt::AlignTop);
    d->layout->setContentsMargins(0, 0, 0, 0);
    d->layout->setSpacing(0);

    QWidget *widget = new QWidget(this);
    widget->setLayout(d->layout);

    QScrollArea *scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    scroll->setWidget(widget);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignTop);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(scroll);

    this->setAttribute(Qt::WA_NoSystemBackground);
    this->setAttribute(Qt::WA_TranslucentBackground);
}

dtkWidgetsOverlayPane::~dtkWidgetsOverlayPane(void)
{
    delete d;
}

int dtkWidgetsOverlayPane::width(void)
{
    return d->width;
}

void dtkWidgetsOverlayPane::setBound(int bound)
{
    d->bound = bound;

    if (this->width() > d->bound) {
        this->setFixedWidth(d->bound);
    }
}

void dtkWidgetsOverlayPane::setWidth(int width)
{
    d->width = width;

    this->setFixedWidth(d->width);

    emit widthChanged();
}

void dtkWidgetsOverlayPane::toggle(void)
{
    if (d->deactivate)
        return;

    qlonglong stt = d->on ? d->bound : 0;
    qlonglong end = d->on ? 0 : d->bound;

    QPropertyAnimation *animation = new QPropertyAnimation(this, "width");
    animation->setDuration(::duration);
    animation->setEasingCurve(QEasingCurve::OutQuad);

    animation->setStartValue(stt);
    animation->setEndValue(end);
    animation->start(QAbstractAnimation::DeleteWhenStopped);

    d->on = !d->on;
}

void dtkWidgetsOverlayPane::toggle(std::function<void(void)> &callback)
{
    if (d->deactivate)
        return;

    qlonglong stt = d->on ? d->bound : 0;
    qlonglong end = d->on ? 0 : d->bound;

    QPropertyAnimation *animation = new QPropertyAnimation(this, "width");
    animation->setDuration(::duration);
    animation->setEasingCurve(QEasingCurve::OutQuad);

    connect(animation, &QAbstractAnimation::finished, callback);

    animation->setStartValue(stt);
    animation->setEndValue(end);
    animation->start(QAbstractAnimation::DeleteWhenStopped);

    d->on = !d->on;
}

void dtkWidgetsOverlayPane::deactivate(void)
{
    d->deactivate = true;
}

void dtkWidgetsOverlayPane::enterEvent(QEvent *)
{
    emit entered();
}

void dtkWidgetsOverlayPane::leaveEvent(QEvent *)
{
    emit left();
}

void dtkWidgetsOverlayPane::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    QPainter painter(this);

    QColor color = dtkThemesEngine::instance()->color("@base4");
    color.setAlpha(178);

    painter.fillRect(event->rect(), color);

    color = dtkThemesEngine::instance()->color("@base1");

    painter.setPen(color);
    painter.drawLine(event->rect().topRight() + QPoint(1, 0),
                     event->rect().bottomRight() + QPoint(1, 0));
}

void dtkWidgetsOverlayPane::addLayout(QLayout *layout)
{
    d->layout->addLayout(layout);
}

void dtkWidgetsOverlayPane::addWidget(QWidget *widget)
{
    d->layout->addWidget(widget);
}

void dtkWidgetsOverlayPane::clear(void)
{
    QLayoutItem *child;

    while ((child = d->layout->takeAt(0)) != 0) {
        delete child->widget();
        delete child;
    }
}

//
// dtkWidgetsOverlayPane.cpp ends here
