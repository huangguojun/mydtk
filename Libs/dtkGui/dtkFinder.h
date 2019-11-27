/* dtkFinder.h ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Sat Jun 12 15:45:12 2010 (+0200)
 * Version: $Id: 9faca5c051d00eee510c44175da742c537b30347 $
 * Last-Updated: Thu Dec  6 12:23:36 2012 (+0100)
 *           By: Julien Wintz
 *     Update #: 37
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKFINDER_H
#define DTKFINDER_H

#include <QtCore>
#include <QtWidgets>

#include <dtkGuiSupportExport.h>

// /////////////////////////////////////////////////////////////////
// dtkFinderToolBar
// /////////////////////////////////////////////////////////////////

class dtkFinderToolBarPrivate;

class DTKGUISUPPORT_EXPORT dtkFinderToolBar : public QToolBar
{
    Q_OBJECT

public:
    dtkFinderToolBar(QWidget *parent = 0);
    ~dtkFinderToolBar(void);

    QSize sizeHint (void) const;

signals:
    void changed(const QString& path);
    void listView (void);
    void treeView (void);
    void showHiddenFiles(bool);

public slots:
    void setPath(const QString& path);
    void onPrev (void);
    void onNext (void);
    void onListView(void);
    void onTreeView(void);
    void onShowHiddenFiles(bool);

protected:

private:
    dtkFinderToolBarPrivate *d;
};

// /////////////////////////////////////////////////////////////////
// dtkFinderSideView
// /////////////////////////////////////////////////////////////////

class dtkFinderSideViewPrivate;

class DTKGUISUPPORT_EXPORT dtkFinderSideView : public QTreeWidget
{
    Q_OBJECT

    Q_PROPERTY(int   headerFontSize
               READ  headerFontSize
               WRITE setHeaderFontSize)

public:
    dtkFinderSideView(QWidget *parent = 0);
    ~dtkFinderSideView(void);

    void populate(void);

    int headerFontSize(void) const;

    QSize sizeHint (void) const;

signals:
    void changed(const QString& path);

public slots:
    void setPath(const QString& path);
    void setHeaderFontSize(int value);

public slots:
    void    addBookmark(const QString& path);
    void removeBookmark(const QString& path);
    void  clearBookmarks(void);

private slots:
    void onItemCicked(QTreeWidgetItem *, int);
    void onContextMenu(const QPoint&);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

    QString driveLabel(QString drive);

private:
    dtkFinderSideViewPrivate *d;
};

// /////////////////////////////////////////////////////////////////
// dtkFinderPathBar
// /////////////////////////////////////////////////////////////////

class dtkFinderPathBarPrivate;

class DTKGUISUPPORT_EXPORT dtkFinderPathBar : public QFrame
{
    Q_OBJECT

public:
    dtkFinderPathBar(QWidget *parent = 0);
    ~dtkFinderPathBar(void);

    QSize sizeHint (void) const;

signals:
    void changed(const QString& path);

public slots:
    void setPath(const QString& path);

protected:
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    dtkFinderPathBarPrivate *d;
};

// /////////////////////////////////////////////////////////////////
// dtkFinderListView
// /////////////////////////////////////////////////////////////////

class dtkFinderListViewPrivate;

class DTKGUISUPPORT_EXPORT dtkFinderListView : public QListView
{
    Q_OBJECT

public:
    dtkFinderListView(QWidget *parent = 0);
    ~dtkFinderListView(void);

    void addContextMenuAction(QAction *action);

    void addDefaultContextMenuAction(QAction *action);

    QString selectedPath() const;

    QStringList selectedPaths() const;

    void allowFileBookmarking(bool isAllowed);

signals:
    void changed(const QString& path);
    void bookmarked(const QString& path);

public slots:
    void onBookmarkSelectedItemsRequested(void);

protected slots:
    void updateContextMenu(const QPoint&);

protected:
    void keyPressEvent(QKeyEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void startDrag(Qt::DropActions supportedActions);

private:
    dtkFinderListViewPrivate *d;
};

// /////////////////////////////////////////////////////////////////
// dtkFinderTreeView
// /////////////////////////////////////////////////////////////////

class dtkFinderTreeViewPrivate;

class DTKGUISUPPORT_EXPORT dtkFinderTreeView : public QTreeView
{
    Q_OBJECT

public:
    dtkFinderTreeView(QWidget *parent = 0);
    ~dtkFinderTreeView(void);

    int sizeHintForColumn(int column) const;

    void addContextMenuAction(QAction *action);

    void addDefaultContextMenuAction(QAction *action);

    QString selectedPath() const;

    QStringList selectedPaths() const;

    void allowFileBookmarking(bool isAllowed);

signals:
    void changed(const QString& path);
    void bookmarked(const QString& path);

public slots:
    void onBookmarkSelectedItemsRequested(void);

protected slots:
    void updateContextMenu(const QPoint&);

protected:
    void keyPressEvent(QKeyEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void startDrag(Qt::DropActions supportedActions);
    void resizeEvent(QResizeEvent *event);

private:
    dtkFinderTreeViewPrivate *d;
};

// /////////////////////////////////////////////////////////////////
// dtkFinder
// /////////////////////////////////////////////////////////////////

class dtkFinderPrivate;

class DTKGUISUPPORT_EXPORT dtkFinder : public QWidget
{
    Q_OBJECT

public:
    dtkFinder(QWidget *parent = 0);
    ~dtkFinder(void);

    void addContextMenuAction(QAction *action);

    QString selectedPath() const;

    QStringList selectedPaths() const;

    void allowFileBookmarking(bool isAllowed);

    void allowMultipleSelection(bool isAllowed);

signals:
    void changed(const QString& path);
    void bookmarked(const QString& path);
    void fileDoubleClicked(const QString& filename);
    void fileClicked(const QFileInfo& info);
    void selectionChanged(const QStringList& paths);
    void nothingSelected();
    void listView (void);
    void treeView (void);
    void showHiddenFiles(bool);

public slots:
    void setPath(const QString& path);
    void switchToListView(void);
    void switchToTreeView(void);
    void onShowHiddenFiles(bool);
    void switchShowHiddenFiles();

    void onBookmarkSelectedItemsRequested(void);

protected slots:
    void onIndexDoubleClicked(QModelIndex);
    void onIndexClicked(QModelIndex);
    void onSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);
    void emitSelectedItems();

private:
    dtkFinderPrivate *d;
};

#endif // DTKFINDER_H
