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

#include <QMainWindow>
#include <dtkWidgetsExport>

class dtkWidgetsMenu;
class dtkWidgetsMenuBar;
class QResizeEvent;

class DTKWIDGETS_EXPORT dtkWidgetsMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    dtkWidgetsMainWindow(QWidget *parent = nullptr);
    ~dtkWidgetsMainWindow(void);

public:
    dtkWidgetsMenuBar *menubar(void);

public:
    void populate(void);
    dtkWidgetsMenu *populateFileMenu(void);
    dtkWidgetsMenu *populateThemesMenu(void);
    dtkWidgetsMenu *populateAboutMenu(void);

protected:
    void resizeEvent(QResizeEvent *event);

private:
    class dtkWidgetsMainWindowPrivate *d;
};

//
// dtkWidgetsMainWindow.h ends here
