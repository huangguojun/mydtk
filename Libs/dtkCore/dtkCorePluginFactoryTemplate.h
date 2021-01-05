// Version: $Id: 62819249b3a497f2a4ce5a076c63f24c80b7e23f $
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

#include <QtCore>

#include <dtkConfig.h>

#include <typeinfo>

#if defined(COMPILER_SUPPORTS_CXX11)
#    include <typeindex>
#endif

// ///////////////////////////////////////////////////////////////////
// dtkCorePluginFactoryTemplate
// ///////////////////////////////////////////////////////////////////

class dtkCorePluginFactoryTemplate : public QObject
{
public:
    dtkCorePluginFactoryTemplate(void) : QObject() { ; }
    ~dtkCorePluginFactoryTemplate(void) { ; }

public:
    template<typename FactoryType>
    FactoryType *pluginFactory(void);

private:
#if defined(COMPILER_SUPPORTS_CXX11)
    QMap<std::type_index, void *> m_factories;
#else
    QMap<QString, void *> m_factories;
#endif
};

// ///////////////////////////////////////////////////////////////////

template<typename FactoryType>
inline FactoryType *dtkCorePluginFactoryTemplate::pluginFactory(void)
{
#if defined(COMPILER_SUPPORTS_CXX11)
    std::type_index index(typeid(FactoryType));

    if (!m_factories.contains(index)) {
        FactoryType *factory = new FactoryType;
        m_factories.insert(index, factory);
    }

    return static_cast<FactoryType *>(m_factories[index]);

#else
    QString factoryTypeName = QString(typeid(FactoryType).name());

    if (!m_factories.contains(factoryTypeName)) {
        FactoryType *factory = new FactoryType;
        m_factories.insert(factoryTypeName, factory);
    }

    return static_cast<FactoryType *>(m_factories[factoryTypeName]);
#endif
}

//
// dtkCorePluginFactoryTemplate.h ends here
