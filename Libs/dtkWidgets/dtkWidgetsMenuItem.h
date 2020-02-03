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

#include <QtCore>
#include <QtWidgets>

class dtkCoreParameter;
class dtkWidgetsParameter;
class dtkWidgetsWorkspaceStackBar;

// /////////////////////////////////////////////////////////////////////////////
// dtkWidgetsMenuItem interface
// /////////////////////////////////////////////////////////////////////////////

class DTKWIDGETS_EXPORT dtkWidgetsMenuItem : public QAction
{
    Q_OBJECT

public:
     dtkWidgetsMenuItem(QObject * = nullptr);
     dtkWidgetsMenuItem(const QString&, QObject * = nullptr);
     dtkWidgetsMenuItem(int, const QString&, QObject * = nullptr);
    ~dtkWidgetsMenuItem(void);

signals:
    void clicked(void);

public:
    const QString& title(void) const;

public:
    int icon(void) const;

public:
    virtual bool isParameter(void) const { return false; }
    virtual bool isCustom(void) const { return false; }

protected:
    class dtkWidgetsMenuItemPrivate *d;
};

// /////////////////////////////////////////////////////////////////////////////
// dtkWidgetsMenuItemParameter interface
// /////////////////////////////////////////////////////////////////////////////

class DTKWIDGETS_EXPORT dtkWidgetsMenuItemParameter : public dtkWidgetsMenuItem
{
    Q_OBJECT

public:
    explicit dtkWidgetsMenuItemParameter(const QString& title, dtkCoreParameter *parameter, const QString& representation);
    ~dtkWidgetsMenuItemParameter(void);

public:
    bool isParameter(void) const override { return true; }

public:
    dtkCoreParameter *parameter(void);
    dtkWidgetsParameter *widget(void);

private:
    class dtkWidgetsMenuItemParameterPrivate *d;
};

// /////////////////////////////////////////////////////////////////////////////
// dtkWidgetsMenuItemCustom interface
// /////////////////////////////////////////////////////////////////////////////

class DTKWIDGETS_EXPORT dtkWidgetsMenuItemCustom : public dtkWidgetsMenuItem
{
    Q_OBJECT

public:
    bool isCustom(void) const override { return true; }

public:
    virtual QWidget *widget(void) = 0;

signals:
    void clicked(void);

protected:
     dtkWidgetsMenuItemCustom(const QString& title);
    ~dtkWidgetsMenuItemCustom(void) = default;
};

//
// dtkWidgetsMenuItem.h ends here
