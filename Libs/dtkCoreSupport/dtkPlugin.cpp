/* dtkPlugin.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Fri Oct 31 14:14:48 2008 (+0100)
 * Version: $Id: 3e570297128af195e2b1933c8246f85374a8aab0 $
 * Last-Updated: Tue Apr 24 15:02:37 2012 (+0200)
 *           By: tkloczko
 *     Update #: 90
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkPlugin.h"
#include "dtkPlugin_p.h"

// /////////////////////////////////////////////////////////////////
// dtkPlugin implementation
// /////////////////////////////////////////////////////////////////

dtkPlugin::dtkPlugin(QObject *parent) : dtkAbstractObject(*new dtkPluginPrivate(this), parent)
{

}

dtkPlugin::dtkPlugin(const dtkPlugin& other) : dtkAbstractObject(*new dtkPluginPrivate(*other.d_func()), other)
{

}

dtkPlugin::~dtkPlugin(void)
{

}

QString dtkPlugin::contact(void) const
{
    return QString();
}

QString dtkPlugin::version(void) const
{
    return QString();
}

QStringList dtkPlugin::authors(void) const
{
    return QStringList();
}

QStringList dtkPlugin::contributors(void) const
{
    return QStringList();
}

QStringList dtkPlugin::dependencies(void) const
{
    return QStringList();
}
