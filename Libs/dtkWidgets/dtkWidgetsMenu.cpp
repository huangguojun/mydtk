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

#include "dtkWidgetsMenu.h"
#include "dtkWidgetsMenu+ux.h"
#include "dtkWidgetsMenuItem.h"

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsMenuPrivate
// ///////////////////////////////////////////////////////////////////

class dtkWidgetsMenuPrivate
{
public:
    QHash<QString, QObject *> object_hash;
    QVector<QObject *> object_list;
    QVector<dtkWidgetsMenu *> menu_list;

public:
    QString title;

public:
    int icon_id;
};

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsMenu
// ///////////////////////////////////////////////////////////////////

dtkWidgetsMenu::dtkWidgetsMenu(int icon_id, const QString &title, QObject *parent)
    : QObject(parent), d(new dtkWidgetsMenuPrivate)
{
    d->title = title;
    d->icon_id = icon_id;
}

dtkWidgetsMenu::~dtkWidgetsMenu(void)
{
    this->clear();

    delete d;
}

dtkWidgetsMenu *dtkWidgetsMenu::addMenu(dtkWidgetsMenu *menu)
{
    if (menu && !d->object_hash.contains(menu->title())) {
        d->object_hash.insert(menu->title(), menu);
        d->object_list << menu;
        d->menu_list << menu;
        return menu;
    }

    return nullptr;
}

dtkWidgetsMenu *dtkWidgetsMenu::addMenu(int icon_id, const QString &title)
{
    dtkWidgetsMenu *menu = new dtkWidgetsMenu(icon_id, title);

    auto m = this->addMenu(menu);

    if (m != menu) {
        delete menu;
    }

    return m;
}

dtkWidgetsMenu *dtkWidgetsMenu::insertMenu(int pos, dtkWidgetsMenu *menu)
{
    if (pos < 0 || pos > d->menu_list.size() || !menu) {
        return nullptr;
    }

    menu->setParent(this);

    d->object_hash.insert(menu->title(), menu);
    d->menu_list.insert(pos, menu);
    d->object_list.insert(pos, menu);
    return menu;
}

dtkWidgetsMenu *dtkWidgetsMenu::insertMenu(int pos, int icon_id, const QString &title)
{
    dtkWidgetsMenu *menu = new dtkWidgetsMenu(icon_id, title);

    auto m = this->insertMenu(pos, menu);

    if (m != menu)
        delete menu;

    return m;
}

void dtkWidgetsMenu::clear(void)
{
    d->object_hash.clear();
    d->object_list.clear();
    d->menu_list.clear();
}

dtkWidgetsMenuItem *dtkWidgetsMenu::addItem(dtkWidgetsMenuItem *item)
{
    if (item && !d->object_hash.contains(item->title())) {
        d->object_hash.insert(item->title(), item);
        d->object_list << item;
        return item;
    }

    return nullptr;
}

dtkWidgetsMenuItem *dtkWidgetsMenu::addItem(int icon_id, const QString &title)
{
    dtkWidgetsMenuItem *item = new dtkWidgetsMenuItem(icon_id, title);

    auto it = this->addItem(item);

    if (it != item) {
        delete item;
    }

    return it;
}

dtkWidgetsMenuItem *dtkWidgetsMenu::insertItem(int pos, dtkWidgetsMenuItem *item)
{
    if (item && !d->object_hash.contains(item->title())) {
        d->object_hash.insert(item->title(), item);
        d->object_list.insert(pos, item);
        return item;
    }

    return nullptr;
}

dtkWidgetsMenuItem *dtkWidgetsMenu::insertItem(int pos, int icon_id, const QString &title)
{
    dtkWidgetsMenuItem *item = new dtkWidgetsMenuItem(icon_id, title);

    auto it = this->insertItem(pos, item);

    if (it != item) {
        delete item;
    }

    return it;
}

dtkWidgetsMenuItem *dtkWidgetsMenu::addSeparator(void)
{
    static int count = 0;

    QString title = QString("separator_%1").arg(count++);

    dtkWidgetsMenuItem *separator = new dtkWidgetsMenuItem(title, this);
    separator->setSeparator(true);

    d->object_hash.insert(title, separator);
    d->object_list << separator;

    return separator;
}

dtkWidgetsMenuItem *dtkWidgetsMenu::addParameter(dtkWidgetsMenuItem *item)
{
    return this->addItem(item);
}

dtkWidgetsMenuItem *dtkWidgetsMenu::addParameter(const QString &title, dtkCoreParameter *parameter,
                                                 const QString &representation)
{
    dtkWidgetsMenuItem *item = new dtkWidgetsMenuItemParameter(title, parameter, representation);

    auto it = this->addItem(item);

    if (it != item) {
        delete item;
    }

    return it;
}

void dtkWidgetsMenu::removeItem(dtkWidgetsMenuItem *item)
{
    if (!(item && d->object_hash.contains(item->title())))
        return;

    d->object_hash.remove(item->title());
    d->object_list.removeAll(item);
}

void dtkWidgetsMenu::removeMenu(dtkWidgetsMenu *menu)
{
    if (menu && d->object_hash.contains(menu->title())) {
        d->object_hash.remove(menu->title());
        d->object_list.removeAll(menu);
        d->menu_list.removeAll(menu);
    }
}

QString dtkWidgetsMenu::title(void) const
{
    return d->title;
}

QVector<dtkWidgetsMenu *> dtkWidgetsMenu::menus(void) const
{
    return d->menu_list;
}

dtkWidgetsMenu *dtkWidgetsMenu::menu(const QString &id) const
{
    if (!d->object_hash.contains(id))
        return nullptr;

    return dynamic_cast<dtkWidgetsMenu *>(d->object_hash[id]);
}

QObject *dtkWidgetsMenu::object(const QString &id)
{
    if (!d->object_hash.contains(id))
        return nullptr;

    return d->object_hash[id];
}

QVector<QObject *> dtkWidgetsMenu::objects(void) const
{
    return d->object_list;
}

int dtkWidgetsMenu::icon(void) const
{
    return d->icon_id;
}

//
// dtkWidgetsMenu.cpp ends here
