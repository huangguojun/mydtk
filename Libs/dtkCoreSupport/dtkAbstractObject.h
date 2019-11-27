/* dtkAbstractObject.h ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Sat Feb 28 17:43:14 2009 (+0100)
 * Version: $Id: b3a863e8dc026395afc89ac54b74da401faa5604 $
 * Last-Updated: lun. févr.  3 12:46:03 2014 (+0100)
 *           By: Nicolas Niclausse
 *     Update #: 128
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKABSTRACTOBJECT_H
#define DTKABSTRACTOBJECT_H

#include <dtkCoreSupportExport.h>
#include "dtkGlobal.h"

#include <typeinfo>

class dtkAbstractObjectPrivate;

class DTKCORESUPPORT_EXPORT dtkAbstractObject : public QObject
{
    Q_OBJECT

public:
    dtkAbstractObject(dtkAbstractObject *parent = 0);
    dtkAbstractObject(const dtkAbstractObject& other);
    virtual ~dtkAbstractObject(void);

public:
    virtual dtkAbstractObject *clone(void);

public:
    dtkAbstractObject& operator = (const dtkAbstractObject& other);

protected:
    virtual void copy(const dtkAbstractObject& other);

public:
    bool operator == (const dtkAbstractObject& other) const;

protected:
    virtual bool isEqual(const dtkAbstractObject& other) const;

public:
    virtual QString description(void) const;
    virtual QString identifier(void) const;
    virtual QString name(void) const;

    int count(void) const;
    int retain(void) const;
    int release(void) const;

    void enableDeferredDeletion(bool value);
    bool     isDeferredDeletionEnabled(void) const;

    bool hasProperty(const QString& key) const;
    void addProperty(const QString& key, const QStringList& values);
    void addProperty(const QString& key, const QString& value);
    void setProperty(const QString& key, const QString& value);
    QStringList propertyList(void) const;
    QStringList propertyValues(const QString& key) const;
    QString property(const QString& key) const;

    bool hasMetaData(const QString& key) const;
    void addMetaData(const QString& key, const QStringList& values);
    void addMetaData(const QString& key, const QString& value);
    void setMetaData(const QString& key, const QStringList& values);
    void setMetaData(const QString& key, const QString& value);
    QStringList metaDataList(void) const;
    QStringList metaDataValues(const QString& key) const;
    QString metadata(const QString& key) const;
    QStringList metadatas(const QString& key) const;
    void copyMetaDataFrom(const dtkAbstractObject *obj);

signals:
    void propertySet(const QString& key, const QString& value);
    void metaDataSet(const QString& key, const QString& value);

public slots:
    virtual void onPropertySet(const QString& key, const QString& value);
    virtual void onMetaDataSet(const QString& key, const QString& value);

protected:
    dtkAbstractObject(dtkAbstractObjectPrivate& dd, QObject *parent);
    dtkAbstractObject(dtkAbstractObjectPrivate& dd, const dtkAbstractObject& other);

protected:
    dtkAbstractObjectPrivate *d_func(void)       {
        return d_ptr;
    }
    const dtkAbstractObjectPrivate *d_func(void) const {
        return d_ptr;
    }

private:
    dtkAbstractObjectPrivate *d_ptr;

private:
    friend class dtkAbstractObjectPrivate;
};

Q_DECLARE_METATYPE(dtkAbstractObject);
Q_DECLARE_METATYPE(dtkAbstractObject *);

#endif
