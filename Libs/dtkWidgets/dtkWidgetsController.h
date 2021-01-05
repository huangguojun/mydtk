// Version: $Id: 4384c2157c9496657db37ecde9a97dc87d97f95b $
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

#include <QtCore/QObject>

class dtkWidgetsWidget;

class DTKWIDGETS_EXPORT dtkWidgetsController : public QObject
{
    Q_OBJECT

public:
    static dtkWidgetsController *instance(void);

public:
    void insert(dtkWidgetsWidget *);

public:
    dtkWidgetsWidget *view(const QString &name);

public:
    QStringList viewNames(void);

public slots:
    void clear(void);

signals:
    void inserted(dtkWidgetsWidget *);

protected:
    dtkWidgetsController(void);
    ~dtkWidgetsController(void);

private:
    static dtkWidgetsController *s_instance;

private:
    class dtkWidgetsControllerPrivate *d;
};

//
// dtkWidgetsController.h ends here
