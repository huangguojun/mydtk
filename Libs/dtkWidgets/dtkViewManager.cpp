// Version: $Id: a5d71ac667471276e2d9bd802e088357b82a5b0f $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "dtkViewManager.h"
#include "dtkViewLayout.h"
#include "dtkViewList.h"
#include "dtkViewListControl.h"
#include "dtkViewWidget.h"

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

    QSplitter *inner = new QSplitter(Qt::Vertical, this);
    inner->setHandleWidth(2);
    inner->addWidget(view_control);
    inner->addWidget(d->view_list);
    inner->addWidget(d->view_inspector);
    inner->setSizes(QList<int>() << 32 << (parent->size().height() - 32) / 8
                                 << (parent->size().height() - 32) / 2);

    QSplitter *splitter = new QSplitter(this);
    splitter->setHandleWidth(2);
    splitter->addWidget(inner);
    splitter->addWidget(d->view_layout);
    splitter->setSizes(QList<int>() << 300 << parent->size().width() - 300);

    QHBoxLayout *main_layout = new QHBoxLayout(this);
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->setSpacing(0);
    main_layout->addWidget(splitter);

    connect(d->view_layout, SIGNAL(focused(dtkViewWidget *)), this,
            SIGNAL(focused(dtkViewWidget *)));
    connect(d->view_layout, SIGNAL(focused(dtkViewWidget *)), this,
            SLOT(onViewFocused(dtkViewWidget *)));
    connect(d->view_layout, SIGNAL(unfocused(dtkViewWidget *)), this,
            SLOT(onViewUnfocused(dtkViewWidget *)));
}

dtkViewManager::~dtkViewManager(void)
{
    delete d;

    d = NULL;
}

void dtkViewManager::clear(void)
{
    d->view_list->clear();
    d->view_layout->clear();
}

void dtkViewManager::onViewFocused(dtkViewWidget *widget)
{
    if (!widget->inspector())
        return;

    d->view_inspector->addWidget(widget->inspector());
    d->view_inspector->setCurrentWidget(widget->inspector());
}

void dtkViewManager::onViewUnfocused(dtkViewWidget *widget)
{
    if (!widget->inspector())
        return;

    d->view_inspector->removeWidget(widget->inspector());
}

dtkViewLayout *dtkViewManager::layout(void)
{
    return d->view_layout;
}

//
// dtkViewManager.cpp ends here
