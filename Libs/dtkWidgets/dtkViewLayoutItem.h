// Version: $Id: f568a071eaddc5c75bbe81b89a9f75cf9bd2aeaa $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#pragma once

#include <dtkWidgetsExport.h>

#include <QtWidgets>

class dtkViewLayout;
class dtkViewLayoutItemPrivate;
class dtkViewWidget;

// /////////////////////////////////////////////////////////////////
// dtkViewLayoutItemProxy
// /////////////////////////////////////////////////////////////////

class dtkViewLayoutItemProxyPrivate;

class DTKWIDGETS_EXPORT dtkViewLayoutItemProxy : public QFrame
{
    Q_OBJECT

public:
    dtkViewLayoutItemProxy(QWidget *parent = 0);
    ~dtkViewLayoutItemProxy(void);

public:
    dtkViewWidget *view(void);

public:
    void setView(dtkViewWidget *view);

signals:
    void focusedIn(void);
    void focusedOut(void);

protected:
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);

private:
    dtkViewLayoutItemProxyPrivate *d;
};

// /////////////////////////////////////////////////////////////////
// dtkViewLayoutItem
// /////////////////////////////////////////////////////////////////

class DTKWIDGETS_EXPORT dtkViewLayoutItem : public QFrame
{
    Q_OBJECT

public:
    dtkViewLayoutItem(dtkViewLayoutItem *parent = 0);
    ~dtkViewLayoutItem(void);

public:
    dtkViewWidget *view(void);

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
    void clear(void);
    void split(void);
    void unsplit(void);
    void maximize(void);

signals:
    void focused(dtkViewWidget *view);
    void unfocused(dtkViewWidget *view);

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
    void mousePressEvent(QMouseEvent *);

private:
    void notify(dtkViewWidget *view);

private:
    dtkViewLayoutItemPrivate *d;

private:
    friend class dtkViewLayoutItemPrivate;
    friend class dtkViewLayoutItemProxy;
};

//
// dtkViewLayoutItem.h ends here
