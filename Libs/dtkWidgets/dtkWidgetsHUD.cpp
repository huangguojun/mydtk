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

#include "dtkWidgetsHUD.h"
#include "dtkWidgetsHUDInfo.h"
#include "dtkWidgetsHUDItem.h"

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsHUDPrivate
// ///////////////////////////////////////////////////////////////////

class dtkWidgetsHUDPrivate
{
public:
    QList<dtkWidgetsHUDItem *> items;
    QList<dtkWidgetsHUDInfo *> infos;
};

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsHUD
// ///////////////////////////////////////////////////////////////////

dtkWidgetsHUD::dtkWidgetsHUD(QWidget *parent) : QFrame(parent), d(new dtkWidgetsHUDPrivate)
{
    this->setAttribute(Qt::WA_NoSystemBackground);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setAttribute(Qt::WA_TransparentForMouseEvents, true);
}

dtkWidgetsHUD::~dtkWidgetsHUD(void)
{
    qDeleteAll(d->items);

    delete d;
}

dtkWidgetsHUDInfo *dtkWidgetsHUD::addInfo(const QString &text)
{
    dtkWidgetsHUDInfo *info = new dtkWidgetsHUDInfo(text, this);

    d->infos << info;

    qApp->postEvent(this, new QResizeEvent(this->size(), this->size() * 0.9));

    QTimer *timer = new QTimer(info);
    timer->setSingleShot(true);
    timer->setInterval(1500);

    connect(timer, &QTimer::timeout, [=]() {
        d->infos.removeOne(info);
        delete info;
    });

    timer->start();

    return info;
}

dtkWidgetsHUDItem *dtkWidgetsHUD::addItem(fa::icon icon)
{
    dtkWidgetsHUDItem *item = new dtkWidgetsHUDItem(icon, this->parentWidget());

    d->items << item;

    return item;
}

void dtkWidgetsHUD::mouseMoveEvent(QMouseEvent *event)
{
    event->ignore();

    QFrame::mouseMoveEvent(event);
}

void dtkWidgetsHUD::mousePressEvent(QMouseEvent *event)
{
    event->ignore();

    QFrame::mousePressEvent(event);
}

void dtkWidgetsHUD::resizeEvent(QResizeEvent *event)
{
    QPoint items_origin = QPoint(event->size().width() - 50, 25);
    QPoint infos_origin = QPoint(event->size().width() / 2, 25);

    for (int i = 0; i < d->items.count(); i++)
        d->items[i]->move(items_origin.x(), items_origin.y() + i * 50);

    for (int i = 0; i < d->infos.count(); i++)
        d->infos[i]->move(infos_origin.x() - 100, infos_origin.y() + i * 100);

    QFrame::resizeEvent(event);
}

//
// dtkWidgetsHUD.cpp ends here
