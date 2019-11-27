/* dtkAnchoredBar.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Tue Mar 16 08:45:44 2010 (+0100)
 * Version: $Id: 88783072f9bc912a5891314260e3768f3e8a2ea7 $
 * Last-Updated: Tue Apr  3 16:07:25 2012 (+0200)
 *           By: tkloczko
 *     Update #: 23
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkAnchoredBar.h"

#include <dtkCoreSupport/dtkGlobal.h>

class dtkAnchoredBarPrivate
{
public:
    QPoint dragPosition;
    int parentHeight;
    int parentWidth;
    bool drag_enabled;
};

dtkAnchoredBar::dtkAnchoredBar(QWidget *parent) : QToolBar(parent), d(new dtkAnchoredBarPrivate)
{
    d->drag_enabled = true;

    this->setMouseTracking(true);
    this->setFixedHeight(23);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->setStyleSheet(dtkReadFile(":dtkGui/dtkAnchoredBar.qss"));
}

void dtkAnchoredBar::setDragEnabled(bool enabled)
{
    d->drag_enabled = enabled;
}

QSize dtkAnchoredBar::sizeHint(void) const
{
    return QSize(100, 23);
}

void dtkAnchoredBar::mouseMoveEvent(QMouseEvent *event)
{
    if (!d->drag_enabled)
        return;

    if (event->pos().x() > this->width() - 23)
        this->setCursor(Qt::SplitHCursor);
    else
        this->setCursor(Qt::ArrowCursor);

    if (!d->dragPosition.isNull())
        parentWidget()->setMaximumWidth(d->parentWidth + (event->pos().x() - d->dragPosition.x()));
}

void dtkAnchoredBar::mousePressEvent(QMouseEvent *event)
{
    if (event->pos().x() > this->width() - 23) {
        d->parentHeight = parentWidget()->height();
        d->parentWidth = parentWidget()->width();
        d->dragPosition = event->pos();
    }
}

void dtkAnchoredBar::mouseReleaseEvent(QMouseEvent *event)
{
    DTK_UNUSED(event);

    d->dragPosition = QPoint();
}
