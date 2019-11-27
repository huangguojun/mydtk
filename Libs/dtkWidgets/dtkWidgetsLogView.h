/* dtkWidgetsLogView.h ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Fri Mar  2 15:08:10 2012 (+0100)
 * Version: $Id: 4b88e3e149fc7c01aff293b398dedcfd4a1673b3 $
 * Last-Updated: Mon Mar  5 14:27:11 2012 (+0100)
 *           By: Julien Wintz
 *     Update #: 9
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKWIDGETSLOGVIEW_H
#define DTKWIDGETSLOGVIEW_H

#include <dtkWidgetsExport.h>

#include <QtWidgets>

class dtkWidgetsLogViewPrivate;

class DTKWIDGETS_EXPORT dtkWidgetsLogView : public QWidget
{
    Q_OBJECT

public:
    dtkWidgetsLogView(QWidget *parent = 0);
    ~dtkWidgetsLogView(void);

protected slots:
    void autoScrollChecked(int state);
    void disableAutoScroll(void);
    void enableAutoScroll(void);

public slots:
    void displayTrace(bool display);
    void displayDebug(bool display);
    void displayInfo(bool display);
    void displayWarn(bool display);
    void displayError(bool display);
    void displayFatal(bool display);

private:
    dtkWidgetsLogViewPrivate *d;
};

#endif
