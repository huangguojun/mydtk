// Version: $Id: f039d8ae74dcf8a83a82e430ab91d2b2ad99ccd6 $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "dtkRecentFilesMenu.h"

#include <QtCore>

dtkRecentFilesMenu::dtkRecentFilesMenu(QWidget *parent) : QMenu(parent), m_maxCount(8)
{
    connect(this, SIGNAL(triggered(QAction *)), this, SLOT(menuTriggered(QAction *)));

    setMaxCount(m_maxCount);
}

dtkRecentFilesMenu::dtkRecentFilesMenu(const QString& title, QWidget *parent) : QMenu(title, parent), m_maxCount(8)
{
    connect(this, SIGNAL(triggered(QAction *)), this, SLOT(menuTriggered(QAction *)));

    setMaxCount(m_maxCount);
}

void dtkRecentFilesMenu::addRecentFile(const QString& fileName)
{
    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();
    files.removeAll(fileName);
    files.prepend(fileName);

    while (files.size() > maxCount())
        files.removeLast();

    settings.setValue("recentFileList", files);

    updateRecentFileActions();
}

void dtkRecentFilesMenu::clearMenu(void)
{
    QSettings settings;
    settings.remove("recentFileList");

    updateRecentFileActions();
}

int dtkRecentFilesMenu::maxCount(void) const
{
    return m_maxCount;
}

void dtkRecentFilesMenu::setMaxCount(int count)
{
    m_maxCount = count;

    updateRecentFileActions();
}

void dtkRecentFilesMenu::menuTriggered(QAction *action)
{
    if (action->data().isValid())
        emit recentFileTriggered(action->data().toString());
}

void dtkRecentFilesMenu::updateRecentFileActions(void)
{
    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();

    int numRecentFiles = qMin(files.size(), maxCount());

    clear();

    for (int i = 0; i < numRecentFiles; ++i) {
        QString strippedName = QFileInfo(files[i]).fileName();
        QString text = QString("%1").arg(strippedName);

        QAction *recentFileAct = addAction(text);

        if (i + 1 == 1)
            recentFileAct->setShortcut(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_1);

        if (i + 1 == 2)
            recentFileAct->setShortcut(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_2);

        if (i + 1 == 3)
            recentFileAct->setShortcut(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_3);

        if (i + 1 == 4)
            recentFileAct->setShortcut(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_4);

        if (i + 1 == 5)
            recentFileAct->setShortcut(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_5);

        if (i + 1 == 6)
            recentFileAct->setShortcut(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_6);

        if (i + 1 == 7)
            recentFileAct->setShortcut(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_7);

        if (i + 1 == 8)
            recentFileAct->setShortcut(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_8);

        if (i + 1 == 9)
            recentFileAct->setShortcut(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_9);

        recentFileAct->setData(files[i]);
        recentFileAct->setVisible(true);
    }

    addSeparator();
    addAction(tr("Clear Menu"), this, SLOT(clearMenu()));

    setEnabled(numRecentFiles > 0);
}

//
// dtkRecentFilesMenu.cpp ends here
