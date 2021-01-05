// Version: $Id: 981d3568e2a3f202f67bc0267e8a1f6a077fc268 $
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

class dtkObjectManager;

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class DTKCORE_EXPORT dtkCorePluginFactoryPrivate : public QObject
{
    Q_OBJECT

signals:
    void created(const QVariant &) const;

public:
    void touch(const QVariant &) const;

public:
    void connect(dtkObjectManager *manager) const;
    void disconnect(dtkObjectManager *manager) const;
};

//
// dtkCorePluginFactory_p.h ends here
