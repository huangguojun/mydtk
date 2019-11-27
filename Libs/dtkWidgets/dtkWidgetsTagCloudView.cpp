// Version: $Id: 3acfb4caf99e9529f3f0bcc977a30b2ad2d0fb9a $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "dtkWidgetsTagCloudDesc.h"
#include "dtkWidgetsTagCloudList.h"
#include "dtkWidgetsTagCloudView.h"

class dtkWidgetsTagCloudViewPrivate
{
public:
    QWidget *parent;

public:
    QEasingCurve::Type type;

public:
    bool vertical;
    bool wrap;
    bool active;

public:
    int speed;
    int now;
    int next;

public:
    QPoint pnow;

public:
    dtkWidgetsTagCloudList *list;
    dtkWidgetsTagCloudDesc *desc;
};

dtkWidgetsTagCloudView::dtkWidgetsTagCloudView(QWidget *parent) : QStackedWidget(parent), d(new dtkWidgetsTagCloudViewPrivate)
{
    d->list = new dtkWidgetsTagCloudList(this);
    d->desc = new dtkWidgetsTagCloudDesc(this);

    if (parent != 0)
        d->parent = parent;
    else
        d->parent = this;

    d->vertical = false;
    d->speed = 500;
    d->type = QEasingCurve::OutBack;
    d->now = 0;
    d->next = 0;
    d->wrap = false;
    d->pnow = QPoint(0, 0);
    d->active = false;

    this->addWidget(d->list);
    this->addWidget(d->desc);

    connect(d->list, SIGNAL(itemClicked(const QString&)), this, SLOT(onItemClicked(const QString&)));
    connect(d->desc, SIGNAL(back()), this, SLOT(slideInPrev()));
}

dtkWidgetsTagCloudView::~dtkWidgetsTagCloudView(void)
{
    delete d;

    d = NULL;
}

dtkWidgetsTagCloudList *dtkWidgetsTagCloudView::list(void)
{
    return d->list;
}

dtkWidgetsTagCloudDesc *dtkWidgetsTagCloudView::desc(void)
{
    return d->desc;
}

void dtkWidgetsTagCloudView::setDark(void)
{
    d->list->setDark();
}

void dtkWidgetsTagCloudView::setDoom(void)
{
    d->list->setDoom();
}

void dtkWidgetsTagCloudView::onItemClicked(const QString& description)
{
    d->desc->setDescription(description);

    this->slideInNext();
}

void dtkWidgetsTagCloudView::setVerticalMode(bool vertical)
{
    d->vertical = vertical;
}

void dtkWidgetsTagCloudView::setSpeed(int speed)
{
    d->speed = speed;
}

void dtkWidgetsTagCloudView::setAnimation(QEasingCurve::Type type)
{
    d->type = type;
}

void dtkWidgetsTagCloudView::setWrap(bool wrap)
{
    d->wrap = wrap;
}

void dtkWidgetsTagCloudView::slideInNext(void)
{
    int now = currentIndex();

    if (d->wrap || (now < count() - 1))
        slideInIdx(now + 1);
}

void dtkWidgetsTagCloudView::slideInPrev(void)
{
    int now = currentIndex();

    if (d->wrap || (now > 0))
        slideInIdx(now - 1);
}

void dtkWidgetsTagCloudView::slideInIdx(int idx, Direction direction)
{
    if (idx > count() - 1) {
        direction = d->vertical ? Top2Bottom : Right2Left;
        idx = (idx) % count();
    } else if (idx < 0) {
        direction =  d->vertical ? Bottom2Top : Left2Right;
        idx = (idx + count()) % count();
    }

    slideInWgt(widget ( idx ), direction);
}

void dtkWidgetsTagCloudView::slideInWgt(QWidget *newwidget, Direction direction)
{
    if (d->active)
        return;
    else
        d->active = true;

    Direction directionhint;
    int now = currentIndex();
    int next = indexOf(newwidget);

    if (now == next) {
        d->active = false;
        return;
    } else if (now < next) {
        directionhint = d->vertical ? Top2Bottom : Right2Left;
    } else {
        directionhint = d->vertical ? Bottom2Top : Left2Right;
    }

    if (direction == Automatic) {
        direction = directionhint;
    }

    int offsetx = frameRect().width();
    int offsety = frameRect().height();

    widget(next)->setGeometry ( 0,  0, offsetx, offsety );

    if (direction == Bottom2Top)  {
        offsetx = 0;
        offsety = -offsety;
    } else if (direction == Top2Bottom) {
        offsetx = 0;
    } else if (direction == Right2Left) {
        offsetx = -offsetx;
        offsety = 0;
    } else if (direction == Left2Right) {
        offsety = 0;
    }

    QPoint pnext = widget(next)->pos();
    QPoint pnow = widget(now)->pos();
    d->pnow = pnow;

    widget(next)->move(pnext.x() - offsetx, pnext.y() - offsety);
    widget(next)->show();
    widget(next)->raise();

    QPropertyAnimation *animnow = new QPropertyAnimation(widget(now), "pos");
    animnow->setDuration(d->speed);
    animnow->setEasingCurve(d->type);
    animnow->setStartValue(QPoint(pnow.x(), pnow.y()));
    animnow->setEndValue(QPoint(offsetx + pnow.x(), offsety + pnow.y()));

    QPropertyAnimation *animnext = new QPropertyAnimation(widget(next), "pos");
    animnext->setDuration(d->speed);
    animnext->setEasingCurve(d->type);
    animnext->setStartValue(QPoint(-offsetx + pnext.x(), offsety + pnext.y()));
    animnext->setEndValue(QPoint(pnext.x(), pnext.y()));

    QParallelAnimationGroup *animgroup = new QParallelAnimationGroup;
    animgroup->addAnimation(animnow);
    animgroup->addAnimation(animnext);

    QObject::connect(animgroup, SIGNAL(finished()), this, SLOT(animationDoneSlot()));

    d->next = next;
    d->now = now;
    d->active = true;

    animgroup->start();
}

void dtkWidgetsTagCloudView::animationDoneSlot(void)
{
    setCurrentIndex(d->next);

    widget(d->now)->hide();
    widget(d->now)->move(d->pnow);

    d->active = false;

    emit animationFinished();
}

//
// dtkWidgetsTagCloudView.cpp ends here
