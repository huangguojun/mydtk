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

#include <dtkWidgetsExport>

#include "dtkWidgetsMenuItem.h"

// /////////////////////////////////////////////////////////////////////////////
// dtkWidgetsMenuItemDIY interface
// /////////////////////////////////////////////////////////////////////////////

class DTKWIDGETS_EXPORT dtkWidgetsMenuItemDIY : public dtkWidgetsMenuItemCustom
{
    Q_OBJECT

public:
     dtkWidgetsMenuItemDIY(const QString& title);
     dtkWidgetsMenuItemDIY(const QString& title, QWidget *widget);
    ~dtkWidgetsMenuItemDIY(void);

public:
    QWidget *widget(void) override;

public:
    void addLayout(QLayout *);
    void addWidget(QWidget *);

public:
    void clear(void);
    
public:
    void setShowTitle(bool);
    bool showTitle(void);

public:
    void setSizePolicy(QSizePolicy::Policy);
    QSizePolicy::Policy policy(void);

private:
    class dtkWidgetsMenuItemDIYPrivate *d;
};

// /////////////////////////////////////////////////////////////////////////////
// dtkWidgetsMenuItemWorkspace interface
// /////////////////////////////////////////////////////////////////////////////

class DTKWIDGETS_EXPORT dtkWidgetsMenuItemWorkspace : public dtkWidgetsMenuItemCustom
{
    Q_OBJECT

public:
     dtkWidgetsMenuItemWorkspace(const QString& title, dtkWidgetsWorkspaceStackBar *bar);
    ~dtkWidgetsMenuItemWorkspace(void);

public:
    QWidget *widget(void) override;

private:
    class dtkWidgetsMenuItemWorkspacePrivate *d;
};

// /////////////////////////////////////////////////////////////////////////////
// dtkWidgetsMenuItemTheme interface
// /////////////////////////////////////////////////////////////////////////////

class DTKWIDGETS_EXPORT dtkWidgetsMenuItemTheme : public dtkWidgetsMenuItemCustom
{
    Q_OBJECT

public:
     dtkWidgetsMenuItemTheme(const QString& theme);
    ~dtkWidgetsMenuItemTheme(void);

public:
    QWidget *widget(void) override;

private:
    class dtkWidgetsMenuItemThemePrivate *d;
};

//
// dtkWidgetsMenuItem+custom.h ends here
