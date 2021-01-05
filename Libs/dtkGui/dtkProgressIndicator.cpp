/* dtkProgressIndicator.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Mon Mar 22 10:20:50 2010 (+0100)
 * Version: $Id: af0690eb140bee2ac0c1414961964bcee1d6d9f5 $
 * Last-Updated: Wed Mar 24 09:34:29 2010 (+0100)
 *           By: Julien Wintz
 *     Update #: 12
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkProgressIndicator.h"

class dtkProgressIndicatorPrivate
{
public:
    int angle;
    int timerId;
    int delay;
    bool displayedWhenStopped;
    QColor color;
};

dtkProgressIndicator::dtkProgressIndicator(QWidget *parent)
    : QWidget(parent), d(new dtkProgressIndicatorPrivate)
{
    d->angle = 0;
    d->timerId = -1;
    d->delay = 40;
    d->displayedWhenStopped = false;
    d->color = QColor(Qt::black);

    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setFocusPolicy(Qt::NoFocus);
}

int dtkProgressIndicator::animationDelay(void) const
{
    return d->delay;
}

const QColor &dtkProgressIndicator::color(void) const
{
    return d->color;
}

bool dtkProgressIndicator::isAnimated(void) const
{
    return (d->timerId != -1);
}

void dtkProgressIndicator::setDisplayedWhenStopped(bool state)
{
    d->displayedWhenStopped = state;

    update();
}

bool dtkProgressIndicator::isDisplayedWhenStopped(void) const
{
    return d->displayedWhenStopped;
}

void dtkProgressIndicator::startAnimation(void)
{
    d->angle = 0;

    if (d->timerId == -1)
        d->timerId = startTimer(d->delay);
}

void dtkProgressIndicator::stopAnimation(void)
{
    if (d->timerId != -1)
        killTimer(d->timerId);

    d->timerId = -1;

    update();
}

void dtkProgressIndicator::setAnimationDelay(int delay)
{
    if (d->timerId != -1)
        killTimer(d->timerId);

    d->delay = delay;

    if (d->timerId != -1)
        d->timerId = startTimer(d->delay);
}

void dtkProgressIndicator::setColor(const QColor &color)
{
    d->color = color;

    update();
}

QSize dtkProgressIndicator::sizeHint(void) const
{
    return QSize(20, 20);
}

int dtkProgressIndicator::heightForWidth(int w) const
{
    return w;
}

void dtkProgressIndicator::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    d->angle = (d->angle + 30) % 360;

    update();
}

void dtkProgressIndicator::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    if (!d->displayedWhenStopped && !isAnimated())
        return;

    int width = qMin(this->width(), this->height());

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    int outerRadius = (width - 1) * 0.5;
    int innerRadius = (width - 1) * 0.5 * 0.38;

    int capsuleHeight = outerRadius - innerRadius;
    int capsuleWidth = (width > 32) ? capsuleHeight * .23 : capsuleHeight * .35;
    int capsuleRadius = capsuleWidth / 2;

    for (int i = 0; i < 12; i++) {
        QColor color = d->color;
        color.setAlphaF(1.0f - (i / 12.0f));
        p.setPen(Qt::NoPen);
        p.setBrush(color);
        p.save();
        p.translate(rect().center());
        p.rotate(d->angle - i * 30.0f);
        p.drawRoundedRect(-capsuleWidth * 0.5, -(innerRadius + capsuleHeight), capsuleWidth,
                          capsuleHeight, capsuleRadius, capsuleRadius);
        p.restore();
    }
}
