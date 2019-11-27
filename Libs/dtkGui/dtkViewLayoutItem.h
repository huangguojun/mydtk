/* dtkViewLayoutItem.h ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Wed May 16 09:38:35 2012 (+0200)
 * Version: $Id: 09a3d627f4f16527e883bc17a76669fd12e97835 $
 * Last-Updated: Tue Jan 21 18:12:13 2014 (+0100)
 *           By: Selim Kraria
 *     Update #: 84
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKVIEWLAYOUTITEM_H
#define DTKVIEWLAYOUTITEM_H

#include <dtkGuiSupportExport.h>

#include <QtWidgets/QFrame>

class dtkAbstractView;

class dtkViewLayout;
class dtkViewLayoutItemPrivate;

// /////////////////////////////////////////////////////////////////
// dtkViewLayoutItemProxy
// /////////////////////////////////////////////////////////////////

class dtkViewLayoutItemProxyPrivate;

class DTKGUISUPPORT_EXPORT dtkViewLayoutItemProxy : public QFrame
{
    Q_OBJECT

public:
    dtkViewLayoutItemProxy(QWidget *parent = 0);
    ~dtkViewLayoutItemProxy(void);

public:
    dtkAbstractView *view(void);

public:
    void setView(dtkAbstractView *view);

signals:
    void focusedIn(void);
    void focusedOut(void);

protected:
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);

protected slots:
    void updateLabel(void);

private:
    dtkViewLayoutItemProxyPrivate *d;
};

// /////////////////////////////////////////////////////////////////
// dtkViewLayoutItem
// /////////////////////////////////////////////////////////////////

class DTKGUISUPPORT_EXPORT dtkViewLayoutItem : public QFrame
{
    Q_OBJECT

public:
    dtkViewLayoutItem(dtkViewLayoutItem *parent = 0);
    ~dtkViewLayoutItem(void);

public:
    dtkAbstractView *view(void);

public:
    dtkViewLayoutItem *parent(void);
    dtkViewLayoutItem *first(void);
    dtkViewLayoutItem *second(void);

public:
    void setOrientation(Qt::Orientation orientation);
    void setSizes(QList<int> sizes);

public:
    int canvasHeight(void);
    int footerHeight(void);
    int handleHeight(void);
    int handleWidth(void);

public:
    dtkViewLayout *layout(void);

public:
    dtkViewLayoutItemProxy *proxy(void);

public:
    void setLayout(dtkViewLayout *layout);

public slots:
    void   clear(void);
    void   split(void);
    void unsplit(void);
    void maximize(void);

signals:
    void focused(dtkAbstractView *view);
    void unfocused(dtkAbstractView *view);

protected slots:
    void close(void);
    void horzt(void);
    void vertc(void);
    void maxmz(void);

protected slots:
    void onFocusedIn(void);
    void onFocusedOut(void);

protected:
    void dragEnterEvent(QDragEnterEvent *);
    void dragMoveEvent(QDragMoveEvent *);
    void dragLeaveEvent(QDragLeaveEvent *);
    void dropEvent(QDropEvent *);

private:
    void notify(dtkAbstractView *view);

private:
    dtkViewLayoutItemPrivate *d;

private:
    friend class dtkViewLayoutItemPrivate;
    friend class dtkViewLayoutItemProxy;
};

#endif
