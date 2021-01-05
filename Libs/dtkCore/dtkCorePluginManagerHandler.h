/* @(#)dtkCorePluginManagerHandler.h ---
 *
 * Author: Nicolas Niclausse
 * Copyright (C) 2016 - Nicolas Niclausse, Inria.
 * Created: 2016/07/04 14:04:27
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#pragma once

#include <dtkCoreExport.h>

#include <QtCore>

class dtkCorePluginManagerHandlerPrivate;

// /////////////////////////////////////////////////////////////////
// dtkCorePluginManagerHandler interface
// /////////////////////////////////////////////////////////////////

class DTKCORE_EXPORT dtkCorePluginManagerHandler
{
public:
    static dtkCorePluginManagerHandler *instance(void);

public:
    QString pluginPath(const QString &name) const;

    QString name(const QString &path) const;
    QString version(const QString &path) const;
    QString concept(const QString &path) const;

public:
    QString pluginsLayerVersion(const QString &path) const;
    QVariantList dependencies(const QString &path) const;

public:
    bool hasName(const QString &name) const;
    bool hasVersion(const QString &version) const;
    bool hasConcept(const QString &concept) const;

public:
    void scan(const QString &path, bool verboseLoading = false);

public:
    QHash<QString, QString>::const_iterator namesBegin(void) const;
    QHash<QString, QString>::const_iterator namesEnd(void) const;

    QHash<QString, QString>::const_iterator versionsBegin(void) const;
    QHash<QString, QString>::const_iterator versionsEnd(void) const;

    QHash<QString, QString>::const_iterator conceptsBegin(void) const;
    QHash<QString, QString>::const_iterator conceptsEnd(void) const;

public:
    QStringList pluginPaths(void) const;

    QStringList names(void) const;
    QStringList versions(void) const;
    QStringList concepts(void) const;

private:
    dtkCorePluginManagerHandler(void);

private:
    static dtkCorePluginManagerHandler *s_instance;

private:
    dtkCorePluginManagerHandlerPrivate *d;
};
