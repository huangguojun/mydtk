/* sislMainWindow.h ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Sat Jun  2 01:15:59 2012 (+0200)
 * Version: $Id$
 * Last-Updated: Sat Jun  2 01:16:46 2012 (+0200)
 *           By: Julien Wintz
 *     Update #: 5
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef CCMAINWINDOW_H
#define CCMAINWINDOW_H

//#include <dtkComposer/dtkComposerWriter.h>

#include "ccMainWindow_p.h"
#include <QtGui>
#include <QMainWindow>
class ccMainWindowPrivate;

class ccMainWindow : public QMainWindow
{
    Q_OBJECT

public:
     ccMainWindow(QWidget *parent = 0);


private:
    ccMainWindowPrivate *d;
};

#endif
