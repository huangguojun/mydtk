/* dtkTagCloud.h ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Sun May  3 10:42:01 2009 (+0200)
 * Version: $Id: 6100e425441a2d9893e28d5f0e4336ba4eb76b5e $
 * Last-Updated: Tue Sep 17 14:00:54 2013 (+0200)
 *           By: Julien Wintz
 *     Update #: 235
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKTAGCLOUD_H
#define DTKTAGCLOUD_H

#include <dtkGuiSupportExport.h>

#include <QtWidgets>

// /////////////////////////////////////////////////////////////////
// dtkTag
// /////////////////////////////////////////////////////////////////

class dtkTagPrivate;

class DTKGUISUPPORT_EXPORT dtkTag
{
public:
    dtkTag(void);
    dtkTag(QString text, int instances);
    dtkTag(QString text, int instances, QStringList items);
    dtkTag(QString text, int instances, QStringList items, QString color);
    dtkTag(const dtkTag &other);
    ~dtkTag(void);

    int count(void) const;
    QString text(void) const;
    QString color(void) const;
    QStringList items(void) const;

    void setCount(int count);
    void setText(QString text);
    void setColor(QString color);
    void setItems(QStringList items);

protected:
    dtkTagPrivate *d;
};

// /////////////////////////////////////////////////////////////////
// dtkTagCloud
// /////////////////////////////////////////////////////////////////

class dtkTagCloudPrivate;

class DTKGUISUPPORT_EXPORT dtkTagCloud : public QTextBrowser
{
    Q_OBJECT

public:
    dtkTagCloud(QWidget *parent = 0);
    ~dtkTagCloud(void);

    enum SortingType { Alpha, Num };

    enum SortingOrder { Asc, Desc };

    void addTag(QString text, int instances);
    void addTag(QString text, int instances, QStringList items);
    void addTag(QString text, int instances, QStringList items, QString color);
    void addTag(dtkTag tag);

    void setFontSize(int size);
    void setFontRange(int range);

    void setSortingType(SortingType type);
    void setSortingOrder(SortingOrder order);

signals:
    void tagClicked(QString tag);
    void tagClicked(QString tag, QStringList items);

public slots:
    void sort(void);
    void clear(void);
    void render(void);

protected slots:
    void onLinkClicked(const QUrl &item);

protected:
    dtkTagCloudPrivate *d;
};

// /////////////////////////////////////////////////////////////////
// dtkTagScopeTag
// /////////////////////////////////////////////////////////////////

class dtkTagScopeTagPrivate;

class dtkTagScopeTag : public QWidget
{
    Q_OBJECT

public:
    dtkTagScopeTag(QWidget *parent = 0);
    ~dtkTagScopeTag(void);

signals:
    void clicked(void);

public:
    QSize sizeHint(void) const;

public:
    QString text(void);

public:
    void setBlue(void);
    void setDark(void);

public slots:
    void setText(const QString &text);
    void setCount(int count);

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

protected:
    void mouseReleaseEvent(QMouseEvent *);

protected:
    dtkTagScopeTagPrivate *d;
};

// /////////////////////////////////////////////////////////////////
// dtkTagScope
// /////////////////////////////////////////////////////////////////

class dtkTagScopePrivate;

class DTKGUISUPPORT_EXPORT dtkTagScope : public QFrame
{
    Q_OBJECT

public:
    dtkTagScope(QWidget *parent = 0);
    ~dtkTagScope(void);

    QSize sizeHint(void) const;

    void render(void);

    void addTag(QString tag);
    void addTag(QString tag, int count);

    void setTags(const QStringList &tags);

    void setBlue(void);
    void setDark(void);

signals:
    void tagSet(QString tag);
    void tagAdded(QString tag);
    void tagRemoved(QString tag);
    void cleared(void);
    void unionMode(bool);

public slots:
    void clear(void);

public slots:
    void toggle(void);

protected slots:
    void onTagAdded(void);
    void onTagRemoved(void);

protected:
    dtkTagScopePrivate *d;
};

// /////////////////////////////////////////////////////////////////
// dtkItem
// /////////////////////////////////////////////////////////////////

class dtkItemPrivate;

class DTKGUISUPPORT_EXPORT dtkItem : public QListWidgetItem
{
public:
    dtkItem(QString name);
    dtkItem(QString name, QString description);
    dtkItem(QString name, QString description, QStringList tags);
    dtkItem(QString name, QString description, QStringList tags, QString kind, QString type);
    dtkItem(const dtkItem &item);
    ~dtkItem(void);

    QString name(void) const;
    QString description(void) const;
    QStringList tags(void) const;
    QString kind(void) const;
    QString type(void) const;

protected:
    dtkItemPrivate *d;
};

// /////////////////////////////////////////////////////////////////
// dtkItemList
// /////////////////////////////////////////////////////////////////

class dtkItemListPrivate;

class DTKGUISUPPORT_EXPORT dtkItemList : public QListWidget
{
    Q_OBJECT

public:
    dtkItemList(QWidget *parent = 0);
    ~dtkItemList(void);

    void addItem(QString name);
    void addItem(QString name, QString description);
    void addItem(QString name, QString description, QStringList tags);
    void addItem(QString name, QString description, QStringList tags, QString kind, QString type);
    void addItem(dtkItem item);

    void clear(void);

public:
    void setBlue(void);
    void setDark(void);

signals:
    void itemClicked(const QString &description);

protected slots:
    void onItemClicked(QListWidgetItem *item);

protected:
    QMimeData *mimeData(const QList<QListWidgetItem *> items) const;
    QStringList mimeTypes(void) const;

protected:
    dtkItemListPrivate *d;

protected:
    friend class dtkItemListDelegate;
    friend class dtkItemBlueDelegate;
    friend class dtkItemDarkDelegate;
};

// /////////////////////////////////////////////////////////////////
// dtkItemDesc
// /////////////////////////////////////////////////////////////////

class dtkItemDescPrivate;

class dtkItemDesc : public QFrame
{
    Q_OBJECT

public:
    dtkItemDesc(QWidget *parent = 0);
    ~dtkItemDesc(void);

signals:
    void back(void);

public slots:
    void clear(void);

public slots:
    void setDescription(const QString &description);

protected:
    dtkItemDescPrivate *d;
};

// /////////////////////////////////////////////////////////////////
// dtkItemListDelegate
// /////////////////////////////////////////////////////////////////

class DTKGUISUPPORT_EXPORT dtkItemListDelegate : public QStyledItemDelegate
{
public:
    dtkItemListDelegate(dtkItemList *list);

public:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option,
                       const QModelIndex &index) const;

public:
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

protected:
    dtkItemList *list;
};

// /////////////////////////////////////////////////////////////////
// dtkItemDarkDelegate
// /////////////////////////////////////////////////////////////////

class DTKGUISUPPORT_EXPORT dtkItemDarkDelegate : public QStyledItemDelegate
{
public:
    dtkItemDarkDelegate(dtkItemList *list);

public:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option,
                       const QModelIndex &index) const;

public:
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

protected:
    dtkItemList *list;
};

// /////////////////////////////////////////////////////////////////
// dtkItemBlueDelegate
// /////////////////////////////////////////////////////////////////

class DTKGUISUPPORT_EXPORT dtkItemBlueDelegate : public QStyledItemDelegate
{
public:
    dtkItemBlueDelegate(dtkItemList *list);

public:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option,
                       const QModelIndex &index) const;

public:
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

protected:
    dtkItemList *list;
};

// /////////////////////////////////////////////////////////////////
// dtkItemView
// /////////////////////////////////////////////////////////////////

class dtkItemViewPrivate;

class DTKGUISUPPORT_EXPORT dtkItemView : public QStackedWidget
{
    Q_OBJECT

public:
    enum Direction { Left2Right, Right2Left, Top2Bottom, Bottom2Top, Automatic };

public:
    dtkItemView(QWidget *parent = 0);
    ~dtkItemView(void);

public:
    dtkItemList *list(void);
    dtkItemDesc *desc(void);

public:
    void setBlue(void);
    void setDark(void);

public slots:
    void onItemClicked(const QString &description);

protected slots:
    void setSpeed(int speed);
    void setAnimation(QEasingCurve::Type type);
    void setVerticalMode(bool vertical = true);
    void setWrap(bool wrap);

protected slots:
    void slideInNext(void);
    void slideInPrev(void);
    void slideInIdx(int idx, Direction direction = Automatic);

signals:
    void animationFinished(void);

protected slots:
    void animationDoneSlot(void);

protected:
    void slideInWgt(QWidget *widget, Direction direction = Automatic);

protected:
    dtkItemViewPrivate *d;
};

// /////////////////////////////////////////////////////////////////
// dtkTagController
// /////////////////////////////////////////////////////////////////

class dtkTagControllerPrivate;

class DTKGUISUPPORT_EXPORT dtkTagController : public QObject
{
    Q_OBJECT

public:
    dtkTagController(void);
    ~dtkTagController(void);

    void attach(dtkItemView *view);
    void attach(dtkTagCloud *cloud);
    void attach(dtkTagScope *scope);

    void addItem(QString name);
    void addItem(QString name, QString description);
    void addItem(QString name, QString description, QStringList tags);
    void addItem(QString name, QString description, QStringList tags, QString kind, QString type);

    void update(void);
    void render(void);

public slots:
    void onUnionMode(bool mode);

protected slots:
    void addFilter(QString tag);
    void setFilter(QString tag);
    void remFilter(QString tag);
    void clear(void);

protected:
    dtkTagControllerPrivate *d;
};

#endif // DTKTAGCLOUD_H
