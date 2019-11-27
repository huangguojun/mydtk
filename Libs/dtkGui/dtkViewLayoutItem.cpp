/* dtkViewLayoutItem.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Wed May 16 09:38:45 2012 (+0200)
 * Version: $Id: 2794f2777e7185896a50bbce38ab4e8da97397e3 $
 * Last-Updated: Tue Jan 21 18:12:02 2014 (+0100)
 *           By: Selim Kraria
 *     Update #: 1049
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkViewLayout.h"
#include "dtkViewLayoutItem.h"

#include <dtkCoreSupport/dtkAbstractView.h>
#include <dtkCoreSupport/dtkAbstractViewFactory.h>

#include <QtWidgets>

// /////////////////////////////////////////////////////////////////
// dtkViewLayoutItemProxyPrivate
// /////////////////////////////////////////////////////////////////

class dtkViewLayoutItemProxyPrivate
{
public:
    dtkAbstractView *view;
};

// /////////////////////////////////////////////////////////////////
// dtkViewLayoutItemPrivate
// /////////////////////////////////////////////////////////////////

class dtkViewLayoutItemPrivate
{
public:
    static dtkViewLayoutItemProxy *firstViewChild(dtkViewLayoutItem *item);

public:
    dtkViewLayoutItem *root;
    dtkViewLayoutItem *parent;

public:
    dtkViewLayoutItem *a;
    dtkViewLayoutItem *b;

public:
    dtkViewLayout *layout;

public:
    dtkViewLayoutItemProxy *proxy;

public:
    QSplitter *splitter;

public:
    QPushButton *close;
    QPushButton *horzt;
    QPushButton *vertc;
    QPushButton *maxmz;

public:
    QLineEdit *label;

public:
    QFrame *footer;

public:
    dtkViewLayoutItem *q;
};

// /////////////////////////////////////////////////////////////////
// dtkViewLayoutItemProxy
// /////////////////////////////////////////////////////////////////

dtkViewLayoutItemProxy::dtkViewLayoutItemProxy(QWidget *parent) : QFrame(parent), d(new dtkViewLayoutItemProxyPrivate)
{
    d->view = NULL;

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    this->setFocusPolicy(Qt::StrongFocus);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

dtkViewLayoutItemProxy::~dtkViewLayoutItemProxy(void)
{
    if (!d->view)
        goto finalize;

    if (!d->view->widget())
        goto finalize;

    if (!d->view->widget()->parentWidget())
        goto finalize;

    if (dtkViewLayoutItemProxy *proxy = dynamic_cast<dtkViewLayoutItemProxy *>(d->view->widget()->parentWidget())) {

        if (proxy == this) {

            layout()->removeWidget(d->view->widget());

            d->view->widget()->setParent(0);

            disconnect(d->view, SIGNAL(focused()), proxy, SIGNAL(focusedIn()));

            d->view = NULL;
        }
    }

finalize:

    delete d;

    d = NULL;
}

dtkAbstractView *dtkViewLayoutItemProxy::view(void)
{
    return d->view;
}

void dtkViewLayoutItemProxy::setView(dtkAbstractView *view)
{
    if (!view)
        return;

    if (!view->widget())
        return;

    if (dtkViewLayoutItemProxy *proxy = dynamic_cast<dtkViewLayoutItemProxy *>(view->widget()->parentWidget())) {
        proxy->layout()->removeWidget(view->widget());
        proxy->d->view = NULL;

        if (dtkViewLayoutItem *item = dynamic_cast<dtkViewLayoutItem *>(proxy->parentWidget()->parentWidget())) {
            item->d->label->clear();
        }

        disconnect(view, SIGNAL(focused()), proxy, SIGNAL(focusedIn()));
    }

    this->layout()->addWidget(view->widget());

    d->view = view;
    d->view->widget()->show();
    connect(view, SIGNAL(focused()), this, SIGNAL(focusedIn()));
    connect(view, SIGNAL(nameChanged()), this, SLOT(updateLabel()));

    if (dtkViewLayoutItem *item = dynamic_cast<dtkViewLayoutItem *>(this->parentWidget()->parentWidget())) {

        this->updateLabel();

        item->d->close->setEnabled(true);
        item->d->vertc->setEnabled(true);
        item->d->horzt->setEnabled(true);
        item->d->maxmz->setEnabled(true);

        emit focusedIn();
    }
}

void dtkViewLayoutItemProxy::updateLabel(void)
{
    if (dtkViewLayoutItem *item = dynamic_cast<dtkViewLayoutItem *>(this->parentWidget()->parentWidget())) {
        QString text = d->view->objectName();

        if (!d->view->name().isEmpty())
            text += " - " + d->view->name();

        item->d->label->setText(text);
    }
}

void dtkViewLayoutItemProxy::focusInEvent(QFocusEvent *event)
{
    QFrame::focusInEvent(event);

    emit focusedIn();
}

void dtkViewLayoutItemProxy::focusOutEvent(QFocusEvent *event)
{
    QFrame::focusOutEvent(event);

    emit focusedOut();
}

// /////////////////////////////////////////////////////////////////
// dtkViewLayoutItemPrivate
// /////////////////////////////////////////////////////////////////

dtkViewLayoutItemProxy *dtkViewLayoutItemPrivate::firstViewChild(dtkViewLayoutItem *item)
{
    if (item->d->proxy)
        return item->d->proxy;

    if (item->d->a)
        return firstViewChild(item->d->a);

    if (item->d->b)
        return firstViewChild(item->d->b);

    return NULL;
}

// /////////////////////////////////////////////////////////////////
// dtkViewLayoutItem
// /////////////////////////////////////////////////////////////////

dtkViewLayoutItem::dtkViewLayoutItem(dtkViewLayoutItem *parent) : QFrame(parent), d(new dtkViewLayoutItemPrivate)
{
    d->a = NULL;
    d->b = NULL;
    d->q = this;

    d->layout = NULL;

    if ((d->parent = parent)) {
        d->root = d->parent->d->root;
        d->layout = d->parent->d->layout;
    } else {
        d->root = this;
    }

    d->proxy = new dtkViewLayoutItemProxy(this);

    d->splitter = new QSplitter(this);
    d->splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    d->splitter->addWidget(d->proxy);

    d->horzt = new QPushButton("Horzt", this);
    d->vertc = new QPushButton("Vertc", this);
    d->close = new QPushButton("Close", this);
    d->maxmz = new QPushButton("Maxmz", this);

    d->label = new QLineEdit(this);
    d->label->setReadOnly(true);

    QHBoxLayout *footer_layout = new QHBoxLayout;
    footer_layout->addWidget(d->label);
    footer_layout->addWidget(d->horzt);
    footer_layout->addWidget(d->vertc);
    footer_layout->addWidget(d->maxmz);
    footer_layout->addWidget(d->close);

    d->footer = new QFrame(this);
    d->footer->setLayout(footer_layout);
    d->footer->setObjectName("dtkViewLayoutItemFooterUnfocused");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->splitter);
    layout->addWidget(d->footer);

    this->setAcceptDrops(true);

    connect(d->close, SIGNAL(clicked()), this, SLOT(close()));
    connect(d->horzt, SIGNAL(clicked()), this, SLOT(horzt()));
    connect(d->vertc, SIGNAL(clicked()), this, SLOT(vertc()));
    connect(d->maxmz, SIGNAL(clicked()), this, SLOT(maxmz()));

    connect(d->proxy, SIGNAL(focusedIn()), this, SLOT(onFocusedIn()));
    connect(d->proxy, SIGNAL(focusedOut()), this, SLOT(onFocusedOut()));

    d->proxy->setFocus(Qt::OtherFocusReason);

    d->close->setEnabled(d->parent != NULL);
    d->vertc->setEnabled(false);
    d->horzt->setEnabled(false);
    d->maxmz->setEnabled(false);
}

dtkViewLayoutItem::~dtkViewLayoutItem(void)
{
    delete d;

    d = NULL;
}

dtkAbstractView *dtkViewLayoutItem::view(void)
{
    if (this->proxy())
        return proxy()->view();
    else
        return NULL;
}

dtkViewLayoutItem *dtkViewLayoutItem::parent(void)
{
    return d->parent;
}

dtkViewLayoutItem *dtkViewLayoutItem::first(void)
{
    return d->a;
}

dtkViewLayoutItem *dtkViewLayoutItem::second(void)
{
    return d->b;
}

void dtkViewLayoutItem::setOrientation(Qt::Orientation orientation)
{
    d->splitter->setOrientation(orientation);
}

void dtkViewLayoutItem::setSizes(QList<int> sizes)
{
    d->splitter->setSizes(sizes);
}

int dtkViewLayoutItem::canvasHeight(void)
{
    return d->splitter->sizes().first();
}

int dtkViewLayoutItem::footerHeight(void)
{
    return d->footer->height();
}

int dtkViewLayoutItem::handleHeight(void)
{
    return 7;
}

int dtkViewLayoutItem::handleWidth(void)
{
    return 7;
}

dtkViewLayout *dtkViewLayoutItem::layout(void)
{
    return d->layout;
}

dtkViewLayoutItemProxy *dtkViewLayoutItem::proxy(void)
{
    return d->proxy;
}

void dtkViewLayoutItem::setLayout(dtkViewLayout *layout)
{
    d->layout = layout;
}

void dtkViewLayoutItem::clear(void)
{
    if (d->proxy && d->proxy->view()) {
        d->proxy->view()->widget()->hide();
        emit unfocused(d->proxy->view());
    }

    delete d->proxy;

    d->proxy = new dtkViewLayoutItemProxy(d->root);

    connect(d->proxy, SIGNAL(focusedIn()), this, SLOT(onFocusedIn()));
    connect(d->proxy, SIGNAL(focusedOut()), this, SLOT(onFocusedOut()));

    d->splitter->addWidget(d->proxy);

    d->proxy->setFocus(Qt::OtherFocusReason);

    d->footer->show();

    this->setUpdatesEnabled(true);

    if (d->a)
        d->a->deleteLater();

    if (d->b)
        d->b->deleteLater();

    d->a = NULL;
    d->b = NULL;

    d->label->clear();
    d->close->setEnabled(false);
    d->vertc->setEnabled(false);
    d->horzt->setEnabled(false);
    d->maxmz->setEnabled(false);
}

void dtkViewLayoutItem::split(void)
{
    if (!d->proxy->view())
        return;

    QSize size = this->size();

    d->a = new dtkViewLayoutItem(this);
    d->b = new dtkViewLayoutItem(this);

    d->splitter->addWidget(d->a);
    d->splitter->addWidget(d->b);

    d->a->d->proxy->setFocus(Qt::OtherFocusReason);
    d->a->d->proxy->setView(d->proxy->view());

    disconnect(d->proxy, SIGNAL(focusedIn()), this, SLOT(onFocusedIn()));
    disconnect(d->proxy, SIGNAL(focusedOut()), this, SLOT(onFocusedOut()));

    delete d->proxy;

    d->proxy = NULL;

    d->footer->hide();

    d->splitter->resize(size);
}

void dtkViewLayoutItem::unsplit(void)
{
    if (!d->a && !d->b)
        return;

    d->root->setUpdatesEnabled(false);

    if (d->layout->current() == d->a) {

        d->a->deleteLater();

        d->a = NULL;

        if (d->b->d->a && d->b->d->b) {

            dtkViewLayoutItem *a = d->b->d->a; a->d->parent = this;
            dtkViewLayoutItem *b = d->b->d->b; b->d->parent = this;

            a->setParent(this);
            b->setParent(this);

            d->splitter->setOrientation(d->b->d->splitter->orientation());

            d->b->deleteLater();

            d->b = NULL;

            d->a = a;
            d->b = b;

            d->splitter->addWidget(d->a);
            d->splitter->addWidget(d->b);

            dtkViewLayoutItemProxy *child = NULL;

            if (!(child = dtkViewLayoutItemPrivate::firstViewChild(d->a)))
                child = dtkViewLayoutItemPrivate::firstViewChild(d->b);

            if (child)
                child->setFocus(Qt::OtherFocusReason);

        } else {

            d->proxy = new dtkViewLayoutItemProxy(this);

            connect(d->proxy, SIGNAL(focusedIn()), this, SLOT(onFocusedIn()));
            connect(d->proxy, SIGNAL(focusedOut()), this, SLOT(onFocusedOut()));

            d->splitter->addWidget(d->proxy);

            d->proxy->setView(d->b->d->proxy->view());

            d->proxy->setFocus(Qt::OtherFocusReason);

            d->b->deleteLater();

            d->b = NULL;

            d->footer->show();
        }
    }

    else if (d->layout->current() == d->b) {

        d->b->deleteLater();

        d->b = NULL;

        if (d->a->d->a && d->a->d->b) {

            dtkViewLayoutItem *a = d->a->d->a; a->d->parent = this;
            dtkViewLayoutItem *b = d->a->d->b; b->d->parent = this;

            a->setParent(this);
            b->setParent(this);

            d->splitter->setOrientation(d->a->d->splitter->orientation());

            d->a->deleteLater();

            d->a = NULL;

            d->a = a;
            d->b = b;

            d->splitter->addWidget(d->a);
            d->splitter->addWidget(d->b);

            dtkViewLayoutItemProxy *child = NULL;

            if (!(child = dtkViewLayoutItemPrivate::firstViewChild(d->a)))
                child = dtkViewLayoutItemPrivate::firstViewChild(d->b);

            if (child)
                child->setFocus(Qt::OtherFocusReason);

        } else {

            d->proxy = new dtkViewLayoutItemProxy(this);

            connect(d->proxy, SIGNAL(focusedIn()), this, SLOT(onFocusedIn()));
            connect(d->proxy, SIGNAL(focusedOut()), this, SLOT(onFocusedOut()));

            d->splitter->addWidget(d->proxy);

            d->proxy->setView(d->a->d->proxy->view());
            d->proxy->setFocus(Qt::OtherFocusReason);

            d->a->deleteLater();

            d->a = NULL;

            d->footer->show();
        }

    } else {

        qDebug() << DTK_PRETTY_FUNCTION << "Unhandled case.";

    }

    d->root->setUpdatesEnabled(true);
}

void dtkViewLayoutItem::maximize(void)
{
    if (d->a && d->b)
        return;

    if (this == d->root)
        return;

    d->root->setUpdatesEnabled(false);

    d->root->d->proxy = new dtkViewLayoutItemProxy(d->root);

    d->root->connect(d->root->d->proxy, SIGNAL(focusedIn()), d->root, SLOT(onFocusedIn()));
    d->root->connect(d->root->d->proxy, SIGNAL(focusedOut()), d->root, SLOT(onFocusedOut()));

    d->root->d->splitter->addWidget(d->root->d->proxy);

    d->root->d->proxy->setView(d->proxy->view());
    d->root->d->proxy->setFocus(Qt::OtherFocusReason);

    d->root->d->footer->show();

    d->root->setUpdatesEnabled(true);

    d->root->d->a->deleteLater();
    d->root->d->b->deleteLater();

    d->root->d->a = NULL;
    d->root->d->b = NULL;
}

void dtkViewLayoutItem::onFocusedIn(void)
{
    if (d->layout->current())
        d->layout->current()->onFocusedOut();

    d->layout->setCurrent(this);

    d->footer->setObjectName("dtkViewLayoutItemFooterFocused");
    d->footer->style()->unpolish(d->footer);
    d->footer->style()->polish(d->footer);
    d->footer->update();

    if (d->proxy->view())
        d->root->notify(d->proxy->view());
}

void dtkViewLayoutItem::onFocusedOut(void)
{
    d->footer->setObjectName("dtkViewLayoutItemFooterUnfocused");
    d->footer->style()->unpolish(d->footer);
    d->footer->style()->polish(d->footer);
    d->footer->update();
}

void dtkViewLayoutItem::close(void)
{
    this->onFocusedIn();

    if (d->parent)
        d->parent->unsplit();
    else
        clear();
}

void dtkViewLayoutItem::horzt(void)
{
    this->onFocusedIn();

    d->splitter->setOrientation(Qt::Horizontal);

    this->split();
}

void dtkViewLayoutItem::vertc(void)
{
    this->onFocusedIn();

    d->splitter->setOrientation(Qt::Vertical);

    this->split();
}

void dtkViewLayoutItem::maxmz(void)
{
    this->onFocusedIn();

    this->maximize();
}

void dtkViewLayoutItem::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void dtkViewLayoutItem::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void dtkViewLayoutItem::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

void dtkViewLayoutItem::dropEvent(QDropEvent *event)
{
    Q_UNUSED(event);

    if (d->a && d->b)
        return;

    if (d->proxy->view())
        return;

    dtkAbstractView *view = dtkAbstractViewFactory::instance()->view(event->mimeData()->text());

    d->proxy->setView(view);
}

void dtkViewLayoutItem::notify(dtkAbstractView *view)
{
    if (d->root == this)
        emit focused(view);
}
