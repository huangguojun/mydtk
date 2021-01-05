// Version: $Id: 4c96fa26d7540561087bbe7b5e1fdfa6553b8884 $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "dtkViewListControl.h"
#include "dtkViewController.h"
#include "dtkViewLayout.h"
#include "dtkViewLayoutItem.h"
#include "dtkViewList.h"

class dtkViewListControlPrivate
{
public:
    dtkViewLayout *layout;
    dtkViewList *list;

public:
    QPushButton *hor;
    QPushButton *ver;
    QPushButton *grd;
    QPushButton *cls;
};

dtkViewListControl::dtkViewListControl(QWidget *parent)
    : QFrame(parent), d(new dtkViewListControlPrivate)
{
    d->layout = NULL;
    d->list = NULL;

    d->hor = new QPushButton("Horizontal", this);
    d->ver = new QPushButton("Vertical", this);
    d->grd = new QPushButton("Grid", this);
    d->cls = new QPushButton("Close All", this);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(d->hor);
    layout->addWidget(d->ver);
    layout->addWidget(d->grd);
    layout->addWidget(d->cls);

    connect(d->hor, SIGNAL(clicked()), this, SLOT(onLayoutHorizontally()));
    connect(d->ver, SIGNAL(clicked()), this, SLOT(onLayoutVertically()));
    connect(d->grd, SIGNAL(clicked()), this, SLOT(onLayoutGrid()));
    connect(d->cls, SIGNAL(clicked()), this, SLOT(onLayoutCloseAll()));
}

dtkViewListControl::~dtkViewListControl(void)
{
    delete d;

    d = NULL;
}

void dtkViewListControl::setLayout(dtkViewLayout *layout)
{
    d->layout = layout;
}

void dtkViewListControl::setList(dtkViewList *list)
{
    d->list = list;
}

bool dtkViewListControl::isEmpty(void) const
{
    if (!d->list)
        return true;

    if (!d->list->count())
        return true;

    if (!d->layout)
        return true;

    return false;
}

void dtkViewListControl::onActorStarted(QString view_name)
{
    if (!d->layout)
        return;

    dtkViewWidget *view = dtkViewController::instance()->view(view_name);

    if (view && !d->layout->current()->proxy()->view())
        d->layout->current()->proxy()->setView(view);
}

void dtkViewListControl::layoutHorizontally(void)
{
    if (this->isEmpty())
        return;

    this->closeAllLayout();

    qApp->sendPostedEvents(0, QEvent::DeferredDelete);

    int w = d->layout->current()->width();
    int n = d->list->count();
    int s = d->layout->current()->handleWidth();
    int v = (w - (n - 1) * s) / n;

    for (int i = 1; i <= n; i++) {

        dtkViewLayoutItem *current = d->layout->current();
        current->setOrientation(Qt::Horizontal);
        current->proxy()->setView(dtkViewController::instance()->view(
                d->list->item(i - 1)->text().split(" ").first()));

        if (i != n) {
            QList<int> sizes = QList<int>() << v << current->width() - s - v;
            current->split();
            current->setSizes(sizes);
        }

        d->layout->setCurrent(current->second());
    }
}

void dtkViewListControl::onLayoutHorizontally(void)
{
    this->layoutHorizontally();
}

void dtkViewListControl::layoutVertically(void)
{
    if (this->isEmpty())
        return;

    this->closeAllLayout();

    qApp->sendPostedEvents(0, QEvent::DeferredDelete);

    int h = d->layout->current()->height();
    int f = d->layout->current()->footerHeight();
    int n = d->list->count();
    int s = d->layout->current()->handleHeight();
    int v = (h - n * f - (n - 1) * s) / n;

    for (int i = 1; i <= n; i++) {

        dtkViewLayoutItem *current = d->layout->current();
        current->setOrientation(Qt::Vertical);
        current->proxy()->setView(dtkViewController::instance()->view(
                d->list->item(i - 1)->text().split(" ").first()));

        if (i != n) {
            QList<int> sizes = QList<int>() << v + f << current->height() - s - v - f;
            current->split();
            current->setSizes(sizes);
        }

        d->layout->setCurrent(current->second());
    }
}

void dtkViewListControl::onLayoutVertically(void)
{
    this->layoutVertically();
}

void dtkViewListControl::layoutGrid(void)
{
    if (this->isEmpty())
        return;

    int n = d->list->count();
    int i = 0;

    typedef QPair<dtkViewLayoutItem *, Qt::Orientation> item_t;

    d->layout->clear();
    d->layout->setCurrent(d->layout->root());
    d->layout->current()->proxy()->setView(
            dtkViewController::instance()->view(d->list->item(i)->text().split(" ").first()));

    QList<item_t> items;
    items << qMakePair(d->layout->current(), Qt::Horizontal);

    for (int i = 1; i < n; i++) {

        item_t item = items.takeFirst();

        d->layout->setCurrent(item.first);

        dtkViewLayoutItem *current = item.first;
        current->setOrientation(item.second);
        current->split();
        current->second()->proxy()->setView(
                dtkViewController::instance()->view(d->list->item(i)->text().split(" ").first()));

        items << qMakePair(current->first(),
                           item.second == Qt::Horizontal ? Qt::Vertical : Qt::Horizontal);
        items << qMakePair(current->second(),
                           item.second == Qt::Horizontal ? Qt::Vertical : Qt::Horizontal);
    }
}

void dtkViewListControl::onLayoutGrid(void)
{
    this->layoutGrid();
}

void dtkViewListControl::closeAllLayout(void)
{
    if (this->isEmpty())
        return;

    d->layout->clear();
    d->layout->setCurrent(d->layout->root());
}

void dtkViewListControl::onLayoutCloseAll(void)
{
    this->closeAllLayout();
}

//
// dtkViewListControl.cpp ends here
