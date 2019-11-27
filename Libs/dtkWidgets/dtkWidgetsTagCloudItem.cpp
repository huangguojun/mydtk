// Version: $Id: 5e25db8dc81092ec13d3cbdd2f1937b30c173ca3 $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "dtkWidgetsTagCloudItem.h"

class dtkWidgetsTagCloudItemPrivate
{
public:
    QString name;
    QString description;
    QStringList tags;
    QString kind;
    QString type;
};

dtkWidgetsTagCloudItem::dtkWidgetsTagCloudItem(QString name) : QListWidgetItem(name)
{
    d = new dtkWidgetsTagCloudItemPrivate;
    d->name = name;
}

dtkWidgetsTagCloudItem::dtkWidgetsTagCloudItem(QString name, QString description) : QListWidgetItem(name)
{
    d = new dtkWidgetsTagCloudItemPrivate;
    d->name = name;
    d->description = description;
}

dtkWidgetsTagCloudItem::dtkWidgetsTagCloudItem(QString name, QString description, QStringList tags) : QListWidgetItem(name)
{
    d = new dtkWidgetsTagCloudItemPrivate;
    d->name = name;
    d->description = description;
    d->tags = tags;
}

dtkWidgetsTagCloudItem::dtkWidgetsTagCloudItem(QString name, QString description, QStringList tags, QString kind, QString type) : QListWidgetItem(name)
{
    d = new dtkWidgetsTagCloudItemPrivate;
    d->name = name;
    d->description = description;
    d->tags = tags;
    d->kind = kind;
    d->type = type;
}

dtkWidgetsTagCloudItem::dtkWidgetsTagCloudItem(const dtkWidgetsTagCloudItem& item) : QListWidgetItem(item.name())
{
    d = new dtkWidgetsTagCloudItemPrivate;
    d->name = item.d->name;
    d->description = item.d->description;
    d->tags = item.d->tags;
    d->kind = item.d->kind;
    d->type = item.d->type;
}

dtkWidgetsTagCloudItem::~dtkWidgetsTagCloudItem(void)
{
    delete d;
}

QString dtkWidgetsTagCloudItem::name(void) const
{
    return d->name;
}

QString dtkWidgetsTagCloudItem::description(void) const
{
    return d->description;
}

QStringList dtkWidgetsTagCloudItem::tags(void) const
{
    return d->tags;
}

QString dtkWidgetsTagCloudItem::kind(void) const
{
    return d->kind;
}

QString dtkWidgetsTagCloudItem::type(void) const
{
    return d->type;
}

//
// dtkWidgetsTagCloudItem.cpp ends here
