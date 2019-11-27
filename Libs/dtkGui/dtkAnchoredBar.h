/* dtkAnchoredBar.h ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Tue Mar 16 08:43:54 2010 (+0100)
 * Version: $Id: 6bfa0f4c205328b207732bde52290aafa68028cc $
 * Last-Updated: Wed Mar 21 11:02:13 2012 (+0100)
 *           By: Julien Wintz
 *     Update #: 9
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKANCHOREDBAR_H
#define DTKANCHOREDBAR_H

#include <QtWidgets>

#include <dtkGuiSupportExport.h>

class dtkAnchoredBarPrivate;

class DTKGUISUPPORT_EXPORT dtkAnchoredBar : public QToolBar
{
    Q_OBJECT

public:
    dtkAnchoredBar(QWidget *parent = 0);

public:
    QSize sizeHint(void) const;

public slots:
    void setDragEnabled(bool enabled);

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    dtkAnchoredBarPrivate *d;
};

#endif
