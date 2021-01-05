// Version: $Id: 15815a6be919e0f14ade3f34ac10109d22f38b77 $
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

#include <QtWidgets/QFrame>

class dtkViewLayout;
class dtkViewManagerPrivate;
class dtkViewWidget;

class DTKWIDGETS_EXPORT dtkViewManager : public QFrame
{
    Q_OBJECT

public:
    dtkViewManager(QWidget *parent = 0);
    ~dtkViewManager(void);

signals:
    void focused(dtkViewWidget *view);

public slots:
    void clear(void);

public slots:
    void onViewFocused(dtkViewWidget *widget);
    void onViewUnfocused(dtkViewWidget *widget);

public:
    dtkViewLayout *layout(void);

private:
    dtkViewManagerPrivate *d;
};

//
// dtkViewManager.h ends here
