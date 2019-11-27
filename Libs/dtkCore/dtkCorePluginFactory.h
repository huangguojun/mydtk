// Version: $Id: 70a792a877c475879911833bb61d51ea5b4eb773 $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#pragma once

#include <dtkCoreExport.h>

#include <QtCore>

#include <functional>

class QWidget;

// ///////////////////////////////////////////////////////////////////
// dtkCorePluginFactoryBase
// ///////////////////////////////////////////////////////////////////

class dtkObjectManager;

class DTKCORE_EXPORT dtkCorePluginFactoryBase
{
public:
    virtual ~dtkCorePluginFactoryBase(void) = default;

    virtual void    connect(dtkObjectManager *manager) const = 0;
    virtual void disconnect(dtkObjectManager *manager) const = 0;
};

// ///////////////////////////////////////////////////////////////////
// dtkCorePluginFactory
// ///////////////////////////////////////////////////////////////////

class dtkCorePluginBase;

template <typename T> class dtkCorePluginFactory : public dtkCorePluginFactoryBase
{
public:
     dtkCorePluginFactory(void);
    ~dtkCorePluginFactory(void);

public:
    typedef T *(*creator) ();
    typedef std::function<QWidget *()> widget_creator;

public:
    void record(const QString& key, creator func);
    void recordPlugin(const QString& key, dtkCorePluginBase *plugin, bool force = false);
    void record(const QString& key, widget_creator func);

public:
    void clear(void);

public:
    T *create(const QString& key) const;

    QWidget *createWidget(const QString& key) const;

public:
    QStringList keys(void) const;

public:
    void    connect(dtkObjectManager *manager) const override;
    void disconnect(dtkObjectManager *manager) const override;

private:
    QHash<QString, creator> creators;
    QHash<QString, dtkCorePluginBase *> creators_plugins;
    QHash<QString, widget_creator> widget_creators;

private:
    class dtkCorePluginFactoryPrivate *m_d;
};

// ///////////////////////////////////////////////////////////////////
// Helper functions
// ///////////////////////////////////////////////////////////////////

template <typename S, typename T> S *dtkCorePluginFactoryCreator(void)
{
    return new T;
}

#include "dtkCorePluginFactory.tpp"

//
// dtkCorePluginFactory.h ends here
