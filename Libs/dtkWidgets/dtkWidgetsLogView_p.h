/* dtkWidgetsLogView_p.h ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Fri Mar  2 15:30:16 2012 (+0100)
 * Version: $Id: 0ea2f82acc7a8be08e14d22d9513e4fa98e71d9b $
 * Last-Updated: Tue Mar  6 10:04:35 2012 (+0100)
 *           By: Julien Wintz
 *     Update #: 66
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKWIDGETSLOGVIEW_P_H
#define DTKWIDGETSLOGVIEW_P_H

#include <dtkLog/dtkLogger.h>

#include <QtWidgets>

class dtkLogModel;

// /////////////////////////////////////////////////////////////////
// dtkWidgetsLogViewBar
// /////////////////////////////////////////////////////////////////

class dtkWidgetsLogViewBar : public QFrame
{
    Q_OBJECT

public:
    dtkWidgetsLogViewBar(QWidget *parent = 0);
    ~dtkWidgetsLogViewBar(void);

signals:
    void displayTrace(bool);
    void displayDebug(bool);
    void displayInfo(bool);
    void displayWarn(bool);
    void displayError(bool);
    void displayFatal(bool);
};

// /////////////////////////////////////////////////////////////////
// dtkWidgetsLogViewTree
// /////////////////////////////////////////////////////////////////

class dtkWidgetsLogViewTree : public QTreeWidget
{
    Q_OBJECT

public:
    dtkWidgetsLogViewTree(QWidget *parent = 0);
    ~dtkWidgetsLogViewTree(void);

signals:
    void runtimeClicked(void);
    void fileClicked(const QString &path);

protected slots:
    void onItemClicked(QTreeWidgetItem *, int);

private:
    QTreeWidgetItem *runtime;
    QTreeWidgetItem *file;
};

// /////////////////////////////////////////////////////////////////
// dtkWidgetsLogViewList
// /////////////////////////////////////////////////////////////////

class dtkWidgetsLogViewList : public QListView
{
    Q_OBJECT

public:
    dtkWidgetsLogViewList(QWidget *parent = 0);
    ~dtkWidgetsLogViewList(void);

public slots:
    void setRuntime(void);
    void setFile(const QString &path);
    void setAutoScroll(bool autoScroll);

public:
    void setFilter(const QRegExp &expression);

private:
    dtkLogModel *model;

private:
    QHash<QString, QStringListModel *> models;

private:
    QSortFilterProxyModel *proxy;
};

// /////////////////////////////////////////////////////////////////
// dtkWidgetsLogViewPrivate
// /////////////////////////////////////////////////////////////////

class dtkWidgetsLogViewPrivate
{
public:
    QRegExp expression(void);

public:
    dtkWidgetsLogViewBar *bar;
    dtkWidgetsLogViewTree *tree;
    dtkWidgetsLogViewList *list;

public:
    QStringList exclude;

public:
    QCheckBox *checkbox_auto_scroll;
};

#endif
