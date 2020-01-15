// Version: $Id: 3e6d7a9fe2882ae8c2c0f403adf5b5e559e2a702 $
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

#include <dtkWidgetsExport>

#include <QtWidgets/QFrame>

#include <QtCore>

class dtkWidgetsLayout;
class dtkWidgetsLayoutItemPrivate;
class dtkWidgetsWidget;

// /////////////////////////////////////////////////////////////////
// dtkWidgetsLayoutItemProxy
// /////////////////////////////////////////////////////////////////

class DTKWIDGETS_EXPORT dtkWidgetsLayoutItemProxy : public QFrame
{
    Q_OBJECT

public:
     dtkWidgetsLayoutItemProxy(QWidget *parent = nullptr);
    ~dtkWidgetsLayoutItemProxy(void);

signals:
    void create(void);
    void create(const QString&);

public:
    dtkWidgetsWidget *view(void);

public:
    void setView(dtkWidgetsWidget *view);

signals:
    void focusedIn(void);
    void focusedOut(void);

protected:
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);

protected:
    void keyPressEvent(QKeyEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);

protected:
    void paintEvent(QPaintEvent *);

private:
    class dtkWidgetsLayoutItemProxyPrivate *d;

private:
    friend class dtkWidgetsLayoutReader;
    friend class dtkWidgetsLayoutWriter;
};

// /////////////////////////////////////////////////////////////////
// dtkWidgetsLayoutItem
// /////////////////////////////////////////////////////////////////

class DTKWIDGETS_EXPORT dtkWidgetsLayoutItem : public QFrame
{
    Q_OBJECT

public:
    using Actions = QMap<QString, QString>;

public:
     dtkWidgetsLayoutItem(dtkWidgetsLayoutItem *parent = 0);
    ~dtkWidgetsLayoutItem(void);

signals:
    void create(void);
    void create(const QString&);

public:
    dtkWidgetsWidget *view(void);

public:
    dtkWidgetsLayoutItem *parent(void);
    dtkWidgetsLayoutItem *first(void);
    dtkWidgetsLayoutItem *second(void);

public:
    void setOrientation(Qt::Orientation orientation);
    void setSizes(const QList<int>& sizes);

public:
    int canvasHeight(void);
    int footerHeight(void);
    int handleHeight(void);
    int handleWidth(void);

public:
    dtkWidgetsLayout *layout(void) const;

public:
    dtkWidgetsLayoutItemProxy *proxy(void);

public:
    void setLayout(dtkWidgetsLayout *layout);

public slots:
    void   clear(void);
    void   split(bool force = false);
    void unsplit(void);
    void maximize(void);

public:
    QFrame *footer(void);

public:
    void setView(dtkWidgetsWidget *view);

public:
    static void setActions(const Actions& actions);

signals:
    void focused(dtkWidgetsWidget *view);
    void unfocused(dtkWidgetsWidget *view);

protected:
    void enterEvent(QEvent *event);

protected slots:
    void close(void);
    void horzt(void);
    void vertc(void);
    void maxmz(void);

protected slots:
    void onFocusedIn(void);
    void onFocusedOut(void);

protected slots:
    void onActivated(const QString);

protected:
    void dragEnterEvent(QDragEnterEvent *);
    void dragMoveEvent(QDragMoveEvent *);
    void dragLeaveEvent(QDragLeaveEvent *);
    void dropEvent(QDropEvent *);
    void keyPressEvent(QKeyEvent *);
    void mousePressEvent(QMouseEvent *);

private:
    static Actions actions;

private:
    class dtkWidgetsLayoutItemPrivate *d;

private:
    friend class dtkWidgetsLayoutItemPrivate;
    friend class dtkWidgetsLayoutItemProxy;
    friend class dtkWidgetsLayoutReader;
    friend class dtkWidgetsLayoutWriter;
};

//
// dtkWidgetsLayoutItem.h ends here
