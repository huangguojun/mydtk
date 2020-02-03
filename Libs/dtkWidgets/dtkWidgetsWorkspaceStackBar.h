#pragma once

#include <dtkWidgetsExport.h>

#include <QtWidgets>

class DTKWIDGETS_EXPORT dtkWidgetsWorkspaceStackBar : public QFrame
{
    Q_OBJECT

public:
     dtkWidgetsWorkspaceStackBar(QWidget *parent = nullptr);
    ~dtkWidgetsWorkspaceStackBar(void);

public:
    QSize sizeHint(void) const override;

public:
    void setStack(QStackedWidget *);

private:
    class dtkWidgetsWorkspaceStackBarPrivate *d;
};
