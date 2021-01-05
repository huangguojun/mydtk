/* dtkUpdater.h ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Mon Jul 20 11:15:03 2009 (+0200)
 * Version: $Id: 56c29ffaf434eead60877ee54cf3cb583188e8c6 $
 * Last-Updated: Mon Feb 15 20:06:19 2010 (+0100)
 *           By: Julien Wintz
 *     Update #: 3
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKUPDATER_H
#define DTKUPDATER_H

#include <QtCore>

#include <dtkCoreSupportExport.h>

class dtkUpdaterPrivate;

class DTKCORESUPPORT_EXPORT dtkUpdater
{
public:
    dtkUpdater(const QUrl &url);
    ~dtkUpdater(void);

private:
    dtkUpdaterPrivate *d;
};

#endif // DTKUPDATER_H
