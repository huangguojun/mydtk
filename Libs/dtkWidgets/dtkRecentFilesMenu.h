// Version: $Id: 65329d6ab67c6a37d49c77b3e72f422bace34db1 $
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

#include <dtkWidgetsExport.h>

class DTKWIDGETS_EXPORT dtkRecentFilesMenu : public QMenu
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
