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

class DTKWIDGETS_EXPORT dtkWidgetsHUDInfo : public QFrame
{
    Q_OBJECT

public:
    dtkWidgetsHUDInfo(const QString &text, QWidget *parent = nullptr);
    ~dtkWidgetsHUDInfo(void);

public:
    QSize sizeHint(void) const;

protected:
    void paintEvent(QPaintEvent *);

private:
    class dtkWidgetsHUDInfoPrivate *d;
};

//
// dtkWidgetsHUDInfo.h ends here
