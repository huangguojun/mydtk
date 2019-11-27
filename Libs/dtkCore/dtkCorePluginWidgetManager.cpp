// Version: $Id: 569d0c31e2360dbf4d47bc680468d748bd7c2079 $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "dtkCorePluginWidgetManager.h"
#include <dtkLog>

// ///////////////////////////////////////////////////////////////////

class dtkCorePluginWidgetManagerPrivate
{
public:
    QHash<const void *, QWidget *> widgets;
    QHash<const void *, QVariant> variants;
};

// ///////////////////////////////////////////////////////////////////


dtkCorePluginWidgetManager& dtkCorePluginWidgetManager::instance(void)
{
    if(!s_instance) {
        s_instance = new dtkCorePluginWidgetManager;
    }
    return *s_instance;
}

void dtkCorePluginWidgetManager::add(const void *ptr, const QVariant& v, QWidget *w)
{
    d->widgets.insert(ptr, w);
    d->variants.insert(ptr, v);

    emit added(v, w);
}

int dtkCorePluginWidgetManager::remove(const void *ptr)
{
    int n_w = d->widgets.remove(ptr);
    int n_v = d->variants.remove(ptr);
    if (n_v != n_w) {
        dtkWarn() << Q_FUNC_INFO << "Not the same number of variants and objects removed from tables !" << n_w << n_v;
    }
    return n_v;
}

int dtkCorePluginWidgetManager::remove(QWidget *w)
{
    QList<const void *> keys;

    for (auto it = d->widgets.begin(), end = d->widgets.end(); it != end; ++it) {
        if (*it == w) {
            keys << it.key();
        }
    }

    for (auto&& k : keys) {
        d->widgets.remove(k);
        d->variants.remove(k);
    }
    return keys.size();
}

void dtkCorePluginWidgetManager::clear(void)
{
    d->widgets.clear();
    d->variants.clear();
}

QWidget *dtkCorePluginWidgetManager::widget(const void *ptr) const
{
    const QWidget *w = d->widgets.value(ptr, nullptr);
    if (w) {
        return const_cast<QWidget *>(w);
    }
    return nullptr;
}

QWidget *dtkCorePluginWidgetManager::take(const void *ptr) const
{
    d->variants.remove(ptr);
    return d->widgets.take(ptr);
}

QString dtkCorePluginWidgetManager::description(void) const
{
    QString msg("dtkCorePluginWidgetManager contains : \n");
    QString content("[ ");
    int i = 0;
    int e = d->widgets.count()-1;
    auto it = d->widgets.cbegin();
    auto end = d->widgets.cend();
    auto vit = d->variants.cbegin();
    for (;it != end; ++it, ++i, ++vit) {
        content += vit.value().typeName() + QStringLiteral(" : ") + reinterpret_cast<const QObject *>(it.value())->objectName();
        if (i < e) {
            content += "\n";
        } else {
            content += " ]";
        }
    }
    return (msg + content);
}

dtkCorePluginWidgetManager::dtkCorePluginWidgetManager(void) : d(new dtkCorePluginWidgetManagerPrivate)
{

}

dtkCorePluginWidgetManager::~dtkCorePluginWidgetManager(void)
{
    delete d;
}

QDebug operator<<(QDebug dbg, dtkCorePluginWidgetManager& m)
{
    dbg.nospace() << m.description();

    return dbg.space();
}

dtkCorePluginWidgetManager *dtkCorePluginWidgetManager::s_instance = nullptr;

//
// dtkCorePluginWidgetManager.cpp ends here
