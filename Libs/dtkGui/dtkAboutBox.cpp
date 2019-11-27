/* dtkAboutBox.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Mon Apr 19 12:52:49 2010 (+0200)
 * Version: $Id: 2c11d26bc48d65396c9606b5ceec6dcf73b87335 $
 * Last-Updated: Mon Apr 19 13:12:04 2010 (+0200)
 *           By: Julien Wintz
 *     Update #: 8
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkAboutBox.h"

class dtkAboutBoxPrivate
{
public:
};

dtkAboutBox::dtkAboutBox(QWidget *parent) : QMessageBox(parent), d(new dtkAboutBoxPrivate)
{
    this->setIconPixmap(QPixmap(":dtkGui/pixmaps/dtk.png").scaledToHeight(150));
}

dtkAboutBox::~dtkAboutBox(void)
{
    delete d;

    d = NULL;
}
