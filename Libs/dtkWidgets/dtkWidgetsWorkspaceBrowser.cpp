// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "dtkWidgetsWorkspaceBrowser.h"

#include <dtkWidgets>
#include <dtk>

class dtkWidgetsWorkspaceBrowserPrivate
{
public:
    dtkFinder *finder;
    dtkFinderPathBar *path;
    dtkFinderToolBar *toolbar;

public:
    // dtkViewVolumic *browse_view;
    dtkViewForm *browse_view;

};

dtkWidgetsWorkspaceBrowser::dtkWidgetsWorkspaceBrowser(QWidget *parent) : dtkWorkspace(parent)
{
    d = new dtkWidgetsWorkspaceBrowserPrivate;

    d->finder = new dtkFinder(this);
    d->finder->switchToTreeView();

    d->path = new dtkFinderPathBar(this);
    d->path->setPath(QDir::currentPath());
    d->path->setFixedHeight(32);

    d->toolbar = new dtkFinderToolBar(this);
    d->toolbar->setPath(QDir::currentPath());

    // d->browse_view = new dtkViewVolumic(this);
    d->browse_view = new dtkViewForm(this);
    d->browse_view->setExportColor(dtkToolBar::browser_color);
    d->browse_view->togglePane();

    QHBoxLayout *toolbar_layout = new QHBoxLayout;
    toolbar_layout->setContentsMargins(0, 0, 0, 0);
    toolbar_layout->setSpacing(0);
    toolbar_layout->addWidget(d->toolbar);
    toolbar_layout->addWidget(d->path);

    QVBoxLayout *finder_layout = new QVBoxLayout;
    finder_layout->setContentsMargins(0, 0, 0, 0);
    finder_layout->setSpacing(0);
    finder_layout->addLayout(toolbar_layout);
    finder_layout->addWidget(d->finder);

    QWidget *finder = new QWidget(this);
    finder->setLayout(finder_layout);

    connect(d->finder, SIGNAL(changed(QString)), d->path,    SLOT(setPath(QString)));
    connect(d->finder, SIGNAL(changed(QString)), d->toolbar, SLOT(setPath(QString)));

    connect(d->path, SIGNAL(changed(QString)), d->finder,  SLOT(setPath(QString)));
    connect(d->path, SIGNAL(changed(QString)), d->toolbar, SLOT(setPath(QString)));

    connect(d->toolbar, SIGNAL(changed(QString)), d->finder, SLOT(setPath(QString)));
    connect(d->toolbar, SIGNAL(changed(QString)), d->path,   SLOT(setPath(QString)));
    connect(d->toolbar, SIGNAL(treeView()),       d->finder, SLOT(switchToTreeView()));
    connect(d->toolbar, SIGNAL(listView()),       d->finder, SLOT(switchToListView()));

    QSplitter *splitter = new QSplitter(this);
    splitter->addWidget(finder);
    splitter->addWidget(d->browse_view);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(splitter);
}

dtkWidgetsWorkspaceBrowser::~dtkWidgetsWorkspaceBrowser(void)
{
    delete d;
}

void dtkWidgetsWorkspaceBrowser::apply(void)
{

}

//
// dtkWidgetsWorkspaceBrowser.cpp ends here
