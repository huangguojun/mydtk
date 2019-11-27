/* dtkPopup.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Fri Mar 18 15:40:14 2011 (+0100)
 * Version: $Id: a7c85e81fe0e0610c9f29f86b7c37493e3967c2c $
 * Last-Updated: Fri Mar 18 19:07:37 2011 (+0100)
 *           By: Julien Wintz
 *     Update #: 26
 */

/* Commentary:
 * See credits at EOF.
 */

/* Change log:
 *
 */

#include "dtkPopup.h"

#include <QtWidgets>

class dtkPopupPrivate
{
public:
    QWidget *widget;
    bool own;
    QWidget *oldParent;
};

dtkPopup::dtkPopup(QWidget *parent) : QFrame(parent, Qt::Popup), d(new dtkPopupPrivate)
{
    d->widget = 0;
    d->own = true;
    d->oldParent = 0;

    this->setAttribute(Qt::WA_WindowPropagation);

    if (parentWidget() == NULL)
        setAttribute(Qt::WA_DeleteOnClose);

    setLayout(new QVBoxLayout());

    layout()->setContentsMargins(0, 0, 0, 0);
}

void dtkPopup::setWidget(QWidget *widget, bool own)
{
    if (d->widget) {
        layout()->removeWidget(d->widget);

        if (d->own) {
            d->widget->setParent(0);
            delete d->widget;
        } else {
            d->widget->setParent(d->oldParent);
        }
    }

    d->widget = widget;
    d->own = own;
    d->oldParent = 0;

    if (d->widget) {
        d->oldParent = d->widget->parentWidget();
        d->widget->setParent(this);
        layout()->addWidget(d->widget);
    }
}

QWidget *dtkPopup::widget(void) const
{
    return d->widget;
}

bool dtkPopup::isOwned(void) const
{
    return d->own;
}

void dtkPopup::show(QPoint coord)
{
    QFrame::show();

    QRect screen(QDesktopWidget().availableGeometry());

    if (coord.x() < screen.x())
        coord.setX(screen.x());

    if (coord.y() < screen.y())
        coord.setY(screen.y());

    if (coord.x() > (screen.right() - width()))
        coord.setX(screen.right() - width());

    if (coord.y() > (screen.bottom() - height()))
        coord.setY(screen.bottom() - height());

    move(coord);
}

// /////////////////////////////////////////////////////////////////
// Credits
// /////////////////////////////////////////////////////////////////

/* QSint library. */
