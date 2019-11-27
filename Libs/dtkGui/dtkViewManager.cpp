/* dtkViewManager.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Wed May 16 09:37:54 2012 (+0200)
 * Version: $Id: e11ec779ebe18197bee5e744cdd5aa8b3880dbfc $
 * Last-Updated: Thu Jan 16 17:22:38 2014 (+0100)
 *           By: Selim Kraria
 *     Update #: 63
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkViewManager.h"
#include "dtkViewList.h"
#include "dtkViewLayout.h"
#include "dtkViewListControl.h"

#include <QtWidgets>

class dtkViewManagerPrivate
{
public:
    dtkViewList *view_list;
    dtkViewLayout *view_layout;
    QStackedWidget *view_inspector;
};

dtkViewManager::dtkViewManager(QWidget *parent) : QFrame(parent), d(new dtkViewManagerPrivate)
{
    d->view_list = new dtkViewList;
    d->view_layout = new dtkViewLayout;
    d->view_inspector = new QStackedWidget;

    dtkViewListControl *view_control = new dtkViewListControl(this);
    view_control->setLayout(d->view_layout);
    view_control->setList(d->view_list);

    QFrame *frame = new QFrame;
    frame->setAutoFillBackground(true);
    frame->setFixedWidth(300);

    QVBoxLayout *layout = new QVBoxLayout(frame);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(view_control);
    layout->addWidget(d->view_list);
    layout->addWidget(d->view_inspector);

    QHBoxLayout *main_layout = new QHBoxLayout(this);
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->setSpacing(0);
    main_layout->addWidget(frame);
    main_layout->addWidget(d->view_layout);

    // Behaviour

    connect(d->view_layout, SIGNAL(focused(dtkAbstractView *)), this, SIGNAL(focused(dtkAbstractView *)));
    connect(d->view_layout, SIGNAL(unfocused(dtkAbstractView *)), this, SLOT(hideCurrentWidget()));
}

dtkViewManager::~dtkViewManager(void)
{
    delete d;

    d = NULL;
}

void dtkViewManager::clear(void)
{
    this->hideCurrentWidget();
    d->view_list->clear();
    d->view_layout->clear();
}

void dtkViewManager::addWidget(QWidget *widget)
{
    d->view_inspector->addWidget(widget);
    widget->setVisible(false);
}

void dtkViewManager::setCurrentWidget(QWidget *widget)
{
    widget->setVisible(true);
    d->view_inspector->setCurrentWidget(widget);
}

void dtkViewManager::hideCurrentWidget(void)
{
    QWidget *widget = d->view_inspector->currentWidget();

    if (widget)
        widget->setVisible(false);
}
