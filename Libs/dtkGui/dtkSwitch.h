/* dtkSwitch.h ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Wed Feb 29 23:53:52 2012 (+0100)
 * Version: $Id: 05d7e228d0cda48d3cf0ae016d5bba74f0fced87 $
 * Last-Updated: lun. févr.  3 14:57:15 2014 (+0100)
 *           By: Thibaud Kloczko
 *     Update #: 14
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKSWITCH_H
#define DTKSWITCH_H

#include <QQuickView>
#include <QtWidgets>

class dtkSwitchPrivate;

class dtkSwitch : public QQuickView
{
    Q_OBJECT

public:
    dtkSwitch(QWindow *parent = 0);
    ~dtkSwitch(void);

signals:
    void toggled(bool);

public:
    Q_INVOKABLE QString foreground(void);
    Q_INVOKABLE QString background(void);

public:
    Q_INVOKABLE QString leftLabel(void);
    Q_INVOKABLE QString rightLabel(void);

public slots:
    void toggle(void);

public slots:
    void setForeground(const QString &pixmap);
    void setBackground(const QString &pixmap);

public slots:
    void setLeftLabel(const QString &label);
    void setRightLabel(const QString &label);

private:
    dtkSwitchPrivate *d;
};

#endif // DTKSWITCH_H
