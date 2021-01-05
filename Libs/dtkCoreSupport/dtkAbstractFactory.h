/* dtkAbstractFactory.h ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Wed Nov 12 11:03:31 2008 (+0100)
 * Version: $Id: 0b3dd6b0e48a3c4a7664caca7f3a3f6bc0d196e1 $
 * Last-Updated: Mon Apr 16 10:40:21 2012 (+0200)
 *           By: Julien Wintz
 *     Update #: 13
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKABSTRACTFACTORY_H
#define DTKABSTRACTFACTORY_H

#include <QtCore/QObject>

#include "dtkGlobal.h"
#include <dtkCoreSupportExport.h>

class DTKCORESUPPORT_EXPORT dtkAbstractFactory : public QObject
{
    Q_OBJECT

public:
    dtkAbstractFactory(void);
    ~dtkAbstractFactory(void);

private slots:
    void clear(void) { delete this; }
};

#endif
