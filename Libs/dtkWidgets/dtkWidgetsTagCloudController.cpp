// Version: $Id: b4e90837adae34b9fc78333cfc4c386482b387c3 $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "dtkWidgetsTagCloudController.h"
#include "dtkWidgetsTagCloud.h"
#include "dtkWidgetsTagCloudItem.h"
#include "dtkWidgetsTagCloudList.h"
#include "dtkWidgetsTagCloudScope.h"
#include "dtkWidgetsTagCloudView.h"

class dtkWidgetsTagCloudControllerPrivate
{
public:
    dtkWidgetsTagCloud *cloud;
    dtkWidgetsTagCloudList *list;
    dtkWidgetsTagCloudScope *scope;

public:
    QList<dtkWidgetsTag> tags;
    QList<dtkWidgetsTagCloudItem> items;

public:
    QStringList filters;

public:
    bool union_mode;
};

dtkWidgetsTagCloudController::dtkWidgetsTagCloudController(void) : QObject()
{
    d = new dtkWidgetsTagCloudControllerPrivate;
    d->list = NULL;
    d->cloud = NULL;
    d->scope = NULL;

    d->union_mode = false;
}

dtkWidgetsTagCloudController::~dtkWidgetsTagCloudController(void)
{
    delete d;
}

void dtkWidgetsTagCloudController::attach(dtkWidgetsTagCloudView *view)
{
    d->list = view->list();
}

void dtkWidgetsTagCloudController::attach(dtkWidgetsTagCloud *cloud)
{
    d->cloud = cloud;

    connect(d->cloud, SIGNAL(tagClicked(QString)), this, SLOT(addFilter(QString)));
}

void dtkWidgetsTagCloudController::attach(dtkWidgetsTagCloudScope *scope)
{
    d->scope = scope;

    connect(d->scope, SIGNAL(tagSet(QString)), this, SLOT(setFilter(QString)));
    connect(d->scope, SIGNAL(tagAdded(QString)), this, SLOT(addFilter(QString)));
    connect(d->scope, SIGNAL(tagRemoved(QString)), this, SLOT(remFilter(QString)));
    connect(d->scope, SIGNAL(cleared(void)), this, SLOT(clear()));
    connect(d->scope, SIGNAL(unionMode(bool)), this, SLOT(onUnionMode(bool)));
}

void dtkWidgetsTagCloudController::addItem(QString name)
{
    d->items << dtkWidgetsTagCloudItem(name, "", QStringList());

    this->update();
    this->render();
}

void dtkWidgetsTagCloudController::addItem(QString name, QString description)
{
    d->items << dtkWidgetsTagCloudItem(name, description, QStringList());

    this->update();
    this->render();
}

void dtkWidgetsTagCloudController::addItem(QString name, QString description, QStringList tags)
{
    d->items << dtkWidgetsTagCloudItem(name, description, tags);

    this->update();
    this->render();
}

void dtkWidgetsTagCloudController::addItem(QString name, QString description, QStringList tags,
                                           QString kind, QString type)
{
    d->items << dtkWidgetsTagCloudItem(name, description, tags, kind, type);

    this->update();
    this->render();
}

void dtkWidgetsTagCloudController::update(void)
{
    // d->tags.clear();

    // QHash<QString, QStringList> tags;

    // foreach(dtkWidgetsTagCloudItem item, d->items)
    //     foreach(QString tag, item.tags())
    //         tags[tag] << item.name();

    // foreach(QString tag, tags.keys())
    //     d->tags << dtkWidgetsTag(tag, tags[tag].count(), tags[tag]);
}

static bool intersect(QStringList l1, QStringList l2)
{
    bool pass = true;

    foreach (QString s, l1)
        if (!l2.contains(s))
            pass = false;

    return pass;
}

static bool unite(QStringList l1, QStringList l2)
{
    if (l1.isEmpty())
        return true;

    bool pass = false;

    foreach (QString s, l1)
        if (l2.contains(s))
            pass = true;

    return pass;
}

void dtkWidgetsTagCloudController::clear(void)
{
    d->filters.clear();

    this->update();
    this->render();
}

void dtkWidgetsTagCloudController::render(void)
{
    d->tags.clear();

    QHash<QString, QStringList> tags;

    foreach (dtkWidgetsTagCloudItem item, d->items)
        foreach (QString tag, item.tags())
            tags[tag] << item.name();

    foreach (QString tag, tags.keys())
        d->tags << dtkWidgetsTag(tag, tags[tag].count(), tags[tag]);

    if (d->list) {
        d->list->clear();

        foreach (dtkWidgetsTagCloudItem item, d->items)
            if ((d->union_mode && unite(d->filters, item.tags()))
                || (!d->union_mode && intersect(d->filters, item.tags())))
                d->list->addItem(item);
    }

    if (d->cloud) {
        d->cloud->clear();

        foreach (dtkWidgetsTag tag, d->tags)
            d->cloud->addTag(tag);

        d->cloud->render();
    }

    if (d->scope) {
        d->scope->clear();

        foreach (QString filter, d->filters)
            d->scope->addTag(filter, tags[filter].count());

        d->scope->render();
    }

    if (d->scope)
        d->scope->setTags(tags.keys());
}

void dtkWidgetsTagCloudController::addFilter(QString filter)
{
    d->filters << filter;

    this->render();
}

void dtkWidgetsTagCloudController::setFilter(QString filter)
{
    d->filters.clear();
    d->filters << filter;

    this->render();
}

void dtkWidgetsTagCloudController::remFilter(QString filter)
{
    d->filters.removeOne(filter);

    this->render();
}

void dtkWidgetsTagCloudController::onUnionMode(bool mode)
{
    d->union_mode = !mode;

    this->render();
}

//
// dtkWidgetsTagCloudController.cpp ends here
