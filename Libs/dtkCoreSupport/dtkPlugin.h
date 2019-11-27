/* dtkPlugin.h ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Fri Oct 31 13:49:50 2008 (+0100)
 * Version: $Id: 1a93d674b4ab30ace9bcf8e0839fbec2970b81d4 $
 * Last-Updated: mar. avril  8 10:03:49 2014 (+0200)
 *           By: Nicolas Niclausse
 *     Update #: 70
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKABSTRACTPLUGIN_H
#define DTKABSTRACTPLUGIN_H

#include "dtkAbstractObject.h"
#include <dtkCoreSupportExport.h>
#include "dtkGlobal.h"

class dtkPluginPrivate;

// /////////////////////////////////////////////////////////////////
// dtkPlugin interface
// /////////////////////////////////////////////////////////////////

class DTKCORESUPPORT_EXPORT dtkPlugin : public dtkAbstractObject
{
    Q_OBJECT

public:
    dtkPlugin(QObject *parent = 0);
    dtkPlugin(const dtkPlugin& other);
    virtual ~dtkPlugin(void);

public:
    virtual bool initialize(void) = 0;
    virtual bool uninitialize(void) = 0;

    virtual QString contact(void) const;
    virtual QString description(void) const = 0;
    virtual QString name(void) const = 0;
    virtual QString version(void) const;

    virtual QStringList authors(void) const;
    virtual QStringList contributors(void) const;
    virtual QStringList dependencies(void) const;
    virtual QStringList tags(void) const = 0;
    virtual QStringList types(void) const = 0;

private:
    DTK_DECLARE_PRIVATE(dtkPlugin);
};

Q_DECLARE_INTERFACE(dtkPlugin, "fr.inria.dtk/1.0.0")

#endif
