// Version: $Id: 764d18f214b1cd8d8ffe469151f6bef17048fdd2 $
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

class dtkViewLayout;
class dtkViewList;
class dtkViewListControlPrivate;

class DTKWIDGETS_EXPORT dtkViewListControl : public QFrame
{
    Q_OBJECT

public:
    dtkViewListControl(QWidget *parent = 0);
    ~dtkViewListControl(void);

public:
    void setLayout(dtkViewLayout *layout);
    void setList(dtkViewList *list);

public:
    bool isEmpty(void) const;

public:
    void closeAllLayout(void);
    void layoutHorizontally(void);
    void layoutVertically(void);
    void layoutGrid(void);

public slots:
    void onActorStarted(QString view_name);

protected slots:
    void onLayoutHorizontally(void);
    void onLayoutVertically(void);
    void onLayoutGrid(void);
    void onLayoutCloseAll(void);

private:
    dtkViewListControlPrivate *d;
};

//
// dtkViewListControl.h ends here
