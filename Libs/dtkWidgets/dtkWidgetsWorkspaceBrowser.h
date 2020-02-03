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

#include <dtkWidgets/dtkWidgets>

class DTKVISUALIZATIONWIDGETS_EXPORT dtkWidgetsWorkspaceBrowser : public dtkWorkspace
{
    Q_OBJECT

public:
     dtkWidgetsWorkspaceBrowser(QWidget *parent = nullptr);
    ~dtkWidgetsWorkspaceBrowser(void);

public:
    void enter(void) override {};
    void leave(void) override {};
    void apply(void) override;

private:
    class dtkWidgetsWorkspaceBrowserPrivate *d;
};

//
// dtkWidgetsWorkspaceBrowser.h ends here
