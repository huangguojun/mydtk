// Version: $Id: 5306270ca478a50a5ccf3eb6158c5a8621678d78 $
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

#include <dtkGuiSupportExport.h>

#include <QtWidgets>

class dtkViewLayout;
class dtkViewList;
class dtkViewListControlPrivate;

class DTKGUISUPPORT_EXPORT dtkViewListControl : public QFrame
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
