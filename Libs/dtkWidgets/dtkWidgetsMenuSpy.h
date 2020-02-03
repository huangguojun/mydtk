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

#include <QtCore/QObject>

class dtkWidgetsMenuBar;
class dtkWidgetsMenuBarContainer;

class DTKWIDGETS_EXPORT dtkWidgetsMenuSpy : public QObject
{
    Q_OBJECT

public:
    dtkWidgetsMenuSpy(QObject *parent = nullptr);
   ~dtkWidgetsMenuSpy(void);

public:
   void setMenuBar         (dtkWidgetsMenuBar *);
   void setMenuBarContainer(dtkWidgetsMenuBarContainer *);

private:
   class dtkWidgetsMenuSpyPrivate *d;
};

//
// dtkWidgetsMenuSpy.h ends here
