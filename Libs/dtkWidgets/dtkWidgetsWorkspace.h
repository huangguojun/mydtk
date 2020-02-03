#pragma once

#include <dtkWidgetsExport>

#include <dtkCore>
#include <QtWidgets/QWidget>

class DTKWIDGETS_EXPORT dtkWidgetsWorkspace: public QWidget
{
    Q_OBJECT

public:
    dtkWidgetsWorkspace(QWidget *parent = nullptr);

public:
    virtual void enter(void) = 0;
    virtual void leave(void) = 0;

public slots:
    virtual void apply(void) = 0;
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (dtkWidgetsWorkspace *)
DTK_DECLARE_PLUGIN        (dtkWidgetsWorkspace, DTKWIDGETS_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(dtkWidgetsWorkspace, DTKWIDGETS_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(dtkWidgetsWorkspace, DTKWIDGETS_EXPORT)

namespace dtk {
    namespace widgets {
        DTK_DECLARE_CONCEPT(dtkWidgetsWorkspace, DTKWIDGETS_EXPORT, workspace);
    }
}
