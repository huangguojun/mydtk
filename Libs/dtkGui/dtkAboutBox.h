/* dtkAboutBox.h ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Mon Apr 19 12:51:22 2010 (+0200)
 * Version: $Id: f73e29983ba9a771869bdd6f6d7f04f757f0a74b $
 * Last-Updated: lun. févr.  3 14:01:25 2014 (+0100)
 *           By: Thibaud Kloczko
 *     Update #: 6
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKABOUTBOX_H
#define DTKABOUTBOX_H

#include <dtkGuiSupportExport.h>

#include <QtWidgets>

class dtkAboutBoxPrivate;

class DTKGUISUPPORT_EXPORT dtkAboutBox : public QMessageBox
{
    Q_OBJECT

public:
    dtkAboutBox(QWidget *parent = 0);
    ~dtkAboutBox(void);

private:
    dtkAboutBoxPrivate *d;
};

#endif
