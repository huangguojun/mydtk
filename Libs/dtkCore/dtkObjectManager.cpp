// Version: $Id: 2a58853df1e996e5d820bb86aaf8742cb9f3c81a $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "dtkObjectManager.h"

class dtkObjectManagerPrivate
{
public:
    QVariantMap instances;
};

dtkObjectManager *dtkObjectManager::instance(void)
{
    if(!s_instance)
        s_instance = new dtkObjectManager;

    return s_instance;
}

int dtkObjectManager::count(void)
{
    return d->instances.count();
}

void dtkObjectManager::add(const QVariant& instance, const QString& name)
{
    d->instances.insert(name, instance);
}

void dtkObjectManager::add(const QVariant& instance)
{
    static int count = 0;

    d->instances.insert(QString("%1 %2").arg(instance.typeName()).arg(count++), instance);
}

QVariant dtkObjectManager::value(const QString& key) const
{
    return d->instances.value(key);
}

QStringList dtkObjectManager::keys(void) const
{
    return d->instances.keys();
}

bool dtkObjectManager::remove(const QVariant& v)
{
    QList<QString> keys;

    for (auto it = d->instances.begin(), end = d->instances.end(); it != end; ++it) {
        if (*it == v) {
            keys << it.key();
        }
    }

    for (auto&& k : keys) {
        d->instances.remove(k);
    }

    return (keys.size());
}

dtkObjectManager::dtkObjectManager(void)
{
    d = new dtkObjectManagerPrivate;
}

dtkObjectManager::~dtkObjectManager(void)
{
    delete d;
}

dtkObjectManager *dtkObjectManager::s_instance = Q_NULLPTR;

//
// dtkObjectManager.cpp ends here
