/* dtkWidgetsLogView.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Fri Mar  2 15:13:52 2012 (+0100)
 * Version: $Id: 8ac517c7b61691e6c84f84ecd3e9f215e26d88da $
 * Last-Updated: Tue Apr 24 12:29:06 2012 (+0200)
 *           By: Julien Wintz
 *     Update #: 362
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include <dtkLog/dtkLog.h>
#include <dtkLog/dtkLogModel.h>

#include "dtkWidgetsLogView.h"
#include "dtkWidgetsLogView_p.h"

// /////////////////////////////////////////////////////////////////
// dtkWidgetsLogViewBar
// /////////////////////////////////////////////////////////////////

dtkWidgetsLogViewBar::dtkWidgetsLogViewBar(QWidget *parent) : QFrame(parent)
{
    QPushButton *b_trace = new QPushButton("Trace", this);
    QPushButton *b_debug = new QPushButton("Debug", this);
    QPushButton *b_info  = new QPushButton("Info", this);
    QPushButton *b_warn  = new QPushButton("Warn", this);
    QPushButton *b_error = new QPushButton("Error", this);
    QPushButton *b_fatal = new QPushButton("Fatal", this);

    b_trace->setCheckable(true);
    b_debug->setCheckable(true);
    b_info->setCheckable(true);
    b_warn->setCheckable(true);
    b_error->setCheckable(true);
    b_fatal->setCheckable(true);

    b_trace->setChecked(true);
    b_debug->setChecked(true);
    b_info->setChecked(true);
    b_warn->setChecked(true);
    b_error->setChecked(true);
    b_fatal->setChecked(true);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(b_trace);
    layout->addWidget(b_debug);
    layout->addWidget(b_info);
    layout->addWidget(b_warn);
    layout->addWidget(b_error);
    layout->addWidget(b_fatal);

    connect(b_trace, SIGNAL(clicked(bool)), this, SIGNAL(displayTrace(bool)));
    connect(b_debug, SIGNAL(clicked(bool)), this, SIGNAL(displayDebug(bool)));
    connect(b_info,  SIGNAL(clicked(bool)), this, SIGNAL(displayInfo(bool)));
    connect(b_warn,  SIGNAL(clicked(bool)), this, SIGNAL(displayWarn(bool)));
    connect(b_error, SIGNAL(clicked(bool)), this, SIGNAL(displayError(bool)));
    connect(b_fatal, SIGNAL(clicked(bool)), this, SIGNAL(displayFatal(bool)));
}

dtkWidgetsLogViewBar::~dtkWidgetsLogViewBar(void)
{

}

// /////////////////////////////////////////////////////////////////
// dtkWidgetsLogViewTree
// /////////////////////////////////////////////////////////////////

dtkWidgetsLogViewTree::dtkWidgetsLogViewTree(QWidget *parent) : QTreeWidget(parent)
{
    this->setAttribute(Qt::WA_MacShowFocusRect, false);
    this->setFrameShape(QFrame::HLine);
    this->setHeaderHidden(true);

    this->runtime = new QTreeWidgetItem(QStringList() << "Runtime log");

    this->file = new QTreeWidgetItem(QStringList() << "File log");
    this->file->addChild(new QTreeWidgetItem(QStringList() << dtkLogPath(qApp)));

    this->addTopLevelItem(this->runtime);
    this->addTopLevelItem(this->file);

    connect(this, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(onItemClicked(QTreeWidgetItem *, int)));
}

dtkWidgetsLogViewTree::~dtkWidgetsLogViewTree(void)
{

}

void dtkWidgetsLogViewTree::onItemClicked(QTreeWidgetItem *item, int)
{
    if (item == this->runtime)
        emit runtimeClicked();
    else if (item == this->file)
        ;
    else
        emit fileClicked(item->text(0));
}

// /////////////////////////////////////////////////////////////////
// dtkWidgetsLogViewList
// /////////////////////////////////////////////////////////////////

dtkWidgetsLogViewList::dtkWidgetsLogViewList(QWidget *parent) : QListView(parent)
{
    this->model = new dtkLogModel(this);

    this->proxy = new QSortFilterProxyModel(this);
    this->proxy->setFilterKeyColumn(0);

    this->setModel(this->proxy);

    this->setAlternatingRowColors(true);
    this->setAttribute(Qt::WA_MacShowFocusRect, false);
    this->setFrameShape(QFrame::NoFrame);
    this->setRuntime();
}

dtkWidgetsLogViewList::~dtkWidgetsLogViewList(void)
{
    qDeleteAll(this->models);
}

void dtkWidgetsLogViewList::setAutoScroll(bool autoScroll)
{
    if (autoScroll)
        connect(this->model, SIGNAL(rowsInserted(const QModelIndex&, int, int)), this, SLOT(scrollToBottom()));
    else
        disconnect(this->model, SIGNAL(rowsInserted(const QModelIndex&, int, int)), this, SLOT(scrollToBottom()));
}

void dtkWidgetsLogViewList::setRuntime(void)
{
    dtkLogger::instance().attachModel(this->model);

    this->proxy->setSourceModel(this->model);
}

void dtkWidgetsLogViewList::setFile(const QString& path)
{
    if (this->models.contains(path))
        this->models.remove(path);

    QFile file(path);

    if (!file.open(QFile::ReadOnly))
        qDebug() << "Unable to read file" << path;

    QString contents = file.readAll();

    file.close();

    QStringListModel *model = new QStringListModel(contents.split("\n"));

    this->proxy->setSourceModel(model);

    this->models.insert(path, model);

    // QFileSystemWatcher *watcher = new QFileSystemWatcher(QStringList() << path, this);

    // connect(watcher, SIGNAL(fileChanged(const QString&)), this, SLOT(setFile(const QString&)));
}

void dtkWidgetsLogViewList::setFilter(const QRegExp& expression)
{
    this->proxy->setFilterRegExp(expression);
}

// /////////////////////////////////////////////////////////////////
// dtkWidgetsLogViewPrivate
// /////////////////////////////////////////////////////////////////

QRegExp dtkWidgetsLogViewPrivate::expression(void)
{
    if (this->exclude.isEmpty())
        return QRegExp();

    QString patterns;

    foreach (QString pattern, this->exclude)
        patterns.append(QString("%1|").arg(pattern));

    patterns.chop(1);

    return QRegExp(QString("^(?!%1).*").arg(patterns), Qt::CaseSensitive, QRegExp::RegExp2);
}

// /////////////////////////////////////////////////////////////////
// dtkWidgetsLogView
// /////////////////////////////////////////////////////////////////

dtkWidgetsLogView::dtkWidgetsLogView(QWidget *parent) : QWidget(parent), d(new dtkWidgetsLogViewPrivate)
{
    d->bar = new dtkWidgetsLogViewBar(this);

    d->tree = new dtkWidgetsLogViewTree(this);
    d->tree->setMaximumWidth(200);

    d->list = new dtkWidgetsLogViewList(this);

    d->checkbox_auto_scroll = new QCheckBox("Auto scroll", this);
    d->checkbox_auto_scroll->setTristate(false);

    QVBoxLayout *list_view_layout = new QVBoxLayout;
    list_view_layout->addWidget(d->checkbox_auto_scroll);
    list_view_layout->addWidget(d->list);

    QHBoxLayout *h_layout = new QHBoxLayout;
    h_layout->setContentsMargins(0, 0, 0, 0);
    h_layout->setSpacing(0);
    h_layout->addWidget(d->tree);
    h_layout->addLayout(list_view_layout);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->bar);
    layout->addLayout(h_layout);

    connect(d->tree, SIGNAL(runtimeClicked()), d->list, SLOT(setRuntime()));
    connect(d->tree, SIGNAL(fileClicked(const QString&)), d->list, SLOT(setFile(const QString&)));

    connect(d->checkbox_auto_scroll, SIGNAL(stateChanged(int)), this, SLOT(autoScrollChecked(int)));
    connect(d->list, SIGNAL(clicked(const QModelIndex&)), this, SLOT(disableAutoScroll()));
    connect(d->bar, SIGNAL(displayTrace(bool)), this, SLOT(displayTrace(bool)));
    connect(d->bar, SIGNAL(displayDebug(bool)), this, SLOT(displayDebug(bool)));
    connect(d->bar, SIGNAL(displayInfo(bool)),  this, SLOT(displayInfo(bool)));
    connect(d->bar, SIGNAL(displayWarn(bool)),  this, SLOT(displayWarn(bool)));
    connect(d->bar, SIGNAL(displayError(bool)), this, SLOT(displayError(bool)));
    connect(d->bar, SIGNAL(displayFatal(bool)), this, SLOT(displayFatal(bool)));

    d->checkbox_auto_scroll->setChecked(true);
}

dtkWidgetsLogView::~dtkWidgetsLogView(void)
{
    delete d;

    d = NULL;
}

void dtkWidgetsLogView::autoScrollChecked(int state)
{
    if (state == Qt::Unchecked)
        this->disableAutoScroll();
    else
        this->enableAutoScroll();
}

void dtkWidgetsLogView::disableAutoScroll(void)
{
    d->list->setAutoScroll(false);
    d->checkbox_auto_scroll->setChecked(false);
}

void dtkWidgetsLogView::enableAutoScroll(void)
{
    d->list->setAutoScroll(true);
    d->checkbox_auto_scroll->setChecked(true);
}

void dtkWidgetsLogView::displayTrace(bool display)
{
    if (!display)
        d->exclude << "TRACE";
    else
        d->exclude.removeAll("TRACE");

    d->list->setFilter(d->expression());
}

void dtkWidgetsLogView::displayDebug(bool display)
{
    if (!display)
        d->exclude << "DEBUG";
    else
        d->exclude.removeAll("DEBUG");

    d->list->setFilter(d->expression());
}

void dtkWidgetsLogView::displayInfo(bool display)
{
    if (!display)
        d->exclude << "INFO";
    else
        d->exclude.removeAll("INFO");

    d->list->setFilter(d->expression());
}

void dtkWidgetsLogView::displayWarn(bool display)
{
    if (!display)
        d->exclude << "WARN";
    else
        d->exclude.removeAll("WARN");

    d->list->setFilter(d->expression());
}

void dtkWidgetsLogView::displayError(bool display)
{
    if (!display)
        d->exclude << "ERROR";
    else
        d->exclude.removeAll("ERROR");

    d->list->setFilter(d->expression());
}

void dtkWidgetsLogView::displayFatal(bool display)
{
    if (!display)
        d->exclude << "FATAL";
    else
        d->exclude.removeAll("FATAL");

    d->list->setFilter(d->expression());
}
