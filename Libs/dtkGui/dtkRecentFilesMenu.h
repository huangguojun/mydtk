// Version: $Id: bb9c7d3c09085cb2d72690e52c34fea40c7863b7 $
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

#include <QtWidgets>

#include <dtkGuiSupportExport.h>

class DTKGUISUPPORT_EXPORT dtkRecentFilesMenu : public QMenu
{
    Q_OBJECT
    Q_PROPERTY(int maxCount READ maxCount WRITE setMaxCount)

public:
    dtkRecentFilesMenu(QWidget *parent = 0);
    dtkRecentFilesMenu(const QString &title, QWidget *parent = 0);

    int maxCount(void) const;

public slots:
    void addRecentFile(const QString &fileName);
    void clearMenu(void);
    void setMaxCount(int);

signals:
    void recentFileTriggered(const QString &filename);

private slots:
    void menuTriggered(QAction *action);
    void updateRecentFileActions(void);

private:
    int m_maxCount;
};

//
// dtkRecentFilesMenu.h ends here
