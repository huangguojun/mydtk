/* dtkFinder.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Sat Jun 12 15:47:45 2010 (+0200)
 * Version: $Id: dab64846635dd0f9c802450b8e4abcaecb28859a $
 * Last-Updated: lun. févr.  3 14:30:48 2014 (+0100)
 *           By: Thibaud Kloczko
 *     Update #: 170
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkFinder.h"

#include <dtkCoreSupport/dtkGlobal.h>

#ifdef Q_OS_WIN
#    include <qt_windows.h>
#endif

#include <QListIterator>

// /////////////////////////////////////////////////////////////////
// dtkFinderToolBar
// /////////////////////////////////////////////////////////////////

class dtkFinderToolBarPrivate
{
public:
    QToolButton *prevButton;
    QToolButton *nextButton;
    QToolButton *listViewButton;
    QToolButton *treeViewButton;
    QToolButton *showHiddenFilesButton;

    QLinkedList<QString> pathList;
    QLinkedList<QString>::iterator iterator;
};

dtkFinderToolBar::dtkFinderToolBar(QWidget *parent)
    : QToolBar(parent), d(new dtkFinderToolBarPrivate)
{
    d->prevButton = new QToolButton(this);
    d->prevButton->setArrowType(Qt::LeftArrow);
    d->prevButton->setEnabled(0);
    d->prevButton->setIconSize(QSize(16, 16));
    d->prevButton->setToolTip(tr("Back"));

    d->nextButton = new QToolButton(this);
    d->nextButton->setArrowType(Qt::RightArrow);
    d->nextButton->setEnabled(0);
    d->nextButton->setIconSize(QSize(16, 16));
    d->nextButton->setToolTip(tr("Next"));

    d->listViewButton = new QToolButton(this);
    d->listViewButton->setCheckable(true);
    d->listViewButton->setChecked(true);
    d->listViewButton->setIcon(QIcon(":dtkGui/pixmaps/dtk-view-list.png"));
    d->listViewButton->setIconSize(QSize(16, 16));
    d->listViewButton->setToolTip(tr("Icon view"));

    d->treeViewButton = new QToolButton(this);
    d->treeViewButton->setCheckable(true);
    d->treeViewButton->setIcon(QIcon(":dtkGui/pixmaps/dtk-view-tree.png"));
    d->treeViewButton->setIconSize(QSize(16, 16));
    d->treeViewButton->setToolTip(tr("List view"));

    d->showHiddenFilesButton = new QToolButton(this);
    d->showHiddenFilesButton->setCheckable(true);
    d->showHiddenFilesButton->setIcon(QIcon(":dtkGui/pixmaps/hidden-folder.png"));
    d->showHiddenFilesButton->setIconSize(QSize(16, 16));
    d->showHiddenFilesButton->setToolTip(tr("Show/Hide hidden files"));
    // By default the showHiddenFilesButton is enabled in MacOS
#ifdef Q_OS_MAC
    d->showHiddenFilesButton->setChecked(Qt::Checked);
#endif

    QButtonGroup *viewButtonGroup = new QButtonGroup(this);
    viewButtonGroup->setExclusive(true);
    viewButtonGroup->addButton(d->listViewButton);
    viewButtonGroup->addButton(d->treeViewButton);

    this->addWidget(d->prevButton);
    this->addWidget(d->nextButton);
    this->addWidget(d->treeViewButton);
    this->addWidget(d->listViewButton);
    this->addWidget(d->showHiddenFilesButton);

    connect(d->prevButton, SIGNAL(clicked()), this, SLOT(onPrev()));
    connect(d->nextButton, SIGNAL(clicked()), this, SLOT(onNext()));

    connect(d->listViewButton, SIGNAL(clicked()), this, SIGNAL(listView()));
    connect(d->treeViewButton, SIGNAL(clicked()), this, SIGNAL(treeView()));

    connect(d->showHiddenFilesButton, SIGNAL(toggled(bool)), this, SIGNAL(showHiddenFiles(bool)));
}

dtkFinderToolBar::~dtkFinderToolBar(void)
{
    delete d;

    d = NULL;
}

QSize dtkFinderToolBar::sizeHint(void) const
{
    QSize size = QToolBar::sizeHint();
    size.setHeight(23);
    return size;
}

void dtkFinderToolBar::setPath(const QString &path)
{
    if (d->pathList.count()) {
        if (d->iterator != d->pathList.end())
            d->pathList.erase(d->pathList.begin(), d->iterator);
    }

    d->pathList.prepend(path);
    d->iterator = d->pathList.begin();

    if (d->pathList.count() > 1)
        d->prevButton->setEnabled(1);
    else
        d->prevButton->setEnabled(0);

    d->nextButton->setEnabled(0);
}

void dtkFinderToolBar::onNext(void)
{
    if (d->iterator != d->pathList.begin()) {
        emit(changed(*(--d->iterator)));
        d->prevButton->setEnabled(1);

        if (d->iterator == d->pathList.begin())
            d->nextButton->setEnabled(0);
    } else
        d->nextButton->setEnabled(0);
}

void dtkFinderToolBar::onPrev(void)
{
    if (d->iterator != --d->pathList.end()) {
        emit(changed(*(++d->iterator)));
        d->nextButton->setEnabled(1);

        if (d->iterator == (--d->pathList.end()))
            d->prevButton->setEnabled(0);
    } else
        d->prevButton->setEnabled(0);
}

void dtkFinderToolBar::onTreeView(void)
{
    d->treeViewButton->setChecked(true);
    d->listViewButton->setChecked(false);
}

void dtkFinderToolBar::onListView(void)
{
    d->treeViewButton->setChecked(false);
    d->listViewButton->setChecked(true);
}

void dtkFinderToolBar::onShowHiddenFiles(bool show)
{
    d->showHiddenFilesButton->setChecked(show);
}

// /////////////////////////////////////////////////////////////////
// dtkFinderSideView
// /////////////////////////////////////////////////////////////////

class dtkFinderSideViewPrivate
{
public:
    QList<QTreeWidgetItem *> items;
    int headerFontSize;
};

dtkFinderSideView::dtkFinderSideView(QWidget *parent)
    : QTreeWidget(parent), d(new dtkFinderSideViewPrivate)
{
    d->headerFontSize = 12;

    this->header()->hide();
    this->setAcceptDrops(true);
    this->setDropIndicatorShown(true);
    this->setDragDropMode(QAbstractItemView::DropOnly);
    this->setIndentation(10);
    this->setFrameStyle(QFrame::NoFrame);
    this->setAttribute(Qt::WA_MacShowFocusRect, false);
    this->setFocusPolicy(Qt::NoFocus);
    this->populate();

    this->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this,
            SLOT(onContextMenu(const QPoint &)));
    connect(this, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this,
            SLOT(onItemCicked(QTreeWidgetItem *, int)));
}

dtkFinderSideView::~dtkFinderSideView(void)
{
    delete d;

    d = NULL;
}

void dtkFinderSideView::populate(void)
{
    this->clear();
    d->items.clear();

    QFont groupFont;
    groupFont.setPointSize(d->headerFontSize);
    groupFont.setBold(true);
    groupFont.setCapitalization(QFont::AllUppercase);

    QBrush groupBrush;
    groupBrush.setColor(QColor("#758192"));

    // QFont itemFont;
    // itemFont.setPointSize(10);

    QFileIconProvider provider;

    QTreeWidgetItem *item1 = new QTreeWidgetItem(this, QStringList() << "Drives");
    item1->setFlags(Qt::ItemIsEnabled);
    item1->setData(0, Qt::FontRole, groupFont);
    item1->setData(0, Qt::ForegroundRole, groupBrush);

    QFileInfoList driveList;
#ifdef Q_OS_MAC
    QDir macDir("/Volumes");
    driveList = macDir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
#else
    driveList = QDir::drives();
#endif

    foreach (QFileInfo info, driveList) {

        QString dlabel = this->driveLabel(info.absoluteFilePath());
        QTreeWidgetItem *item =
                new QTreeWidgetItem(item1, QStringList() << (dlabel.isEmpty() ? "HD" : dlabel));
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        // item->setData(0, Qt::FontRole, itemFont);
        item->setData(0, Qt::UserRole, info.absoluteFilePath());
        item->setIcon(0, provider.icon(QFileIconProvider::Drive));

        d->items << item;
    }

    QTreeWidgetItem *item2 = new QTreeWidgetItem(this, QStringList() << "Places");
    item2->setFlags(Qt::ItemIsEnabled);
    item2->setData(0, Qt::FontRole, groupFont);
    item2->setData(0, Qt::ForegroundRole, groupBrush);

    QTreeWidgetItem *item21 = new QTreeWidgetItem(item2, QStringList() << "Desktop");
    item21->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    // item21->setData(0, Qt::FontRole, itemFont);
    item21->setData(0, Qt::UserRole,
                    QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    item21->setIcon(0,
                    provider.icon(QFileInfo(
                            QStandardPaths::writableLocation(QStandardPaths::DesktopLocation))));

    QTreeWidgetItem *item22 = new QTreeWidgetItem(item2, QStringList() << "Home");
    item22->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    // item22->setData(0, Qt::FontRole, itemFont);
    item22->setData(0, Qt::UserRole,
                    QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
    item22->setIcon(0,
                    provider.icon(QFileInfo(
                            QStandardPaths::writableLocation(QStandardPaths::HomeLocation))));

    QTreeWidgetItem *item23 = new QTreeWidgetItem(item2, QStringList() << "Documents");
    item23->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    // item23->setData(0, Qt::FontRole, itemFont);
    item23->setData(0, Qt::UserRole,
                    QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    item23->setIcon(0,
                    provider.icon(QFileInfo(
                            QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation))));

    d->items << item21 << item22 << item23;

    QTreeWidgetItem *item3 = new QTreeWidgetItem(this, QStringList() << "Bookmarks");
    item3->setFlags(Qt::ItemIsEnabled);
    item3->setData(0, Qt::FontRole, groupFont);
    item3->setData(0, Qt::ForegroundRole, groupBrush);

    QSettings settings;
    QStringList bookmarks = settings.value("dtkFinderBookmarks").toStringList();

    foreach (QString path, bookmarks) {

        QFileInfo info(path);

        if (info.exists()) {
            QTreeWidgetItem *item = new QTreeWidgetItem(item3, QStringList() << info.baseName());
            item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            // item->setData(0, Qt::FontRole, itemFont);
            item->setData(0, Qt::UserRole, info.absoluteFilePath());
            item->setIcon(0, provider.icon(info));

            d->items << item;
        }
    }

    this->expandItem(item1);
    this->expandItem(item2);
    this->expandItem(item3);
}

void dtkFinderSideView::setHeaderFontSize(int value)
{
    d->headerFontSize = value;
    this->populate();
}

int dtkFinderSideView::headerFontSize(void) const
{
    return d->headerFontSize;
}

QSize dtkFinderSideView::sizeHint(void) const
{
    return QSize(-1, -1);
}

void dtkFinderSideView::setPath(const QString &path)
{
    foreach (QTreeWidgetItem *item, d->items)
        if (item->data(0, Qt::UserRole).toString() == path)
            item->setSelected(true);
        else
            item->setSelected(false);
}

void dtkFinderSideView::addBookmark(const QString &path)
{
    QFileInfo info(path);

    if (!info.isDir())
        return;

    QSettings settings;
    QStringList bookmarks = settings.value("dtkFinderBookmarks").toStringList();
    bookmarks.removeAll(path);
    bookmarks.append(path);
    settings.setValue("dtkFinderBookmarks", bookmarks);

    this->populate();
}

void dtkFinderSideView::removeBookmark(const QString &path)
{
    QFileInfo info(path);

    if (!info.isDir())
        return;

    QSettings settings;
    QStringList bookmarks = settings.value("dtkFinderBookmarks").toStringList();
    bookmarks.removeAll(path);
    settings.setValue("dtkFinderBookmarks", bookmarks);

    this->populate();
}

void dtkFinderSideView::clearBookmarks(void)
{
    QSettings settings;
    settings.remove("dtkFinderBookmarks");

    this->populate();
}

void dtkFinderSideView::onItemCicked(QTreeWidgetItem *item, int)
{
    emit changed(item->data(0, Qt::UserRole).toString());
}

void dtkFinderSideView::onContextMenu(const QPoint &position)
{
    QTreeWidgetItem *item = this->itemAt(position);

    if (!item)
        return;

    QSettings settings;
    QStringList bookmarks = settings.value("dtkFinderBookmarks").toStringList();
    QString bookmark = item->data(0, Qt::UserRole).toString();

    if (!bookmarks.contains(bookmark))
        return;

    QMenu menu;
    menu.addAction("Remove bookmark");

    if (menu.exec(this->mapToGlobal(position)))
        this->removeBookmark(bookmark);
}

void dtkFinderSideView::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {

        QFileInfo info(event->mimeData()->urls().first().toLocalFile());

        if (!info.isDir()) {
            event->ignore();
            return;
        } else {
            event->accept();
            return;
        }
    }

    event->ignore();
}

void dtkFinderSideView::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasUrls()) {

        QFileInfo info(event->mimeData()->urls().first().toLocalFile());

        if (!info.isDir()) {
            event->ignore();
            return;
        } else {
            event->accept();
            return;
        }
    }

    event->ignore();
}

void dtkFinderSideView::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        QFileInfo info(event->mimeData()->urls().first().toLocalFile());
        this->addBookmark(info.absoluteFilePath());
        event->accept();
        return;
    }

    event->ignore();
}

QString dtkFinderSideView::driveLabel(QString drive)
{
#ifdef Q_OS_WIN
    drive.replace("/", "\\");
    TCHAR szVolumeName[256];
    TCHAR szFileSystemName[256];
    DWORD dwSerialNumber = 0;
    DWORD dwMaxFileNameLength = 256;
    DWORD dwFileSystemFlags = 0;
    bool ret =
            GetVolumeInformation(drive.toLatin1().constData(), szVolumeName, 256, &dwSerialNumber,
                                 &dwMaxFileNameLength, &dwFileSystemFlags, szFileSystemName, 256);

    if (!ret) {
        drive.remove("\\");
        QString decoratedDrive = "(" + drive + ")";
        return decoratedDrive;
    }

    QString vName = QString::fromLatin1(szVolumeName);
    vName.trimmed();
    drive.remove("\\");
    vName += " (" + drive + ")";
    return vName;
#endif

#ifdef Q_OS_MAC
    return QFileInfo(drive).baseName();
#endif

    return drive;
}

// /////////////////////////////////////////////////////////////////
// dtkFinderPathBarItem
// /////////////////////////////////////////////////////////////////

class dtkFinderPathBarItem
{
public:
    QString text;
    QIcon icon;
    QRect rect;
    QDir dir;
};

// /////////////////////////////////////////////////////////////////
// dtkFinderPathBar
// /////////////////////////////////////////////////////////////////

class dtkFinderPathBarPrivate
{
public:
    QString path;

    QList<dtkFinderPathBarItem *> items;
};

dtkFinderPathBar::dtkFinderPathBar(QWidget *parent) : QFrame(parent), d(new dtkFinderPathBarPrivate)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    // this->setFixedHeight(23);
    // this->setStyleSheet("font: 11px; border-top: 1px solid #d4d4d4;");
}

dtkFinderPathBar::~dtkFinderPathBar(void)
{
    foreach (dtkFinderPathBarItem *item, d->items)
        delete item;

    delete d;

    d = NULL;
}

QSize dtkFinderPathBar::sizeHint(void) const
{
    return QSize(-1, 23);
}

void dtkFinderPathBar::setPath(const QString &path)
{
    d->path = path;

    foreach (dtkFinderPathBarItem *item, d->items)
        delete item;

    d->items.clear();

    QFileIconProvider provider;

    QDir dir(d->path);
    do {

        dtkFinderPathBarItem *item = new dtkFinderPathBarItem;
        item->text = dir.dirName().isEmpty() ? dir.absolutePath() : dir.dirName();

        if (!dir.entryInfoList(QStringList() << ".").isEmpty())
            item->icon = provider.icon(dir.entryInfoList(QStringList() << ".").first());

        item->dir = dir;
        d->items.prepend(item);

    } while (dir.cdUp());

    this->update();
}

void dtkFinderPathBar::mousePressEvent(QMouseEvent *event)
{
    foreach (dtkFinderPathBarItem *item, d->items) {
        if (item->rect.contains(event->x(), event->y())) {
            // setPath will delete the items. Store the string so that it is
            // still valid afterwards.
            QString path(item->dir.absolutePath());
            this->setPath(path);
            emit changed(path);
            return;
        }
    }
}

void dtkFinderPathBar::paintEvent(QPaintEvent *event)
{
    DTK_UNUSED(event);

    QRect rect = this->rect();

    int x = 10;
    int y = (rect.height() - 16) / 2;

    QPainter painter(this);

    foreach (dtkFinderPathBarItem *item, d->items) {
        painter.drawPixmap(x, y, item->icon.pixmap(16, 16));
        painter.drawText(x + 16 + 4, y + 14, item->text);

        int delta = 16 + 4 + painter.fontMetrics().width(item->text) + 10;

        item->rect = QRect(x, 0, delta, rect.height());

        x += delta;
    }

    if (d->items.last()->rect.right() >= this->width()) {
        QRect prect(this->width() - 30, 0, 30, rect.height());
        painter.fillRect(prect, this->palette().background());
        painter.drawText(prect.left(), y + 14, "...");
    }
}

// /////////////////////////////////////////////////////////////////
// dtkFinderListView
// /////////////////////////////////////////////////////////////////

class dtkFinderListViewPrivate
{
public:
    QMenu *menu;
    QList<QAction *> defaultActions;
    QList<QAction *> customActions;
    QAction *bookmarkAction;
    bool allowFileBookmarking;
};

dtkFinderListView::dtkFinderListView(QWidget *parent)
    : QListView(parent), d(new dtkFinderListViewPrivate)
{
    this->setViewMode(QListView::IconMode);
    this->setWordWrap(true);
    this->setWrapping(true);
    this->setIconSize(QSize(64, 64));
    this->setResizeMode(QListView::Adjust);
    this->setGridSize(QSize(128, 96));
    this->setFrameStyle(QFrame::NoFrame);
    this->setAttribute(Qt::WA_MacShowFocusRect, false);

    this->setContextMenuPolicy(Qt::CustomContextMenu);

    d->menu = new QMenu(this);
    d->allowFileBookmarking = true;
    d->bookmarkAction = new QAction(tr("Bookmark"), this);
    d->bookmarkAction->setIconVisibleInMenu(true);
    d->bookmarkAction->setIcon(QIcon(":dtkGui/pixmaps/star.svg"));
    connect(d->bookmarkAction, SIGNAL(triggered()), this, SLOT(onBookmarkSelectedItemsRequested()));
    d->menu->addAction(d->bookmarkAction);

    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this,
            SLOT(updateContextMenu(const QPoint &)));
}

dtkFinderListView::~dtkFinderListView(void)
{
    delete d;

    d = NULL;
}

void dtkFinderListView::addContextMenuAction(QAction *action)
{
    d->customActions.append(action);
}

void dtkFinderListView::addDefaultContextMenuAction(QAction *action)
{
    d->defaultActions.append(action);
}

/** Returns the currently selected path, or the first one if more than one item
 * is selected. */

QString dtkFinderListView::selectedPath() const
{
    if (!selectedIndexes().count())
        return QString();

    return this->selectedPaths()[0];
}

/** Returns the currently selected paths. */

QStringList dtkFinderListView::selectedPaths() const
{
    if (!selectedIndexes().count())
        return QStringList();

    if (QFileSystemModel *model = qobject_cast<QFileSystemModel *>(this->model())) {
        QStringList selectedPaths = *(new QStringList());

        foreach (QModelIndex index, selectedIndexes())
            selectedPaths << model->filePath(index);

        return selectedPaths;
    } else
        return QStringList();
}

/**
 * Set the allowance of file bookmarking.
 * @param isAllowed - whether is allowed to bookmark files
 **/

void dtkFinderListView::allowFileBookmarking(bool isAllowed)
{
    d->allowFileBookmarking = isAllowed;
}

void dtkFinderListView::updateContextMenu(const QPoint &point)
{
    QModelIndex index = this->indexAt(point);

    d->menu->clear();

    if (index.isValid()) {
        if (!d->allowFileBookmarking) {
            bool removed = false;
            QString selectedPath = this->selectedPath();

            if (!selectedPath.isEmpty()) {
                QFileInfo fileInfo = QFileInfo(selectedPath);

                if (fileInfo.isFile()) {
                    d->menu->removeAction(d->bookmarkAction);
                    removed = true;
                }
            }

            if (!removed) {
                if (d->menu->actions().size() > 0)
                    d->menu->insertAction(d->menu->actions()[0], d->bookmarkAction);
                else
                    d->menu->addAction(d->bookmarkAction);
            }
        }

        // Add custom actions
        for (int i = 0; i < d->customActions.size(); i++) {
            d->menu->addAction(d->customActions.value(i));
        }

        d->menu->addSeparator();
    }

    // By default add default action
    for (int i = 0; i < d->defaultActions.size(); i++) {
        d->menu->addAction(d->defaultActions.value(i));
    }

    d->menu->exec(mapToGlobal(point));
}

void dtkFinderListView::onBookmarkSelectedItemsRequested(void)
{
    if (!selectedIndexes().count())
        return;

    foreach (QString path, this->selectedPaths())
        emit bookmarked(path);
}

void dtkFinderListView::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Up && (event->modifiers() & Qt::ControlModifier)) {
        if (QFileSystemModel *model = qobject_cast<QFileSystemModel *>(this->model())) {
            QDir dir = QDir(model->filePath(this->rootIndex()));
            dir.cdUp();
            this->setRootIndex(model->index(dir.absolutePath()));
            emit changed(dir.absolutePath());
            event->accept();
            return;
        }
    }

    if (event->key() == Qt::Key_Down && (event->modifiers() & Qt::ControlModifier)) {
        if (QFileSystemModel *model = qobject_cast<QFileSystemModel *>(this->model())) {

            if (!this->selectionModel()->selectedIndexes().count())
                return;

            QFileInfo selection =
                    model->fileInfo(this->selectionModel()->selectedIndexes().first());

            if (selection.isDir()) {
                this->setRootIndex(model->index(selection.filePath()));
                emit changed(selection.absoluteFilePath());
                event->accept();
            }

            return;
        }
    }

    QListView::keyPressEvent(event);
}

void dtkFinderListView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (QFileSystemModel *model = qobject_cast<QFileSystemModel *>(this->model())) {

        Q_UNUSED(model);

        QModelIndex index = indexAt(event->pos());

        if (!index.isValid())
            return;

        QListView::mouseDoubleClickEvent(event);
    }
}

void dtkFinderListView::startDrag(Qt::DropActions supportedActions)
{
    QModelIndexList indexes = selectedIndexes();

    if (indexes.count() > 0) {

        QMimeData *data = model()->mimeData(indexes);

        if (!data)
            return;

        QFileIconProvider provider;

        QPixmap pixmap =
                provider.icon(QFileInfo(data->urls().first().toLocalFile())).pixmap(64, 64);
        QDrag *drag = new QDrag(this);
        drag->setPixmap(pixmap);
        drag->setMimeData(data);
        drag->setHotSpot(QPoint(pixmap.width() / 2, pixmap.height() / 2));
        drag->exec(supportedActions, Qt::IgnoreAction);
    }
}

// /////////////////////////////////////////////////////////////////
// dtkFinderTreeView
// /////////////////////////////////////////////////////////////////

class dtkFinderTreeViewPrivate
{
public:
    QMenu *menu;
    QList<QAction *> defaultActions;
    QList<QAction *> customActions;
    QAction *bookmarkAction;
    bool allowFileBookmarking;
};

dtkFinderTreeView::dtkFinderTreeView(QWidget *parent)
    : QTreeView(parent), d(new dtkFinderTreeViewPrivate)
{
    this->setDragEnabled(true);
    this->setFrameStyle(QFrame::NoFrame);
    this->setAttribute(Qt::WA_MacShowFocusRect, false);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->setSortingEnabled(true);
    this->sortByColumn(0, Qt::AscendingOrder);

    d->menu = new QMenu(this);
    d->allowFileBookmarking = true;
    d->bookmarkAction = new QAction(tr("Bookmark"), this);
    d->bookmarkAction->setIconVisibleInMenu(true);
    d->bookmarkAction->setIcon(QIcon(":dtkGui/pixmaps/star.svg"));
    connect(d->bookmarkAction, SIGNAL(triggered()), this, SLOT(onBookmarkSelectedItemsRequested()));
    d->menu->addAction(d->bookmarkAction);

    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this,
            SLOT(updateContextMenu(const QPoint &)));
}

dtkFinderTreeView::~dtkFinderTreeView(void)
{
    delete d;

    d = NULL;
}

int dtkFinderTreeView::sizeHintForColumn(int column) const
{
    if (column == 1)
        return 400;

    return 100;
}

void dtkFinderTreeView::addContextMenuAction(QAction *action)
{
    d->customActions.append(action);
}

void dtkFinderTreeView::addDefaultContextMenuAction(QAction *action)
{
    d->defaultActions.append(action);
}

/** Returns the currently selected path, or the first one if more than one item
 * is selected. */

QString dtkFinderTreeView::selectedPath() const
{
    if (!selectedIndexes().count())
        return QString();

    return this->selectedPaths()[0];
}

/** Returns the currently selected paths. */

QStringList dtkFinderTreeView::selectedPaths() const
{
    if (!selectedIndexes().count())
        return QStringList();

    // the treeview considers each cell as a selected item
    // hence we will need to group items by row
    // or take one item per row

    QList<int> alreadyReadRows;

    if (QFileSystemModel *model = qobject_cast<QFileSystemModel *>(this->model())) {
        QStringList selectedPaths = *(new QStringList());

        foreach (QModelIndex index, selectedIndexes()) {
            if (!alreadyReadRows.contains(index.row())) {
                selectedPaths << model->filePath(index);
                alreadyReadRows << index.row();
            }
        }

        return selectedPaths;
    } else
        return QStringList();
}

/**
 * Set the allowance of file bookmarking.
 * @param isAllowed - whether is allowed to bookmark files
 **/

void dtkFinderTreeView::allowFileBookmarking(bool isAllowed)
{
    d->allowFileBookmarking = isAllowed;
}

void dtkFinderTreeView::updateContextMenu(const QPoint &point)
{
    QModelIndex index = this->indexAt(point);

    d->menu->clear();

    if (index.isValid()) {
        if (!d->allowFileBookmarking) {
            bool removed = false;
            QString selectedPath = this->selectedPath();

            if (!selectedPath.isEmpty()) {
                QFileInfo fileInfo = QFileInfo(selectedPath);

                if (fileInfo.isFile()) {
                    d->menu->removeAction(d->bookmarkAction);
                    removed = true;
                }
            }

            if (!removed) {
                if (d->menu->actions().size() > 0)
                    d->menu->insertAction(d->menu->actions()[0], d->bookmarkAction);
                else
                    d->menu->addAction(d->bookmarkAction);
            }
        }

        // Add custom actions
        for (int i = 0; i < d->customActions.size(); i++) {
            d->menu->addAction(d->customActions.value(i));
        }

        d->menu->addSeparator();
    }

    // By default add default action
    for (int i = 0; i < d->defaultActions.size(); i++) {
        d->menu->addAction(d->defaultActions.value(i));
    }

    d->menu->exec(mapToGlobal(point));
}

void dtkFinderTreeView::onBookmarkSelectedItemsRequested(void)
{
    if (!selectedIndexes().count())
        return;

    foreach (QString path, this->selectedPaths())
        emit bookmarked(path);
}

void dtkFinderTreeView::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Up && (event->modifiers() & Qt::ControlModifier)) {
        if (QFileSystemModel *model = qobject_cast<QFileSystemModel *>(this->model())) {
            QDir dir = QDir(model->filePath(this->rootIndex()));
            dir.cdUp();
            this->setRootIndex(model->index(dir.absolutePath()));
            emit changed(dir.absolutePath());
            event->accept();
            return;
        }
    }

    if (event->key() == Qt::Key_Down && (event->modifiers() & Qt::ControlModifier)) {
        if (QFileSystemModel *model = qobject_cast<QFileSystemModel *>(this->model())) {

            if (!this->selectionModel()->selectedIndexes().count())
                return;

            QFileInfo selection =
                    model->fileInfo(this->selectionModel()->selectedIndexes().first());

            if (selection.isDir()) {
                this->setRootIndex(model->index(selection.filePath()));
                emit changed(selection.absoluteFilePath());
                event->accept();
            }

            return;
        }
    }

    QTreeView::keyPressEvent(event);
}

void dtkFinderTreeView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (QFileSystemModel *model = qobject_cast<QFileSystemModel *>(this->model())) {

        Q_UNUSED(model);

        QModelIndex index = indexAt(event->pos());

        if (!index.isValid())
            return;

        QTreeView::mouseDoubleClickEvent(event);
    }
}

void dtkFinderTreeView::startDrag(Qt::DropActions supportedActions)
{
    QModelIndexList indexes = selectedIndexes();

    if (indexes.count() > 0) {

        QMimeData *data = model()->mimeData(indexes);

        if (!data)
            return;

        QFileIconProvider provider;

        QPixmap pixmap =
                provider.icon(QFileInfo(data->urls().first().toLocalFile())).pixmap(64, 64);
        QDrag *drag = new QDrag(this);
        drag->setPixmap(pixmap);
        drag->setMimeData(data);
        drag->setHotSpot(QPoint(pixmap.width() / 2, pixmap.height() / 2));
        drag->exec(supportedActions, Qt::IgnoreAction);
    }
}

void dtkFinderTreeView::resizeEvent(QResizeEvent *event)
{
    DTK_UNUSED(event);

    this->setColumnWidth(0, this->width() / 2);
}

// /////////////////////////////////////////////////////////////////
// dtkFinder
// /////////////////////////////////////////////////////////////////

class dtkFinderPrivate
{
public:
    QFileSystemModel *model;
    bool isAllowedMultipleSelection;
    bool hiddenFilesShown;

    QAction *iconviewAction;
    QAction *listviewAction;
    QAction *showHideAction;

    dtkFinderListView *list;
    dtkFinderTreeView *tree;

    QStackedWidget *stack;
};

dtkFinder::dtkFinder(QWidget *parent) : QWidget(parent), d(new dtkFinderPrivate)
{
    d->model = new QFileSystemModel(this);
    d->model->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    d->hiddenFilesShown = false;
    // By default the showHiddenFilesButton is enabled in MacOS
#ifdef Q_OS_MAC
    d->hiddenFilesShown = true;
    d->model->setFilter(QDir::AllEntries | QDir::Hidden | QDir::NoDotAndDotDot);
#endif
    d->model->setRootPath(QDir::rootPath());

    d->list = new dtkFinderListView(this);
    d->list->setModel(d->model);
    d->list->setRootIndex(d->model->index(QDir::currentPath()));

    d->tree = new dtkFinderTreeView(this);
    d->tree->setModel(d->model);
    d->tree->setRootIndex(d->model->index(QDir::currentPath()));
    d->tree->setSelectionBehavior(QAbstractItemView::SelectRows);
    d->tree->setAllColumnsShowFocus(true);

    d->iconviewAction = new QAction(tr("Icon view"), this);
    d->iconviewAction->setIconVisibleInMenu(true);
    d->iconviewAction->setIcon(QIcon(":dtkGui/pixmaps/dtk-view-list.png"));
    d->listviewAction = new QAction(tr("List view"), this);
    d->listviewAction->setIconVisibleInMenu(true);
    d->listviewAction->setIcon(QIcon(":dtkGui/pixmaps/dtk-view-tree.png"));
    d->showHideAction = new QAction(tr("Show hidden files"), this);

    // By default the showHiddenFilesButton is enabled in MacOS
#ifdef Q_OS_MAC
    d->showHideAction->setText(tr("Hide hidden files"));
#endif
    d->showHideAction->setIconVisibleInMenu(true);
    d->showHideAction->setIcon(QIcon(":dtkGui/pixmaps/hidden-folder.png"));

    d->list->addDefaultContextMenuAction(d->listviewAction);
    d->list->addDefaultContextMenuAction(d->showHideAction);

    d->tree->addDefaultContextMenuAction(d->iconviewAction);
    d->tree->addDefaultContextMenuAction(d->showHideAction);

    d->stack = new QStackedWidget(this);
    d->stack->addWidget(d->list);
    d->stack->addWidget(d->tree);
    d->stack->setCurrentIndex(0);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->stack);

    connect(d->iconviewAction, SIGNAL(triggered()), this, SLOT(switchToListView()));
    connect(d->listviewAction, SIGNAL(triggered()), this, SLOT(switchToTreeView()));
    connect(d->showHideAction, SIGNAL(triggered()), this, SLOT(switchShowHiddenFiles()));

    connect(d->list, SIGNAL(doubleClicked(QModelIndex)), this,
            SLOT(onIndexDoubleClicked(QModelIndex)));
    connect(d->tree, SIGNAL(doubleClicked(QModelIndex)), this,
            SLOT(onIndexDoubleClicked(QModelIndex)));

    connect(d->list, SIGNAL(clicked(QModelIndex)), this, SLOT(onIndexClicked(QModelIndex)));
    connect(d->tree, SIGNAL(clicked(QModelIndex)), this, SLOT(onIndexClicked(QModelIndex)));

    connect(d->list, SIGNAL(changed(QString)), this, SLOT(setPath(QString)));
    connect(d->tree, SIGNAL(changed(QString)), this, SLOT(setPath(QString)));

    connect(d->list, SIGNAL(changed(QString)), this, SIGNAL(changed(QString)));
    connect(d->tree, SIGNAL(changed(QString)), this, SIGNAL(changed(QString)));

    connect(d->list, SIGNAL(bookmarked(QString)), this, SIGNAL(bookmarked(QString)));
    connect(d->tree, SIGNAL(bookmarked(QString)), this, SIGNAL(bookmarked(QString)));

    connect(d->list->selectionModel(),
            SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this,
            SLOT(onSelectionChanged(const QItemSelection &, const QItemSelection &)));
    connect(d->tree->selectionModel(),
            SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this,
            SLOT(onSelectionChanged(const QItemSelection &, const QItemSelection &)));

    QAction *switchToListViewAction = new QAction(this);
    QAction *switchToTreeViewAction = new QAction(this);

    switchToListViewAction->setShortcut(Qt::ControlModifier + Qt::Key_1);
    switchToTreeViewAction->setShortcut(Qt::ControlModifier + Qt::Key_2);

    this->addAction(switchToListViewAction);
    this->addAction(switchToTreeViewAction);

    connect(switchToListViewAction, SIGNAL(triggered()), this, SLOT(switchToListView()));
    connect(switchToTreeViewAction, SIGNAL(triggered()), this, SLOT(switchToTreeView()));
}

dtkFinder::~dtkFinder(void)
{
    delete d;

    d = NULL;
}

void dtkFinder::addContextMenuAction(QAction *action)
{
    d->list->addContextMenuAction(action);
    d->tree->addContextMenuAction(action);
}

/** Returns the currently selected path, or the first one if more than one item
 * is selected. */

QString dtkFinder::selectedPath(void) const
{
    if (d->stack->currentIndex() == 0)
        return d->list->selectedPath();

    if (d->stack->currentIndex() == 1)
        return d->tree->selectedPath();

    return QString();
}

/** Returns the currently selected paths. */

QStringList dtkFinder::selectedPaths() const
{
    if (d->stack->currentIndex() == 0)
        return d->list->selectedPaths();

    if (d->stack->currentIndex() == 1)
        return d->tree->selectedPaths();

    return QStringList();
}
/**
 * Set the allowance of file bookmarking.
 * @param isAllowed - whether is allowed to bookmark files
 **/

void dtkFinder::allowFileBookmarking(bool isAllowed)
{
    d->list->allowFileBookmarking(isAllowed);
    d->tree->allowFileBookmarking(isAllowed);
}

/** Set whether multiple files can be selected at the same time. */

void dtkFinder::allowMultipleSelection(bool isAllowed)
{
    d->isAllowedMultipleSelection = isAllowed;

    if (isAllowed) {
        d->list->setSelectionMode(QAbstractItemView::ExtendedSelection);
        d->tree->setSelectionMode(QAbstractItemView::ExtendedSelection);
    } else {
        d->list->setSelectionMode(QAbstractItemView::SingleSelection);
        d->tree->setSelectionMode(QAbstractItemView::SingleSelection);
    }
}

void dtkFinder::setPath(const QString &path)
{
    if (sender() != d->list)
        d->list->setRootIndex(d->model->index(path));

    if (sender() != d->tree)
        d->tree->setRootIndex(d->model->index(path));
}

void dtkFinder::switchToListView(void)
{
    emit listView();
    d->stack->setCurrentIndex(0);
    emitSelectedItems();
}

void dtkFinder::switchToTreeView(void)
{
    emit treeView();
    d->stack->setCurrentIndex(1);
    emitSelectedItems();
}

void dtkFinder::onShowHiddenFiles(bool show)
{
    if (show) {
        d->hiddenFilesShown = true;
        d->showHideAction->setText(tr("Hide hidden files"));
        d->model->setFilter(QDir::Hidden | QDir::AllEntries | QDir::NoDotAndDotDot);
    } else {
        d->hiddenFilesShown = false;
        d->showHideAction->setText(tr("Show hidden files"));
        d->model->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    }
}

void dtkFinder::switchShowHiddenFiles()
{
    //    this->onShowHiddenFiles(!d->hiddenFilesShown);
    emit showHiddenFiles(!d->hiddenFilesShown);
}

void dtkFinder::onIndexClicked(QModelIndex index)
{
    emit fileClicked(d->model->fileInfo(index));
}

void dtkFinder::onIndexDoubleClicked(QModelIndex index)
{
    QFileInfo selection = d->model->fileInfo(index);

    if (selection.isDir()) {
        QModelIndex idx = d->model->index(d->model->filePath(index));

        d->list->setRootIndex(idx);
        d->tree->setRootIndex(idx);

        emit changed(selection.absoluteFilePath());
    } else
        emit fileDoubleClicked(selection.absoluteFilePath());

    emit nothingSelected();
}

/** Bookmarks the currently selected item(s). */

void dtkFinder::onBookmarkSelectedItemsRequested(void)
{
    if (d->stack->currentIndex() == 0)
        d->list->onBookmarkSelectedItemsRequested();

    if (d->stack->currentIndex() == 1)
        d->tree->onBookmarkSelectedItemsRequested();
}

void dtkFinder::emitSelectedItems()
{
    QStringList selectedPaths = *(new QStringList());

    if (d->stack->currentIndex() == 0)
        selectedPaths = d->list->selectedPaths();

    if (d->stack->currentIndex() == 1)
        selectedPaths = d->tree->selectedPaths();

    emit selectionChanged(selectedPaths);

    if (!selectedPaths.size())
        emit nothingSelected();
}

void dtkFinder::onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    // note that only the recently selected items are in the "selected" variable
    // items previously selected are not

    emitSelectedItems();
}
