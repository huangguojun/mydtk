// Version: $Id: 868ffab7b18a3e3b240bce68765dee7e1918a2be $
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

class dtkViewLayoutItem;
class dtkViewLayoutPrivate;
class dtkViewWidget;

class DTKWIDGETS_EXPORT dtkViewLayout : public QFrame
{
    Q_OBJECT

public:
    dtkViewLayout(QWidget *parent = 0);
    ~dtkViewLayout(void);

public:
    dtkViewLayoutItem *root(void);
    dtkViewLayoutItem *current(void);

public:
    void setCurrent(dtkViewLayoutItem *item);

signals:
    void focused(dtkViewWidget *view);
    void unfocused(dtkViewWidget *view);

public slots:
    void clear(void);

private:
    dtkViewLayoutPrivate *d;
};

//
// dtkViewLayout.h ends here
