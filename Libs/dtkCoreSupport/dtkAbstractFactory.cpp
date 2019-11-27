/* dtkAbstractFactory.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Wed Nov 12 11:05:50 2008 (+0100)
 * Version: $Id: 468c26c0b64abb3a3ed761c1888d2370c0f246ce $
 * Last-Updated: Wed Sep  9 23:19:51 2009 (+0200)
 *           By: Julien Wintz
 *     Update #: 6
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include <QtCore/QtGlobal>

#include "dtkAbstractFactory.h"

dtkAbstractFactory::dtkAbstractFactory(void)
{
    connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(clear()));
}

dtkAbstractFactory::~dtkAbstractFactory(void)
{

}
