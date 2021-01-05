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

#include <QtWidgets/QFrame>

class DTKWIDGETS_EXPORT dtkWidgetsOverlayPaneItem : public QFrame
{
    Q_OBJECT

public:
    dtkWidgetsOverlayPaneItem(QWidget *parent = nullptr);
    ~dtkWidgetsOverlayPaneItem(void);

public slots:
    void setTitle(const QString &title);

public slots:
    void toggle(void);

public slots:
    void addLayout(QLayout *);
    void addWidget(QWidget *);

protected:
    void setSlider(QWidget *);

private:
    class dtkWidgetsOverlayPaneItemPrivate *d;
};

//
// dtkWidgetsOverlayPaneItem.h ends here
