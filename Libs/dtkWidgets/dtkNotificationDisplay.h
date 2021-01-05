// Version: $Id: 33e2e5e6d4edea3be7aec4c8e08c2e856caf077b $
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

#include "dtkNotifiable.h"
#include <dtkWidgetsExport.h>

#include <QtWidgets>

class dtkNotificationDisplayPrivate;

class DTKWIDGETS_EXPORT dtkNotificationDisplay : public QFrame, public dtkNotifiable
{
    Q_OBJECT

public:
    dtkNotificationDisplay(QWidget *parent = 0);
    ~dtkNotificationDisplay(void);

public slots:
    void clear(void);
    void next(void);
    void previous(void);
    void dismiss(void);
    void dismissible(bool dismissible);
    void display(const QString &message);
    void setPersistentCount(int count);
    void setNonPersistentCount(int count);

public:
    QSize sizeHint(void) const;

private:
    dtkNotificationDisplayPrivate *d;
};

//
// dtkNotificationDisplay.h ends here
