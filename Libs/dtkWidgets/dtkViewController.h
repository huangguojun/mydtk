// Version: $Id: cef71cef6adbf900117cc695323b842edadc0b39 $
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

#include <QtCore>

class dtkViewWidget;

class DTKWIDGETS_EXPORT dtkViewController : public QObject
{
    Q_OBJECT

public:
    static dtkViewController *instance(void);

public:
    void insert(dtkViewWidget *);

public:
    dtkViewWidget *view(const QString& name);

public:
    QStringList viewNames(void);

signals:
    void inserted(dtkViewWidget *, const QString&);

protected:
     dtkViewController(void);
    ~dtkViewController(void);

private:
    static dtkViewController *s_instance;

private:
    class dtkViewControllerPrivate *d;
};

//
// dtkViewController.h ends here
