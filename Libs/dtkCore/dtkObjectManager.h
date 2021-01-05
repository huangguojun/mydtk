// Version: $Id: 0eff14143f404099a7f90d6fe6985d6fd0c54b9a $
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

#include <dtkCoreExport.h>

class DTKCORE_EXPORT dtkObjectManager : public QObject
{
    Q_OBJECT

public:
    static dtkObjectManager *instance(void);

public:
    int count(void);

public slots:
    void add(const QVariant &, const QString &);
    void add(const QVariant &);

    bool remove(const QVariant &);

public:
    QVariant value(const QString &key) const;
    QStringList keys(void) const;

protected:
    static dtkObjectManager *s_instance;

private:
    dtkObjectManager(void);
    ~dtkObjectManager(void);

private:
    class dtkObjectManagerPrivate *d;
};

//
// dtkObjectManager.h ends here
