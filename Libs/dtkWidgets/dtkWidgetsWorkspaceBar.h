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

#pragma once

#include <dtkWidgetsExport.h>

#include <QtWidgets>

class dtkWidgetsWorkspace;
class QStackedWidget;

class DTKWIDGETS_EXPORT dtkWidgetsWorkspaceBar : public QTabBar
{
    Q_OBJECT

public:
    dtkWidgetsWorkspaceBar(QWidget *parent = nullptr);
    virtual ~dtkWidgetsWorkspaceBar();

public:
    void setStack(QStackedWidget *stack);

signals:
    void indexChanged(int);
    void indexDeleted(int);

signals:
    void created(const QString &);

public slots:
    void addWorkspaceInMenu(const QString &group, const QString &group_description,
                            const QString &type, const QString &type_description,
                            const QString &workspace_name = QString(),
                            const QColor &color = QColor());

public:
    void buildFromFactory(void);

public slots:
    void createWorkspace(const QString &type);
    void createWorkspace(const QString &name, const QString &type, bool display_destroy = true);

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

public:
    void setDynamic(bool);

public Q_SLOTS:
    void setCurrentIndex(int index);

public:
    QSize sizeHint(void) const;

private:
    class dtkWidgetsWorkspaceBarPrivate *d;
};

//
// dtkWidgetsWorkspaceBar.h ends here
