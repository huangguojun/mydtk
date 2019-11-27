// Version: $Id: 379f9d667a85dc61983fd5a1d3b3ced3949bab0c $
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

#include <QtCore>

class dtkNotificationEventPrivate;

class DTKWIDGETS_EXPORT dtkNotificationEvent : public QEvent
{
public:
    enum Type {
        Persistent,
        NonPersistent
    };

public:
     dtkNotificationEvent(void);
     dtkNotificationEvent(const dtkNotificationEvent& other);
    ~dtkNotificationEvent(void);

public:
    dtkNotificationEvent& operator=(const dtkNotificationEvent& other);

public:
    Type type(void);
    QString message(void) const;
    int duration(void) const;

public:
    void setType(Type type);
    void setMessage(const QString& messqge);
    void setDuration(int ms);

private:
    dtkNotificationEventPrivate *d;
};

//
// dtkNotificationEvent.h ends here
